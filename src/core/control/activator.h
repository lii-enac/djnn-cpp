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

#include "core/ontology/process.h"

#include <string>

namespace djnn {

  class Activator : public FatProcess
  {
  public:
    Activator (FatProcess* parent, const std::string& name, CoreProcess* src);
    Activator (FatProcess* parent, const std::string& name, CoreProcess* src, const std::string& spec);
    virtual ~Activator ();
    void impl_activate () override { _action->activate (); };
    void impl_deactivate () override {}
  private:
    CoreProcess *_action;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };
}
