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

#include "process.h"

namespace djnn {
  using namespace std;

  class Blank: public Process {
  public:
    Blank () : Process () {}
    Blank (Process* parent, string name) : Process (parent, name) { Process::finalize (); }
    virtual ~Blank () {};
    Process* clone () override;
  protected:
    void pre_activate () override {}
    void activate () override { _activation_state = activated; }
    void post_activate () override {}
    void pre_deactivate () override {}
    void deactivate () override { _activation_state = deactivated; }
    void post_deactivate () override {}
    void serialize (const string& format) override;
  };

}
