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
#include "core/property/int_property.h"


namespace djnn {

  class Exit : public FatProcess
  {
  public:
    Exit (ParentProcess* parent, const string& name, int value, bool is_model);
    virtual ~Exit ();
    void impl_activate () override;
    void impl_deactivate () override {}
  private:
    IntProperty _value;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
  };

}
