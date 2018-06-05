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

namespace djnn
{
  std::shared_ptr<Graph> Graph::_instance;
  std::once_flag Graph::onceFlag;

  enum
  {
    NOT_MARKED, BROWSING, MARKED
  };

  bool
  sort_vertices (std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex>v2)
  {
    return (v1->get_end_date () > v2->get_end_date ());
  }

  Vertex::Vertex (Process* c) :
      _vertex (c), _mark (NOT_MARKED), _start_date (0), _end_date (0), _count_egdes_in(0)
  {
  }

  void
  Vertex::add_edge (std::shared_ptr<Vertex> dst) 
  { 
    //TODO: does push_back check the doubles.
    _edges.push_back (dst);
    dst->_count_egdes_in++; 
  }

  void
  Vertex::remove_edge (std::shared_ptr<Vertex> dst)
  {
    Vertex::vertices_t::iterator newend = _edges.end ();

    //remove
    newend = std::remove (_edges.begin (), _edges.end (), dst);

    //check if end has changed and erase
    if (newend != _edges.end ()){
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

  std::shared_ptr<Vertex>
  Graph::get_vertex (Process* c)
  {
    for (auto v : _vertices) {
      if (v->get_component () == c)
        return (v);
    }
    return nullptr;
  }

  std::shared_ptr<Vertex>
  Graph::add_vertex (Process* c)
  {
    // check if c is already in the graph
    for (auto v : _vertices) {
      if (v->get_component () == c)
        return (v);
    }
    std::shared_ptr<Vertex> v = make_shared<Vertex> (c);
    _vertices.push_back (v);
    _sorted = false;
    return (v);
  }

  void
  Graph::add_output_node (Process* c)
  {
    // check if c is already in the graph
    for (auto v : _output_nodes) {
      if (v->get_component () == c)
        return;
    }
    std::shared_ptr<Vertex> v = make_shared<Vertex> (c);
    _output_nodes.push_back (v);
  }

  void
  Graph::remove_output_node (Process* c)
  {
    /*int i = 0;
    for (auto v : _output_nodes) {
      if (v->get_component () == c)
        _output_nodes.erase (_output_nodes.begin () + i);
      i++;
    }*/
    _output_nodes.erase(std::remove_if (_output_nodes.begin (), _output_nodes.end (),
      [c](Vertex::vertices_t::iterator::value_type v) {return v->get_component () == c;}
      ),
    _output_nodes.end());
  }

  void
  Graph::add_edge (Process* src, Process* dst)
  {
    std::shared_ptr<Vertex> s = get_vertex (src);
    if (s == nullptr) {
      s = add_vertex (src);
    }

    std::shared_ptr<Vertex> d = get_vertex (dst);
    if (d == nullptr) {
      d = add_vertex (dst);
    }
    s->add_edge (d);
    _sorted = false;
  }

  void
  Graph::remove_edge (Process* src, Process* dst)
  {
    std::shared_ptr<Vertex> s = get_vertex (src);
    std::shared_ptr<Vertex> d = get_vertex (dst);
    if (s == nullptr || d == nullptr)
      return;
    s->remove_edge (d);

    // remove vertex if no more out_edges and in_edges
    Vertex::vertices_t::iterator newend = _vertices.end ();
    if ((s->get_edges ().size () == 0) && (s->get_count_egdes_in () == 0))
      newend = std::remove (_vertices.begin (), _vertices.end (), s);
    if ((d->get_edges ().size () == 0) && (d->get_count_egdes_in () == 0))
      newend = std::remove (_vertices.begin (), newend, d);

    // erase only once 
    if (newend != _vertices.end ())
      _vertices.erase(newend, _vertices.end ());

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
      cout << v->get_component ()->get_name () << " (" << v->get_start_date () << ", " << v->get_end_date () << ")\n";
    }
  }

  Graph&
  Graph::instance ()
  {
    std::call_once (Graph::onceFlag, [] () {
      _instance.reset(new Graph);
    });

    return *(_instance.get ());
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
  Graph::remove_properties ()
  {
    /*auto v_it = _sorted_vertices.begin ();
    while (v_it != _sorted_vertices.end ()) {
      auto& v = *v_it;
      AbstractProperty* p = dynamic_cast<AbstractProperty*> (v->get_component ());
      if (p != nullptr)
        v_it = _sorted_vertices.erase (v_it);
      else
        ++v_it;
    }*/
    _sorted_vertices.erase(
      std::remove_if (
        _sorted_vertices.begin (), _sorted_vertices.end (),
        [](Vertex::vertices_t::iterator::value_type v) {return (dynamic_cast<AbstractProperty*> (v->get_component ()) != nullptr); }
      ), _sorted_vertices.end());
  }

  void
  Graph::browse_in_depth (std::shared_ptr<Vertex> v)
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
  Graph::clear ()
  {
    _sorted_vertices.clear ();
    _vertices.clear ();
    _output_nodes.clear ();
  }

  void
  Graph::exec ()
  {
    if (!_sorted)
      sort ();
    for (auto v : _sorted_vertices) {
      int action = v->get_component ()->get_activation_flag ();
      switch (action)
        {
        case ACTIVATION:
          v->get_component ()->activation ();
          break;
        case DEACTIVATION:
          v->get_component ()->deactivation ();
          break;
        default:
          ;
        }
      v->get_component ()->set_activation_flag (NONE);
    }
  }
}
