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

namespace djnn {
  //using namespace std;

  class Container : public Process
  {

  typedef std::map<std::string, Process*> context_t;
  public:
    typedef std::vector<Process*> children_t;
    Container (Process* parent, const std::string& name);
    virtual process_type_e get_cpnt_type () const override { return COMPONENT_T; }
    void add_child (Process* c, const std::string& name) override;
    void move_child (Process *child_to_move, child_position_e spec, Process *child = 0) override;
    void remove_child_from_children_only (Process* c);
    void remove_child (Process* c) override;
    void remove_child (const std::string& name) override;
    void swap_children (int i, int j);
    void set_child (Process *child, int i);
    void update_drawing () override;
    void draw () override;
    void pick () override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    Process* clone () override;
    void impl_activate () override;
    void impl_deactivate () override;
    void print_children ();
    void clean_up_content ();
    virtual ~Container ();
    children_t children () { return _children; }
    void
    add_to_context (const std::string& k, Process *v)
    {
      context_t::iterator it = _cur_context.find (k);
      if (it != _cur_context.end ()) it->second = v;
      else _cur_context.insert (std::make_pair (k, v));
    }
    Process*
    get_from_context (const std::string& k)
    {
      context_t::iterator it = _cur_context.find (k);
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
    Component (Process* parent, const std::string& name) : Container (parent, name) { Process::finalize_construction (parent, name); }
    Process* clone () override;
    void serialize (const std::string& format) override;
  };

  class AssignmentSequence : public Container
  {
  public:
    AssignmentSequence ();
    AssignmentSequence (Process *parent, const std::string& name, bool is_model);
    virtual ~AssignmentSequence () {};
    void add_child (Process* c, const std::string& name) override;
    void draw () override {}
    void impl_activate () override;
    void post_activate () override;
    void impl_deactivate () override {};
    void serialize (const std::string& format) override;
  };
}
