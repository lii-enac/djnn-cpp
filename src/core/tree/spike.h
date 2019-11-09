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

#include "../ontology/process.h"

namespace djnn {
  using namespace std;

  class Spike: public Process {
  public:
    Spike () : Process () { set_is_model (true); };
    Spike (Process* parent, const string& name) : Process (name) { set_is_model (true); Process::finalize_construction (parent, name); };
    virtual ~Spike () {};
    void serialize (const string& format) override;
    Process* clone () override;
  protected:
    bool pre_activate () override;
    void post_activate () override;
    void impl_activate () override;
    void impl_deactivate () override {};
  };

}
