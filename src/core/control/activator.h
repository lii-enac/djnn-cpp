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
    Activator (ParentProcess* parent, const djnn::string& name, CoreProcess* src);
    Activator (ParentProcess* parent, const djnn::string& name, CoreProcess* src, const djnn::string& spec);
    virtual ~Activator ();
    void impl_activate () override { _action->set_activation_flag (ACTIVATION); };
    void impl_deactivate () override {}
  private:
    CoreProcess *_action;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const djnn::string& format) override;
#endif
  };
}
