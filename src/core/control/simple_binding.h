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
	// this is what could a SimpleBinding ressemble, which a much smaller size: TODO use it in smala!
  class SimpleBinding : public Process
  {
  public:
    SimpleBinding (Process* parent, const std::string& name, Process* src, Process* dst)
    : Process (name), _c(src, ACTIVATION, dst, ACTIVATION)
    {
      Graph::instance ().add_edge (src, dst);
      finalize_construction (parent, name);
    }

    SimpleBinding (Process* parent, const std::string& name, Process* src, activation_flag_e src_flag, Process* dst, activation_flag_e dst_flag)
    : Process (name), _c(src, src_flag, dst, dst_flag) {
      Graph::instance ().add_edge (src, dst);
      finalize_construction (parent, name);
    }
    
    ~SimpleBinding () {
      Graph::instance ().remove_edge (get_src(), get_dst());
    }

    void impl_activate   () override { _c.enable  (); };
    void impl_deactivate () override { _c.disable (); }

    Process* get_src() { return _c.get_src (); } // delegate to coupling to save space
    Process* get_dst() { return _c.get_dst (); }

  private:
    //void set_parent (Process* p) override;

    //Process *_src
    //Process *_dst;
    //BindingAction _action;
    Coupling _c;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };
}