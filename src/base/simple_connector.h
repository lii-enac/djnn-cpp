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
#include "core/control/action.h"
#include "core/control/simple_binding.h"
#include "core/control/simple_assignment.h"

namespace djnn { 

  class CoreConnector : public CoreProcess
  {
  public:
    CoreConnector (CoreProcess* src, CoreProcess* dst, bool copy_on_activation=true)
    : _assignment (src, dst, true),
      _binding (src, &_assignment),
      _copy_on_activation (copy_on_activation)
      {
        // no need to add edge to graph, assignment already did it
      }

  protected:
    void impl_activate   () override { _assignment.activate (); _binding.activate(); if(_copy_on_activation) _assignment.notify_activation (); }
    void impl_deactivate () override { _assignment.deactivate (); _binding.deactivate(); }

  private:
    CoreAssignment _assignment;
    CoreBinding _binding;
    bool _copy_on_activation;
public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

  class SimpleConnector : public FatProcess
  {
  public:
    SimpleConnector (FatProcess* parent, const std::string& name, CoreProcess* src, CoreProcess* dst, bool copy_on_activation=true)
    : FatProcess (name),
      //_assignment (this, "", src, dst, true),
      _assignment (src, dst, true),
      _binding (src, &_assignment),
      _copy_on_activation (copy_on_activation)
      {
        // no need to add edge to graph, assignment already did it
        finalize_construction (parent, name);
      }

  protected:
    void impl_activate   () override { _assignment.activate (); _binding.activate(); if (_copy_on_activation) _assignment.notify_activation (); }
    void impl_deactivate () override { _assignment.deactivate (); _binding.deactivate(); }

  private:
    //SimpleAssignment _assignment;
    CoreAssignment _assignment;
    CoreBinding _binding;
    bool _copy_on_activation;
public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };
}