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

#include "utils/debug.h"

#include "exec_env/exec_env-dev.h"

#include <algorithm>

#if !defined(DJNN_NO_DEBUG) || !defined(DJNN_NO_SERIALIZE)
#include <iostream>
#endif


 #if _DEBUG_SEE_ACTIVATION_SEQUENCE || !defined(DJNN_NO_DEBUG)
#include <boost/core/demangle.hpp> 
#include <typeinfo>
#endif


#ifndef DJNN_NO_DEBUG

#if _DEBUG_SEE_GRAPH_INFO_PREF
//#include "core/utils/utils-dev.h"
static int graph_counter = 0;
static double graph_total = 0.0;
static double graph_average = 0.0;
static int sorted_counter = 0;
static double sorted_total = 0.0;
static double sorted_average = 0.0;

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

  static void
  _get_monotonic_time (struct timespec *ts)
  {
#ifdef __MACH__
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service (mach_host_self (), SYSTEM_CLOCK, &cclock);
    clock_get_time (cclock, &mts);
    mach_port_deallocate (mach_task_self(), cclock);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;
#endif
  }

  static struct timespec before;
  static struct timespec after;
  static int init = 0;

  static void
  _t1 ()
  {
    _get_monotonic_time (&before);
    init = 1;
  }

  static double
  _t2 (const std::string& msg, bool display_stderr)
  {
    if (!init)
      return 0.0;
    _get_monotonic_time (&after);
    double elapsedTime = (after.tv_sec * 1000 + after.tv_nsec * 1e-6) - (before.tv_sec * 1000 + before.tv_nsec * 1e-6);
    if(display_stderr)
      std::cout << msg << " elapsedTime = " << elapsedTime << " ms" << std::endl;
    init = 0;
    return elapsedTime;
  }
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

  Vertex::Vertex (CoreProcess* p) :
      _process (p), _mark (NOT_MARKED), _timestamp (0), _count_edges_in(0), _is_invalid (false)
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
    std::cout << "vertex (" <<
    boost::core::demangle(typeid(*pp).name()) << ":" << 
    ( pp && pp->get_debug_parent () ? pp->get_debug_parent ()->get_debug_name () + "/" : "") <<
    ( pp ? pp->get_debug_name () : "") << ") - [" << 
    _count_edges_in << ", " << _edges.size () << "] :\t";

    if( _edges.size () == 0)
      cout << "EMPTY" << endl;
    else {
      for (auto e : _edges) {
         auto result = _map_edges.find(e);
         auto * ppe = e->_process;
         if (ppe) {
          std::cout << boost::core::demangle(typeid(*ppe).name()) << ":" << 
          ( ppe->get_debug_parent () ?  ppe->get_debug_parent ()->get_debug_name () + "/" : "" ) << ppe->get_debug_name () << " [x"
            << result->second << "] \t" ;
         }
      }
      std::cout << std::endl;
    }
#endif
  }

  Graph * Graph::_instance;

  Graph&
  Graph::instance ()
  {
    static std::atomic_flag onceFlag = ATOMIC_FLAG_INIT;
    if(!onceFlag.test_and_set()) {
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
  }

  void
  Graph::clear ()
  {
    /* nothing to delete because vertices are own by _vertices. */
    _activation_vector.clear ();

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

    // std::cerr << "remove_edge: " <<
    //   ( p_src->get_parent () ? p_src->get_parent ()->get_name () + "/" : "" ) << p_src->get_name () << " - " << 
    //   ( p_dst->get_parent () ? p_dst->get_parent ()->get_name () + "/" : "" ) << p_dst->get_name () << endl;

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
      std::cerr << "Graph remove_edge: " << boost::core::demangle(typeid(*p_src).name()) + ":" + 
      (ppsrc ? get_hierarchy_name (ppsrc) : "") << "  " << vs << " - " << boost::core::demangle(typeid(*p_dst).name()) + ":" +
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
    cout << " --- GRAPH --- " << endl ;
    for (auto v : _vertices) {
      v->print_vertex ();
    }
    cout << " --- END GRAPH --- " << endl << endl;
#endif
  }

  void
  Graph::print_sorted () const
  {
#ifndef DJNN_NO_DEBUG
    for (auto v : _activation_vector) {
      auto * pp = v->get_process ();
      if (pp && pp->get_debug_parent())
        cerr << pp->get_debug_parent()->get_debug_name () << "/";
      cerr << pp->get_debug_name () << " (" << v->get_timestamp () << ")\n";
    }
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
      _activation_vector.push_back (v);

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

  void
  Graph::sort ()
  {
    if (_sorted)
      return;

    #if _DEBUG_SEE_GRAPH_INFO_PREF
    _t1 ();
    #endif
    _cur_date = 0;
    _activation_vector.clear ();
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
    std::sort (_activation_vector.begin (), _activation_vector.end (), cmp_vertices);

    // append ouptut nodes
    _activation_vector.insert (_activation_vector.end (), _output_nodes.begin (), _output_nodes.end ());

    _sorted = true;

    #if _DEBUG_SEE_GRAPH_INFO_PREF
    cerr << "\033[1;33m" << endl;
    double time = _t2 ("SORT_GRAPH : ", true);
    sorted_counter = sorted_counter + 1;
    sorted_total = sorted_total + time ;
    sorted_average = sorted_total / sorted_counter;
    cerr << "\033[0m"  << endl;
    #endif
  }

  void
  Graph::schedule_activation (CoreProcess *p)
  {
    _scheduled_activation_processes.push_back(p);
  }

  void
  Graph::exec ()
  {
    #if _DEBUG_SEE_GRAPH_INFO_PREF
    _t1 ();
    #endif
    
    //pre_execution : notify_activation *only once* per _scheduled_activation_processes before real graph execution 
    {
      std::map<CoreProcess*, int> already_done;
      for (auto p: _scheduled_activation_processes) {
        if (already_done.find (p) == already_done.end ()) {
          p->notify_activation ();
          already_done[p];
        }
        // DEBUG DUPLICATES
        //else
        //  cerr << "DUPLICATES " << (p->get_parent () ? p->get_parent ()->get_name () : "NUUULL") << "/" << p->get_name () << endl;
      }
      _scheduled_activation_processes.clear ();
    }

    bool is_end = false;
    while (!is_end) {
      is_end = true;
      for (auto v : _activation_vector) {
      	if (!_sorted) break;
        if (v->is_invalid ()) continue;
        auto * p = v->get_process ();
        p->trigger_activation_flag ();
        p->set_activation_flag (NONE_ACTIVATION);
      }
      if (!_sorted) {
        sort ();
        is_end = false;
      }
    }
    
    // then execute delayed delete on processes 
    for (auto p: _scheduled_delete_processes) {
      delete p;
    }
    _scheduled_delete_processes.clear ();

    #if _DEBUG_SEE_ACTIVATION_SEQUENCE
    display_exec_stats ();

    init_exec_stats();
    #endif

    #if _DEBUG_SEE_GRAPH_INFO_PREF
    // print in GREEN
    cerr << "\033[1;32m" << endl;
    double time = _t2 ("GRAPH_EXEC : ", true);
    graph_counter = graph_counter + 1;
    graph_total = graph_total + time ;
    graph_average = graph_total / graph_counter;
    cerr << "GRAPH_EXEC : " << graph_counter << " - avg: " << graph_average << endl;
    cerr << "GRAPH #vertices : " << _vertices.size () << endl;
    
    int all_edges = 0;
    for (auto v : _vertices) {
      all_edges += v->get_edges ().size ();
    }
    cerr << "GRAPH #edges: " << all_edges << endl ;

    cerr << "\033[1;33m";
    cerr << "SORTED_GRAPH #vertices: " << _sorted_vertices.size () << endl;

    int sorted_edges = 0;
    for (auto v : _sorted_vertices) {
      sorted_edges += v->get_edges ().size ();
    }

    cerr << "SORTED_GRAPH #edges: " << sorted_edges << endl ;    
    cerr << "SORT_GRAPH : " << sorted_counter << " - avg: " << sorted_average << endl;
    cerr << "\033[0m"  << endl;
    #endif

  }

  /* ----------- DEBUG ------------- */
  #if _DEBUG_SEE_ACTIVATION_SEQUENCE

  static int _total_num_exec = 0;

  void 
  init_exec_stats(){
    for (int i=0 ; i < __activation_order.size (); i++){
      auto * p = __activation_order[i].second;
      p->__nb_activation.first = 0;
      p->__nb_activation.second = 0;
    }
    __activation_order.clear ();
  }

  void
  display_exec_stats (){
    cerr << "\033[1;33m" << endl;
    cerr << "EXEC " << _total_num_exec++ << endl;
    
    for (int i=0 ; i < __activation_order.size (); i++){
      auto * p = __activation_order[i].second;
      if ( __activation_order[i].first || (p->__nb_activation.first != p->__nb_activation.second) ) {
        cerr << "[" << i << "] \t- " << 
          (__activation_order[i].first ? "ACT   " : "DEACT ") << "- " <<
          boost::core::demangle(typeid(*p).name()) << " - " <<
          (p->get_debug_parent () ? p->get_debug_parent ()->get_debug_name () : "") << "/" << p->get_debug_name () <<
          "\t- [" << p->__nb_activation.first << ", " << p->__nb_activation.second << "]" ;
          if (p->__nb_activation.first > 1 || p->__nb_activation.second > 1) {
            cerr << "\033[1;31m";
            cerr  << "\t\t !!! MORE than 1 act/deact - should never happen";
            cerr << "\033[1;33m";
          }
          cerr << endl;
      }
      else {
        cerr << "\033[1;30m";
        cerr << "[" << i << "] \t- " << 
          "DEACT - " <<
          boost::core::demangle(typeid(*p).name()) << " - " <<
          (p->get_debug_parent () ? p->get_debug_parent ()->get_debug_name () : "") << "/" << p->get_debug_name () <<
          "\t- [" << p->__nb_activation.first << ", " << p->__nb_activation.second << "]" ;
          if (p->__nb_activation.first > 1 || p->__nb_activation.second > 1) {
            cerr << "\033[1;31m";
            cerr  << "\t\t !!! MORE than 1 act/deact - should never happen";
            cerr << "\033[1;33m";
          }
        cerr << endl << "\033[1;33m";
      }
    }
    
    cerr << endl << endl;
    cerr << "\033[0m"  << endl;
  }

  #endif
  /* ----------- END DEBUG ------------- */
}
