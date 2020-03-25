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

#include "core/ontology/process.h"

#include <vector>
#include <list>
#include <memory>

#if !defined(DJNN_NO_DEBUG) || !defined(DJNN_NO_SERIALIZE)
#include <iostream>
#endif

//#define DBGG std::cerr << "'" << __FUNCTION__ << " calling graph exec " << __FILE__ << ":" << __LINE__ << std::endl;
#define DBGG

#define GRAPH_EXEC { DBGG; djnn::Graph::instance ().exec (); }

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
    std::map< Vertex*, int>& get_map_edges () { return _map_edges; }
    int get_count_edges_in () { return _count_edges_in; }
    int get_count_edges_out () { return _edges.size ();}
    void set_position_in_vertices (std::list <Vertex *>::iterator end) { _pos = std::prev(end); }
    std::list< Vertex* >::iterator get_position_in_vertices () { return _pos; }

    void invalidate   () { _is_invalid = true; }
    bool is_invalid   () { return _is_invalid; }

    void set_mark (int m)  { _mark = m; }
    int  get_mark () const { return _mark; }

    void set_timestamp (int date) { _timestamp = date; };
    int  get_timestamp () const   { return _timestamp; }

    Process*       get_process ()       { return _process; }
    const Process* get_process () const { return _process; }

    void print_vertex () const;

  private:
    Process* _process;
    vertices_t _edges;
    std::list< Vertex* >::iterator _pos;
    int _mark, _timestamp;
    int _count_edges_in;
    std::map< Vertex*, int> _map_edges; /* try to deal with duplicate */
    bool _is_invalid;
  };

  class Graph
  {
  public:
    static Graph& instance ();
    virtual ~Graph ();
    
    void add_edge (Process* src, Process* dst);
    void add_native_edge (Process *src, Process* dst);
    void remove_edge (Process* src, Process* dst);
    void add_output_node (Process* c);
    void remove_output_node (Process* c);

    int  size () {return _vertices.size ();}
    void sort ();
    void exec ();
    void clear ();
    void schedule_delete (Process *p) { _scheduled_delete_processes.push_back (p); }
    void schedule_activation (Process* p);
    const Vertex::vertices_t& get_sorted () const { return _sorted_vertices; }

    void print_graph  () const;
    void print_sorted () const; 

  private:
    static Graph _instance;
    Graph ();
    void traverse_depth_first (Vertex* v);
    Vertex* add_vertex (Process* c);
    Vertex* get_vertex (Process* c);
    
    //Vertex::vertices_t _vertices;
    std::list< Vertex* > _vertices;

    Vertex::vertices_t _sorted_vertices;
    Vertex::vertices_t _output_nodes;
    std::vector<Process*> _scheduled_delete_processes;
    std::vector<Process*> _scheduled_activation_processes;
    int _cur_date;
    bool _sorted;
  };

}
