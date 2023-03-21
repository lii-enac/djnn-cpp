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
#include "utils/debug.h"

#include <deque>
#include <memory>

namespace djnn
{
  using vertices_t = list<Vertex*>;
  using edges_t = vector<Vertex*>;
  using ordered_vertices_t = vector<Vertex*>;

  class Vertex
  {
  public:
    // constructor
    Vertex (CoreProcess* c);
    virtual ~Vertex ();
    
    // management
          CoreProcess* get_process ()       { return _process; }
    const CoreProcess* get_process () const { return _process; }
    
    void add_edge (Vertex* dst);
    void remove_edge (Vertex* dst);
          edges_t& get_edges ()       { return _edges; }
    const edges_t& get_edges () const { return _edges; }
    map<Vertex*, int>& get_map_edges () { return _map_edges; }
    size_t get_count_edges_in  () { return _count_edges_in; }
    size_t get_count_edges_out () { return _edges.size ();}
                    void set_position_in_graph_vertices (vertices_t::iterator end) { _pos = prev(end); }
    vertices_t::iterator get_position_in_graph_vertices () { return _pos; }

    void invalidate   () { _is_invalid = true; }
    bool is_invalid   () { return _is_invalid; }

    void set_mark (int m)  { _mark = m; }
    int  get_mark () const { return _mark; }

    void set_timestamp (int date) { _timestamp = date; }
    int  get_timestamp () const   { return _timestamp; }

    void set_sorted_index (int index) { _sorted_index = index; }
    int  get_sorted_index () const    { return _sorted_index; }

#if !defined(DJNN_NO_DEBUG)
    void print_vertex () const;
    void print_full_vertex () ;
#endif

  private:
    CoreProcess* _process;
    edges_t _edges;
    
    // sort management
    int _mark, _timestamp;
    int _sorted_index;

    // deletion management
    bool _is_invalid;
    size_t _count_edges_in;
    map<Vertex*, int> _map_edges;
    vertices_t::iterator _pos;
  };

  class Graph
  {
  public:
    // constructor
    static Graph& instance ();
    virtual ~Graph ();
    void clear ();
    
    // management
    void add_edge (CoreProcess* src, CoreProcess* dst);
    void add_native_edge (CoreProcess *src, CoreProcess* dst);
    void remove_edge (CoreProcess* src, CoreProcess* dst);
    void add_output_node (CoreProcess* c);
    void remove_output_node (CoreProcess* c);
    void clear_activation (); // only for re-init (e.g unit tests)

    // activation management during exec
    void add_in_activation (Vertex *v);
    void schedule_activation (CoreProcess* p) { _scheduled_activation_processes.push_back(p); }
    void schedule_deletion   (CoreProcess* p) { _scheduled_delete_processes.push_back (p); }

    // behavior
    void exec ();
    void sort (Vertex* v_root);
    
#if !defined(DJNN_NO_DEBUG)
    // debug
    const ordered_vertices_t& get_sorted () const { return _ordered_vertices; }
    vertices_t vertices () { return _vertices; }
    size_t size () { return _vertices.size (); }
    void print_graph  () const;
    void print_sorted () const;
    void print_activation () const; 
    void print_full_vertex (Vertex* v) { v->print_full_vertex ();}
    void print_order (CoreProcess *p1, CoreProcess *p2);
#endif

#if _DEBUG_ENABLE_CHECK_ORDER
    void check_order (CoreProcess *p1, CoreProcess *p2) { _pair_to_check.push_back (std::make_pair(p1, p2)); }
#endif

  private:
    // constructor
    //static Graph _instance;
    static Graph *_instance;
    Graph ();

    // management
    Vertex* add_vertex (CoreProcess* c);
    Vertex* get_vertex (CoreProcess* c);

    // behavior
    void traverse_depth_first (Vertex* v);
    void reset_vertices_mark ();

    vertices_t _vertices;
    ordered_vertices_t _ordered_vertices;
    ordered_vertices_t _output_nodes;
    vector<CoreProcess*> _scheduled_delete_processes;
    vector<CoreProcess*> _scheduled_activation_processes;
    std::deque<Vertex*> _activation_deque;
    vector<Vertex*> _activation_triggers_to_sort;

#if _DEBUG_ENABLE_CHECK_ORDER
    vector < pair <CoreProcess*, CoreProcess*> > _pair_to_check;
#endif

    int _cur_date;
    bool _sorted;
  };


  #if _DEBUG_SEE_ACTIVATION_SEQUENCE
  void init_exec_stats();
  void display_exec_stats ();
  #endif

}
