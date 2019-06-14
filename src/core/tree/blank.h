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

  class Blank: public Process {
  public:
    Blank () : Process () {}
    Blank (Process* parent, string name) : Process (parent, name) { Process::finalize (); }
    virtual ~Blank () {};
    Process* clone () override;
  protected:
    bool pre_activate () override { return true; }
    //void activate () override { _activation_state = activated; }
    void activate () override { set_activated (); }
    void post_activate () override { set_deactivated (); }
    bool pre_deactivate () override { return true;}
    //void deactivate () override { _activation_state = deactivated; }
    void deactivate () override { set_deactivated (); }
    void post_deactivate () override {}
    void serialize (const string& format) override;
  };

}
