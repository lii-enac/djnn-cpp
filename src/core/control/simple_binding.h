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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 */

#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/execution/graph.h"


namespace djnn {

  class SimpleBinding : public FatProcess
  {
  public:
    SimpleBinding (FatProcess* parent, const std::string& name, FatProcess* src, FatProcess* dst)
    : FatProcess (name), _c(src, ACTIVATION, dst, ACTIVATION)
    {
      Graph::instance ().add_edge (src, dst);
      finalize_construction (parent, name);
    }

    SimpleBinding (FatProcess* parent, const std::string& name, FatProcess* src, activation_flag_e src_flag, FatProcess* dst, activation_flag_e dst_flag)
    : FatProcess (name), _c(src, src_flag, dst, dst_flag) {
      Graph::instance ().add_edge (src, dst);
      finalize_construction (parent, name);
    }
    
    ~SimpleBinding () {
      Graph::instance ().remove_edge (get_src(), get_dst());
    }

    void impl_activate   () override { _c.enable  (); };
    void impl_deactivate () override { _c.disable (); }

    CoreProcess * get_src() { return _c.get_src (); } // delegate to coupling to save space
    CoreProcess * get_dst() { return _c.get_dst (); }

  private:
    Coupling _c;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };
}