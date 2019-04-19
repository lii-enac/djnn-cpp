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

#include "../tree/process.h"
#include "coupling.h"

#include <string>

namespace djnn {
  using namespace std;

  class Synchronizer : public Process
  {
    friend class BindingAction;
  private:
    class SynchronizerAction : public Process
    {
    public:
      SynchronizerAction (Process* parent, const string &name) : Process (parent, name) {}
      virtual ~SynchronizerAction () {};
      void activate () override {
        ((Synchronizer*) _parent)->propagate ();
      }
      void deactivate () override {}
    };
  public:
    Synchronizer (Process* parent, const string &name, Process* dst, const string & dspec);
    virtual ~Synchronizer ();
    void add_source (Process* src, const string &spec);
    void activate () override;
    void deactivate () override;
  private:
    void propagate ();
    Process *_dst;
    std::vector<Coupling*>_c_list;
    SynchronizerAction *_action;
  };
}
