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

//#include "graph.h"
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

    void set_timestamp (int date) { _timestamp = date; };
    int get_timestamp () const { return _timestamp; }

    Process* get_process () { return _vertex; }
    const Process* get_process () const { return _vertex; }

    void print_vertex () const;
    void invalidate () { _is_invalid = true; }
    bool is_invalid () { return _is_invalid; }

  private:
    Process* _vertex;
    vertices_t _edges;
    int _mark, _timestamp, _count_egdes_in;
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
    void sort ();
    void exec ();
    void clear ();
    void print_graph ();
    void print_sorted ();
    void add_process_to_delete (Process *p) { _to_delete.push_back (p); }
    Vertex::vertices_t get_sorted () { return _sorted_vertices; }

  private:
    static Graph* _instance;
    static std::once_flag onceFlag;
    std::vector<Process*> _to_delete;
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
