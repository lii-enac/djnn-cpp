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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

#include <vector>
#include <map>
#include <string>

namespace djnn {

  // process types
  enum process_type_e {
    UNDEFINED_T,
    PROPERTY_T,
    COMPONENT_T,
    FSM_T,
    SWITCH_T,
    WINDOW_T,
    GOBJ_T,
    WORLD_T,
    DEFS_T
  };

  // activation types
  enum activation_flag_e { // FIXME DEACTIVATION should be 0, ACTIVATION 1 and NONE 2, would make A/D 1 bit-only
    NONE_ACTIVATION,
    ACTIVATION,
    DEACTIVATION
  };

  enum activation_state_e {
    ACTIVATING,
    ACTIVATED,
    DEACTIVATING,
    DEACTIVATED
  };

  enum notify_mask {
    notify_none               = 1 << 0,
    notify_generic            = 1 << 1,
    notify_damaged_geometry   = 1 << 2,
    notify_damaged_transform  = 1 << 3,
    notify_damaged_style      = 1 << 4
  };

  // child position spec
  enum child_position_e {
    FIRST,
    BEFORE,
    AFTER,
    LAST
  };

  class Coupling;
  class Vertex;

  class AbstractGShape;
  class PickAnalyticalContext;

  class Process
  {
  public:
    Process (const std::string& name, bool model = false);
    virtual ~Process ();
    virtual process_type_e get_cpnt_type () const { return UNDEFINED_T; }

    // main public API
    void activate ();
    void deactivate ();

    // coupling
    void    add_activation_coupling (Coupling* c);
    void remove_activation_coupling (Coupling* c);
    void    add_deactivation_coupling (Coupling* c);
    void remove_deactivation_coupling (Coupling* c);
    typedef std::vector<Coupling*> couplings_t;
    const couplings_t& get_activation_couplings () const { return _activation_couplings; }
    const couplings_t& get_deactivation_couplings () const { return _deactivation_couplings; }
    bool    has_coupling () const { return !get_activation_couplings ().empty() || !get_deactivation_couplings ().empty(); }
    virtual void coupling_activation_hook () {};
    virtual void coupling_deactivation_hook () {};
    void         notify_activation ();
    void         schedule_activation ();
    void         schedule_delete ();
    void         notify_deactivation ();
    virtual void notify_change ( unsigned int notify_mask_ ) {} // pseudo, graph-less coupling for efficiency reasons
    Process* state_dependency () { return _state_dependency; } // for control flow change and execution scheduling
    void set_state_dependency (Process* s) { _state_dependency = s; }
    // for NativeAction, should be protected or at least raise an exception since it works only for NativeAction
    virtual void     set_activation_source (Process* src) {}
    virtual Process* get_activation_source () { return nullptr; }

    // execution graph
    void     set_vertex (Vertex *v) 
    { 
      if (_vertex && v && _vertex != v) print_set_vertex_err_msg (v);
      _vertex = v; 
    }
    void print_set_vertex_err_msg (Vertex *v);
    Vertex*  vertex () { return _vertex; };

    // actions to be redefined by subclasses
    virtual     void update_drawing () {}
    virtual     void draw () {}
    virtual     void pick () {}
    virtual     AbstractGShape* pick_analytical (PickAnalyticalContext&) { return nullptr; }
    virtual     void serialize (const std::string& format);
    virtual Process* clone ();

    // tree, component, symtable 
    Process* get_parent () { return _parent; }
    virtual void   set_parent (Process* p);
    virtual void   add_child (Process* c, const std::string& name);
    virtual void   remove_child (Process* c);
    virtual void   remove_child (const std::string& name);
    virtual void     move_child (Process *child_to_move, child_position_e spec, Process *child = 0) {}
    friend  void merge_children (Process *p1, const std::string& sy1, Process *p2, const std::string& sy2); // strange, only used in gradient...
    virtual Process* find_component (const std::string&); // FIXME: should be find_child
    virtual Process* find_component (int index) { return nullptr; }
    static  Process* find_component (Process* p, const std::string& path);
    virtual const std::string& find_component_name (const Process* child) const; // FIXME : low efficiency function cause by linear search. use with care !
    void    add_symbol (const std::string& name, Process* c); // FIXME: should be alias
    void remove_symbol (const std::string& name);

    typedef std::map<std::string, Process*> symtable_t;
    symtable_t& symtable () { return _symtable; }
    const symtable_t& symtable () const { return _symtable; }
    static std::string default_name;
    const std::string& get_name () const;
    
    // data
    void     set_data (Process* data);
    Process* get_data ();

  protected:
    void finalize_construction (Process* parent, const std::string& name, Process* state=nullptr);

    virtual bool pre_activate ();
    virtual void impl_activate () = 0;
    virtual void post_activate ();

    virtual bool pre_deactivate ();
    virtual void impl_deactivate () = 0;
    virtual void post_deactivate ();

  private:
    static long int _nb_anonymous;

  public:
    struct DebugInfo {
      std::string filepath;
      int lineno;
    };
    virtual  void dump (int level=0);
    const DebugInfo& debug_info () { return _dbg_info; }

// >>instance fields start here
  private:
    Vertex *_vertex;
    couplings_t _activation_couplings;
    couplings_t _deactivation_couplings;
    Process *_parent;
    Process *_state_dependency;
    Process *_data;
    unsigned int _bitset;
    symtable_t _symtable;
    //string _name;

#ifdef DJNN_NO_DEBUG
    static DebugInfo _dbg_info;
#else
    DebugInfo _dbg_info;
#endif

// <<instance fields end here

  public:
    enum bit_shift {
        MODEL_SHIFT            = 0 ,
        ACTIVATION_FLAG_SHIFT  = 1 ,
        ACTIVATION_STATE_SHIFT = 3 ,
        BINDING_ACTION_SHIFT   = 5
    };

    enum bit_mask {
        MODEL_MASK             = 0b1  << MODEL_SHIFT ,
        ACTIVATION_FLAG_MASK   = 0b11 << ACTIVATION_FLAG_SHIFT ,
        ACTIVATION_STATE_MASK  = 0b11 << ACTIVATION_STATE_SHIFT ,
        BINDING_ACTION_MASK    = 0b1  << BINDING_ACTION_SHIFT
    };

    int  get_bitset (bit_mask MASK, bit_shift SHIFT) const       { return    (_bitset &  MASK) >> SHIFT; }
    void set_bitset (bit_mask MASK, bit_shift SHIFT, int VALUE)  { _bitset = (_bitset & ~MASK) | (VALUE << SHIFT); }

  public:
    bool is_model () const         { return get_bitset (MODEL_MASK, MODEL_SHIFT); }
    void set_is_model (bool VALUE) {        set_bitset (MODEL_MASK, MODEL_SHIFT, VALUE); }

    activation_flag_e  get_activation_flag () const  { return static_cast<activation_flag_e> (get_bitset (ACTIVATION_FLAG_MASK, ACTIVATION_FLAG_SHIFT)); }
    void set_activation_flag (activation_flag_e VALUE) {                                      set_bitset (ACTIVATION_FLAG_MASK, ACTIVATION_FLAG_SHIFT, VALUE); }

    activation_state_e get_activation_state () const { return static_cast<activation_state_e>(get_bitset (ACTIVATION_STATE_MASK, ACTIVATION_STATE_SHIFT)); }
    void set_activation_state (activation_state_e VALUE) {                                    set_bitset (ACTIVATION_STATE_MASK, ACTIVATION_STATE_SHIFT, VALUE); }

  public:
    bool is_activated ()         const { return get_activation_state () == ACTIVATED; } // kept it for legacy reasons in test suite
    bool is_deactivated ()       const { return get_activation_state () == DEACTIVATED; }
    bool somehow_activating ()   const { return get_activation_state () <= ACTIVATED; }
    bool somehow_deactivating () const { return get_activation_state () >= DEACTIVATING; }

    void trigger_activation_flag () {
        switch( get_activation_flag () ) {
          case NONE_ACTIVATION: break;
          case ACTIVATION:      activate (); break;
          case DEACTIVATION:    deactivate (); break;
        }
    }
  };

  void alias_children (Process *p, Process *to);
  void alias (Process *parent, const std::string& name, Process* from);
  void merge_children (Process *p1, const std::string& sy1, Process *p2, const std::string& sy2);
  void add_state_dependency (Process *_parent, Process *p);
  void remove_state_dependency (Process *_parent, Process *p);
  inline Process* find (Process *p) { return p; }
  inline Process* find (Process *p, const std::string& path) { return p->find_component (path); }
  inline Process* find (const std::string& path) { return Process::find_component (nullptr, path); }
  inline Process* clone (Process *p) { return p->clone (); }

}
