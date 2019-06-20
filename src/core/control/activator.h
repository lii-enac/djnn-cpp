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

#include "../ontology/process.h"

#include <string>

namespace djnn {
  using namespace std;

  class Activator : public Process
  {
  public:
    Activator (Process* parent, const string &name, Process* src);
    Activator (Process* parent, const string &name, Process* src, const string &spec);
    virtual ~Activator ();
    void impl_activate () override { _action->activation(); };
    void impl_deactivate () override {}
    void serialize (const string& format) override;
  private:
    Process *_action;
  };
}
