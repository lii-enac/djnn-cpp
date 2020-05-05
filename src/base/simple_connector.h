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
//#include "core/execution/graph.h"
#include "core/control/action.h"
#include "core/control/simple_binding.h"
#include "core/control/simple_assignment.h"

namespace djnn { 

  class SimpleConnector : public Process
  {
  public:
    SimpleConnector (Process* parent, const std::string& name, Process* src, Process* dst, bool copy_on_activation=true)
    : Process (name),
      _assignment (this, "", src, dst, true),
      _binding (this, "", src, &_assignment),
      _copy_on_activation (copy_on_activation)
      {
        //Graph::instance ().add_edge (src, dst); // no need, assignment already did it
        finalize_construction (parent, name);
      }
    ~SimpleConnector () {
        //Graph::instance ().remove_edge (_binding.get_src(), _assignment.get_dst());
    }

    void impl_activate   () override { _assignment.activate (); _binding.activate(); if(_copy_on_activation) _assignment.notify_activation (); }
    void impl_deactivate () override { _assignment.deactivate (); _binding.deactivate(); }

  private:
    SimpleAssignment _assignment;
    SimpleBinding _binding;
    bool _copy_on_activation;
public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };
}