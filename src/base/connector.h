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

#include "core/control/src_to_dst_link.h"
#include "core/ontology/coupling.h"
#include "core/tree/abstract_property.h"

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
    Connector (Process *p, const string& n, Process *src, const string& ispec, Process *dst, const string& dspec, bool copy_on_activation=true,
      string src_ref_spec = string(), string dst_ref_spec = string()); // hack to create temporary string objects for init of refs
    Connector (Process *src, const string& ispec, Process *dst, const string& dspec, bool copy_on_activation=true,
      string src_ref_spec = string(), string dst_ref_spec = string()); // hack to create temporary string objects for init of refs
    void impl_activate () override;
    void impl_deactivate () override;
    void update_graph () override;
    void about_to_update_graph () override;
    void serialize (const string& type) override;
    virtual ~Connector ();

  protected:
    void set_parent (Process* p) override;

    struct Init { Init(Connector *, Process *src, const string& ispec, Process *dst, const string& dspec,
      string& src_ref_spec, string& dst_ref_spec); };
    friend struct Init;
    void check_init(const string& ispec, const string& dspec);

    ref_info _ref_info_src, _ref_info_dst;
    Init _init; // will be "created" third
    AbstractProperty *_src, *_dst;
    ref_update _ref_update_src, _ref_update_dst;
    ConnectorAction _action;
    Coupling _c_src;
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
