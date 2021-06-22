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
#include "core/control/binding.h"
#include "core/control/assignment.h"

#define DJNN_connector_INCLUDED 1


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
    
    CoreConnector (ParentProcess* parent, const djnn::string& name, CoreProcess* src, CoreProcess* dst, bool copy_on_activation=true)
    : CoreConnector (src, dst, copy_on_activation)
    {
      finalize_construction (parent, name);
    }

  protected:
    void impl_activate   () override {  if (_copy_on_activation)  _assignment.perform_action () /* better - instead of calling activate*/;   _binding.activate ();}
    void impl_deactivate () override { /*_assignment.deactivate (); - does nothing so removed */ _binding.deactivate (); }

  private:
    CoreAssignment _assignment;
    CoreBinding _binding;
    bool _copy_on_activation;
public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const djnn::string& format) override;
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
    
    CorePausedConnector (ParentProcess* parent, const djnn::string& name, CoreProcess* src, CoreProcess* dst)
    : CorePausedConnector (src, dst)
    {
      finalize_construction (parent, name);
    }

  protected:
    void impl_activate   () override { _paused_assignment.perform_action () /* better - instead of calling activate*/;  _binding.activate (); }
    void impl_deactivate () override { /*_paused_assignment.deactivate ();- does nothing so removed */ _binding.deactivate (); }

  private:
    CorePausedAssignment _paused_assignment;
    CoreBinding _binding;
public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const djnn::string& format) override;
#endif
  };



  class Connector : public FatProcess
  {
  public:
    Connector (ParentProcess* parent, const djnn::string& name, CoreProcess* src, CoreProcess* dst, bool copy_on_activation=true)
    : FatProcess (name)
    , _assignment (src, dst, false)
    , _binding (src, &_assignment)
    , _copy_on_activation (copy_on_activation)
    {
      // no need to add edge to graph, assignment already did it
      finalize_construction (parent, name);
    }

    // for legacy reason, to get rid of?
    Connector (ParentProcess* parent, const djnn::string& name,
                   CoreProcess* src, const djnn::string& sspec,
                   CoreProcess* dst, const djnn::string& dspec,
                   bool copy_on_activation=true)
    : Connector (parent, name, src->find_child_impl (sspec), dst->find_child_impl (dspec), copy_on_activation)
    {}

  protected:
    void impl_activate   () override { if (_copy_on_activation) _assignment.perform_action ()/* better - instead of calling activate*/; _binding.activate ();}
    void impl_deactivate () override { /* _assignment.deactivate (); - does nothing so removed */ _binding.deactivate (); }

  private:
    CoreAssignment _assignment;
    CoreBinding _binding;
    bool _copy_on_activation;
public:
    Connector* clone () override { /* TBD --- but annoying when loading text from svg */ return nullptr;}
#ifndef DJNN_NO_SERIALIZE
    void serialize (const djnn::string& format) override;
#endif
  };

  class PausedConnector : public FatProcess
  {
  public:
    PausedConnector (ParentProcess* parent, const djnn::string& name, CoreProcess* src, CoreProcess* dst, bool copy_on_activation=true)
    : FatProcess (name)
    , _paused_assignment (src, dst, false)
    , _binding (src, &_paused_assignment)
    , _copy_on_activation (copy_on_activation)
    {
      // no need to add edge to graph, assignment already did it
      finalize_construction (parent, name);
    }

    // for legacy reason, to get rid of?
    PausedConnector (ParentProcess* parent, const djnn::string& name,
                   CoreProcess* src, const djnn::string& sspec,
                   CoreProcess* dst, const djnn::string& dspec)
    : PausedConnector (parent, name, src->find_child_impl (sspec), dst->find_child_impl (dspec))
    {}

  protected:
    void impl_activate   () override { if (_copy_on_activation)  _paused_assignment.activate ()/* better - instead of calling activate*/;  _binding.activate ();}
    void impl_deactivate () override { /* _paused_assignment.deactivate () - does nothing so removed */ _binding.deactivate (); }

  private:
    CorePausedAssignment _paused_assignment;
    CoreBinding _binding;
    bool _copy_on_activation;
public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const djnn::string& format) override;
#endif
  };

  void MultiConnector (ParentProcess* parent, CoreProcess* src, vector <djnn::string> src_props, CoreProcess* dst, vector <djnn::string> dst_props, bool copy_on_activation=true);
  void MultiConnector (ParentProcess* parent, CoreProcess* src, CoreProcess* dst, bool copy_on_activation=true);
}
