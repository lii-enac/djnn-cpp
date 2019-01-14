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

#include "process.h"
#include <list>

namespace djnn {
  using namespace std;

  class Container : public Process
  {
  typedef std::vector<Process*> children_t;
  typedef std::map<string, Process*> context_t;
  public:
    Container ();
    Container (Process* parent, const string& name);
    virtual int get_cpnt_type () override { return COMPONENT_T; }
    void add_child (Process* c, const string& name) override;
    void move_child (Process *child_to_move, int spec, Process *child = 0) override;
    void remove_child (Process* c) override;
    void remove_child (const string& name) override;
    void draw () override;
    Process* clone () override;
    void activate () override;
    void deactivate () override;
    void print_children ();
    virtual ~Container ();
    children_t children () { return _children; }
    void
    add_to_context (string k, Process *v)
    {
      std::map<string, Process*>::iterator it = _cur_context.find (k);
      if (it != _cur_context.end ()) it->second = v;
      else _cur_context.insert (std::make_pair (k, v));
    }
    Process*
    get_from_context (string k)
    {
      std::map<string, Process*>::iterator it = _cur_context.find (k);
      if (it != _cur_context.end ()) return it->second;
      else return nullptr;
    }
    void init_context (context_t &context) {
      if (!_cur_context.empty ()) {
        context.insert (_cur_context.begin (), _cur_context.end ());
      }
    }
  protected:
    children_t _children;
    context_t _cur_context;
  };

  class Component : public Container
  {
  public:
    Component () : Container () {}
    Component (Process* parent, const string& name) : Container (parent, name) { Process::finalize (); }
    Process* clone () override;
    void serialize (const string& format) override;
  };

  class AssignmentSequence : public Container
  {
  public:
    AssignmentSequence ();
    AssignmentSequence (Process *p, const string &n, bool is_model);
    virtual ~AssignmentSequence () {};
    void add_child (Process* c, const string& name) override;
    void draw () override {}
    void activate () override;
    void post_activate () override;
    void deactivate () override {};
    void serialize (const string& format) override;
  };
}
