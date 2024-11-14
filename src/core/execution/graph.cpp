/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Poirier <mathieu.poiriert@enac.fr>
 *
 */

#include "core/utils/algorithm.h"

#if !defined(DJNN_NO_DEBUG) && defined(_DEBUG_GRAPH_INSERT_TIME) || !defined(_DEBUG_SEE_GRAPH_INFO_PREF)
#include <chrono>
#endif

#if !defined(DJNN_NO_DEBUG) || !defined(DJNN_NO_SERIALIZE)
#include "core/utils/iostream.h"
using djnnstl::cerr;
using djnnstl::cout;
using djnnstl::endl;
#include <cstring>
#include <fstream>
#include <algorithm>
#include <random>
#endif

#include "core/utils/error.h"
#include "core/utils/remotery.h"
#include "core/utils/utils-dev.h"
#include "exec_env/exec_env-dev.h"
#include "graph.h"
#include "utils/debug.h"

#ifndef DJNN_NO_DEBUG
#include "core/control/assignment.h"
#include "core/utils/utils-dev.h"

static int graph_counter_act = 0;
static bool _display_info_once = false;

#if _DEBUG_GRAPH_INSERT_TIME
static int nb_insert_by_graph_exec       = 0;
static int acc_insert_time_by_graph_exec = 0;
static int max_insert_time_all_graph     = 0;
static int max_insert_for_this_size      = 0;
#endif

#if _DEBUG_SEE_GRAPH_INFO_PREF
// #include "core/utils/utils-dev.h"
static int    graph_counter  = 0;
static double graph_total    = 0.0;
static double graph_average  = 0.0;
static int    sorted_counter = 0;
static double sorted_total   = 0.0;
static double sorted_average = 0.0;
#endif
#endif

namespace djnn {

using namespace djnnstl;

// -----------------------------------------------------------------------
// Vertex

Vertex::Vertex (CoreProcess* p)
    : _process (p), _execution_round (0), _order_stamp (0), _sorted_index (-1), _is_invalid (false), _count_edges_in (0)
{
}

Vertex::~Vertex ()
{
}

void
Vertex::add_edge (Vertex* dst)
{
    /*
       NOTE : We must avoid duplicated edges in the _edges vector to prevent multiple activations in a single execution.
       _edges is a vector because the insert order is important and used in sorting (traverse_depth_first)
       we can't use std::set, std::unordered_set or map because it would not keep this order.
       BUT the find operation on std::vector O(n) is more complex than that of std::set O(log(n)) or std::unordered_set O(1)
       so we add another _map_edges to manage duplicated info properly and remove it when there is no such an edge anymore
       FIXME?: maybe adapt the sort algorithm to use set or better unordered_set OR maybe replace current vector by _map_edges
    */

    auto edge = _map_edges.find (dst);

    if (edge == _map_edges.end ()) { // it's a NEW edge
        _edges.push_back (dst);
        dst->_count_edges_in += 1;
        _map_edges[dst] = 1;
    } else // it's a duplicate
        edge->second += 1;
}

void
Vertex::remove_edge (Vertex* dst)
{
    auto edge = _map_edges.find (dst);

    /* remove duplicate */
    if (edge != _map_edges.end () && edge->second > 1) {
        edge->second -= 1;
    } else {

        /* NOTE:
          for now we keep this complicated way of erasing instead of
            _edges.erase(djnnstl::remove (_edges.begin (), _edges.end (), dst), _edges.end ());
          to manage wrong removing, such as in unit tests
        */

        /* remove dst from _edges vector */
        auto newend = djnnstl::remove (_edges.begin (), _edges.end (), dst);

        /* check if end has changed and erase */
        if (newend != _edges.end ()) {
            /* erase them from _edges */
            _edges.erase (newend, _edges.end ());
            _map_edges.erase (dst);
            dst->_count_edges_in -= 1;
        }
    }
    // cerr << "remove_edge : " << "\t between " << get_hierarchy_name(get_process())  << " - " << get_hierarchy_name(dst->get_process()) << endl;
}

void
Vertex::set_position_in_graph_vertices (vertices_t::iterator end)
{
    _pos = djnnstl::prev (end);
}

// -----------------------------------------------------------------------
// Graph construction / destruction

Graph* Graph::_instance;

Graph&
Graph::instance ()
{
    if (!_instance)
        _instance = new Graph ();
    return *(_instance);
}

Graph::Graph ()
    : _cur_stamp (0), _sorted (false)
{
}

Graph::~Graph ()
{
    clear ();
    _instance = nullptr;
}

void
Graph::clear ()
{
    /* nothing to delete because vertices are owned by _vertices */
    _ordered_vertices.clear ();

    /* nothing to delete because vertices are owned by _vertices */
    _activation_deque.clear ();

    /* delete vertices from _vertices and clear */
    for (auto* v : _vertices)
        delete v;
    _vertices.clear ();

    /* delete output_vertices from _output_nodes and clear */
    for (auto* node : _output_nodes)
        delete node;
    _output_nodes.clear ();
}

// -----------------------------------------------------------------------
// Graph management

Vertex*
Graph::add_vertex (CoreProcess* c)
{
    auto* v = new Vertex (c);
    _vertices.push_back (v);
    v->set_position_in_graph_vertices (_vertices.end ());
    //_sorted = false; // adding a vertex keeps the global order, so do not invalidate it
    return v;
}

void
Graph::add_edge (CoreProcess* src, CoreProcess* dst)
{
    // cerr << "add_edge: " << get_hierarchy_name(src) << " - " << get_hierarchy_name(dst) << endl;

    Vertex* vs = src->vertex ();
    if (vs == nullptr) {
        vs = add_vertex (src);
        src->set_vertex (vs);
    }

    Vertex* vd = dst->vertex ();
    if (vd == nullptr) {
        vd = add_vertex (dst);
        dst->set_vertex (vd);
    }

    vs->add_edge (vd);
    _sorted = false;
}

void
Graph::remove_edge (CoreProcess* p_src, CoreProcess* p_dst)
{
    // cerr << "remove_edge: " << get_hierarchy_name(src) << " - " << get_hierarchy_name(dst) << endl;

    Vertex* vs = p_src->vertex ();
    Vertex* vd = p_dst->vertex ();

    /* note:
       this code is here to prevent bugs
       this should NEVER happen
       vertex should NOT be nullptr at this place
       if it is null, something (FatProcess or edge dependency) IS NOT well deleted
    */
    if (vs == nullptr || vd == nullptr) {
        warning (nullptr, " Graph::remove_edge - - vertex vs or vd is NULL and it SHOULD NOT HAPPEN (except in unit test) \n");
#ifndef DJNN_NO_DEBUG
        auto* ppsrc = p_src;
        auto* ppdst = p_dst;
        cerr << "Graph remove_edge: " << cpp_demangle (typeid (*p_src).name ()) + ":" + (ppsrc ? get_hierarchy_name (ppsrc) : "") << "  " << vs << " - " << cpp_demangle (typeid (*p_dst).name ()) + ":" + (ppdst ? get_hierarchy_name (ppdst) : "") << "  " << vd << endl;
#endif
        return;
    }

    vs->remove_edge (vd);

    /*
      delete vertex if they have no more out_edges and in_edges
    */

    // remove src if necessary
    if (vs->get_edges ().empty () && (vs->get_count_edges_in () == 0)) {
        _vertices.erase (vs->get_position_in_graph_vertices ());
        p_src->set_vertex (nullptr);
        delete vs;
    }

    // remove dst if necessary
    if (vd->get_edges ().empty () && (vd->get_count_edges_in () == 0)) {
        _vertices.erase (vd->get_position_in_graph_vertices ());
        p_dst->set_vertex (nullptr);
        delete vd;
    }

    //_sorted = false; // removing an edge should not break the order, so do not invalidate it
}

void
Graph::add_output_node (CoreProcess* c)
{
    // check if c is already in the graph
    for (auto v : _output_nodes) {
        if (v->get_process () == c)
            return; // what about multiple gui windows?
                    // assert (0); // programming error
    }
    auto* v = new Vertex (c);
    _output_nodes.push_back (v);
}

void
Graph::remove_output_node (CoreProcess* c)
{
    auto new_end = djnnstl::remove_if (_output_nodes.begin (), _output_nodes.end (),
                                       [c] (Vertex* v) { return v->get_process () == c; });

    if (new_end != _output_nodes.end ()) {
        // delete nodes
        for (auto it = new_end; it != _output_nodes.end (); ++it)
            delete *it;

        // erase from vector
        _output_nodes.erase (new_end, _output_nodes.end ());
    }
}

#ifndef DJNN_NO_DEBUG
static Vertex*                     current_v;
static map<Vertex*, list<Vertex*>> data_flow_paths_save;
static vector<string>              data_flow_result;
static string                      print_process_fileno (CoreProcess* p);
static string                      print_process_full_name (CoreProcess* p);
static string                      print_process_debug_info (CoreProcess* p);
static string                      extract_filename (string s);
static string                      extract_code_from_file (string filepath, int lineno);

void
print_data_flow_paths_save_single (Vertex* v, int index, Vertex* start_v)
{

    string save = data_flow_result[index] + " --- index: " + to_string (v->get_sorted_index ()) + " -- " + print_process_full_name (v->get_process ()) + print_process_debug_info (v->get_process ()) + "\n";
    auto   it   = data_flow_paths_save.find (v);
    if (it != data_flow_paths_save.end () && it->first != start_v /* ou egale au process de départ si boucle - A TESTER */) {
        int i = 0;
        for (auto vv : it->second) {
            if (i != 0) {
                data_flow_result.push_back (save);
                index++;
            }
            data_flow_result[index] = save;
            if (start_v == nullptr)
                print_data_flow_paths_save_single (vv, index, v); // initiliaze the first time
            else
                print_data_flow_paths_save_single (vv, index, start_v);
            i++;
        }
    } else
        data_flow_result[index] = save;
}

void
print_data_flow_paths_save (Vertex* v)
{

    data_flow_result.clear ();
    // create first string
    data_flow_result.push_back ("");

    std::cerr << std::endl
              << "data_flow_paths_save: ";
    print_data_flow_paths_save_single (v, 0, nullptr);

    if (data_flow_result.size () > 1)
        std::cerr << "!!! Multiples flow detected ";
    std::cerr << std::endl;
    int index = 0;
    for (auto s : data_flow_result)
        std::cerr << index++ << ":" << std::endl
                  << s << std::endl;

    // debug - print full data
    // std::cerr << std::endl << "FULL data_flow_paths_save: " << std::endl;
    // for (auto it2 : data_flow_paths_save) {
    //   std:cerr << "key: " << print_process_full_name(it2.first->get_process ()) << " : " << std::endl;
    //   for (auto caller : it2.second)
    //     std::cerr << "\t\t --------- " << print_process_full_name(caller->get_process ()) << std::endl;
    // }
}
#endif

void
Graph::add_in_activation (Vertex* v)
{
#if _DEBUG_GRAPH_INSERT_TIME
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now ();
#endif

    if (_sorted) {
        if (_activation_deque.empty ()) {
            _activation_deque.push_front (v);
        } else {
            auto pos = find_if (_activation_deque.begin (), _activation_deque.end (),
                                [v] (const Vertex* v1) { return v->get_sorted_index () < v1->get_sorted_index (); });

            _activation_deque.insert (pos, v);
        }
    } else {
        _activation_deque.push_front (v);
    }

#ifndef DJNN_NO_DEBUG
    if (_DEBUG_GRAPH_CYCLE_DETECT || _AUTHORIZE_CYCLE)
        (data_flow_paths_save[v]).push_back (current_v);
#endif

#if _DEBUG_GRAPH_INSERT_TIME
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now ();
    nb_insert_by_graph_exec++;
    int time                      = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count ();
    acc_insert_time_by_graph_exec = acc_insert_time_by_graph_exec + time;
    if (time > max_insert_time_all_graph) {
        max_insert_time_all_graph = time;
        max_insert_for_this_size  = _activation_deque.size ();
    }
    cerr << "!! insert time :" << time << endl
         << endl;
#endif
}

void
Graph::clear_activation ()
{
    _activation_deque.clear ();
}

// -----------------------------------------------------------------------
// Graph behavior: exec, sort, traverse_depth_first

#ifndef DJNN_NO_DEBUG
void
display_cycle_analysis_stack (map<Vertex*, int>& vertex_already_activated, int count_activation, Vertex* v);
#endif

// TODO: explain what EXECUTION_ROUND is
static int EXECUTION_ROUND = 0;

void
Graph::exec ()
{
#ifndef DJNN_NO_DEBUG
    if (_display_info_once == false) {
        if (_DEBUG_ENABLE_STRESS_TEST == 1) {
            std::cerr << "STRESS_TEST MODE ENABLED : shuffle" << std::endl;
        }
        else if (_DEBUG_ENABLE_STRESS_TEST == 2) {
            std::cerr << "STRESS_TEST MODE ENABLED : reverse" << std::endl;
        }
        _display_info_once = true;
    }
#endif

    // remotery start
    rmt_BeginCPUSample (Graph_exec, RMTSF_None);

#ifndef DJNN_NO_DEBUG
    bool display_sequence_on_target_location = false;
    if (_DEBUG_SEE_ACTIVATION_SEQUENCE && strcmp (_DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_LOCATION, "") == 0)
        display_sequence_on_target_location = true;
#endif

    // pre_execution : notify_activation *only once* per _scheduled_activations before real graph execution
    {
#ifndef DJNN_NO_DEBUG
        // if (_DEBUG_SEE_ACTIVATION_SEQUENCE)
        if (display_sequence_on_target_location)
            cerr << endl
                 << endl
                 << " -------- ACTIVATION TRIGGERS QUEUE ------ " << endl;
#endif

        _activation_triggers_to_sort.clear ();

        map<CoreProcess*, int> already_done;
        for (auto p : _scheduled_activations) {
            if (already_done.find (p) == already_done.end ()) {
                /*  note:
                    As a source of activation, nobody will actually active it.
                    We have to do it here, for each one, so it can propagate
                    their activations to their linked processes. Then we store
                    the vertex as the sub-root of the graph that we want
                    to sort and work on.
                */
                p->notify_activation ();
                already_done[p];
                if (p->vertex ())
                    _activation_triggers_to_sort.push_back (p->vertex ());
#ifndef DJNN_NO_DEBUG
                // if (_DEBUG_SEE_ACTIVATION_SEQUENCE)
                if (display_sequence_on_target_location)
                    cerr << "Scheduled ------ " << print_process_full_name (p) << " \t\t--  v: " << p->vertex () << endl;
#endif
            }
        }

        _scheduled_activations.clear ();

#ifndef DJNN_NO_DEBUG
        if (_DEBUG_SEE_ACTIVATION_SEQUENCE) {
            cerr << " ----------------------------------------- " << endl;
            int i = 0;
            for (auto v : _activation_triggers_to_sort) {
                if (strcmp (_DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_LOCATION, print_process_fileno (v->get_process ()).c_str ()) == 0)
                    display_sequence_on_target_location = true;
                if (display_sequence_on_target_location)
                    cerr << i++ << " - triggers ------ " << print_process_full_name (v->get_process ()) << "\t\t" << print_process_debug_info (v->get_process ()) << endl;
            }
            if (display_sequence_on_target_location)
                cerr << " ----------------------------------------- " << endl;
        }
#endif
    }

#ifndef DJNN_NO_DEBUG
    size_t count_activation = 0;

    int count_real_activation = 0;
    int count_targeted        = 0;
    graph_counter_act++;
    if (graph_counter_act == 0) {
        // reset to 0
        reset_vertices_execution_round ();
        // and start at 1
        graph_counter_act++;
    }
    EXECUTION_ROUND                                            = graph_counter_act;
    int                                          _sorted_break = 0;
    static std::chrono::steady_clock::time_point begin_act, begin_process_act, end_process_act;
    static std::chrono::steady_clock::time_point begin_delete, end_delete, begin_output, end_output;
    if (_DEBUG_SEE_ACTIVATION_SEQUENCE) {
        begin_act = std::chrono::steady_clock::now ();
        cerr << endl;
    }
#endif

    _sorted     = false;
    bool is_end = false;

    while (!is_end) {
        is_end = true;

#ifndef DJNN_NO_DEBUG
        map<Vertex*, int> _vertex_already_activated;
        if (_DEBUG_GRAPH_CYCLE_DETECT || _AUTHORIZE_CYCLE)
            data_flow_paths_save.clear ();
#endif

        // use only of _DEBUG_GRAPH_CYCLE_DETECT
        //  note: These two variables are merely a safeguard against the
        //  detection of false cycles caused by an erroneous topological sort.
        //  The MAX is merely a palliative value and is not a solution in itself.
        int local_cycle_counter = 0;
        int MAX_cycle_accepted  = 2;

        Vertex* v = nullptr;
        while (!_activation_deque.empty ()) {
            v = _activation_deque.front ();

            if (!_sorted) {
#ifndef DJNN_NO_DEBUG
                if (_DEBUG_SEE_ACTIVATION_SEQUENCE && display_sequence_on_target_location) {
                    _sorted_break++;
                    cerr << "\033[1;33m"
                         << "--- break to sort #" << _sorted_break << "\033[0m" << endl;
                }
#endif
                break;
            }
            // pop only if sorted otherwise the process activation will be skipped
            _activation_deque.pop_front ();

            if (v->is_invalid ())
                continue;
            auto* p = v->get_process ();

#ifndef DJNN_NO_DEBUG
            // detect cycles
            count_activation += 1;

            if (_DEBUG_GRAPH_CYCLE_DETECT || _AUTHORIZE_CYCLE) {
                auto it = _vertex_already_activated.find (v);
                if (it != _vertex_already_activated.end ()) {
                    if (_DEBUG_GRAPH_CYCLE_DETECT && _activation_triggers_to_sort.size () != 0) { // if _activation_triggers_to_sort is empty we are in initialisation phase
                        local_cycle_counter++;
                        if (local_cycle_counter >= MAX_cycle_accepted) {
                            cerr << "\033[1;31m";
                            cerr << endl
                                 << "djnn Warning - \tWe detected a cycle in GRAPH Execution" << endl;
                            cerr << "\t\t" << print_process_full_name (v->get_process ()) << " has already been activated in this execution.\n";
                            display_cycle_analysis_stack (_vertex_already_activated, count_activation, v);
                            print_data_flow_paths_save (v);
                            cerr << "\033[0m";
                        }
                    }
                    p->set_activation_flag (NONE_ACTIVATION);
                    continue;
                } else
                    _vertex_already_activated[v] = count_activation;
            } else {
                // default guard against cycles if _DEBUG_GRAPH_CYCLE_DETECT = 0
                if (count_activation > _vertices.size ()) {
                    cerr << "\033[1;31m";
                    cerr << "djnn Warning - \tWe detected more activations in only one graph execution than vertices in the graph !" << endl;
                    cerr << "\t\tIt seems there is a cycle in your program !! \n\t\tWe stopped it and broke out of GRAPH::EXEC" << endl;
                    cerr << "\t\tPlease activate debug option _DEBUG_GRAPH_CYCLE_DETECT to have full information about this cycle." << endl;
                    cerr << "\033[0m";
                    continue;
                }
            }

            if (_DEBUG_SEE_ACTIVATION_SEQUENCE) {
                if (p->get_activation_flag () != NONE_ACTIVATION) { // should be removed ??
                    begin_process_act = std::chrono::steady_clock::now ();
                    count_real_activation++;
                } else {
                    // why this process is in the list but his activation is now: none.
                    cerr << "\033[1;31m";
                    cerr << "djnn Warning - " << print_process_full_name (p) << " is in activation list but never activated " << endl;
                    cerr << "\t\t you may have a probleme of simultaneous flux " << endl;
                    cerr << "\033[0m";
                }
            }
#endif

#ifndef DJNN_NO_DEBUG
            if (_DEBUG_GRAPH_CYCLE_DETECT || _AUTHORIZE_CYCLE)
                current_v = v;
#endif

            p->trigger_activation_flag ();
            p->set_activation_flag (NONE_ACTIVATION);

#ifndef DJNN_NO_DEBUG
            if (_DEBUG_SEE_ACTIVATION_SEQUENCE && display_sequence_on_target_location) {
                end_process_act   = std::chrono::steady_clock::now ();
                int _process_time = std::chrono::duration_cast<std::chrono::microseconds> (end_process_act - begin_process_act).count ();
                if (_process_time > _DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_TIME_US)
                    cerr << "\033[1;36m";
                if (_process_time > _DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_TIME_US || !_DEBUG_SEE_ACTIVATION_SEQUENCE_ONLY_TARGETED) {
                    if (v->get_sorted_index () == -1)
                        cerr << "\033[1;35m";
                    if (display_sequence_on_target_location)
                        cerr << count_real_activation << " -- targeted i:" << ++count_targeted << " ---- i: " << v->get_sorted_index () << "---- process time act/deact = " << _process_time << "[us]"
                             << " --- " << print_process_full_name (p) << print_process_debug_info (p) << endl;
                }
                cerr << "\033[0m";
            }
#endif
        }

        // note:
        // we have to keep both systems sorted:
        // either it is triggered by an external source
        // or by a internal property
        if (!_sorted && !_activation_triggers_to_sort.empty ()) {
            sort (nullptr);
            is_end = false;
        } else if (!_sorted && v) {
            sort (v);
            is_end = false;
        }
    }

// execute scheduled deletion of processes
#ifndef DJNN_NO_DEBUG
    begin_delete = std::chrono::steady_clock::now ();
#endif

    for (auto p : _scheduled_deletions)
        delete p;
    _scheduled_deletions.clear ();

#ifndef DJNN_NO_DEBUG
    end_delete = std::chrono::steady_clock::now ();
#endif

// execute _output_nodes
#ifndef DJNN_NO_DEBUG
    begin_output = std::chrono::steady_clock::now ();
#endif

    for (auto v : _output_nodes) {
        if (v->is_invalid ())
            continue;
        auto* p = v->get_process ();
        p->trigger_activation_flag ();
        p->set_activation_flag (NONE_ACTIVATION);
    }

#ifndef DJNN_NO_DEBUG
    end_output = std::chrono::steady_clock::now ();

    if (_DEBUG_SEE_ACTIVATION_SEQUENCE && display_sequence_on_target_location) {
        std::chrono::steady_clock::time_point end_act   = std::chrono::steady_clock::now ();
        int                                   time_exec = std::chrono::duration_cast<std::chrono::microseconds> (end_delete - begin_delete).count ();
        cerr << "\t_scheduled_deletions time = " << time_exec << "[us]" << endl;
        time_exec = std::chrono::duration_cast<std::chrono::microseconds> (end_output - begin_output).count ();
        cerr << "\t_output_nodes time = " << time_exec << "[us]" << endl;
        time_exec = std::chrono::duration_cast<std::chrono::microseconds> (end_act - begin_act).count ();
        cerr << "----\tGRAPH_EXEC # " << graph_counter_act << " time = " << time_exec << "[us]";
        if (time_exec > 1000) {
            cerr << "\033[1;31m";
            cerr << " - or " << time_exec / 1000.0 << "[ms]" << endl;
            cerr << "\033[0m";
        } else
            cerr << endl;
        double pourcent_graph = (count_real_activation / (double)count_activation) * 100;
        if (pourcent_graph < 50)
            cerr << "\033[1;31m";
        cerr << "nb action = " << count_real_activation << "/" << count_activation << "(" << pourcent_graph << "%)";
        cerr << "--- nb sorted_break " << _sorted_break << endl
             << endl
             << endl;
        cerr << "\033[0m";
    }

#if (!_EXEC_FULL_ORDERED_VERTICES && _DEBUG_GRAPH_INSERT_TIME)
    cerr << "\033[1;37m";
    if (nb_insert_by_graph_exec > 0) {
        cerr << "nb insert: " << nb_insert_by_graph_exec << " - av insert time = " << acc_insert_time_by_graph_exec / nb_insert_by_graph_exec << "[ns]";
        cerr << " - MAX insert time: " << max_insert_time_all_graph << "[nanos] for size: " << max_insert_for_this_size << endl;
    } else
        cerr << "NO insert" << endl;
    cerr << "\033[0m";
#endif

#if _DEBUG_SEE_GRAPH_INFO_PREF
    // print in GREEN
    cerr << "\033[1;32m" << endl;
    std::chrono::steady_clock::time_point end_GRAPH_EXEC = std::chrono::steady_clock::now ();
    int                                   time           = std::chrono::duration_cast<std::chrono::microseconds> (end_GRAPH_EXEC - begin_GRAPH_EXEC).count ();
    cerr << "GRAPH_EXEC = " << time << "[us]";
    if (time > 1000)
        cerr << " - or " << time / 1000.0 << "[ms]" << endl;
    else
        cerr << endl;
    graph_counter = graph_counter + 1;
    graph_total   = graph_total + time;
    graph_average = graph_total / graph_counter;
    cerr << "GRAPH_EXEC : " << graph_counter << " - avg: " << graph_average << "[us]" << endl;
    cerr << "GRAPH #vertices : " << _vertices.size () << endl;

    int all_edges = 0;
    for (auto v : _vertices) {
        all_edges += v->get_edges ().size ();
    }
    cerr << "GRAPH #edges: " << all_edges << endl;

    cerr << "\033[1;33m";
    cerr << "SORTED_GRAPH #vertices: " << _ordered_vertices.size () << endl;

    int sorted_edges = 0;
    for (auto v : _ordered_vertices) {
        sorted_edges += v->get_edges ().size ();
    }

    cerr << "SORTED_GRAPH #edges: " << sorted_edges << endl;
    cerr << "SORT_GRAPH : " << sorted_counter << " - avg: " << sorted_average << "[us]" << endl;
    cerr << "\033[0m" << endl;
#endif
#endif

    rmt_EndCPUSample ();
}

// -----------------------------------------------------------------------

void
Graph::sort (Vertex* v_root)
{
    if (_sorted)
        return;

    rmt_BeginCPUSample (Graph_sort, RMTSF_Aggregate);

#if _DEBUG_SEE_GRAPH_INFO_PREF
    std::chrono::steady_clock::time_point begin_GRAPH_SORT = std::chrono::steady_clock::now ();
#endif

    _cur_stamp = 0;
    _ordered_vertices.clear ();

    // if !_activation_triggers_to_sort is not empty
    // then this was triggered by an external source
    // otherwise this was triggered by an internal property
    if (!_activation_triggers_to_sort.empty ()) {
        for (auto v : _activation_triggers_to_sort) {
            if (v->get_execution_round () < EXECUTION_ROUND)
                traverse_depth_first (v);
        }
    } else
        traverse_depth_first (v_root);

    std::sort (_ordered_vertices.begin (), _ordered_vertices.end (),
               [] (const Vertex* v1, const Vertex* v2) { return v1->get_order_stamp () > v2->get_order_stamp (); });

#if !_EXEC_FULL_ORDERED_VERTICES
    // sorted_index
    int index = 0;
    for (auto v : _ordered_vertices)
        v->set_sorted_index (index++);

    std::sort (_activation_deque.begin (), _activation_deque.end (),
               [] (const Vertex* v1, const Vertex* v2) { return v1->get_sorted_index () < v2->get_sorted_index (); });
#endif

    _sorted = true;

    //print_sorted ();

#if _DEBUG_ENABLE_CHECK_ORDER
    if (!_pair_to_check.empty ()) {
        for (auto p : _pair_to_check) {
            print_order (p.first, p.second);
        }
    }
#endif

#if _DEBUG_SEE_GRAPH_INFO_PREF
    cerr << "\033[1;33m" << endl;
    std::chrono::steady_clock::time_point end_GRAPH_SORT = std::chrono::steady_clock::now ();
    int                                   time           = std::chrono::duration_cast<std::chrono::microseconds> (end_GRAPH_SORT - begin_GRAPH_SORT).count ();
    cerr << "SORT_GRAPH = " << time << "[us]";
    if (time > 1000)
        cerr << " - or " << time / 1000.0 << "[ms]" << endl;
    else
        cerr << endl;
    sorted_counter = sorted_counter + 1;
    sorted_total   = sorted_total + time;
    sorted_average = sorted_total / sorted_counter;
    cerr << "\033[0m" << endl;
#endif
    rmt_EndCPUSample (); // end  of sort
}

// -----------------------------------------------------------------------

/*
  Note on cycle detection:
    One would like to detect cycles during the Depth-First Search (DFS) traversal
    because the algorithm is rather simple. However, the DFS traverses a larger graph
    than the one that will actually be executed, resulting in the presence of
    multiple paths that, in reality, cannot be executed simultaneously
    (for example, a path resulting from the value of a boolean).
    The boolean cannot have both "true" and "false" values within a single execution loop,
    yet both paths will be present during the graph traversal because there can
    be a subscription to both true and false values.
*/
void
Graph::traverse_depth_first (Vertex* v)
{
    // skip invalid vertex
    // FIXME: we should not use this code anymore - check with coverage result
    if (v->is_invalid ()) {
        warning (v->get_process (), "useful code after all!!! Graph::traverse_depth_first - _vertex is invalid - CHECK THE ORDER OF DELETE\n");
        // v->set_execution_round (EXECUTION_ROUND);
        // return;
    }

    if ((v->get_process ()->get_process_type () == PROPERTY_T)) {
        // don't put a property in the _vertices since there is no action in properties
    } else {
        _ordered_vertices.push_back (v); // put anything else
    }

    v->set_execution_round (EXECUTION_ROUND);

    if (_DEBUG_ENABLE_STRESS_TEST == 0) {
        for (auto* v2 : v->get_edges ()) {
            if (v2->get_execution_round () < EXECUTION_ROUND) {
                traverse_depth_first (v2);
            }
        }
    }
    else if (_DEBUG_ENABLE_STRESS_TEST == 1) {
        auto __shuffle_edges = v->get_edges ();
        std::random_device rd;
        std::mt19937 g(rd());
    
        // debug
        // std::cerr << "shuffle " << __shuffle_edges.size () << " edges" << std::endl;
        
        std::shuffle(__shuffle_edges.begin(), __shuffle_edges.end(), g);
        for (auto* v2 : __shuffle_edges) {
            if (v2->get_execution_round () < EXECUTION_ROUND) {
                traverse_depth_first (v2);
            }
        }
    }
    else if (_DEBUG_ENABLE_STRESS_TEST == 2) {
        auto __reverse_edges = v->get_edges ();
    
        // debug
        //std::cerr << "reverse " << __reverse_edges.size () << " edges" << std::endl;

        std::reverse(__reverse_edges.begin(), __reverse_edges.end());
        for (auto* v2 : __reverse_edges) {
            if (v2->get_execution_round () < EXECUTION_ROUND) {
                traverse_depth_first (v2);
            }
        }
    }

    v->set_order_stamp (++_cur_stamp);
}

// -----------------------------------------------------------------------
// Vertex and Graph debug

#ifndef DJNN_NO_DEBUG

void
Graph::reset_vertices_execution_round ()
{
    for (auto* v : _vertices) {
        v->set_execution_round (0);
    }
}

static string
extract_filename (string s)
{
    std::string::size_type n;
    n                = s.rfind ('/');
    std::string sub1 = s.substr (n + 1);
    // std::cout << sub1 << '\n';
    return sub1;
}

static string
extract_code_from_file (string filepath, int lineno)
{
    std::ifstream file (filepath); // ouverture du fichier
    string        line;
    int           current_line = 0;
    while (getline (file, line)) {
        current_line++;
        if (current_line == lineno) {
            line.erase (0, line.find_first_not_of (" \t"));
            return line;
        }
    }
    // si on n'a pas trouvé la ligne demandée
    return " line not found in file ! ";
}

static string
print_process_full_name (CoreProcess* p)
{
    string          postfix;
    CoreAssignment* ca = dynamic_cast<CoreAssignment*> (p);
    if (ca != nullptr) {
        postfix = " src: " + ca->get_src ()->get_debug_name () + " dst: " + ca->get_dst ()->get_debug_name ();
    }
    return cpp_demangle (typeid (*p).name ()) + "- (" +
           (p && p->get_debug_parent () ? p->get_debug_parent ()->get_debug_name () + "/" : "") +
           (p ? p->get_debug_name () : "") + ")" + postfix;
}

static string
print_process_fileno (CoreProcess* p)
{
    return extract_filename (p->debug_info ().filepath) + ":" + to_string (p->debug_info ().lineno);
}

static string
print_process_debug_info (CoreProcess* p)
{
    return " ---- from " + print_process_fileno (p) + " ---- " + extract_code_from_file (p->debug_info ().filepath, p->debug_info ().lineno);
}

void
Vertex::print_vertex () const
{
    auto* pp = _process;
    cerr << "vertex (" << print_process_full_name (pp) << " - [" << _count_edges_in << ", " << _edges.size () << "] :\t";

    if (_edges.size () == 0)
        cerr << "EMPTY" << endl;
    else {
        for (auto e : _edges) {
            auto  result = _map_edges.find (e);
            auto* ppe    = e->_process;
            if (ppe) {
                cerr << print_process_full_name (ppe) << ppe->get_debug_name () << " [x" << result->second << "] \t";
            }
        }
        cerr << endl;
    }
}

void
Vertex::print_full_vertex ()
{
    cerr << "\033[1;31m";
    cerr << "Execution Graph - details on vertex: " << endl;

    auto* pp = _process;
    cerr << "vertex (" << print_process_full_name (pp) << " - [" << _count_edges_in << ", " << _edges.size () << "] :\n";

    cerr << "edge in " << _count_edges_in << ":\n";

    if (_count_edges_in == 0)
        cerr << "\tEMPTY" << endl;
    else {
        int i = 0;
        for (auto v : Graph::instance ().vertices ()) {
            map<Vertex*, int> m  = v->get_map_edges ();
            auto              it = m.find (this);
            if (it != m.end ()) {
                if (v->is_invalid () == 0) {
                    auto* ppv = v->_process;
                    if (ppv) {
                        cerr << "\t" << ++i << " - " << print_process_full_name (ppv) << ppv->get_debug_name () << " [x" << it->second << "]\n";
                    }
                } else {
                    cerr << "\t" << ++i << " - THIS VERTEX HAS BEEN INVALIDATED -- associated process has been deleted\n";
                }
            }
        }
        cerr << endl;
    }

    cerr << "edge out " << _edges.size () << ":\n";

    if (_edges.size () == 0)
        cerr << "\tEMPTY" << endl;
    else {
        int i = 0;
        for (auto e : _edges) {
            if (e->is_invalid () == 0) {
                auto  result = _map_edges.find (e);
                auto* ppe    = e->_process;
                if (ppe)
                    cerr << "\t" << ++i << " - " << print_process_full_name (ppe) << ppe->get_debug_name () << " [x" << result->second << "]\n";
            } else {
                cerr << "\t" << ++i << " - THIS VERTEX HAS BEEN INVALIDATED -- associated process has been deleted\n";
            }
        }
        cerr << endl;
    }
    cerr << "\033[0m";
}

void
Graph::print_graph () const
{
    cerr << " --- GRAPH --- " << endl;
    for (auto v : _vertices) {
        v->print_vertex ();
    }
    cerr << " --- END GRAPH --- " << endl
         << endl;
}

void
Graph::print_sorted () const
{
    cerr << " --- SORTED GRAPH --- " << endl;
    for (auto v : _ordered_vertices) {
        auto* pp = v->get_process ();
        cerr << "index: " << v->get_sorted_index () << " - order_stamp: " << v->get_order_stamp () << " - ";
        cerr << print_process_full_name (pp) << print_process_debug_info (pp) << endl;
    }
    cerr << " --- END SORTED GRAPH --- " << endl
         << endl;
}

void
Graph::print_activation () const
{
    cerr << " --- SORTED ACTIVATION --- " << endl;
    for (auto v : _activation_deque) {
        auto* pp = v->get_process ();
        cerr << "index: " << v->get_sorted_index () << " - order_stamp: " << v->get_order_stamp () << " - p: " << pp << " - ";
        cerr << print_process_full_name (pp) << endl;
    }
    cerr << " --- END ACTIVATION --- " << endl
         << endl;
}

void
Graph::print_order (CoreProcess* p1, CoreProcess* p2)
{
    int i_p1 = -1;
    int i_p2 = -1;
    for (size_t i = 0; i < _ordered_vertices.size (); i++) {
        if (_ordered_vertices.at (i)->get_process () == p1)
            i_p1 = i;
        if (_ordered_vertices.at (i)->get_process () == p2)
            i_p2 = i;
    }
    string p1_name = (p1 && p1->get_parent ()) ? p1->get_name (p1->get_parent ()) : "";
    string p2_name = (p2 && p2->get_parent ()) ? p2->get_name (p2->get_parent ()) : "";
    if (i_p1 == -1)
        cout << "p1 " << p1_name << " not found\n";
    if (i_p2 == -1)
        cout << "p2 " << p2_name << " not found\n";

    if (i_p1 >= 0 && i_p2 >= 0)
        cout << "p1 "
             << p1_name
             << (i_p1 < i_p2 ? " before " : " after ") << " p2 "
             << p2_name << endl;
}

pair<Vertex*, int>
find_pair_from_value_in_map (map<Vertex*, int>& vertex_already_activated, int value)
{
    for (auto pair : vertex_already_activated) {
        if (pair.second == value)
            return pair;
    }
    return pair<Vertex*, int> (nullptr, -1);
}

void
display_cycle_analysis_stack (map<Vertex*, int>& vertex_already_activated, int count_activation, Vertex* v)
{

    cerr << endl
         << "----- CYCLE ANALYSIS - reversed activation stack ---- " << endl;
    cerr << "\033[1;36m" << count_activation << " --- index: " << v->get_sorted_index () << " -- " << print_process_full_name (v->get_process ()) << print_process_debug_info (v->get_process ()) << "\033[1;31m" << endl;

    pair<Vertex*, int> pair;
    // while we don't find the beginning of the cycle ... which is the vertex already activated
    do {
        pair = find_pair_from_value_in_map (vertex_already_activated, --count_activation);
        if (pair.first) {
            if (pair.first == v)
                cerr << "\033[1;36m";
            cerr << pair.second << " --- index: " << pair.first->get_sorted_index () << " -- " << print_process_full_name (pair.first->get_process ()) << print_process_debug_info (pair.first->get_process ()) << endl;
            if (pair.first == v)
                cerr << "\033[1;31m";
        }
    } while (pair.first && pair.first != v);
    // One last time to know who is the caller the starting cycle
    pair = find_pair_from_value_in_map (vertex_already_activated, --count_activation);
    if (pair.first)
        cerr << pair.second << " --- index: " << pair.first->get_sorted_index () << " -- " << print_process_full_name (pair.first->get_process ()) << endl;

    cerr << "--------------------------------------------------- " << endl;
}
#endif

} // namespace djnn
