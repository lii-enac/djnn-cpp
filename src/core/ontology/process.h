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
  using namespace std;

  // process types
  enum {
    UNDEFINED_T,
    PROPERTY_T,
    COMPONENT_T,
    FSM_T,
    SWITCH_T,
    WINDOW_T,
    GOBJ_T
  };

  // activation types
  enum {
    NONE,
    ACTIVATION,
    DEACTIVATION
  };

  // child position spec
  enum {
    FIRST,
    BEFORE,
    AFTER,
    LAST
  };

  enum notify_mask {
    notify_none               = 1 << 0,
    notify_generic            = 1 << 1,
    notify_damaged_geometry   = 1 << 2,
    notify_damaged_transform  = 1 << 3,
    notify_damaged_style      = 1 << 4
  };

  class Coupling;
  class Vertex;

  typedef vector<Coupling*> couplings_t;

  class Process
  {
  public:
    Process (Process *parent, const string& name, bool model = false);
    Process (bool model = false);
    virtual ~Process ();

    // main public API
    void activation ();
    void deactivation ();

    // coupling
    void add_activation_coupling (Coupling* c);
    void add_deactivation_coupling (Coupling* c);
    void remove_activation_coupling (Coupling* c);
    void remove_deactivation_coupling (Coupling* c);
    couplings_t& get_activation_couplings ();
    couplings_t& get_deactivation_couplings ();
    bool has_coupling () { return !get_activation_couplings ().empty() ||  !get_deactivation_couplings ().empty(); } ;

    virtual void coupling_activation_hook () {};
    virtual void coupling_deactivation_hook () {};
    void notify_activation ();
    void notify_deactivation ();
    virtual void notify_change ( unsigned int notify_mask_ ) {} // pseudo, graph-less coupling for efficiency reasons

    // execution graph
    void set_vertex (Vertex *v) { _vertex = v; }
    Vertex* vertex () { return _vertex; };
    Process* state_dependency () { return _state_dependency; } // for execution scheduling

    // actions to be redefined by subclasses
    virtual void draw () {}
    virtual void pick() {}
    virtual void serialize (const string& format); // { cout << "serialize is not yet implemented for '" << _name << "'" << endl; }
    virtual Process* clone (); // { cout << "clone not implemented for " << _name << "\n"; return nullptr; };

    // for NativeAction, should be protected or at least raise an exception since it works only for NativeAction
    virtual void set_activation_source (Process* src) {}
    virtual Process* get_activation_source () { return nullptr; }

    // tree, component, symtable 
    virtual void add_child (Process* c, const string& name);
    virtual void remove_child (Process* c);
    virtual void remove_child (const string& name);
    virtual void move_child (Process *child_to_move, int spec, Process *child = 0) {}
    virtual Process* find_component (const string&); // FIXME: should be find_child
    static Process* find_component (Process* p, const string &path);
    // FIXME : low efficiency function cause by linear search. use with care !
    virtual string find_component_name (Process* child);
    void add_symbol (const string &name, Process* c);
    void remove_symbol (const string& name);
    map<string, Process*>& symtable () { return _symtable; }
    virtual int get_cpnt_type () { return UNDEFINED_T; }

    const string& get_name () const;
    Process* get_parent ();
    void set_parent (Process* p) { _parent = p; }
    void set_data (Process* data);
    Process* get_data ();

    // debug
    virtual void dump (int level=0);
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
    static int _nb_anonymous;

// <instance fields start here>
    couplings_t _activation_couplings;
    couplings_t _deactivation_couplings;
    Vertex *_vertex;
    string _dbg_info;

  protected:
    map<string, Process*> _symtable;
    string _name;
    Process *_parent;
    Process *_state_dependency;
    Process *_data;

  protected:
    unsigned int _bitset;
// </instance fields end here>
    
    enum bit_shift {
        MODEL_SHIFT               = 0 ,
        ACTIVATION_FLAG_SHIFT     = 1 ,
        ACTIVATION_STATE_SHIFT    = 3 ,
        BINDING_ACTION_SHIFT      = 5
    };

    enum bit_mask {
        MODEL_MASK                = 0b1  << MODEL_SHIFT ,
        ACTIVATION_FLAG_MASK      = 0b11 << ACTIVATION_FLAG_SHIFT ,
        ACTIVATION_STATE_MASK     = 0b11 << ACTIVATION_STATE_SHIFT ,
        BINDING_ACTION_MASK       = 0b1  << BINDING_ACTION_SHIFT
    };

    enum bit_value {
        MODEL_FALSE                   = 0b0  << MODEL_SHIFT ,
        MODEL_TRUE                    = 0b1  << MODEL_SHIFT ,

        ACTIVATION_FLAG_NONE          = 0b00 << ACTIVATION_FLAG_SHIFT ,
        ACTIVATION_FLAG_ACTIVATION    = 0b01 << ACTIVATION_FLAG_SHIFT ,
        ACTIVATION_FLAG_DEACTIVATION  = 0b10 << ACTIVATION_FLAG_SHIFT ,

        ACTIVATION_STATE_ACTIVATING   = 0b00 << ACTIVATION_STATE_SHIFT ,
        ACTIVATION_STATE_ACTIVATED    = 0b01 << ACTIVATION_STATE_SHIFT ,
        ACTIVATION_STATE_DEACTIVATING = 0b10 << ACTIVATION_STATE_SHIFT ,
        ACTIVATION_STATE_DEACTIVATED  = 0b11 << ACTIVATION_STATE_SHIFT ,

        BINDING_ACTION_ACTIVATE       = 0b0  << BINDING_ACTION_SHIFT ,
        BINDING_ACTION_DEACTIVATE     = 0b1  << BINDING_ACTION_SHIFT
    };

    void set_flag    (bit_mask MASK, bit_value VALUE) { _bitset = (_bitset & ~MASK) |  VALUE; }
    bool is_flag_set (bit_mask MASK, bit_value VALUE) { return    (_bitset &  MASK) == VALUE; }

  public:
    // model
    void set_is_model (bool v)        { if(v) set_is_model (); else (unset_is_model());   }
    void set_is_model ()              {        set_flag    (MODEL_MASK, MODEL_TRUE);  }
    void unset_is_model ()            {        set_flag    (MODEL_MASK, MODEL_FALSE); }
    bool is_model ()                  { return is_flag_set (MODEL_MASK, MODEL_TRUE);  }

    // (future?) activation flag
    void unset_activation_flag ()     {         set_flag    (ACTIVATION_FLAG_MASK, ACTIVATION_FLAG_NONE); }
    bool is_unset_activation_flag ()  { return  is_flag_set (ACTIVATION_FLAG_MASK, ACTIVATION_FLAG_NONE); }
    bool is_set_activation_flag ()    { return !is_flag_set (ACTIVATION_FLAG_MASK, ACTIVATION_FLAG_NONE); }

    void request_activation ()        {         set_flag    (ACTIVATION_FLAG_MASK, ACTIVATION_FLAG_ACTIVATION); }
    bool is_activation_requested ()   { return  is_flag_set (ACTIVATION_FLAG_MASK, ACTIVATION_FLAG_ACTIVATION); }

    void request_deactivation ()      {         set_flag    (ACTIVATION_FLAG_MASK, ACTIVATION_FLAG_DEACTIVATION);  }
    bool is_deactivation_requested () { return  is_flag_set (ACTIVATION_FLAG_MASK, ACTIVATION_FLAG_DEACTIVATION);  }

    // activation state
    void set_activating ()            {         set_flag    (ACTIVATION_STATE_MASK, ACTIVATION_STATE_ACTIVATING); }
    bool is_activating ()             { return  is_flag_set (ACTIVATION_STATE_MASK, ACTIVATION_STATE_ACTIVATING); }
    void set_activated ()             {         set_flag    (ACTIVATION_STATE_MASK, ACTIVATION_STATE_ACTIVATED); }
    bool is_activated ()              { return  is_flag_set (ACTIVATION_STATE_MASK, ACTIVATION_STATE_ACTIVATED); }

    void set_deactivating ()          {         set_flag    (ACTIVATION_STATE_MASK, ACTIVATION_STATE_DEACTIVATING); }
    bool is_deactivating ()           { return  is_flag_set (ACTIVATION_STATE_MASK, ACTIVATION_STATE_DEACTIVATING); }
    void set_deactivated ()           {         set_flag    (ACTIVATION_STATE_MASK, ACTIVATION_STATE_DEACTIVATED); }
    bool is_deactivated ()            { return  is_flag_set (ACTIVATION_STATE_MASK, ACTIVATION_STATE_DEACTIVATED); }

    bool somehow_activating ()        { return    (_bitset & ACTIVATION_STATE_MASK) < ACTIVATION_STATE_DEACTIVATING; }
    bool somehow_deactivating ()      { return    (_bitset & ACTIVATION_STATE_MASK) > ACTIVATION_STATE_ACTIVATED;    }

    void do_something_according_to_activation_flag () {
        if(is_set_activation_flag ()) {
            if (is_activation_requested ()) activation ();
            else deactivation ();
        }
    }

  };

  class Action : public Process {
  public:
    Action (bool model = false) : Process (model) {}
    Action (Process *p, const string &n, bool model = false) : Process (p, n, model) {}
    virtual ~Action () {}
  protected:
    virtual bool pre_activate () override {
      if ((_parent != 0 && !_parent->somehow_activating() ))
        return false;
      set_activating ();
      return true;
    }
    void post_activate () override { notify_activation (); set_deactivated (); }
  };

  void alias_children (Process *p, Process *to);
  void alias (Process *p, const string &name, Process* from);
  void merge_children (Process *p1, const string &sy1, Process *p2, const string &sy2);
  inline Process* find (Process *p) { return p; }
  inline Process* find (Process *p, const string& path) { return p->find_component (path); }
  inline Process* find (const string& path) { return Process::find_component (nullptr, path); }
  inline Process* clone (Process *p) { return p->clone(); }
}
