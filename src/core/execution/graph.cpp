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
#include "../tree/process.h"
#include "../tree/abstract_property.h"

#include <algorithm>
#include <iostream>

#define DBG std::cerr << __FUNCTION__ << " " << __FILE__ << ":" << __LINE__ << std::endl;

#define _PERF_TEST 0
#if _PERF_TEST
#include "../utils-dev.h"
static int graph_counter = 0;
static double graph_total = 0.0;
static double graph_average = 0.0;
#endif

namespace djnn
{
  Graph* Graph::_instance;
  std::once_flag Graph::onceFlag;

  static std::mutex graph_mutex;

  enum
  {
    NOT_MARKED, BROWSING, MARKED
  };

  Vertex::Vertex (Process* c) :
      _vertex (c), _mark (NOT_MARKED), _start_date (0), _end_date (0), _count_egdes_in(0), _is_invalid (false)
  {
  }

  Vertex::~Vertex () {
    // _edges.clear (); // useless
  }

  void
  Vertex::add_edge (Vertex *dst) 
  { 
    //TODO: does push_back check the doubles.
    _edges.push_back (dst);
    dst->_count_egdes_in++;
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
    std::cout << "vertex (" << _vertex->get_name () << ") - " << _count_egdes_in << ", " << _edges.size () << " :\t";
    if( _edges.size () == 0)
      cout << "EMPTY" << endl;
    else {
      for (auto e : _edges) {
        std::cout << e->_vertex->get_name () << " ";
      }
      std::cout << std::endl;
    }
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
    for (Vertex::vertices_t::iterator it = _vertices.begin (); it != _vertices.end (); ++it)
        if (*it) { delete *it; } // *it=nullptr;} // useless since clear is called next
    _vertices.clear ();

    // delete output_vertices from _outpur_nodes and clear 
    for (Vertex::vertices_t::iterator it = _output_nodes.begin (); it != _output_nodes.end (); ++it)
        if (*it) { delete *it; } // *it=nullptr;} // useless since clear is called next
    _output_nodes.clear ();
  }

  Vertex*
  Graph::get_vertex (Process* c)
  {
    for (auto v : _vertices) {
      if (v->get_process () == c)
        return (v);
    }
    return nullptr;
  }

  Vertex*
  Graph::add_vertex (Process* c)
  {
    Vertex* v = new Vertex (c);
    _vertices.push_back (v);
    _sorted = false;
    return (v);
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
        if (*it) { delete *it; } // useless: *it=nullptr;}

      //erase from vector
      _output_nodes.erase( new_end, _output_nodes.end ());
    }
  }

  void
  Graph::add_edge (Process* src, Process* dst)
  {
    Vertex *s = src->vertex ();
    if (s == nullptr) {
      s = add_vertex (src);
      src->set_vertex (s);
    }

    Vertex *d = dst->vertex ();
    if (d == nullptr) {
      d = add_vertex (dst);
      dst->set_vertex (d);
    }
    s->add_edge (d);
    _sorted = false;
  }

  void
  Graph::remove_edge (Process* src, Process* dst)
  {
    Vertex *s = get_vertex (src);
    Vertex *d = get_vertex (dst);
    if (s == nullptr || d == nullptr)
      return;
    s->remove_edge (d);

    /* 
      note :
      remove and delete vertex if they have no more out_edges and in_edges
     */

    // 1 - remove src if necessary
    Vertex::vertices_t::iterator newend = _vertices.end ();
    if ((s->get_edges ().size () == 0) && (s->get_count_egdes_in () == 0)) {
      newend = std::remove (_vertices.begin (), _vertices.end (), s);
      _vertices.erase(newend, _vertices.end ());
      if (s) { delete s; } // useless: s =nullptr;} // ==  src->set_vertex (nullptr);
    }

    // 2 - remove dst if necessary
    newend = _vertices.end ();
    if ((d->get_edges ().size () == 0) && (d->get_count_egdes_in () == 0)){
      newend = std::remove (_vertices.begin (), _vertices.end (), d);
      _vertices.erase(newend, _vertices.end ());
      if (d) { delete d; } // useless: d =nullptr;} // ==  dst->set_vertex (nullptr);
    }

    _sorted = false;
  }

  void
  Graph::print_graph ()
  {
    cout << " --- GRAPH --- " << endl ;
    for (auto v : _vertices) {
      v->print_vertex ();
    }
    cout << " --- END GRAPH --- " << endl << endl;
  }

  void
  Graph::print_sorted ()
  {
    for (auto v : _sorted_vertices) {
      if (v->get_process ()->get_parent())
        cerr << v->get_process ()->get_parent()->get_name () << "/";
      cerr << v->get_process  ()->get_name () << " (" << v->get_start_date () << ", " << v->get_end_date () << ")\n";
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

  void
  Graph::browse_in_depth (Vertex *v)
  {
    v->set_mark (BROWSING);
    v->set_start_date (++_cur_date);
    for (auto v2 : v->get_edges ()) {
      if (v2->get_mark () == NOT_MARKED) {
        browse_in_depth (v2);
      }
    }
    v->set_end_date (++_cur_date);
    v->set_mark (MARKED);
    _sorted_vertices.push_back (v);
  }

  void
  Graph::remove_properties ()
  {
    // remove_if 
    Vertex::vertices_t::iterator new_end = _sorted_vertices.end ();

    new_end = std::remove_if ( _sorted_vertices.begin (), _sorted_vertices.end (),
        [](Vertex::vertices_t::iterator::value_type v) { return v->is_invalid () || (dynamic_cast<AbstractProperty*> (v->get_process  ()) != nullptr); });

    /* note : 
       DO NOT delete the vertices from _sorted_vertices
       because they are own by _vertices.
       we just manage ptr on the vertices in _sorted_vertices.
    */
     
    // erase them from _sorted_vertices
    _sorted_vertices.erase( new_end, _sorted_vertices.end ());
  }

  bool
  sort_vertices (Vertex* v1, Vertex *v2)
  {
    return (v1->get_end_date () > v2->get_end_date ());
  }

  void
  Graph::sort ()
  {
    if (_sorted)
      return;

    _cur_date = 0;
    _sorted_vertices.clear ();

    // set every vertex as NOT_MARKED before sorting them
    for (auto v : _vertices) {
      v->set_mark (NOT_MARKED);
    }

    for (auto v : _vertices) {
      if (v->get_mark () == NOT_MARKED)
        browse_in_depth (v);
    }
    std::sort (_sorted_vertices.begin (), _sorted_vertices.end (), sort_vertices);
    remove_properties ();
    _sorted_vertices.insert (_sorted_vertices.end (), _output_nodes.begin (), _output_nodes.end ());
    _sorted = true;
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
        int action = v->get_process ()->get_activation_flag ();
        switch (action) {
          case ACTIVATION:
            v->get_process ()->activation ();
            break;
          case DEACTIVATION:
            v->get_process ()->deactivation ();
            break;
          default:;
        }
        v->get_process ()->set_activation_flag (NONE);
    }
    if (!_sorted) {
      sort ();
      is_end = false;
    }
   }
   //graph_mutex.unlock ();

#if _PERF_TEST
      double time = t2 ("\nGRAPH_EXEC : ");
      graph_counter = graph_counter + 1;
      graph_total = graph_total + time ;
      graph_average = graph_total / graph_counter;
      cerr << "GRAPH_EXEC : " << graph_counter << " - avg: " << graph_average << endl; 
#endif
  }

}
