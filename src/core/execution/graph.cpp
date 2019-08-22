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

#include <algorithm>
#include <iostream>

#define DBG std::cerr << __FUNCTION__ << " " << __FILE__ << ":" << __LINE__ << std::endl;

#include "../utils/error.h"
#define _PERF_TEST 0
#if _PERF_TEST
#include "../utils/utils-dev.h"
static int graph_counter = 0;
static double graph_total = 0.0;
static double graph_average = 0.0;
static int sorted_counter = 0;
static double sorted_total = 0.0;
static double sorted_average = 0.0;
#endif

namespace djnn
{
  Graph* Graph::_instance;
  std::once_flag Graph::onceFlag;

  static std::mutex graph_mutex;

  using std::cout;
  using std::cerr;
  using std::endl;

  enum mark_e {
    NOT_MARKED,
    BROWSING,
    MARKED
  };

  Vertex::Vertex (Process* c) :
      _process (c), _mark (NOT_MARKED), _timestamp (0), _count_edges_in(0), _is_invalid (false)
  {
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
    if (result != _map_edges.end () && result->second > 1)
      result->second = --result->second;
    
    else {
      
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
        dst->_count_edges_in--;

        // print debug
        // cerr << "remove_edge : " << "\t between " << 
        // ( this->_process->get_parent () ? this->_process->get_parent ()->get_name () + "/" : "") <<
        // this->_process->get_name () << " - " <<
        // ( dst->_process->get_parent () ? dst->_process->get_parent ()->get_name () + "/" : "") <<
        // dst->_process->get_name () << endl;
      }
    }
  }

  void
  Vertex::print_vertex () const
  {
    std::cout << "vertex (" <<
    ( _process->get_parent () ? _process->get_parent ()->get_name () + "/" : "") <<
    _process->get_name () << ") - [" << 
    _count_edges_in << ", " << _edges.size () << "] :\t";

    if( _edges.size () == 0)
      cout << "EMPTY" << endl;
    else {
      for (auto e : _edges) {
         auto result = _map_edges.find(e);
         std::cout << ( e->_process->get_parent () ?  e->_process->get_parent ()->get_name () + "/" : "" ) << e->_process->get_name () << " [x"
         << result->second << "] \t" ;
      }
      std::cout << std::endl;
    }
  }

  Graph&
  Graph::instance ()
  {
    std::call_once (Graph::onceFlag, [] () {
      _instance = new Graph();
    });

    return *(_instance);
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
    _sorted_vertices.clear ();

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
  Graph::add_vertex (Process* c)
  {
    Vertex* v = new Vertex (c);
    _vertices.push_back (v);
    v->set_position_in_vertices (_vertices.end ());
    _sorted = false;
    return v;
  }

  void
  Graph::add_output_node (Process* c)
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
  Graph::remove_output_node (Process* c)
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
  Graph::add_edge (Process* src, Process* dst)
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
  Graph::remove_edge (Process* p_src, Process* p_dst)
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
       if not, something (Process or edge dependency) IS NOT well deleted
    */
    if (vs == nullptr || vd == nullptr) {

      std::cerr << "remove_edge: " <<
      ( p_src->get_parent () ? p_src->get_parent ()->get_name () + "/" : "" ) << p_src->get_name () << "  " << vs << " - " << 
      ( p_dst->get_parent () ? p_dst->get_parent ()->get_name () + "/" : "" ) << p_dst->get_name () << "  " << vd << endl;

      warning ( nullptr,  " Graph::remove_edge - - vertex vs or vd is NULL and it SHOULD NOT HAPPEN (except in unit test) \n");
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
    cout << " --- GRAPH --- " << endl ;
    for (auto v : _vertices) {
      v->print_vertex ();
    }
    cout << " --- END GRAPH --- " << endl << endl;
  }

  void
  Graph::print_sorted () const
  {
    for (auto v : _sorted_vertices) {
      if (v->get_process ()->get_parent())
        cerr << v->get_process ()->get_parent()->get_name () << "/";
      cerr << v->get_process  ()->get_name () << " (" << v->get_timestamp () << ")\n";
    }
  }

  void
  Graph::traverse_depth_first (Vertex *v)
  {

    // skip invalid vertex
    //FIXME: we shound use this code anymore - check with coverage result
    if (v->is_invalid ()) {
      //assert (0);
      warning ( nullptr, " Graph::traverse_depth_first - _vertex is invalid - CHECK THE ORDER OF DELETE\n");
      cerr << "vertex: " << v << "- v->process " << v->get_process ();
      v->set_mark (MARKED);
      return;
    }

    // add vertex if it's not a property
    if ( v->get_process ()->get_cpnt_type() != PROPERTY_T)
      _sorted_vertices.push_back (v);

    v->set_mark (BROWSING);

    for (auto& v2 : v->get_edges ()) {
      if (v2->get_mark () == NOT_MARKED) {
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

    #if _PERF_TEST
    t1 ();
    #endif

    _cur_date = 0;
    _sorted_vertices.clear ();

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
    std::sort (_sorted_vertices.begin (), _sorted_vertices.end (), cmp_vertices);

    // append ouptut nodes
    _sorted_vertices.insert (_sorted_vertices.end (), _output_nodes.begin (), _output_nodes.end ());

    _sorted = true;

    #if _PERF_TEST
    // print in YELLOW
    cerr << "\033[1;33m" << endl;
    double time = t2 ("SORT_GRAPH : ");
    sorted_counter = sorted_counter + 1;
    sorted_total = sorted_total + time ;
    sorted_average = sorted_total / sorted_counter;
    cerr << "SORT_GRAPH : " << sorted_counter << " - avg: " << sorted_average << endl;
    cerr << "GRAPH size: " << _vertices.size () << endl;
    cerr << "SORTED_GRAPH size: " << _sorted_vertices.size () << endl;
    cerr << "\033[0m"  << endl;
    #endif
  }

  void
  Graph::exec ()
  {

    #if _PERF_TEST
    t1 ();
    #endif

    //graph_mutex.lock ();

    bool is_end = false;
    while (!is_end) {
      is_end = true;
      for (auto v : _sorted_vertices) {
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
   if (!_to_delete.empty ()) {
    for (auto p: _to_delete) {
      delete p;
      //p = nullptr;
    }
    _to_delete.clear ();
   }

   //graph_mutex.unlock ();

    #if _PERF_TEST
    // print in GREEN
    cerr << "\033[1;32m" << endl;
    double time = t2 ("GRAPH_EXEC : ");
    graph_counter = graph_counter + 1;
    graph_total = graph_total + time ;
    graph_average = graph_total / graph_counter;
    cerr << "GRAPH_EXEC : " << graph_counter << " - avg: " << graph_average << endl;
    cerr << "GRAPH size: " << _vertices.size () << endl;
    cerr << "SORTED_GRAPH size: " << _sorted_vertices.size () << endl;
    cerr << "\033[0m"  << endl;
    #endif
  }

}
