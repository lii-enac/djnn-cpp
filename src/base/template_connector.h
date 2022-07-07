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
#include "core/control/template_assignment.h"

#define DJNN_connector_INCLUDED 1


namespace djnn { 


  template <typename X, typename Y>
  class TCoreConnector : public CoreProcess
  {
  public:
    TCoreConnector (X* src, Y* dst, bool copy_on_activation=true)
    : _assignment (src, dst, false)
    , _binding (src, &_assignment)
    , _copy_on_activation (copy_on_activation)
    {
      // no need to add edge to graph, assignment already did it
    }
    
    TCoreConnector (ParentProcess* parent, const string& name, X* src, Y* dst, bool copy_on_activation=true)
    : TCoreConnector (src, dst, copy_on_activation)
    {
      finalize_construction (parent, name);
    }

  protected:
    void impl_activate   () override {  if (_copy_on_activation)  _assignment.perform_action () /* better - instead of calling activate*/;   _binding.activate ();}
    void impl_deactivate () override { /*_assignment.deactivate (); - does nothing so removed */ _binding.deactivate (); }

  private:
    TCoreAssignment<X,Y> _assignment;
    CoreBinding _binding;
    bool _copy_on_activation;
public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
  };

  template <typename X, typename Y>
  class TCorePausedConnector : public CoreProcess
  {
  public:
    TCorePausedConnector (X* src, Y* dst)
    : _paused_assignment (src, dst, false)
    , _binding (src, &_paused_assignment)
    {
      // no need to add edge to graph, assignment already did it
    }
    
    TCorePausedConnector (ParentProcess* parent, const string& name, X* src, Y* dst)
    : TCorePausedConnector (src, dst)
    {
      finalize_construction (parent, name);
    }

  protected:
    void impl_activate   () override { _paused_assignment.perform_action () /* better - instead of calling activate*/;  _binding.activate (); }
    void impl_deactivate () override { /*_paused_assignment.deactivate ();- does nothing so removed */ _binding.deactivate (); }

  private:
    TCorePausedAssignment<X,Y> _paused_assignment;
    CoreBinding _binding;
public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
  };

  template <typename X, typename Y>
  class TCoreLazyConnector : public CoreProcess
  {
  public:
    TCoreLazyConnector (X* src, Y* dst)
    : _lazy_assignment (src, dst, false)
    , _binding (src, &_lazy_assignment)
    {
      // no need to add edge to graph, assignment already did it
    }
    
    TCoreLazyConnector (ParentProcess* parent, const string& name, X* src, Y* dst)
    : TCoreLazyConnector (src, dst)
    {
      finalize_construction (parent, name);
    }

  protected:
    void impl_activate   () override { _lazy_assignment.perform_action () /* better - instead of calling activate*/;  _binding.activate (); }
    void impl_deactivate () override { /*_paused_assignment.deactivate ();- does nothing so removed */ _binding.deactivate (); }

  private:
    TCoreLazyAssignment<X,Y> _lazy_assignment;
    CoreBinding _binding;
public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
  };


  template <typename X, typename Y>
  class TConnector : public FatProcess
  {
  public:
    TConnector (ParentProcess* parent, const string& name, X* src, Y* dst, bool copy_on_activation=true)
    : FatProcess (name)
    , _assignment (src, dst, false)
    , _binding (src, &_assignment)
    , _copy_on_activation (copy_on_activation)
    {
      // no need to add edge to graph, assignment already did it
      finalize_construction (parent, name);
    }

    // for legacy reason, to get rid of?
    TConnector (ParentProcess* parent, const string& name,
                   X* src, const string& sspec,
                   Y* dst, const string& dspec,
                   bool copy_on_activation=true)
    : TConnector (parent, name, src->find_child_impl (sspec), dst->find_child_impl (dspec), copy_on_activation)
    {}

  protected:
    void impl_activate   () override { if (_copy_on_activation) _assignment.perform_action ()/* better - instead of calling activate*/; _binding.activate ();}
    void impl_deactivate () override { /* _assignment.deactivate (); - does nothing so removed */ _binding.deactivate (); }

  private:
    TCoreAssignment<X,Y> _assignment;
    CoreBinding _binding;
    bool _copy_on_activation;
public:
    //TConnector* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
#ifndef DJNN_NO_SERIALIZE
    //void serialize (const string& format) override;
#endif
  };

  template <typename X, typename Y>
  class TPausedConnector : public FatProcess
  {
  public:
    TPausedConnector (ParentProcess* parent, const string& name, X* src, Y* dst, bool copy_on_activation=true)
    : FatProcess (name)
    , _paused_assignment (src, dst, false)
    , _binding (src, &_paused_assignment)
    , _copy_on_activation (copy_on_activation)
    {
      // no need to add edge to graph, assignment already did it
      finalize_construction (parent, name);
    }

    // for legacy reason, to get rid of?
    TPausedConnector (ParentProcess* parent, const string& name,
                   X* src, const string& sspec,
                   Y* dst, const string& dspec)
    : TPausedConnector (parent, name, src->find_child_impl (sspec), dst->find_child_impl (dspec))
    {}

  protected:
    void impl_activate   () override { if (_copy_on_activation)  _paused_assignment.activate ()/* better - instead of calling activate*/;  _binding.activate ();}
    void impl_deactivate () override { /* _paused_assignment.deactivate () - does nothing so removed */ _binding.deactivate (); }

  private:
    TCorePausedAssignment<X,Y> _paused_assignment;
    CoreBinding _binding;
    bool _copy_on_activation;
public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
  };

  template <typename X, typename Y>
  class TLazyConnector : public FatProcess
  {
  public:
    TLazyConnector (ParentProcess* parent, const string& name, X* src, Y* dst, bool copy_on_activation=true)
    : FatProcess (name)
    , _lazy_assignment (src, dst, false)
    , _binding (src, &_lazy_assignment)
    , _copy_on_activation (copy_on_activation)
    {
      // no need to add edge to graph, assignment already did it
      finalize_construction (parent, name);
    }

    // for legacy reason, to get rid of?
    TLazyConnector (ParentProcess* parent, const string& name,
                   X* src, const string& sspec,
                   Y* dst, const string& dspec)
    : TLazyConnector (parent, name, src->find_child_impl (sspec), dst->find_child_impl (dspec))
    {}

  protected:
    void impl_activate   () override { if (_copy_on_activation)  _lazy_assignment.activate ()/* better - instead of calling activate*/;  _binding.activate ();}
    void impl_deactivate () override { /* _paused_assignment.deactivate () - does nothing so removed */ _binding.deactivate (); }

  private:
    TCoreLazyAssignment<X,Y> _lazy_assignment;
    CoreBinding _binding;
    bool _copy_on_activation;
public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
  };

  template <typename X, typename Y>
  void TMultiConnector (ParentProcess* parent, X* src, vector <string> src_props, Y* dst, vector <string> dst_props, bool copy_on_activation=true);
  
  template <typename X, typename Y>
  void TMultiConnector (ParentProcess* parent, X* src, Y* dst, bool copy_on_activation=true);
}
