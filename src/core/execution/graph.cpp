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

#include "../utils/utils-dev.h"

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
      _process (c), _mark (NOT_MARKED), _timestamp (0), _count_egdes_in(0), _is_invalid (false)
  {
  }

  Vertex::~Vertex () {
  }

  void
  Vertex::add_edge (Vertex *dst) 
  { 
    
    /* 
       NOTE : We SHOULD avoid duplicate in this vector.
       here a vector is used because the insert order is importante and used in sorting (traverse_depth_first)
       we can't use std::set, std::unordered_set or std::map because it break this order.
       BUT the find on stc::vector O(n) is more complex than std::set O(log(n)) or std::unordered_set O(1)
    */

    // FIXME: maybe adapte the sort algorithm to use set or better unordered_set 

    auto result = std::find(_edges.begin(), _edges.end(), dst);
 
    /* if it is a new edge */
    if (result == _edges.end()) {
      _edges.push_back (dst);
      dst->_count_egdes_in++;
    }
    
  }

  void
  Vertex::remove_edge (Vertex *dst)
  {
    //remove
    Vertex::vertices_t::iterator newend = _edges.end ();
    newend = std::remove (_edges.begin (), _edges.end (), dst);

    //check if end has changed and erase
    if (newend != _edges.end ()){
      // erase them from _edges
      _edges.erase(newend, _edges.end ());
      dst->_count_egdes_in--;
    }
  }

  void
  Vertex::print_vertex () const
  {
    std::cout << "vertex (" <<
    ( _process->get_parent () ? _process->get_parent ()->get_name () + "/" : "") <<
    _process->get_name () << ") - " << 
    _count_egdes_in << ", " << _edges.size () << " :\t";

    if( _edges.size () == 0)
      cout << "EMPTY" << endl;
    else {
      for (auto e : _edges) {
         std::cout << ( e->_process->get_parent () ?  e->_process->get_parent ()->get_name () + "/" : "" ) << e->_process->get_name () << " " ;
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
    // nothing to delete because vertices are own by _vertices.
    _sorted_vertices.clear ();

    // delete vertices from _vertices and clear.
    for (std::list< Vertex* >::iterator it = _vertices.begin (); it != _vertices.end (); ++it)
        if (*it) delete *it;
    _vertices.clear ();

    // delete output_vertices from _outpur_nodes and clear 
    for (Vertex::vertices_t::iterator it = _output_nodes.begin (); it != _output_nodes.end (); ++it)
        if (*it) delete *it;
    _output_nodes.clear ();
  }

  Vertex*
  Graph::get_vertex (Process* c)
  {
    for (auto v : _vertices) {
      if (v->get_process () == c)
        return v;
    }
    return nullptr;
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
    // check if c is already in the graph
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
        if (*it) delete *it;

      //erase from vector
      _output_nodes.erase( new_end, _output_nodes.end ());
    }
  }

  void
  Graph::add_edge (Process* src, Process* dst)
  {
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
  Graph::remove_edge (Process* src, Process* dst)
  {
    Vertex *s2 = get_vertex (src);
    Vertex *s = src->vertex ();

    Vertex *d2 = get_vertex (dst);
    Vertex *d = dst->vertex ();

    //cerr << "vertices_size : " << _vertices.size () << endl;

    if (s != s2) {
      if (s && s2 && s->get_process () && s2->get_process ())
         cerr << "WRONG SRC !!! " << s2 << " - " << s << " - src: " <<  src->get_name () << " - \"get_vertex ()\" " << s2->get_process ()->get_name () << " - \"->vertex\" " << s->get_process ()->get_name ()  << endl;
       else
        cerr << "WRONG SRC !!! AND one is NULLLL" << endl;
    }


    if (d != d2) {
      if (d && d2 && d->get_process () && d2->get_process ())
       cerr << "WRONG DST !!! " << d2 << " - " << d << " - dst " <<  dst->get_name () << " - \"get_vertex ()\" " << d2->get_process ()->get_name () << " - \"->vertex\" " << d->get_process ()->get_name ()  << endl;
      else
        cerr << "WRONG DST !!! AND one is NULLLL" << endl;
    }

    if (s == nullptr || d == nullptr)
      return;
    s->remove_edge (d);

    /* 
      note :
      remove and delete vertex if they have no more out_edges and in_edges
     */

    //t1 ();
    // 1 - remove src if necessary
    //std::list< Vertex* >::iterator newend = _vertices.end ();
    if ( s->get_edges ().empty () && (s->get_count_edges_in () == 0)) {
      //newend = std::remove (_vertices.begin (), _vertices.end (), s);
      //_vertices.erase(newend, _vertices.end ());
      _vertices.erase(s->get_position_in_vertices ());
      src->set_vertex (nullptr);
      delete s; 
    }
    //t2 ("remove SRC node:  ");

    // 2 - remove dst if necessary
    //t1 ();
    //newend = _vertices.end ();
    if (d->get_edges ().empty () && (d->get_count_edges_in () == 0)){
      //newend = std::remove (_vertices.begin (), _vertices.end (), d);
      //_vertices.erase(newend, _vertices.end ());
      _vertices.erase(d->get_position_in_vertices ());
      dst->set_vertex (nullptr);
      delete d;
    }
    //t2 ("remove DST node:  ");

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
    if (v->is_invalid ()) {
      v->set_mark (MARKED);
      return;
    }

    // add vertex if it's not a property
    if ( v->get_process ()->get_cpnt_type() != PROPERTY_T)
      _sorted_vertices.push_back (v);

    v->set_mark (BROWSING);

    for (auto & v2 : v->get_edges ()) {
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
      p = nullptr;
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
