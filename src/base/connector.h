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

  class Connector : public SrcToDstLink
  {
    friend class PausedConnector;
   
  private:
    class ConnectorAction : public Action
    {
    public:
      ConnectorAction (FatProcess* parent, const std::string& name, AbstractProperty** src, AbstractProperty** dst, bool propagate);
      virtual ~ConnectorAction () {};
      void impl_activate () override;
      void impl_deactivate () override {};
      
    private:
      AbstractProperty** _src;
      AbstractProperty** _dst;
      bool _propagate;
    };

  public:
    Connector (FatProcess *parent, const std::string& name, FatProcess *src, const std::string& ispec, FatProcess *dst, const std::string& dspec, bool copy_on_activation=true,
      std::string src_ref_spec = std::string(), std::string dst_ref_spec = std::string()); // hack to create temporary string objects for init of refs
    Connector (FatProcess *src, const std::string& ispec, FatProcess *dst, const std::string& dspec, bool copy_on_activation=true,
      std::string src_ref_spec = std::string(), std::string dst_ref_spec = std::string()); // hack to create temporary string objects for init of refs
    void impl_activate () override;
    void impl_deactivate () override;
    void update_graph () override;
    void about_to_update_graph () override;
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const std::string& format) override;
#endif
    virtual ~Connector ();

  protected:
    void set_parent (FatProcess* p) override;

    struct Init { Init(Connector *, FatProcess *src, const std::string& ispec, FatProcess *dst, const std::string& dspec,
      std::string& src_ref_spec, std::string& dst_ref_spec); };
    friend struct Init;
    void check_init(const std::string& ispec, const std::string& dspec);

    ref_info _ref_info_src, _ref_info_dst;
    Init _init; // will be "created" third
    AbstractProperty *_src, *_dst;
    ref_update _ref_update_src, _ref_update_dst;
    ConnectorAction _action;
    Coupling _c_src;
    bool _has_coupling;
    bool _copy_on_activation;
  };

  class PausedConnector : public FatProcess
  {
  public:
    PausedConnector (FatProcess *parent, const std::string& name, FatProcess *src, const std::string& ispec, FatProcess *dst, const std::string& dspec, bool copy_on_activation=true);
    PausedConnector (FatProcess *src, const std::string& ispec, FatProcess *dst, const std::string& dspec, bool copy_on_activation=true);
    void impl_activate () override;
    void impl_deactivate () override;
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const std::string& format) override;
#endif
    virtual ~PausedConnector ();

  protected:
    void set_parent (FatProcess* p) override;
    void init_pausedconnector (FatProcess *src, const std::string& ispec, FatProcess *dst, const std::string& dspec);
    AbstractProperty* _src;
    AbstractProperty* _dst;
    Coupling *_c_src;
    Action *_action;
    bool _copy_on_activation;
  };


  // TODO SimpleConnector, with a much smaller size!

}
