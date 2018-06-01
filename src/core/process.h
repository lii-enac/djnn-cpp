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

#include "core_types.h"
#include "execution/graph.h"
#include <vector>
#include <map>
#include <string>

namespace djnn {
  using namespace std;

  enum {
    UNDEFINED,
    PROPERTY,
    COMPONENT,
    WINDOW,
    GOBJ
  };

  enum {
    NONE,
    ACTIVATION,
    DEACTIVATION
  };

  typedef vector<Coupling*> couplings_t;

  class Process
  {
  public:
    Process (Process *parent, const string& name);
    Process ();
    virtual ~Process ();
    void finalize ();
    bool is_model ();
    virtual void activation ();
    virtual void deactivation ();

    virtual void set_activation_flag (int flag);
    int get_activation_flag ();
    void add_activation_coupling (Coupling* c);
    void add_deactivation_coupling (Coupling* c);
    void remove_activation_coupling (Coupling* c);
    void remove_deactivation_coupling (Coupling* c);
    void notify_activation ();
    void notify_deactivation ();

    virtual Process* find_component (const string&);
    static Process* find_component (Process*p, const string &path) { cout << "Warning: static find component not yet implemented\n"; return nullptr; }
    void add_symbol (const string &name, Process* c);
    void remove_symbol (const string& name);
    map<string, Process*> symtable () { return _symtable; }

    virtual void add_child (Process* c, const string& name);
    virtual void remove_child (Process* c);
    virtual void remove_child (const string& name);

    Process* state_dependency () { return _state_dependency; }
    activation_state get_state ();
    Process* get_parent ();
    void set_parent (Process* p) { _parent = p; }
    const string& get_name () const;

    int get_cpnt_type ();
    couplings_t get_activation_couplings ();
    couplings_t get_deactivation_couplings ();
    bool has_coupling () { return _has_couplings; } ;

    void set_source (Process* src);
    Process* get_activation_source ();
    void set_data (Process* data);
    Process* get_data ();

    virtual void dump (int level=0);

    // Actions
    virtual void draw () {};
    virtual Process* clone () { cout << "clone not implemented for " << _name << "\n"; return nullptr; };
  private:
    static int _nb_anonymous;
    couplings_t _activation_couplings;
    couplings_t _deactivation_couplings;

  protected:
    virtual void pre_activate ();
    virtual void activate () = 0;
    virtual void post_activate ();

    virtual void pre_deactivate ();
    virtual void deactivate () = 0;
    virtual void post_deactivate ();

    map<string, Process*> _symtable;
    string _name;
    Process *_parent, *_state_dependency;
    Process* _source, *_data;
    activation_state _activation_state;
    bool _model;
    int _activation_flag;
    int _cpnt_type;
    bool _has_couplings;
  };

  void
  alias_children (Process *p, Process *to);
  void
  alias (Process *p, const string &name, Process* from);
  void
  merge_children (Process *p1, const string &sy1, Process *p2, const string &sy2);
}
