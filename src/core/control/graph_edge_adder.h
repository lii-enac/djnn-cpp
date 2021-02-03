/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2021)
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

#include <string>

namespace djnn {

  class GraphEdgeAdder : public FatProcess
  {
  public:
    GraphEdgeAdder (ParentProcess* parent, const std::string& name, CoreProcess* src, CoreProcess* dst);
    virtual ~GraphEdgeAdder ();
    void impl_activate () override {};
    void impl_deactivate () override {}
  private:
    CoreProcess *_src, *_dst;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };
}
