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
#include "../core/control/coupling.h"
#include "../core/tree/text_property.h"

namespace djnn {
  using namespace std;

  class Switch : public Container
  {
  private:
    class SwitchAction : public Process
    {
    public:
      SwitchAction (Switch *parent, const string &name) :
	      Process (parent, "action"),  _sw (parent) { 
          /* removed but keep comment as legacy
           * avoid to add the action in Container::_children list
           * otherwise there is a side effect on ~switch which 
           * occured after ~container which already deleted _children
           */ 
          //Process::finalize (); 
        }
      virtual ~SwitchAction () {};
      void activate () override { _sw->change_branch(); };
      void deactivate () override {};
      void serialize (const string& type) override {};
    private:
      Switch* _sw;
    };
  public:
    Switch (Process *parent, const string &name, const string &initial);
    Switch (const string &initial);
    virtual int get_cpnt_type () override { return SWITCH_T; }
    void activate () override;
    void deactivate () override;
    void draw () override;
    virtual ~Switch ();
    void serialize (const string& type) override;
  private:
    void init_switch (const string &initial);
    void change_branch ();
    friend void SwitchAction::activate ();
    string _initial;
    Coupling* _c_branch;
    TextProperty* _branch_name;
    Process* _action;
    Process* _cur_branch;
  };

}
