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

  class Spike: public Process {
  public:
    Spike () : Process () { _model = true; };
    Spike (Process* parent, string name) : Process (parent, name) { _model = true; Process::finalize (); };
    virtual ~Spike () {};
    void serialize (const string& format) override;
  protected:
    void pre_activate () override;
    void post_activate () override;
    void activate () override;
    void deactivate () override {};
    void deactivation () override {};
  };

}
