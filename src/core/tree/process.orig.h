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

#include "../core_types.h"
#include "../execution/graph.h"

#include <vector>
#include <map>
#include <string>
#include <bitset>

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

  typedef vector<Coupling*> couplings_t;

  class Process
  {
  public:
    Process (Process *parent, const string& name, bool model = false);
    Process (bool model = false);
    virtual ~Process ();
    void finalize ();
    virtual void activation ();
    virtual void deactivation ();

    virtual void exec (int flag) { _set_activation_flag (flag); }

    void add_activation_coupling (Coupling* c);
    void add_deactivation_coupling (Coupling* c);
    void remove_activation_coupling (Coupling* c);
    void remove_deactivation_coupling (Coupling* c);
    void notify_activation ();
    void notify_deactivation ();

    virtual Process* find_component (const string&);
    static Process* find_component (Process* p, const string &path);
    // FIXME : low efficiency function cause by linear search. use with care !
    virtual string find_component_name (Process* child);
    void add_symbol (const string &name, Process* c);
    void remove_symbol (const string& name);
    map<string, Process*>& symtable () { return _symtable; }

    virtual void add_child (Process* c, const string& name);
    virtual void remove_child (Process* c);
    virtual void remove_child (const string& name);
    virtual void move_child (Process *child_to_move, int spec, Process *child = 0) {}
    virtual void coupling_activation_hook () {};
    virtual void coupling_deactivation_hook () {};

    Process* state_dependency () { return _state_dependency; }
    //void set_state (activation_state state) { _activation_state = state; }
    //activation_state get_state ();
    void set_vertex (Vertex *v) { _vertex = v; }
    Vertex* vertex () { return _vertex; };
    Process* get_parent ();
    void set_parent (Process* p) { _parent = p; }
    const string& get_name () const;

    virtual int get_cpnt_type () { return UNDEFINED_T; }
    couplings_t& get_activation_couplings ();
    couplings_t& get_deactivation_couplings ();
    bool has_coupling () { return !get_activation_couplings ().empty() ||  !get_deactivation_couplings ().empty(); } ;

    void set_data (Process* data);
    Process* get_data ();

    // for NativeAction, should be protected or at least raise an exception since it works only for NativeAction
    virtual void set_activation_source (Process* src) {}
    virtual Process* get_activation_source () { return nullptr; }
    //friend class Binding;
    //friend class Coupling;

    virtual void dump (int level=0);

    string debug_info () { return _dbg_info; }
    // Actions
    virtual void draw () {};
    virtual void serialize (const string& format) { cout << "serialize is not yet implemented for '" << _name << "'" << endl; }
    virtual Process* clone () { cout << "clone not implemented for " << _name << "\n"; return nullptr; };

  protected:
    virtual void pre_activate ();
    virtual void activate () = 0;
    virtual void post_activate ();

    virtual void pre_deactivate ();
    virtual void deactivate () = 0;
    virtual void post_deactivate ();

  private:
    static int _nb_anonymous;

    couplings_t _activation_couplings;
    couplings_t _deactivation_couplings;
    Vertex *_vertex;
    string _dbg_info;

  protected:
    map<string, Process*> _symtable;
    string _name;
    Process *_parent, *_state_dependency;
    Process *_data;

  private:
    // bitset<8> _bitset;
    // enum {
    //     MODEL_BIT,
    //     ACTIVATION_NONE_BIT,
    //     ACTIVATE_OR_DEACTIVATE_BIT,

    //     ACTIVATE_STATE_LOW_BIT,
    //     ACTIVATE_STATE_HIGH_BIT,
    // };

    

  public:
    // void set_is_model (bool v) { _bitset[MODEL_BIT]=v; }
    // bool is_model () { return _bitset[MODEL_BIT]; }

    // void set_activation_flag ()      {        _bitset[ACTIVATION_NONE_BIT]= false; }
    // void unset_activation_flag ()    {        _bitset[ACTIVATION_NONE_BIT]= true;  }
    // bool is_unset_activation_flag () { return _bitset[ACTIVATION_NONE_BIT]==true;  }
    // bool is_set_activation_flag ()   { return _bitset[ACTIVATION_NONE_BIT]==false; }

    // void request_activation ()        {        set_activation_flag();      _bitset[ACTIVATE_OR_DEACTIVATE_BIT]= true; }
    // bool is_activation_requested ()   { return is_set_activation_flag() && _bitset[ACTIVATE_OR_DEACTIVATE_BIT]==true; }

    // void request_deactivation ()      {        set_activation_flag();      _bitset[ACTIVATE_OR_DEACTIVATE_BIT]= false; }
    // bool is_deactivation_requested () { return is_set_activation_flag() && _bitset[ACTIVATE_OR_DEACTIVATE_BIT]==false; }

  private:
    unsigned int _bitset;
    bool _is_model;
// instance fields end

    static const unsigned int
    MODEL_SHIFT                   = 0 ,
    MODEL_MASK                    = 0b1 ,//<< MODEL_SHIFT ,
    MODEL_FALSE                   = 0b0 ,//<< MODEL_SHIFT ,
    MODEL_TRUE                    = 0b1 ,//<< MODEL_SHIFT ,

    ACTIVATION_FLAG_SHIFT         = 1 ,
    ACTIVATION_FLAG_MASK          = 0b11 << ACTIVATION_FLAG_SHIFT ,
    ACTIVATION_FLAG_NONE          = 0b00 << ACTIVATION_FLAG_SHIFT ,
    ACTIVATION_FLAG_ACTIVATION    = 0b01 << ACTIVATION_FLAG_SHIFT ,
    ACTIVATION_FLAG_DEACTIVATION  = 0b10 << ACTIVATION_FLAG_SHIFT ,

    ACTIVATION_STATE_SHIFT        = 3 ,
    ACTIVATION_STATE_MASK         = 0b11 << ACTIVATION_STATE_SHIFT ,
    ACTIVATION_STATE_ACTIVATING   = 0b00 << ACTIVATION_STATE_SHIFT ,
    ACTIVATION_STATE_ACTIVATED    = 0b01 << ACTIVATION_STATE_SHIFT ,
    ACTIVATION_STATE_DEACTIVATING = 0b10 << ACTIVATION_STATE_SHIFT ,
    ACTIVATION_STATE_DEACTIVATED  = 0b11 << ACTIVATION_STATE_SHIFT ,

    BITSET_END;

public:
    // model
    //unsigned int _bbitset;
    void set_is_model (bool v)        { _bitset = (_bitset & ~MODEL_MASK) | (v == false ? MODEL_FALSE : MODEL_TRUE); }
    bool is_model ()                  { return    (_bitset & MODEL_MASK) == MODEL_TRUE;         }
    //void set_is_model (bool v)        { _is_model=v; }
    //bool is_model ()                  { return _is_model;   }

    // (future?) activation flag
    void unset_activation_flag ()     { _bitset = (_bitset & ~ACTIVATION_FLAG_MASK) |  ACTIVATION_FLAG_NONE;  }
    bool is_unset_activation_flag ()  { return    (_bitset &  ACTIVATION_FLAG_MASK) == ACTIVATION_FLAG_NONE;  }
    bool is_set_activation_flag ()    { return    (_bitset &  ACTIVATION_FLAG_MASK) != ACTIVATION_FLAG_NONE;  }

    void request_activation ()        { _bitset = (_bitset & ~ACTIVATION_FLAG_MASK) |  ACTIVATION_FLAG_ACTIVATION; }
    bool is_activation_requested ()   { return    (_bitset &  ACTIVATION_FLAG_MASK) == ACTIVATION_FLAG_ACTIVATION; }

    void request_deactivation ()      { _bitset = (_bitset & ~ACTIVATION_FLAG_MASK) |  ACTIVATION_FLAG_DEACTIVATION; }
    bool is_deactivation_requested () { return    (_bitset &  ACTIVATION_FLAG_MASK) == ACTIVATION_FLAG_DEACTIVATION; }

    void do_something_according_to_activation_flag () {
        if(is_set_activation_flag ()) {
            if (is_activation_requested ()) activation ();
            else deactivation ();
        }
    }

    // old API, still here because of exec
    void _set_activation_flag (int flag) {
        switch(flag) {
            case NONE: unset_activation_flag (); break;
            case ACTIVATION: request_activation (); break;
            case DEACTIVATION: request_deactivation (); break;
        }
    }

    //void set_activating ()     {        _bitset[ACTIVATED_STATE_LOW_BIT] = false; _bitset[ACTIVATED_STATE_HI_BIT] = false; }
    //void set_activating ()     {        _bitset.to_ulong() &= 0 ACTIVATED_STATE_LOW_BIT ; }

    // void set_activating ()      {       _bitset[ACTIVATED_BIT] =  false; }
    // bool is_activating ()      { return _bitset[ACTIVATED_BIT] == false; }
    // void set_activated ()      {        _bitset[ACTIVATED_BIT] =  true; }
    // bool is_activated ()       { return _bitset[ACTIVATED_BIT] == true; }
    // bool somehow_activating () { return _activation_state < deactivating; }

    // void set_deactivating ()   {        _bitset[DEACTIVATED_BIT] =  false; }
    // bool is_deactivating ()    { return _bitset[DEACTIVATED_BIT] == false; }
    // void set_deactivated ()    {        _bitset[DEACTIVATED_BIT] =  true; }
    // bool is_deactivated ()     { return _bitset[DEACTIVATED_BIT] == true; }

public:
    enum activation_state {
        activating,
        activated,
        deactivating,
        deactivated
    };
    activation_state _activation_state;

    void set_activating ()     {        _activation_state =  activating; }
    bool is_activating ()      { return _activation_state == activating; }
    void set_activated ()      {        _activation_state =  activated; }
    bool is_activated ()       { return _activation_state == activated; }
    bool somehow_activating () { return _activation_state < deactivating; }
    bool somehow_deactivating () { return _activation_state > activated; }

    void set_deactivating ()   {        _activation_state =  deactivating; }
    bool is_deactivating ()    { return _activation_state == deactivating; }
    void set_deactivated ()    {        _activation_state =  deactivated; }
    bool is_deactivated ()     { return _activation_state == deactivated; }

  };

  void
  alias_children (Process *p, Process *to);
  void
  alias (Process *p, const string &name, Process* from);
  void
  merge_children (Process *p1, const string &sy1, Process *p2, const string &sy2);
}
