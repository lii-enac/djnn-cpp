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

#include <vector>
#include <list>
#include <deque>
#include <memory>

#if !defined(DJNN_NO_DEBUG) || !defined(DJNN_NO_SERIALIZE)
//#include <__iostream>
#endif
//#define DBGG std::cerr << "'" << __FUNCTION__ << " calling graph exec " << __FILE__ << ":" << __LINE__ << std::endl;
#define DBGG

namespace djnn
{
  class Vertex
  {
  public:
    Vertex (CoreProcess* c);
    virtual ~Vertex ();
    
    typedef vector<Vertex* > vertices_t;
    void add_edge (Vertex* dst);
    void remove_edge (Vertex* dst);
    vertices_t& get_edges () { return _edges; }
    const vertices_t& get_edges () const { return _edges; }
    map<Vertex*, int>& get_map_edges () { return _map_edges; }
    size_t get_count_edges_in () { return _count_edges_in; }
    size_t get_count_edges_out () { return _edges.size ();}
    void set_position_in_vertices (std::list <Vertex *>::iterator end) { _pos = std::prev(end); }
    std::list<Vertex* >::iterator get_position_in_vertices () { return _pos; }

    void invalidate   () { _is_invalid = true; }
    bool is_invalid   () { return _is_invalid; }

    void set_mark (int m)  { _mark = m; }
    int  get_mark () const { return _mark; }

    void set_timestamp (int date) { _timestamp = date; }
    int  get_timestamp () const   { return _timestamp; }

    void set_sorted_index (int index) { _sorted_index = index; }
    int get_sorted_index () const { return _sorted_index; }

    CoreProcess*       get_process ()       { return _process; }
    const CoreProcess* get_process () const { return _process; }

    void print_vertex () const;

  private:
    CoreProcess* _process;
    vertices_t _edges;
    std::list<Vertex* >::iterator _pos;
    int _mark, _timestamp;
    size_t _count_edges_in;
    map<Vertex*, int> _map_edges; /* try to deal with duplicate */
    bool _is_invalid;
    int _sorted_index;
  };

  class Graph
  {
  public:
    static Graph& instance ();
    virtual ~Graph ();
    
    void add_edge (CoreProcess* src, CoreProcess* dst);
    void add_native_edge (CoreProcess *src, CoreProcess* dst);
    void remove_edge (CoreProcess* src, CoreProcess* dst);
    void add_output_node (CoreProcess* c);
    void remove_output_node (CoreProcess* c);

    size_t  size () {return _vertices.size ();}
    void sort ();
    void exec ();
    void clear ();
    void schedule_delete (CoreProcess *p) { _scheduled_delete_processes.push_back (p); }
    void schedule_activation (CoreProcess* p);
    const Vertex::vertices_t& get_sorted () const { return _ordered_vertices; }

    void print_graph  () const;
    void print_sorted () const;
    void print_activation () const; 

    void add_in_activation (Vertex *v);
    void clear_activation ();

#if _DEBUG_ENABLE_CHECK_ORDER
    void check_order (CoreProcess *p1, CoreProcess *p2) { _pair_to_check.push_back (std::make_pair(p1, p2)); }
#endif

  private:
    //static Graph _instance;
    static Graph *_instance;
    Graph ();
    void traverse_depth_first (Vertex* v);
    Vertex* add_vertex (CoreProcess* c);
    Vertex* get_vertex (CoreProcess* c);

    void print_order (CoreProcess *p1, CoreProcess *p2);

    //Vertex::vertices_t _vertices;
    std::list< Vertex* > _vertices;

    Vertex::vertices_t _ordered_vertices;
    Vertex::vertices_t _output_nodes;
    vector<CoreProcess*> _scheduled_delete_processes;
    vector<CoreProcess*> _scheduled_activation_processes;

    std::deque < Vertex*> _activation_deque; // try skip_list ?

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
