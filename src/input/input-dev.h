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

// #include "input.h"
#include "core/ontology/process.h"
// #include "core/property/bool_property.h"

namespace djnn {
extern djnnstl::vector<djnnstl::string> loadedModules;
extern FatProcess *                     InputDevices, *Mice, *TouchPanels, *GPIOs;

void init_input ();
void clear_input ();

typedef enum direction_e {
    IN,
    OUT
} direction_e;

class GPIOProxy : public FatProcess {
  public:
    GPIOProxy (CoreProcess* parent, const string& n)
        : FatProcess (n) { set_activation_state (ACTIVATED); }
    virtual ~GPIOProxy () {}
    CoreProcess* find_child_impl (const string& path) override;

  protected:
    void impl_activate () override {}
    void impl_deactivate () override {}
};
} // namespace djnn
