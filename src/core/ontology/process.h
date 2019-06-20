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
    GOBJ_T
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
  using std::string;
  using std::vector;
  using std::map;

  class Process
  {
  public:
    Process (Process *parent, const string& name, bool model = false);
    Process (bool model = false);
    virtual ~Process ();
    virtual process_type_e get_cpnt_type () const { return UNDEFINED_T; }

    // main public API
    void activation ();   // FIXME: should be activate ()
    void deactivation (); // FIXME: should be deactivate ()

    // coupling
    void    add_activation_coupling (Coupling* c);
    void remove_activation_coupling (Coupling* c);
    void    add_deactivation_coupling (Coupling* c);
    void remove_deactivation_coupling (Coupling* c);
    bool    has_coupling () const { return !get_activation_couplings ().empty() || !get_deactivation_couplings ().empty(); }

    virtual void coupling_activation_hook () {};
    virtual void coupling_deactivation_hook () {};
    void         notify_activation ();
    void         notify_deactivation ();
    virtual void notify_change ( unsigned int notify_mask_ ) {} // pseudo, graph-less coupling for efficiency reasons
  
    // execution graph
    void     set_vertex (Vertex *v) { _vertex = v; }
    Vertex*  vertex () { return _vertex; };
    Process* state_dependency () { return _state_dependency; } // for execution scheduling

    // actions to be redefined by subclasses
    virtual     void draw () {}
    virtual     void pick () {}
    virtual     void serialize (const string& format); // { cout << "serialize is not yet implemented for '" << _name << "'" << endl; }
    virtual Process* clone (); // { cout << "clone not implemented for " << _name << "\n"; return nullptr; };

    // for NativeAction, should be protected or at least raise an exception since it works only for NativeAction
    virtual void     set_activation_source (Process* src) {}
    virtual Process* get_activation_source () { return nullptr; }

    // tree, component, symtable 
    virtual void      add_child (Process* c, const string& name);
    virtual void   remove_child (Process* c);
    virtual void   remove_child (const string& name);
    virtual void     move_child (Process *child_to_move, int spec, Process *child = 0) {}
    virtual Process* find_component (const string&); // FIXME: should be find_child
    static  Process* find_component (Process* p, const string &path);
    virtual string   find_component_name (Process* child); // FIXME : low efficiency function cause by linear search. use with care !
    void    add_symbol (const string &name, Process* c);
    void remove_symbol (const string& name);

    typedef map<string, Process*> symtable_t;
    symtable_t& symtable () { return _symtable; }
    const string& get_name () const;
    Process* get_parent ();
    void     set_parent (Process* p) { _parent = p; }
    void     set_data (Process* data);
    Process* get_data ();

    // debug
    virtual  void dump (int level=0);
    const string& debug_info () { return _dbg_info; }

  protected:
    void finalize_construction ();

    virtual bool pre_activate ();
    virtual void activate () = 0;
    virtual void post_activate ();

    virtual bool pre_deactivate ();
    virtual void deactivate () = 0;
    virtual void post_deactivate ();

  private:
    typedef vector<Coupling*> couplings_t;
    const couplings_t& get_activation_couplings () const;
    const couplings_t& get_deactivation_couplings () const;

  private:
    static int _nb_anonymous;

// >>instance fields start here
  private:
    couplings_t _activation_couplings;
    couplings_t _deactivation_couplings;
    Vertex *_vertex;
    string _dbg_info;

  protected:
    symtable_t _symtable;
    string _name;
    Process *_parent;
    Process *_state_dependency;
    Process *_data;

  protected:
    unsigned int _bitset;
// <<instance fields end here
    
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

    void do_something_according_to_activation_flag () {
        switch( get_activation_flag () ) {
          case NONE_ACTIVATION: break;
          case ACTIVATION:      activation (); break;
          case DEACTIVATION:    deactivation (); break;
        }
    }

    // strange, only used in gradient...
    friend void merge_children (Process *p1, const string &sy1, Process *p2, const string &sy2);
  };

  void alias_children (Process *p, Process *to);
  void alias (Process *p, const string &name, Process* from);
  void merge_children (Process *p1, const string &sy1, Process *p2, const string &sy2);
  inline Process* find (Process *p) { return p; }
  inline Process* find (Process *p, const string& path) { return p->find_component (path); }
  inline Process* find (const string& path) { return Process::find_component (nullptr, path); }
  inline Process* clone (Process *p) { return p->clone (); }


  class Action : public Process {
  public:
    Action (bool model = false) : Process (model) {}
    Action (Process *p, const std::string &n, bool model = false) : Process (p, n, model) {}
    virtual ~Action () {}
  protected:
    virtual bool pre_activate () override {
      if (_parent != 0 && !_parent->somehow_activating () )
        return false;
      set_activation_state(ACTIVATING);
      return true;
    }
    void post_activate () override { notify_activation (); set_activation_state (DEACTIVATED); }
  };

}
