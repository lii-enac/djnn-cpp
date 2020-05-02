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
#include <list>
#include <map>
#include <string>

#include "utils/debug.h"

namespace djnn {

  // process types
  enum process_type_e {
    UNDEFINED_T,
    PROPERTY_T,
    // BOOL_PROPERTY_T,
    // INT_PROPERTY_T,
    // DOUBLE_PROPERTY_T,
    // TEXT_PROPERTY_T,
    // REF_PROPERTY_T,
    ACTION_T,
    NATIVE_ACTION_T,
    CONTAINER_T,
    FSM_T,
    //SWITCH_T,
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
  class FatProcess;

  class AbstractGShape;
  class PickAnalyticalContext;

  class CoreProcess {
    public:
    CoreProcess (bool model = false);
    virtual ~CoreProcess ();

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
    bool  has_coupling () const { return !get_activation_couplings ().empty() || !get_deactivation_couplings ().empty(); }
    void  notify_activation ();
    void  notify_deactivation ();
    void  schedule_activation (); // for gui picking only: why is there a problem?
    void  schedule_delete ();

    // execution graph
    Vertex*  vertex () { return _vertex; }
#ifdef DJNN_NO_DEBUG
    void     set_vertex (Vertex *v) { _vertex = v; }
#else
    void     set_vertex (Vertex *v);
#endif

    #if _DEBUG_SEE_ACTIVATION_SEQUENCE
     std::pair<int, int> __nb_activation;
    #endif
    #if _DEBUG_SEE_CREATION_DESTRUCTION_ORDER
     std::list<std::pair<Process*, long int>>::iterator __position_in_creation;
    #endif

  protected:
    virtual bool pre_activate ();
    virtual void impl_activate () = 0;
    virtual void post_activate ();
            void post_activate_auto_deactivate ();
    virtual bool pre_deactivate ();
    virtual void impl_deactivate () = 0;
    virtual void post_deactivate ();

    virtual void finalize_construction (FatProcess* parent, const std::string& name, CoreProcess* state=nullptr) {}

  private:
    // >>instance fields start here
    Vertex *_vertex;
    couplings_t _activation_couplings;
    couplings_t _deactivation_couplings;
    unsigned int _bitset;
    // <<instance fields end here

  public:
    virtual CoreProcess* clone ();
    virtual process_type_e get_process_type () const { return UNDEFINED_T; }

    //static std::string default_name;
    //virtual const std::string& get_name () const { return default_name; }
    const std::string& get_name (FatProcess* parent) const;
    virtual FatProcess* get_parent() { return nullptr; }
    virtual const FatProcess* get_parent() const { return nullptr; }

#ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const std::string& format);
#endif

    struct DebugInfo {
      std::string filepath;
      int lineno;
    };

#ifndef DJNN_NO_DEBUG
    virtual  void dump (int level=0);
    const DebugInfo& debug_info () { return _dbg_info; }
    DebugInfo _dbg_info;
#else
    static DebugInfo _dbg_info;
#endif

  // bitfield
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
    void set_bitset (bit_mask MASK, bit_shift SHIFT, unsigned int VALUE)  { _bitset = (_bitset & ~MASK) | (VALUE << SHIFT); }

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


  public:
  // tree, component, symtable 
    virtual void   set_parent (FatProcess* p) {}
    virtual void   add_child (FatProcess* c, const std::string& name) {}
    virtual void   remove_child (FatProcess* c) {}
    virtual void   remove_child (const std::string& name) {}
    virtual void     move_child (FatProcess */*child_to_move*/, child_position_e /*spec*/, FatProcess */*child*/ = nullptr) {}
    //friend  void merge_children (FatProcess *p1, const std::string& sy1, FatProcess *p2, const std::string& sy2) {} // strange, only used in gradient...
    virtual FatProcess* find_child (const std::string&) { return nullptr; }
    virtual FatProcess* find_child (int /*index*/) { return nullptr; }
    static  FatProcess* find_child (FatProcess* p, const std::string& path) { return nullptr; }
    //static std::string default_name;
    //virtual const std::string& find_child_name (const CoreProcess* child) const { return default_name; } // WARNING : low efficiency function cause by linear search. use with care !
  
    virtual void     set_data (CoreProcess* data) {}
    virtual CoreProcess* get_data () { return nullptr; }

    // for NativeAction, should be protected or at least raise an exception since it works only for NativeAction
    virtual void     set_activation_source (CoreProcess*) {}
    virtual CoreProcess* get_activation_source () { return nullptr; }
  
  };

  class ChildProcess : public CoreProcess
  {
  public:
    ChildProcess (bool model = false) : CoreProcess(model), _parent(nullptr), _state_dependency(nullptr) {}
    virtual void set_parent(FatProcess* p); // { _parent = p; }
    FatProcess* get_parent() { return _parent; }
    const FatProcess* get_parent () const { return _parent; }
    CoreProcess* state_dependency () { return _state_dependency; } // for control flow change and execution scheduling
    void set_state_dependency (CoreProcess* s) { _state_dependency = s; }
  protected:
    //void finalize_construction (FatProcess* parent, const std::string& name);
    FatProcess *_parent;
    CoreProcess *_state_dependency;
  };

  class FatProcess : public ChildProcess
  {
  public:
    FatProcess (const std::string& name, bool model = false);
    virtual ~FatProcess ();
    virtual FatProcess* clone () override { CoreProcess::clone (); return nullptr; }
    
    virtual void notify_change ( unsigned int /*notify_mask_*/ ) {} // pseudo, graph-less coupling for efficiency reasons in gui

    // actions to be redefined by subclasses
    virtual     void update_drawing () {}
    virtual     void draw () {}
    virtual     void pick () {}
    virtual     AbstractGShape* pick_analytical (PickAnalyticalContext&) { return nullptr; }
    
    // tree, component, symtable 
    //FatProcess* get_parent () override { return _parent; }
    //const FatProcess* get_parent () const override { return _parent; }
    //virtual void   set_parent (FatProcess* p) override;
    virtual void   add_child (FatProcess* c, const std::string& name) override;
    virtual void   remove_child (FatProcess* c) override;
    virtual void   remove_child (const std::string& name) override;
    virtual void     move_child (FatProcess */*child_to_move*/, child_position_e /*spec*/, FatProcess */*child*/ = nullptr) override {}
    friend  void merge_children (FatProcess *p1, const std::string& sy1, FatProcess *p2, const std::string& sy2); // strange, only used in gradient...
    virtual FatProcess* find_child (const std::string&) override;
    virtual FatProcess* find_child (int /*index*/) override { return nullptr; }
    static  FatProcess* find_child (FatProcess* p, const std::string& path);
    virtual const std::string& find_child_name (const CoreProcess* child) const; // WARNING : low efficiency function cause by linear search. use with care !
    
    typedef std::map<std::string, FatProcess*> symtable_t;
    symtable_t::iterator find_child_iterator (const std::string& name) { return _symtable.find (name); }
    symtable_t::iterator children_end () { return _symtable.end (); }
    bool children_empty () { return _symtable.empty (); }
    size_t children_size () const { return _symtable.size(); }
    symtable_t& children () { return _symtable; }
    const symtable_t& children () const { return _symtable; } 
    
    void    add_symbol (const std::string& name, FatProcess* c); // FIXME: should be alias
    void remove_symbol (const std::string& name);
  
  protected:
    virtual bool pre_activate () override;

  private:
    
    symtable_t& symtable () { return _symtable; }
    const symtable_t& symtable () const { return _symtable; }  

  public:
    static std::string default_name;
    const std::string& get_name () const;

    #ifndef DJNN_NO_DEBUG
    virtual  void dump (int level=0) override;
    #endif
    
    // data
    void     set_data (CoreProcess* data) override;
    CoreProcess* get_data () override;

  protected:
    void finalize_construction (FatProcess* parent, const std::string& name, CoreProcess* state=nullptr) override;

  private:
    static long int _nb_anonymous;

// >>instance fields start here
  private:
    
    //FatProcess *_parent;
    //FatProcess *_state_dependency;
    CoreProcess *_data;
    symtable_t _symtable;
    //string _name;
  // <<instance fields end here
  };

  #if _DEBUG_SEE_ACTIVATION_SEQUENCE
   typedef std::pair<bool , Process*> __stat_exec;
   extern std::vector<__stat_exec> __activation_order;
  #endif
  #if _DEBUG_SEE_CREATION_DESTRUCTION_ORDER
   extern std::list<std::pair<Process*, long int>> __creation_stat_order;
  #endif

  void alias_children (FatProcess *p, FatProcess *to);
  void alias (FatProcess *parent, const std::string& name, FatProcess* from);
  void merge_children (FatProcess *p1, const std::string& sy1, FatProcess *p2, const std::string& sy2);
  inline FatProcess* find (FatProcess *p) { return p; }
  inline FatProcess* find (FatProcess *p, const std::string& path) { return p->find_child (path); }
  // inline FatProcess* find (const std::string& path) { return FatProcess::find_child (nullptr, path); }

  void add_state_dependency (FatProcess *_parent, CoreProcess *p);
  void remove_state_dependency (FatProcess *_parent, CoreProcess *p);
  inline CoreProcess* clone (CoreProcess *p) { return p->clone (); }

}
