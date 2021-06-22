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

  /*
   * WARNING This component does nothing.
   * It cannot be used to notify its subscribers from a coupling.
   * But a component, for example an external source, can use it and call explicitly its notify_(de)activate method.
   */

  class Blank : public FatProcess {
  public:
    Blank (ParentProcess* parent, const djnn::string& name) : FatProcess (name) { finalize_construction (parent, name); }
    virtual ~Blank () {};
    FatProcess* clone () override;
  protected:
    bool pre_activate () override { return true; }
    void impl_activate () override { set_activation_state(ACTIVATED); }
    void post_activate () override { set_activation_state (DEACTIVATED); }
    bool pre_deactivate () override { return true;}
    void impl_deactivate () override { set_activation_state(DEACTIVATED); }
    void post_deactivate () override {}

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const djnn::string& format) override;
#endif
  };

}
