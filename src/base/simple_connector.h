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

#define DJNN_SIMPLE_CONNECTOR_INCLUDED 1


namespace djnn { 

  class CoreConnector : public CoreProcess
  {
  public:
    CoreConnector (CoreProcess* src, CoreProcess* dst, bool copy_on_activation=true)
    : _assignment (src, dst, false)
    , _binding (src, &_assignment)
    , _copy_on_activation (copy_on_activation)
    {
      // no need to add edge to graph, assignment already did it
    }
    
    CoreConnector (FatProcess* parent, const std::string& name, CoreProcess* src, CoreProcess* dst, bool copy_on_activation=true)
    : CoreConnector (src, dst, copy_on_activation)
    {
      finalize_construction (parent, name);
    }

  protected:
    void impl_activate   () override { _assignment.activate ();   _binding.activate ();   if (_copy_on_activation) _assignment.notify_activation (); }
    void impl_deactivate () override { _assignment.deactivate (); _binding.deactivate (); }

  private:
    CoreAssignment _assignment;
    CoreBinding _binding;
    bool _copy_on_activation;
public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

  class CorePausedConnector : public CoreProcess
  {
  public:
    CorePausedConnector (CoreProcess* src, CoreProcess* dst)
    : _paused_assignment (src, dst, false)
    , _binding (src, &_paused_assignment)
    {
      // no need to add edge to graph, assignment already did it
    }
    
    CorePausedConnector (FatProcess* parent, const std::string& name, CoreProcess* src, CoreProcess* dst)
    : CorePausedConnector (src, dst)
    {
      finalize_construction (parent, name);
    }

     protected:
    void impl_activate   () override { _paused_assignment.activate ();   _binding.activate (); }
    void impl_deactivate () override { _paused_assignment.deactivate (); _binding.deactivate (); }

  private:
    CorePausedAssignment _paused_assignment;
    CoreBinding _binding;
public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };



  class SimpleConnector : public FatProcess
  {
  public:
    SimpleConnector (FatProcess* parent, const std::string& name, CoreProcess* src, CoreProcess* dst, bool copy_on_activation=true)
    : FatProcess (name)
    , _assignment (src, dst, false)
    , _binding (src, &_assignment)
    , _copy_on_activation (copy_on_activation)
    {
      // no need to add edge to graph, assignment already did it
      finalize_construction (parent, name);
    }

    // for legacy reason, to get rid of?
    SimpleConnector (FatProcess* parent, const std::string& name,
                   CoreProcess* src, const std::string& sspec,
                   CoreProcess* dst, const std::string& dspec,
                   bool copy_on_activation=true)
    : SimpleConnector (parent, name, src->find_child (sspec), dst->find_child (dspec), copy_on_activation)
    {}

  protected:
    void impl_activate   () override { _assignment.activate ();   _binding.activate ();   if (_copy_on_activation) _assignment.notify_activation (); }
    void impl_deactivate () override { _assignment.deactivate (); _binding.deactivate (); }

  private:
    CoreAssignment _assignment;
    CoreBinding _binding;
    bool _copy_on_activation;
public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

  class SimplePausedConnector : public FatProcess
  {
  public:
    SimplePausedConnector (FatProcess* parent, const std::string& name, CoreProcess* src, CoreProcess* dst)
    : FatProcess (name)
    , _paused_assignment (src, dst, false)
    , _binding (src, &_paused_assignment)
    {
      // no need to add edge to graph, assignment already did it
      finalize_construction (parent, name);
    }

    // for legacy reason, to get rid of?
    SimplePausedConnector (FatProcess* parent, const std::string& name,
                   CoreProcess* src, const std::string& sspec,
                   CoreProcess* dst, const std::string& dspec)
    : SimplePausedConnector (parent, name, src->find_child (sspec), dst->find_child (dspec))
    {}

    protected:
    void impl_activate   () override { _paused_assignment.activate ();  _binding.activate (); }
    void impl_deactivate () override { _paused_assignment.deactivate (); _binding.deactivate (); }

  private:
    CorePausedAssignment _paused_assignment;
    CoreBinding _binding;
public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

}