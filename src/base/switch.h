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

#include "../core/component.h"
#include "../core/coupling.h"
#include "../core/text_property.h"

namespace djnn {
  using namespace std;

  class Switch : public Container
  {
  private:
    class SwitchAction : public Process
    {
    public:
      SwitchAction (Switch *parent, const string &name) :
	Process (parent, "action"),  _sw (parent) { Process::finalize (); }
      virtual ~SwitchAction () {};
      void activate () { _sw->change_branch(); };
      void deactivate () {};
    private:
      Switch* _sw;
    };
  public:
    Switch (Process *parent, const string &name, const string &initial);
    Switch (const string &initial);
    void activate () override;
    void deactivate () override;
    void draw () override;
    virtual ~Switch ();
  private:
    void init_switch (const string &initial);
    void change_branch ();
    friend void SwitchAction::activate ();
    string _initial;
    unique_ptr<Coupling> _c_branch;
    shared_ptr<TextProperty> _branch_name;
    shared_ptr<Process> _action;
    Process* _cur_branch;
  };

}
