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

#pragma once

#include "graph.h"
#include "../core_types.h"

#include <mutex>
#include <vector>
#include <iostream>
#include <memory>

#define DBGG std::cerr << "'" << __FUNCTION__ << " calling graph exec " << __FILE__ << ":" << __LINE__ << std::endl;
#define GRAPH_EXEC { Graph::instance ().exec (); }

namespace djnn
{
  class Vertex
  {
  public:
    Vertex (Process* c);
    virtual ~Vertex ();
    typedef std::vector< Vertex* > vertices_t;

    void add_edge (Vertex* dst);
    void remove_edge (Vertex* dst);
    vertices_t& get_edges () { return _edges; }
    const vertices_t& get_edges () const { return _edges; }
    int get_count_egdes_in () { return _count_egdes_in; }

    void set_mark (int m) { _mark = m; }
    int get_mark () const { return _mark; }

    void set_start_date (int date) { _start_date = date; };
    int get_start_date () const { return _start_date; }

    void set_end_date (int date) { _end_date = date; };
    int get_end_date () const { return _end_date; }

    Process* get_process () { return _vertex; }
    const Process* get_process () const { return _vertex; }

    void print_vertex () const;
    void invalidate () { _is_invalid = true; }
    bool is_invalid () { return _is_invalid; }

  private:
    Process* _vertex;
    vertices_t _edges;
    int _mark, _start_date, _end_date, _count_egdes_in;
    bool _is_invalid;
  };

  class Graph
  {
  public:
    virtual ~Graph ();
    static Graph& instance ();
    void add_output_node (Process* c);
    void remove_output_node (Process* c);
    void add_edge (Process* src, Process* dst);
    void remove_edge (Process* src, Process* dst);
    void remove_properties ();
    void sort ();
    void exec ();
    void clear ();
    void print_graph ();
    void print_sorted ();
    Vertex::vertices_t get_sorted () { return _sorted_vertices; }

  private:
    static Graph* _instance;
    static std::once_flag onceFlag;
    Graph ();
    void browse_in_depth (Vertex* v);
    Vertex* add_vertex (Process* c);
    Vertex* get_vertex (Process* c);
    Vertex::vertices_t _vertices;
    Vertex::vertices_t _sorted_vertices;
    Vertex::vertices_t _output_nodes;
    int _cur_date;
    bool _sorted;
  };

}
