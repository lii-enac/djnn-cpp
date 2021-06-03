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
 *
 */

#include "graph.h"

#include "core/utils/utils-dev.h"
#include "core/utils/error.h"
#include "core/utils/ext/remotery/Remotery.h"
#include "utils/debug.h"
#include "exec_env/exec_env-dev.h"

#include <algorithm>

#if !defined(DJNN_NO_DEBUG) || !defined(DJNN_NO_SERIALIZE)
#include <iostream>
#endif

#ifndef DJNN_NO_DEBUG

static int graph_counter_act = 0;

#if _DEBUG_GRAPH_INSERT_TIME
static int nb_insert_by_graph_exec = 0;
static int acc_insert_time_by_graph_exec = 0;
static int max_insert_time_all_graph = 0;
static int max_insert_for_this_size = 0;
#endif 

#if _DEBUG_SEE_GRAPH_INFO_PREF
//#include "core/utils/utils-dev.h"
static int graph_counter = 0;
static double graph_total = 0.0;
static double graph_average = 0.0;
static int sorted_counter = 0;
static double sorted_total = 0.0;
static double sorted_average = 0.0;

//#include <chrono>
#endif
#endif

namespace djnn
{
#ifndef DJNN_NO_DEBUG
  using std::cout;
  using std::cerr;
  using std::endl;
#endif

  enum mark_e {
    NOT_MARKED,
    BROWSING,
    MARKED
  };

#ifndef DJNN_NO_DEBUG
  static std::string 
  print_process_full_name (CoreProcess *p) {
    return cpp_demangle(typeid(*p).name()) + "- (" + 
    ( p && p->get_debug_parent () ? p->get_debug_parent ()->get_debug_name () + "/" : "") +
    ( p ? p->get_debug_name () : "") + ")";
  }
#endif

  Vertex::Vertex (CoreProcess* p) :
      _process (p), _mark (NOT_MARKED), _timestamp (0), _count_edges_in(0), _is_invalid (false), _sorted_index (-1)
  {
    //std::cerr << __PRETTY_FUNCTION__ << " vertex:" << this << " process:" << p << " " << (p->get_parent() ? p->get_parent()->get_name () + "/"  : "") << p->get_name() << std::endl; 
  }

  Vertex::~Vertex () {
  }

  void
  Vertex::add_edge (Vertex *dst) 
  { 
    
    /* 
       NOTE : We SHOULD avoid duplicate edges in this vector.
       here a vector is used because the insert order is importante and used in sorting (traverse_depth_first)
       we can't use std::set, std::unordered_set or std::map because it break this order.
       BUT the find on stc::vector O(n) is more complex than std::set O(log(n)) or std::unordered_set O(1)

       We add another _map_edges to manage duplicate info !
       FIXME: maybe adapte the sort algorithm to use set or better unordered_set OR maybe replace current vector by _map_edges
    */

    auto result = _map_edges.find(dst);
 
    /* if is a NEW edge */
    if (result == _map_edges.end()) {
      _edges.push_back (dst);
      dst->_count_edges_in++;
      _map_edges[dst] = 1;

      // print debug
      // cerr << "add_edge : " << "\t between " << 
      // ( this->_process->get_parent () ? this->_process->get_parent ()->get_name () + "/" : "") <<
      // this->_process->get_name () << " - " <<
      // ( dst->_process->get_parent () ? dst->_process->get_parent ()->get_name () + "/" : "") <<
      // dst->_process->get_name () << endl;
    }
    /* it a duplicate */
    else
      result->second = ++result->second ;
    
    
  }

  void
  Vertex::remove_edge (Vertex *dst)
  {
    auto result = _map_edges.find(dst);

    /* remove duplicate */
    if (result != _map_edges.end () && result->second > 1) {
      result->second = --result->second;
    } else {
      
      /* NOTE: 
         for now we keep this complicated way to erase instead of 
         _edges.erase(std::remove (_edges.begin (), _edges.end (), dst), _edges.end ());
         to manage wrong removing, as in unit-test
      */

      /* or remove dst from _edges vector  */
      Vertex::vertices_t::iterator newend = _edges.end ();
      newend = std::remove (_edges.begin (), _edges.end (), dst);

      /* check if end has changed and erase */
      if (newend != _edges.end ()) {
        /* erase them from _edges */
        _edges.erase(newend, _edges.end ());
        _map_edges.erase(dst); 
        --dst->_count_edges_in;
      }
    }
    // print debug
    // cerr << "remove_edge : " << "\t between " << 
    // ( this->_process->get_parent () ? this->_process->get_parent ()->get_name () + "/" : "") <<
    // this->_process->get_name () << " - " <<
    // ( dst->_process->get_parent () ? dst->_process->get_parent ()->get_name () + "/" : "") <<
    // dst->_process->get_name () << " _count_edges_in: " << dst->_count_edges_in << endl;
  }

  void
  Vertex::print_vertex () const
  {
#ifndef DJNN_NO_DEBUG
    auto * pp = _process;
    std::cerr << "vertex (" << print_process_full_name (pp) << " - [" << 
    _count_edges_in << ", " << _edges.size () << "] :\t";

    if( _edges.size () == 0)
      std::cerr << "EMPTY" << endl;
    else {
      for (auto e : _edges) {
         auto result = _map_edges.find(e);
         auto * ppe = e->_process;
         if (ppe) {
          std::cerr << print_process_full_name (ppe) << ppe->get_debug_name () << " [x" << result->second << "] \t" ;
         }
      }
      std::cerr << std::endl;
    }
#endif
  }

  Graph * Graph::_instance;

  Graph&
  Graph::instance ()
  {
    //static std::atomic_flag onceFlag = ATOMIC_FLAG_INIT;
    //if(!onceFlag.test_and_set()) {
    if (!_instance) {
      _instance = new Graph();
    }

    return *(_instance);
    //return _instance;
  }

  Graph::Graph () :
      _cur_date (0), _sorted (false)
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
    /* nothing to delete because vertices are own by _vertices. */
    _ordered_vertices.clear ();

    /* nothing to delete because vertices are own by _vertices.  */
    _activation_deque.clear ();

    /* delete vertices from _vertices and clear.*/
    for (std::list< Vertex* >::iterator it = _vertices.begin (); it != _vertices.end (); ++it)
        delete *it;
    _vertices.clear ();

    /* delete output_vertices from _outpur_nodes and clear */
    for (Vertex::vertices_t::iterator it = _output_nodes.begin (); it != _output_nodes.end (); ++it)
        delete *it;
    _output_nodes.clear ();
  }

  Vertex*
  Graph::add_vertex (CoreProcess* c)
  {
    Vertex* v = new Vertex (c);
    _vertices.push_back (v);
    v->set_position_in_vertices (_vertices.end ());
    _sorted = false;
    return v;
  }

  void 
  Graph::add_in_activation (Vertex *v)
  {

#if _DEBUG_GRAPH_INSERT_TIME
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
#endif 

    /*
      note : if the graph has not been sorted yet.
      we do not know the order so we just push_back and the
      _new_activ will be reordered during sorted process.
    */
    if (_sorted) {
      if (_activation_deque.empty ())
        _activation_deque.push_front (v);
      else {
        std::deque <Vertex *>::iterator it = _activation_deque.begin () ;
        bool is_inserted = false;

        while ( !is_inserted && it != _activation_deque.end () ) {
          // do not insert duplicate
          if ((*it)->get_sorted_index () == v->get_sorted_index ()) return ; 
          if ((*it)->get_sorted_index () > v->get_sorted_index ()) {
            _activation_deque.insert (it, v);
            is_inserted = true;
          }
          it++;
        }
        //if not inserted : it is the last one
        if (!is_inserted)
          _activation_deque.push_back (v);
      }
    }
    else {
      // but should avoid duplicate
      for (auto vv: _activation_deque)
        if (vv == v) return ;

      // new we add it 
      _activation_deque.push_front (v);
    }

#if _DEBUG_GRAPH_INSERT_TIME
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  nb_insert_by_graph_exec++ ;
  int time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
  acc_insert_time_by_graph_exec = acc_insert_time_by_graph_exec + time;
  if (time > max_insert_time_all_graph) {
    max_insert_time_all_graph = time;
    max_insert_for_this_size = _activation_deque.size ();
  }
#endif     
  }

  void 
  Graph::clear_activation ()
  {
    _activation_deque.clear ();
  } 

  void
  Graph::add_output_node (CoreProcess* c)
  {
    /* check if c is already in the graph */
    for (auto v : _output_nodes) {
      if (v->get_process () == c)
        return;
    }
    Vertex *v = new Vertex (c);
    _output_nodes.push_back (v);
  }

  void
  Graph::remove_output_node (CoreProcess* c)
  {
    //remove_if 
    Vertex::vertices_t::iterator new_end = _output_nodes.end ();

    new_end = std::remove_if (_output_nodes.begin (), _output_nodes.end (),
      [c](Vertex::vertices_t::iterator::value_type v) {return v->get_process () == c;});

    if (new_end != _output_nodes.end ()) {
      // delete nodes
      for (Vertex::vertices_t::iterator it = new_end ; it != _output_nodes.end(); ++it)
        delete *it;

      //erase from vector
      _output_nodes.erase( new_end, _output_nodes.end ());
    }
  }

  void
  Graph::add_edge (CoreProcess* src, CoreProcess* dst)
  {

    // std::cerr << "add_edge: " <<
    //   ( src->get_parent () ? src->get_parent ()->get_name () + "/" : "" ) << src->get_name () << " - " << 
    //   ( dst->get_parent () ? dst->get_parent ()->get_name () + "/" : "" ) << dst->get_name () << endl;

    Vertex *vs = src->vertex ();
    if (vs == nullptr) {
      vs = add_vertex (src);
      src->set_vertex (vs);
    }

    Vertex *vd = dst->vertex ();
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

    //std::cerr << "remove_edge: " <<
    //( p_src->get_parent () ? p_src->get_debug_parent ()->get_debug_name () + "/" : "" ) << p_src->get_debug_name () << " - " << 
    //( p_dst->get_parent () ? p_dst->get_debug_parent ()->get_debug_name () + "/" : "" ) << p_dst->get_debug_name () << endl;

    Vertex *vs = p_src->vertex ();
    Vertex *vd = p_dst->vertex ();

    /* note: 
       this code is to prevent bugs 
       this should NEVER happen
       vertex should NOT be nullptr at this place
       if not, something (FatProcess or edge dependency) IS NOT well deleted
    */
    if (vs == nullptr || vd == nullptr) {

      warning ( nullptr,  " Graph::remove_edge - - vertex vs or vd is NULL and it SHOULD NOT HAPPEN (except in unit test) \n");

#ifndef DJNN_NO_DEBUG
      auto * ppsrc = p_src;
      auto * ppdst = p_dst;
      std::cerr << "Graph remove_edge: " << cpp_demangle(typeid(*p_src).name()) + ":" + 
      (ppsrc ? get_hierarchy_name (ppsrc) : "") << "  " << vs << " - " << cpp_demangle(typeid(*p_dst).name()) + ":" +
      (ppdst ? get_hierarchy_name (ppdst) : "") << "  " << vd << endl;
#endif
      //assert(0);
      return;
    }

    vs->remove_edge (vd);

    /* 
      note :
      delete vertex if they have no more out_edges and in_edges
     */

    // 1 - remove src if necessary
    if ( vs->get_edges ().empty () && (vs->get_count_edges_in () == 0)) {
      _vertices.erase(vs->get_position_in_vertices ());
      p_src->set_vertex (nullptr);
      delete vs; 
    }

    // 2 - remove dst if necessary
    if (vd->get_edges ().empty () && (vd->get_count_edges_in () == 0)){
      _vertices.erase(vd->get_position_in_vertices ());
      p_dst->set_vertex (nullptr);
      delete vd;
    }

    _sorted = false;
  }

  void
  Graph::print_graph () const
  {
#ifndef DJNN_NO_DEBUG
    std::cerr << " --- GRAPH --- " << endl ;
    for (auto v : _vertices) {
      v->print_vertex ();
    }
    std::cerr << " --- END GRAPH --- " << endl << endl;
#endif
  }

  void
  Graph::print_sorted () const
  {
#ifndef DJNN_NO_DEBUG
    std::cerr << " --- SORTED GRAPH --- " << endl ;
    for (auto v : _ordered_vertices) {
      auto * pp = v->get_process ();
      cerr << "index: " << v->get_sorted_index () << " - timestamp: " << v->get_timestamp () << " - ";
      cerr << print_process_full_name (pp) << endl;
    }
    std::cerr << " --- END SORTED GRAPH --- " << endl << endl;
#endif
  }

  void
  Graph::print_activation () const
  {
#ifndef DJNN_NO_DEBUG
    std::cerr << " --- SORTED ACTIVATION --- " << endl ;
    for (auto v : _activation_deque) {
      auto * pp = v->get_process ();
      cerr << "index: " << v->get_sorted_index () << " - timestamp: " << v->get_timestamp () << " - p: " << pp << " - ";
      cerr << print_process_full_name (pp) << endl;
    }
    std::cerr << " --- END ACTIVATION --- " << endl << endl;
#endif
  }

  void
  Graph::traverse_depth_first (Vertex *v)
  {

    // skip invalid vertex
    //FIXME: we should not use this code anymore - check with coverage result
    if (v->is_invalid ()) {
      //assert (0);
      warning ( nullptr, " Graph::traverse_depth_first - _vertex is invalid - CHECK THE ORDER OF DELETE\n");
#ifndef DJNN_NO_DEBUG
      cerr << "vertex: " << v << "- v->process " << v->get_process ();
#endif
      v->set_mark (MARKED);
      return;
    }

    if (false
#ifndef DJNN_NO_OPTIM_NO_PROPERTIES_IN_PROCESS_VECTOR
    // add vertex if it's not a property: its timestamp is taken into account anyway
    || (v->get_process ()->get_process_type() != PROPERTY_T)
#endif
// #ifndef DJNN_NO_OPTIM_NO_SINGLE_DST_IN_PROCESS_VECTOR
//     || (v->get_count_edges_in () > 1)
// #endif
    )
      _ordered_vertices.push_back (v);

    v->set_mark (BROWSING);

    for (auto& v2 : v->get_edges ()) {
      if (v2->get_mark () == NOT_MARKED
      ) {
        traverse_depth_first (v2);
      }
    }
    
    v->set_timestamp (++_cur_date);
    v->set_mark (MARKED); 
  }

  bool
  cmp_vertices (const Vertex* v1, const Vertex *v2)
  {
    return v2->get_timestamp () < v1->get_timestamp ();
  }

  bool
  cmp_index (const Vertex* v1, const Vertex *v2) {
    return v1 ->get_sorted_index () < v2->get_sorted_index ();
  }

  void
  Graph::print_order (CoreProcess *p1, CoreProcess *p2)
  {
    int i_p1 = -1;
    int i_p2 = -1;
    for (int i = 0; i < _ordered_vertices.size (); i++)
    {
      if (_ordered_vertices.at (i)->get_process () == p1) i_p1 = i;
      if (_ordered_vertices.at (i)->get_process () == p2) i_p2 = i;
    }
    std::string p1_name = (p1 && p1->get_parent()) ? p1->get_name (p1->get_parent ()) : "";
    std::string p2_name = (p2 && p2->get_parent()) ? p2->get_name (p2->get_parent ()) : "";
    if (i_p1 == -1) std::cout << "p1 " << p1_name << " not found\n";
    if (i_p2 == -1) std::cout << "p2 " << p2_name << " not found\n";

    if (i_p1 >= 0 && i_p2 >= 0) std::cout << "p1 "
        << p1_name
        << (i_p1 < i_p2 ? " before " : " after ") << " p2 "
        << p2_name << std::endl;
  }

  void
  Graph::sort ()
  {
    if (_sorted)
      return;
//rmt_BeginCPUSample(Graph_sort, RMTSF_Recursive);
    #if _DEBUG_SEE_GRAPH_INFO_PREF
    std::chrono::steady_clock::time_point begin_GRAPH_SORT = std::chrono::steady_clock::now();
    #endif
    _cur_date = 0;
    _ordered_vertices.clear ();
    //warning(nullptr, std::string("num vertices: ")+__to_string(_vertices.size()));

    // set every vertex as NOT_MARKED before sorting them
    for (auto v : _vertices) {
      v->set_mark (NOT_MARKED);
    }
    // traverse
    for (auto v : _vertices) {
      if (v->get_mark () == NOT_MARKED)
        traverse_depth_first (v);
    }

    // sort
    std::sort (_ordered_vertices.begin (), _ordered_vertices.end (), cmp_vertices);

#if !_EXEC_FULL_ORDERED_VERTICES
    //give sorted_index
    int index = 0 ;
    for (auto v : _ordered_vertices) {
      v->set_sorted_index (index++);
    }

    std::sort (_activation_deque.begin (), _activation_deque.end (), cmp_index);
#endif

    _sorted = true;

#if _DEBUG_ENABLE_CHECK_ORDER
    if (!_pair_to_check.empty ()) {
      for (auto p: _pair_to_check) {
        print_order (p.first, p.second);
      }
    }
#endif


    #if _DEBUG_SEE_GRAPH_INFO_PREF
    cerr << "\033[1;33m" << endl;
    std::chrono::steady_clock::time_point end_GRAPH_SORT = std::chrono::steady_clock::now();
    int time = std::chrono::duration_cast<std::chrono::microseconds>(end_GRAPH_SORT - begin_GRAPH_SORT).count();
    std::cerr << "SORT_GRAPH = " << time << "[us]" ;
    if (time > 1000 )
        std::cerr << " - or " << time / 1000.0 <<  "[ms]" << std::endl;
      else
        std::cerr << std::endl;
    sorted_counter = sorted_counter + 1;
    sorted_total = sorted_total + time ;
    sorted_average = sorted_total / sorted_counter;
    cerr << "\033[0m"  << endl;
    #endif
//rmt_EndCPUSample();
  }

  void
  Graph::schedule_activation (CoreProcess *p)
  {
    _scheduled_activation_processes.push_back(p);
  }

#ifndef DJNN_NO_DEBUG

  std::pair<Vertex*, int>
  find_pair_from_value_in_map (std::map<Vertex*, int> &vertex_already_activated, int value){
    for (auto pair : vertex_already_activated){
      if (pair.second == value)
        return pair;
    }
    return std::pair<Vertex*, int> (nullptr, -1);
  }
  

  void 
  display_cycle_analysis_stack (std::map<Vertex*, int> &vertex_already_activated, int count_activation, Vertex* v){

    cerr << "----- CYCLE ANALYSIS - revers activation stack ---- " << endl;
    cerr << count_activation << " --- " << print_process_full_name (v->get_process ()) << endl;
    
    std::pair<Vertex*, int> pair ;
    // while we don't find the beginning of the cycle ... which is the vertex already activated
    do {
      pair = find_pair_from_value_in_map (vertex_already_activated, --count_activation);
      if (pair.first)
        cerr << pair.second << " --- " << print_process_full_name (pair.first->get_process ()) << endl;    
    } while (pair.first && pair.first != v);

    cerr << "--------------------------------------------------- " << endl;

  }

#endif

  void
  Graph::exec ()
  {
#if _DEBUG_GRAPH_INSERT_TIME
    nb_insert_by_graph_exec = 0;
    acc_insert_time_by_graph_exec = 0;
#endif

    #if _DEBUG_SEE_GRAPH_INFO_PREF
    std::chrono::steady_clock::time_point begin_GRAPH_EXEC = std::chrono::steady_clock::now();
    #endif
rmt_BeginCPUSample(Graph_exec, 0);
    //pre_execution : notify_activation *only once* per _scheduled_activation_processes before real graph execution 
    // notify_activation of event : mouse, touch, etc... which do not have vertex
    {
      std::map<CoreProcess*, int> already_done;
      for (auto p : _scheduled_activation_processes) {
        if (already_done.find(p) == already_done.end()) {
          p->notify_activation();
          already_done[p];
          if (_DEBUG_SEE_ACTIVATION_SEQUENCE)
            std::cerr << "Scheduled event notifying ------ " << print_process_full_name(p) << std::endl;
        }
        // DEBUG DUPLICATES
        //else
        //  cerr << "DUPLICATES " << (p->get_parent () ? p->get_parent ()->get_name () : "NUUULL") << "/" << p->get_name () << endl;
      }
      _scheduled_activation_processes.clear ();
    }

    int count_activation = 0;

#ifndef DJNN_NO_DEBUG
    int count_real_activation = 0 ;
    graph_counter_act++;
    int _sorted_break = 0;
    std::chrono::steady_clock::time_point begin_act, begin_process_act, end_process_act;
    if (_DEBUG_SEE_ACTIVATION_SEQUENCE) {
      begin_act = std::chrono::steady_clock::now();
      std::cerr << std::endl;
    }
#endif

#if _EXEC_FULL_ORDERED_VERTICES
    bool is_end = false;
    while (!is_end) {
      is_end = true;

      for (auto v : _ordered_vertices) {

        if (!_sorted) {
          if (_DEBUG_SEE_ACTIVATION_SEQUENCE) {
            _sorted_break++;
            std::cerr << "\033[1;33m" << "--- break to sort #" << _sorted_break << "\033[0m" << endl;
          }
          break;
        }
        if (v->is_invalid()) continue;
        auto* p = v->get_process();

        if (_DEBUG_SEE_ACTIVATION_SEQUENCE) {
          count_activation++;
          if (p->get_activation_flag() != NONE_ACTIVATION) {
            begin_process_act = std::chrono::steady_clock::now();
            count_real_activation++;
          }
        }

        p->trigger_activation_flag();
        p->set_activation_flag(NONE_ACTIVATION);

        if (_DEBUG_SEE_ACTIVATION_SEQUENCE) {
          end_process_act = std::chrono::steady_clock::now();
          int _process_time = std::chrono::duration_cast<std::chrono::microseconds>(end_process_act - begin_process_act).count();
          if (_process_time > _DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_TIME_US)
            cerr << "\033[1;36m";
          if (_process_time > _DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_TIME_US || !_DEBUG_SEE_ACTIVATION_SEQUENCE_ONLY_TARGETED)
            std::cerr << count_real_activation << " ------ " << print_process_full_name(p) << "---- process time act/deact = " << _process_time << "[us]" << std::endl;
          cerr << "\033[0m";
        }
      }

      if (!_sorted) {
        sort ();
        is_end = false;
      }
    }

#else //between OLD and NEW execution

    bool is_end = false;
    while (!is_end) {
      is_end = true;

#ifndef DJNN_NO_DEBUG
      std::map<Vertex*, int> _vertex_already_activated;
#endif

      while (!_activation_deque.empty ()) {
        auto * v = _activation_deque.front ();
      
        if (!_sorted) {
          if (_DEBUG_SEE_ACTIVATION_SEQUENCE) {
            _sorted_break++;
            std::cerr << "\033[1;33m" << "--- break to sort #" << _sorted_break << "\033[0m" << endl;
          }
          break;
        }
        // pop only if sorted else .. the process activation willl be skip
        _activation_deque.pop_front();

        if (v->is_invalid())
          continue;
        auto* p = v->get_process();
        count_activation++;


#ifndef DJNN_NO_DEBUG
        if (_DEBUG_GRAPH_CYCLE_DETECT) {
          if (_vertex_already_activated.find(v) != _vertex_already_activated.end()) {
            cerr << "\033[1;31m";
            cerr << "djnn Warning - \tWe detected a cycle in GRAPH Execution" << endl;
            cerr << "\t\t" << print_process_full_name(v->get_process()) << " have already been activated in this cycle \n";
            display_cycle_analysis_stack(_vertex_already_activated, count_activation, v);
            cerr << "\033[0m";
            break;
          }
          else
            _vertex_already_activated[v] = count_activation;
        }
        else {
          // default guard against cycles if _DEBUG_GRAPH_CYCLE_DETECT = 0
          if (count_activation > _vertices.size()) {
            cerr << "\033[1;31m";
            cerr << "djnn Warning - \tWe detected more activations in only one graph execution than vertices in the graph !" << endl;
            cerr << "\t\tIt seems there is a cycle in your program !! \n\t\tWe stopped it and break the GRAPH::EXEC" << endl;
            cerr << "\t\tPlease activate debug option _DEBUG_GRAPH_CYCLE_DETECT to have full information about this detected cycle." << endl;
            cerr << "\033[0m";
            continue;
          }
        }

        if (_DEBUG_SEE_ACTIVATION_SEQUENCE) {
          if (p->get_activation_flag() != NONE_ACTIVATION) { // should be removed ??
            begin_process_act = std::chrono::steady_clock::now();
            count_real_activation++;
          }
        }
#endif

        p->trigger_activation_flag();
        p->set_activation_flag(NONE_ACTIVATION);

#ifndef DJNN_NO_DEBUG
        if (_DEBUG_SEE_ACTIVATION_SEQUENCE) {
          end_process_act = std::chrono::steady_clock::now();
          int _process_time = std::chrono::duration_cast<std::chrono::microseconds>(end_process_act - begin_process_act).count();
          if (_process_time > _DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_TIME_US)
            cerr << "\033[1;36m";
          if (_process_time > _DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_TIME_US || !_DEBUG_SEE_ACTIVATION_SEQUENCE_ONLY_TARGETED)
            std::cerr << count_real_activation << " ------ i: " << v->get_sorted_index() << " --- " << print_process_full_name(p) << "---- process time act/deact = " << _process_time << "[us]" << std::endl;
          cerr << "\033[0m";
        }
#endif
      }

      if (!_sorted) {
        sort();
        is_end = false;
      }
#ifndef DJNN_NO_DEBUG      
      if (_DEBUG_GRAPH_CYCLE_DETECT)
        _vertex_already_activated.clear ();
#endif
    }
    #endif

    // then execute delayed delete on processes 
    for (auto p : _scheduled_delete_processes) {
      delete p;
    }
    _scheduled_delete_processes.clear();

    // execute _output_nodes
    for (auto v : _output_nodes) {
      if (v->is_invalid()) continue;
      auto* p = v->get_process();
      p->trigger_activation_flag();
      p->set_activation_flag(NONE_ACTIVATION);
    }

    rmt_EndCPUSample();

    if (_DEBUG_SEE_ACTIVATION_SEQUENCE) {
      std::chrono::steady_clock::time_point end_act = std::chrono::steady_clock::now();
      int time_exec = std::chrono::duration_cast<std::chrono::microseconds>(end_act - begin_act).count();
      std::cerr << " ---- GRAPH_EXEC # " << graph_counter_act << " time = " << time_exec << "[us]";
      if (time_exec > 1000) {
        cerr << "\033[1;31m";
        std::cerr << " - or " << time_exec / 1000.0 << "[ms]" << std::endl;
        cerr << "\033[0m";
      }
      else
        std::cerr << std::endl;
      double pourcent_graph = (count_real_activation / (double)count_activation) * 100;
      if (pourcent_graph < 50)
        cerr << "\033[1;31m";
      std::cerr << "nb action = " << count_real_activation << "/" << count_activation << "(" << pourcent_graph << "%)";
      std::cerr << "--- nb sorted_break " << _sorted_break << endl << endl << endl;
      cerr << "\033[0m";
    }

#if (!_EXEC_FULL_ORDERED_VERTICES && _DEBUG_GRAPH_INSERT_TIME)
    cerr << "\033[1;37m";
    if (nb_insert_by_graph_exec > 0) {
      std::cerr << "nb insert: " << nb_insert_by_graph_exec << " - av insert time = " << acc_insert_time_by_graph_exec / nb_insert_by_graph_exec << "[us]";
      std::cerr << " - MAX insert time: " << max_insert_time_all_graph << "[nanos] for size: " << max_insert_for_this_size << std::endl;
    }
    else
      std::cerr << "NO insert" << std::endl;
    cerr << "\033[0m";
#endif 

#if _DEBUG_SEE_GRAPH_INFO_PREF
    // print in GREEN
    cerr << "\033[1;32m" << endl;
    std::chrono::steady_clock::time_point end_GRAPH_EXEC = std::chrono::steady_clock::now();
    int time = std::chrono::duration_cast<std::chrono::microseconds>(end_GRAPH_EXEC - begin_GRAPH_EXEC).count();
    std::cerr << "GRAPH_EXEC = " << time << "[us]";
    if (time > 1000 )
        std::cerr << " - or " << time / 1000.0 <<  "[ms]" << std::endl;
      else
        std::cerr << std::endl;
    graph_counter = graph_counter + 1;
    graph_total = graph_total + time ;
    graph_average = graph_total / graph_counter;
    cerr << "GRAPH_EXEC : " << graph_counter << " - avg: " << graph_average << "[us]" << endl;
    cerr << "GRAPH #vertices : " << _vertices.size () << endl;
    
    int all_edges = 0;
    for (auto v : _vertices) {
      all_edges += v->get_edges ().size ();
    }
    cerr << "GRAPH #edges: " << all_edges << endl ;

    cerr << "\033[1;33m";
    cerr << "SORTED_GRAPH #vertices: " << _ordered_vertices.size () << endl;

    int sorted_edges = 0;
    for (auto v : _ordered_vertices) {
      sorted_edges += v->get_edges ().size ();
    }

    cerr << "SORTED_GRAPH #edges: " << sorted_edges << endl ;    
    cerr << "SORT_GRAPH : " << sorted_counter << " - avg: " << sorted_average << "[us]" << endl;
    cerr << "\033[0m"  << endl;
#endif

  }
}
