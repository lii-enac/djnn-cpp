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

#include "core/ontology/process.h"

namespace djnn {

  class Spike : public FatProcess {
  public:
    Spike (ParentProcess* parent, const std::string& name, bool model = false) : FatProcess (name, model) { set_is_model (true); finalize_construction (parent, name); };
    virtual ~Spike () {};
    FatProcess* clone () override;
  protected:
    bool pre_activate () override;
    void post_activate () override;
    void impl_activate () override {};
    void impl_deactivate () override {};

#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

}
