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

#include "../ontology/process.h"

#include <mutex>
#include <vector>
#include <memory>

//#include <iostream>
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
    int get_count_edges_in () { return _count_egdes_in; }
    int get_count_edges_out () { return _edges.size ();}

    void invalidate   () { _is_invalid = true; }
    bool is_invalid   () { return _is_invalid; }

    void set_mark (int m)  { _mark = m; }
    int  get_mark () const { return _mark; }

    void set_timestamp (int date) { _timestamp = date; };
    int  get_timestamp () const   { return _timestamp; }

    Process*       get_process ()       { return _vertex; }
    const Process* get_process () const { return _vertex; }

    void print_vertex () const;

  private:
    Process* _vertex;
    vertices_t _edges;
    int _mark, _timestamp;
    int _count_egdes_in;
    bool _is_invalid;
  };

  class Graph
  {
  public:
    static Graph& instance ();
    virtual ~Graph ();
    
    void add_edge (Process* src, Process* dst);
    void remove_edge (Process* src, Process* dst);
    void add_output_node (Process* c);
    void remove_output_node (Process* c);

    int  size () {return _vertices.size ();}
    void sort ();
    void exec ();
    void clear ();
    void add_process_to_delete (Process *p) { _to_delete.push_back (p); }
    const Vertex::vertices_t& get_sorted () const { return _sorted_vertices; }

    void print_graph  () const;
    void print_sorted () const; 

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
