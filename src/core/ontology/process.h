/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2020)
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

#include "core/utils/containers.h"
#include "utils/debug.h"

namespace djnn {

  // process types
  enum
  //class
  process_type_e {
    UNDEFINED_T,
    PROPERTY_T,
    ACTION_T,
    NATIVE_ACTION_T,
    NATIVE_COLLECTION_ACTION_T,
    CONTAINER_T,
    FSM_T,
    WINDOW_T,
    GOBJ_T,
    WORLD_T,
    DEFS_T,
    LAYER_T,
    SYNCHRONIZER_T,
    Z_ORDERED_GROUP_T
  };
  //using enum process_type_e;

  enum
  //class
  activation_flag_e {
    NONE_ACTIVATION,
    ACTIVATION,
    DEACTIVATION
  };
  //using enum activation_flag_e;

  enum
  //class
  activation_state_e {
    ACTIVATING,
    ACTIVATED,
    DEACTIVATING,
    DEACTIVATED
  };
  //using enum activation_state_e;

  enum notify_mask {
    notify_none               =      0,
    notify_generic            = 1 << 0,
    notify_damaged_geometry   = 1 << 1,
    notify_damaged_transform  = 1 << 2,
    notify_damaged_style      = 1 << 3
  };

  enum
  //class
  child_position_e {
    FIRST,
    BEFORE,
    AFTER,
    LAST
  };

  //using enum child_position_e;

  class Coupling;
  class Vertex;

  class FatProcess;

  // for pick_analytical
  class AbstractGShape;
  class PickAnalyticalContext;

  class CoreProcess {
    public:

    CoreProcess (bool model = false);
    virtual ~CoreProcess ();

    // activation
    void activate ();
    void deactivate ();

    bool is_activated ()         const { return get_activation_state () == ACTIVATED; }
    bool is_deactivated ()       const { return get_activation_state () == DEACTIVATED; }
    bool somehow_activating ()   const { return get_activation_state () <= ACTIVATED; }
    bool somehow_deactivating () const { return get_activation_state () >= DEACTIVATING; }

    // coupling
    virtual void    add_activation_coupling   (Coupling* c) {}
    virtual void remove_activation_coupling   (Coupling* c) {}
    virtual void    add_deactivation_coupling (Coupling* c) {}
    virtual void remove_deactivation_coupling (Coupling* c) {}
    typedef vector<Coupling*> couplings_t;
    virtual const couplings_t& get_activation_couplings ()   const { return default_couplings; }
    virtual const couplings_t& get_deactivation_couplings () const { return default_couplings; }
    virtual bool  has_coupling () const { return false; }

    void  notify_activation ();
    void  notify_deactivation ();
    void  schedule_activation (); // for gui picking only: why is there a problem?
    void  schedule_deletion ();
    void  schedule_delete (); // deprecated

    // model
    bool is_model () const         { return get_bitset (MODEL_MASK, MODEL_SHIFT); }
    void set_is_model (bool VALUE) {        set_bitset (MODEL_MASK, MODEL_SHIFT, VALUE); }

    // clone and utils
              CoreProcess* clone ();
    virtual process_type_e get_process_type () const { return UNDEFINED_T; }
    virtual void           set_data (CoreProcess* data) {}
    virtual CoreProcess*   get_data () { return nullptr; }
    #ifndef DJNN_NO_SERIALIZE
    virtual void           serialize (const string& format);
    #endif


    // starting from here, methods are empty or return nullptr by default, and are redefined by more specialized processes

    // actions
    virtual void update_drawing () {}
    virtual void draw () {}
    virtual void pick () {}
    virtual AbstractGShape* pick_analytical (PickAnalyticalContext&) { return nullptr; }


    // parent, state dependency
    virtual       FatProcess* get_parent() { return nullptr; }
    virtual const FatProcess* get_parent() const { return nullptr; }
    virtual              void set_parent (CoreProcess* p) {}

    virtual CoreProcess* get_state_dependency () { return nullptr; } // for control flow change and execution scheduling
    virtual         void set_state_dependency (CoreProcess* s) {}
    virtual CoreProcess* state_dependency (); // deprecated

     // children, tree, component, symtable
    virtual void      add_child (CoreProcess* c, const string& name) {}
    virtual void   remove_child (CoreProcess* c) {}
    virtual void   remove_child (const string& name) {}
    virtual void     move_child (CoreProcess */*child_to_move*/, child_position_e /*spec*/, CoreProcess */*child*/ = nullptr) {}
    virtual size_t children_size () const { return 0; }

    CoreProcess* find_child (const string&);
    CoreProcess* find_child (int /*index*/);
    CoreProcess* find_optional_child (const string&); // child might not exist, no warning in debug mode
    static CoreProcess* find_child (CoreProcess* p, const string& path) { return nullptr; }
    virtual const string& find_child_name (const CoreProcess* child) const { return default_name; } // WARNING : low efficiency function cause by linear search. use with care !

    virtual void add_symbol (const string& name, CoreProcess* c) {}
    typedef map<string, CoreProcess*> symtable_t;
    static symtable_t default_symtable;
    virtual const symtable_t& symtable () const { return default_symtable; }
  

  // implementation

  private:
    // >>instance fields start here
    Vertex *_vertex;
    unsigned int _bitset;
    // <<instance fields end here

    static couplings_t default_couplings; // default_couplings is a static empty couplings_t

  public:
    static string default_name;
    

  public:
    // execution graph
    Vertex*  vertex () { return _vertex; }
#ifdef DJNN_NO_DEBUG
    void     set_vertex (Vertex *v) { _vertex = v; }
#else
    void     set_vertex (Vertex *v);
#endif

  protected:
    // constructor
    virtual void finalize_construction (CoreProcess* parent, const string& name, CoreProcess* state=nullptr);

    // activation
    virtual bool pre_activate ();
    virtual void impl_activate () = 0;
    virtual void post_activate ();
            void post_activate_auto_deactivate ();
    virtual bool pre_deactivate ();
    virtual void impl_deactivate () = 0;
    virtual void post_deactivate ();
  
  // should be protected
  public: 
    virtual CoreProcess* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones);
  
  // should be protected
  public:
    virtual CoreProcess* find_child_impl (const string&);
    virtual CoreProcess* find_child_impl (int /*index*/);

  
  // bitfield for various state info: model, activation flag, activation state, binding
  public:
    enum bit_shift {
        MODEL_SHIFT            = 0 ,
        ACTIVATION_FLAG_SHIFT  = 1 ,
        ACTIVATION_STATE_SHIFT = 3 ,
        BINDING_ACTION_SHIFT   = 5
    };

    enum bit_mask {
        MODEL_MASK             = 0b1  << MODEL_SHIFT , // FIXME ?! shouldn't it be 0?
        ACTIVATION_FLAG_MASK   = 0b11 << ACTIVATION_FLAG_SHIFT ,
        ACTIVATION_STATE_MASK  = 0b11 << ACTIVATION_STATE_SHIFT ,
        BINDING_ACTION_MASK    = 0b1  << BINDING_ACTION_SHIFT
    };

    int  get_bitset (bit_mask MASK, bit_shift SHIFT) const                { return    (_bitset &  MASK) >> SHIFT; }
    void set_bitset (bit_mask MASK, bit_shift SHIFT, unsigned int VALUE)  { _bitset = (_bitset & ~MASK) | (VALUE << SHIFT); }

  public:

    activation_flag_e  get_activation_flag () const  { return static_cast<activation_flag_e> (get_bitset (ACTIVATION_FLAG_MASK, ACTIVATION_FLAG_SHIFT)); }
    virtual void set_activation_flag (activation_flag_e VALUE);

    activation_state_e get_activation_state () const { return static_cast<activation_state_e>(get_bitset (ACTIVATION_STATE_MASK, ACTIVATION_STATE_SHIFT)); }
    void set_activation_state (activation_state_e VALUE) {                                    set_bitset (ACTIVATION_STATE_MASK, ACTIVATION_STATE_SHIFT, static_cast<unsigned int>(VALUE)); }

    void trigger_activation_flag () {
        switch( get_activation_flag () ) {
          case NONE_ACTIVATION: break;
          case ACTIVATION:      activate (); break;
          case DEACTIVATION:    deactivate (); break;
        }
    }

    // for NativeAction, should be protected or at least raise an exception since it works only for NativeAction
    // also used by ActiveView
    virtual void         set_activation_source (CoreProcess*) {}
    virtual CoreProcess* get_activation_source () { return nullptr; }
  

  // debug stuff
  public:

#ifndef DJNN_NO_DEBUG
    FatProcess* get_debug_parent ();
    void set_debug_parent (CoreProcess* parent) { _debug_parent = parent; }
    const string& get_debug_name () const { return _debug_name; }
    void set_debug_name (const string& n) { _debug_name = n; }
    private:
      CoreProcess* _debug_parent;
      string _debug_name;
    public:
#else
  FatProcess* get_debug_parent () { return nullptr; }
  void set_debug_parent (CoreProcess*) {}
  const string& get_debug_name () const { return default_name; }
  void set_debug_name (const string& n) {}
#endif

    const string& get_name (CoreProcess* parent) const;  // WARNING : low efficiency function cause by linear search. use with care !
    static vector<string> default_properties_name;
    virtual const vector<string>& get_properties_name () const;// { vector<string> res; return res; }
    struct DebugInfo {
      string filepath;
      int lineno;
    };

#ifndef DJNN_NO_DEBUG
    virtual  void dump (int level=0);
    const DebugInfo& debug_info () const { return _dbg_info; }
    DebugInfo _dbg_info;
#else
    static DebugInfo _dbg_info;
#endif

    public:
    #if _DEBUG_SEE_ACTIVATION_SEQUENCE
      pair<int, int> __nb_activation;
    #endif
    #if _DEBUG_SEE_CREATION_DESTRUCTION_ORDER
      list<pair<CoreProcess*, long int>>::iterator __position_in_creation;
    #endif

  };

  class VoidProcess : public CoreProcess {
    public:
      VoidProcess () : CoreProcess () {}
      ~VoidProcess () {}
      void impl_activate () override {}
      void impl_deactivate () override {}
  };

  class CouplingProcess : public CoreProcess {
  public:
    CouplingProcess (bool model = false) : CoreProcess(model) {}
    virtual ~CouplingProcess ();

    // coupling
    void    add_activation_coupling   (Coupling* c) override;
    void remove_activation_coupling   (Coupling* c) override;
    void    add_deactivation_coupling (Coupling* c) override;
    void remove_deactivation_coupling (Coupling* c) override;
    bool  has_coupling () const override { return !get_activation_couplings ().empty() || !get_deactivation_couplings ().empty(); }
    
    const couplings_t& get_activation_couplings () const override { return _activation_couplings; }
    const couplings_t& get_deactivation_couplings () const override { return _deactivation_couplings; }
  private:
    couplings_t _activation_couplings;
    couplings_t _deactivation_couplings;
  };

  class ChildProcess : public CouplingProcess
  {
  public:
    ChildProcess (bool model = false) : CouplingProcess (model), _parent (nullptr), _state_dependency (nullptr) {}
    virtual void set_parent (CoreProcess* p) override;
    FatProcess*  get_parent () override { return _parent; }
    const FatProcess* get_parent () const override { return _parent; }

    virtual CoreProcess* get_state_dependency () override { return _state_dependency; } // for control flow change and execution scheduling
    virtual void set_state_dependency (CoreProcess* s) override { _state_dependency = s; }

  protected:
    void finalize_construction (CoreProcess* parent, const string& name, CoreProcess* state=nullptr) override;
    FatProcess  *_parent;
    CoreProcess *_state_dependency;
  };

  class FatProcess : public ChildProcess
  {
  public:
    FatProcess (const string& name, bool model = false);
    
    virtual void notify_change ( unsigned int /*notify_mask_*/ ); // pseudo, graph-less coupling for efficiency reasons in gui

    // tree, component, symtable
    virtual void      add_child (CoreProcess* c, const string& name) override;
    virtual void   remove_child (CoreProcess* c) override;
    virtual void   remove_child (const string& name) override;
    friend  void merge_children (CoreProcess *p1, const string& sy1, CoreProcess *p2, const string& sy2); // strange, only used in gradient...
    virtual const string& find_child_name (const CoreProcess* child) const override; // WARNING : low efficiency function caused by linear search. use with care !

    // symbol and children-related methods only used in FatProcess
    // symtable_t::iterator find_child_iterator (const string& name) { return _symtable.find (name); }
    // symtable_t::iterator children_end () { return _symtable.end (); }
    // bool children_empty () { return _symtable.empty (); }
    // size_t children_size () const override { return _symtable.size(); }
    symtable_t::iterator find_child_iterator (const string& name);// { return _symtable.find (name); }
    symtable_t::iterator children_end ();// { return _symtable.end (); }
    bool children_empty ();// { return _symtable.empty (); }
    size_t children_size () const override;// { return _symtable.size(); }
    
    symtable_t& symtable () { return _symtable; }
    const symtable_t& symtable () const override { return _symtable; }
    
    virtual void add_symbol (const string& name, CoreProcess* c) override;
    void      remove_symbol (const string& name);

  protected:
    virtual bool pre_activate () override;

  public:
    virtual CoreProcess* find_child_impl (const string&) override;
    //virtual CoreProcess* find_child (int /*index*/) override { return nullptr; }
    static  CoreProcess* find_child_impl (CoreProcess* p, const string& path);

  public:
    const string& get_name () const;

#ifndef DJNN_NO_DEBUG
    virtual  void dump (int level=0) override;
#endif

    // data
    void         set_data (CoreProcess* data) override;
    CoreProcess* get_data () override;

  private:
    // >>instance fields start here
    CoreProcess *_data;
    symtable_t _symtable;
    // <<instance fields end here
  };

  #if _DEBUG_SEE_CREATION_DESTRUCTION_ORDER
    extern list<pair<CoreProcess*, long int>> __dbg_creation_stat_order;
  #endif
  
  void alias_children (CoreProcess* parent, FatProcess *to);
  void alias (CoreProcess* parent, const string& name, CoreProcess* from);
  void merge_children (CoreProcess* parent1, const string& sy1, CoreProcess* parent2, const string& sy2);
  inline FatProcess* find (FatProcess* p) { return p; } // helper for smalac
  inline FatProcess* find (CoreProcess* p) { return dynamic_cast<FatProcess*>(p); } // helper for smalac
  inline CoreProcess* find (CoreProcess* parent, const string& path) { return parent->find_child (path); }

  void remove_from_parentless_name (CoreProcess* child);
  
  void add_state_dependency (CoreProcess* parent, CoreProcess *p);
  void remove_state_dependency (CoreProcess* parent, CoreProcess *p);
  //inline CoreProcess* clone (CoreProcess *p) { return p->clone (); }
  template <typename P> P* clone (P *p) { return dynamic_cast<P*> (p->clone ()); } // FIXME will make code size grow :-/...

  inline void activate (CoreProcess* p) { p->activate (); }
  inline void deactivate (CoreProcess* p) { p->deactivate (); }

  // to be removed
  using FatChildProcess = CoreProcess;
  using ParentProcess = CoreProcess;
}
