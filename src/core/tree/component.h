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

  class Container : public FatProcess
  {

  typedef std::map<std::string, FatChildProcess*> context_t;
  public:
    typedef std::vector<FatChildProcess*> ordered_children_t;
    Container (ParentProcess* parent, const std::string& name);
    virtual process_type_e get_process_type () const override { return CONTAINER_T; }
    void add_child (FatChildProcess* c, const std::string& name) override;
    void move_child (FatChildProcess *child_to_move, child_position_e spec, FatChildProcess *child = 0) override;
    void remove_child_from_children_only (FatChildProcess* c);
    void remove_child (FatChildProcess* c) override;
    void remove_child (const std::string& name) override;
    void swap_children (int i, int j);
    void set_child (FatChildProcess *child, int i);
    void update_drawing () override;
    void draw () override;
    void pick () override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    Container* clone () override;
    void impl_activate () override;
    void impl_deactivate () override;
    void print_children ();
    void clean_up_content ();
    virtual ~Container ();
    const ordered_children_t& children () const { return _children; }
    void
    add_to_context (const std::string& k, FatChildProcess *v)
    {
      context_t::iterator it = _cur_context.find (k);
      if (it != _cur_context.end ()) it->second = v;
      else _cur_context.insert (std::make_pair (k, v));
    }
    FatChildProcess*
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
    ordered_children_t _children;
    context_t _cur_context; // FIXME horrible: every single container has that, though it's only for font style !!!!
  };

  class Component : public Container
  {
  public:
    Component (ParentProcess* parent, const std::string& name) : Container (parent, name) { finalize_construction (parent, name); }
    //virtual process_type_e get_process_type () const override { return CONTAINER_T; }
    Component* clone () override;
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

  class AssignmentSequence : public Container
  {
  public:
    AssignmentSequence ();
    AssignmentSequence (ParentProcess* parent, const std::string& name, bool is_model);
    virtual ~AssignmentSequence () {};
    void add_child (FatChildProcess* c, const std::string& name) override;
    void draw () override {}
    void impl_activate () override;
    void post_activate () override;
    void impl_deactivate () override {};

#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };
}
