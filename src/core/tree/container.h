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

  typedef map<string, FatChildProcess*> context_t;
  public:
    typedef vector<FatChildProcess*> ordered_children_t;
    Container (ParentProcess* parent, const string& name, bool is_model=false);
    virtual process_type_e get_process_type () const override { return CONTAINER_T; }
    void add_child (FatChildProcess* c, const string& name) override;
    void move_child (FatChildProcess *child_to_move, child_position_e spec, FatChildProcess *child = 0) override;
    void remove_child_from_children_only (FatChildProcess* c);
    void remove_child (FatChildProcess* c) override;
    void remove_child (const string& name) override;
    void swap_children (int i, int j);
    void set_child (FatChildProcess *child, int i);
    void update_drawing () override;
    void draw () override;
    void pick () override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    void impl_activate () override;
    void impl_deactivate () override;
    void print_children ();
    virtual void clean_up_content ();
    virtual ~Container ();
    const ordered_children_t& children () const { return _children; }
    context_t& get_context () { return _cur_context; }
    void add_to_context (const string& k, FatChildProcess *v);
    FatChildProcess* get_from_context (const string& k);
    void init_context (context_t &context);
  protected:
    ordered_children_t _children;
    context_t _cur_context; // FIXME horrible: every single container has that, though it's only for font style !!!!
  };
}
