/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "../ontology/process.h"
#include "../ontology/coupling.h"
#include "../control/action.h"


namespace djnn {
  using namespace std;

  class Synchronizer : public Process
  {
    friend class BindingAction;
  private:
    class SynchronizerAction : public Action
    {
    public:
      SynchronizerAction (Process* parent, const string &name) : Action (parent, name) {}
      virtual ~SynchronizerAction () {};
      void impl_activate () override {
        if (_parent->somehow_activating ())
          ((Synchronizer*) _parent)->propagate ();
      }
      void impl_deactivate () override {}
    };
  public:
    Synchronizer (Process* parent, const string &name, Process* dst, const string & dspec);
    virtual ~Synchronizer ();
    void add_source (Process* src, const string &spec);
    void impl_activate () override;
    void impl_deactivate () override;
  private:
    void propagate ();
    Process *_dst;
    std::vector<Coupling*> _c_list;
    SynchronizerAction *_action;
  };
}
