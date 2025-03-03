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

#include "core/utils/error.h"
#include "core/utils/remotery.h"
#include "core/utils/utils-dev.h"
#include "exec_env/exec_env-dev.h"
#include "graph.h"
#include "utils/debug.h"

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

void
Graph::add_in_activation (Vertex* v)
{

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

}

void
Graph::clear_activation ()
{
    _activation_deque.clear ();
}

// -----------------------------------------------------------------------
// Graph behavior: exec, sort, traverse_depth_first

// TODO: explain what EXECUTION_ROUND is
static int EXECUTION_ROUND = 0;

void
Graph::exec ()
{

    // remotery start
    rmt_BeginCPUSample (Graph_exec, RMTSF_None);

    // pre_execution : notify_activation *only once* per _scheduled_activations before real graph execution
    {

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

            }
        }

        _scheduled_activations.clear ();

    }

    _sorted     = false;
    bool is_end = false;

    while (!is_end) {
        is_end = true;

        Vertex* v = nullptr;
        while (!_activation_deque.empty ()) {
            v = _activation_deque.front ();

            if (!_sorted) {

                break;
            }
            // pop only if sorted otherwise the process activation will be skipped
            _activation_deque.pop_front ();

            if (v->is_invalid ())
                continue;
            auto* p = v->get_process ();

            p->trigger_activation_flag ();
            p->set_activation_flag (NONE_ACTIVATION);

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

    for (auto p : _scheduled_deletions)
        delete p;
    _scheduled_deletions.clear ();

// execute _output_nodes

    for (auto v : _output_nodes) {
        if (v->is_invalid ())
            continue;
        auto* p = v->get_process ();
        p->trigger_activation_flag ();
        p->set_activation_flag (NONE_ACTIVATION);
    }

    rmt_EndCPUSample ();
}

// -----------------------------------------------------------------------

void
Graph::sort (Vertex* v_root)
{
    if (_sorted)
        return;

    rmt_BeginCPUSample (Graph_sort, RMTSF_Aggregate);

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

    _sorted = true;

    //print_sorted ();

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

    v->set_order_stamp (++_cur_stamp);
}

// -----------------------------------------------------------------------
// Vertex and Graph debug

} // namespace djnn
