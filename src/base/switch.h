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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "../core/tree/component.h"
#include "../core/ontology/coupling.h"
#include "../core/tree/text_property.h"

namespace djnn {
  using namespace std;

  class Switch : public Container
  {
  private:
    class SwitchAction : public Action
    {
    public:
      SwitchAction (Switch *parent, const string &name) :
        Action (parent, "action"),  _sw (parent) {
          /* removed but keep comment as legacy
           * avoid to add the action in Container::_children list
           * otherwise there is a side effect on ~switch which 
           * occured after ~container which already deleted _children
           */ 
          //Process::finalize_construction (); 
        }
      virtual ~SwitchAction () {};
      void impl_activate () override { _sw->change_branch(); };
      void impl_deactivate () override {};
      void serialize (const string& type) override {};
    private:
      Switch* _sw;
    };
  public:
    Switch (Process *parent, const string &name, const string &initial);
    Switch (const string &initial);
    virtual process_type_e get_cpnt_type () const override { return SWITCH_T; }
    void impl_activate () override;
    void impl_deactivate () override;
    void draw () override;
    void pick () override;
    virtual ~Switch ();
    void serialize (const string& type) override;
  private:
    void init_switch (const string &initial);
    void change_branch ();
    friend void SwitchAction::impl_activate ();
    string _initial;
    Coupling* _c_branch;
    TextProperty* _branch_name;
    Process* _action;
    Process* _cur_branch;
  };

}
