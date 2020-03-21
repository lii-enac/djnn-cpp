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

#include "input.h"
#include "core/ontology/process.h"
#include "core/tree/bool_property.h"

namespace djnn {
  typedef enum direction_e {
    IN, OUT
  } direction_e;

  class GPIOProxy : public Process {
  public:
    GPIOProxy (Process *p, const std::string& n) : Process (n) { set_activation_state (ACTIVATED); }
    virtual ~GPIOProxy () {}
    Process* find_child (const std::string& path) override;
  protected:
    void impl_activate () override {}
    void impl_deactivate () override {}
  };
}
