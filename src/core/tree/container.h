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

  typedef map<string, CoreProcess*> context_t;
  public:
    typedef vector<CoreProcess*> ordered_children_t;
    Container (CoreProcess* parent, const string& name, bool is_model=false);
    virtual process_type_e get_process_type () const override { return CONTAINER_T; }
    void push_back_child (CoreProcess* child);
    void add_child (CoreProcess* c, const string& name) override;
    void move_child (CoreProcess *child_to_move, child_position_e spec, CoreProcess *child = 0) override;
    void remove_child_from_children_only (CoreProcess* c);
    void remove_child (CoreProcess* c) override;
    void remove_child (const string& name) override;
    void swap_children (int i, int j);
    void set_child (CoreProcess *child, int i);
    void update_drawing () override;
    void draw () override;
    void pick () override;
#ifndef DJNN_NO_DEBUG
    void dump (int level=0) override;
#endif
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    void impl_activate () override;
    void impl_deactivate () override;
    void print_children ();
    virtual void clean_up_content ();
    virtual ~Container ();
    const ordered_children_t& children () const { return _children; }
    context_t& get_context () { return _cur_context; }
    void add_to_context (const string& k, CoreProcess *v);
    CoreProcess* get_from_context (const string& k);
    void init_context (context_t &context);
  protected:
    ordered_children_t _children;
    ordered_children_t * _unaltered_children; // to enable proper deletion, store original children sequence as soon as move_child is used on the container 
    bool is_altered () const { return _unaltered_children != nullptr; } // has move_child been used on the container or not?

    context_t _cur_context; // FIXME horrible: every single container has that, though it's only for font style !!!
    
  };
}