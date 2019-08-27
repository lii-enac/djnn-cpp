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

#include "../core/ontology/process.h"
#include "../core/ontology/coupling.h"
#include "../core/control/action.h"
#include "../core/tree/abstract_property.h"
#include "../core/control/assignment.h"
#include "../core/control/action.h"


namespace djnn {
  using namespace std;

  class Connector : public SrcToDstLink
  {
    friend class PausedConnector;
   
  private:
    class ConnectorAction : public Action
    {
    public:
      ConnectorAction (Process* p, const string &n, AbstractProperty** src, AbstractProperty** dst, bool propagate);
      virtual ~ConnectorAction () {};
      void impl_activate () override;
      void impl_deactivate () override {};
      
    private:
      AbstractProperty** _src;
      AbstractProperty** _dst;
      bool _propagate;
    };

  public:
    Connector (Process *p, string n, Process *src, string ispec, Process *dst, string dspec, bool copy_on_activation=true);
    Connector (Process *src, string ispec, Process *dst, string dspec, bool copy_on_activation=true);
    void impl_activate () override;
    void impl_deactivate () override;
    void update_graph () override;
    void serialize (const string& type) override;
    virtual ~Connector ();

  protected:
    void set_parent (Process* p) override;
    void init_connector (Process *src, string ispec, Process *dst, string dspec);
    AbstractProperty* _src;
    AbstractProperty* _dst;
    RefProperty *_ref_src, *_ref_dst;
    Coupling *_c_src;
    Process *_action;
    UpdateSrcOrDst* _update_src, *_update_dst;
    Coupling *_c_update_src, *_c_update_dst;
    bool _has_coupling;
    bool _copy_on_activation;
  };

  class PausedConnector : public Process
  {
  public:
    PausedConnector (Process *p, string n, Process *src, string ispec, Process *dst, string dspec, bool copy_on_activation=true);
    PausedConnector (Process *src, string ispec, Process *dst, string dspec, bool copy_on_activation=true);
    void impl_activate () override;
    void impl_deactivate () override;
    void serialize (const string& type) override;
    virtual ~PausedConnector ();

  protected:
    void set_parent (Process* p) override;
    void init_pausedconnector (Process *src, string ispec, Process *dst, string dspec);
    AbstractProperty* _src;
    AbstractProperty* _dst;
    Coupling *_c_src;
    Process *_action;
    bool _copy_on_activation;
  };
}
