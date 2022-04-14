/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "socket.h"

#include "core/property/remote_property.h"
#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/property/bool_property.h"
#include "core/tree/spike.h"

#include "exec_env/global_mutex.h"
#include "exec_env/external_source.h"


namespace djnn
{

  class RemoteProc : public FatProcess
  {

  /*** private Class SendAction ***/
  private:
    class SendAction : public Action
    {
    public:
      SendAction (ParentProcess* parent, const string& name) :
        Action (parent, name) { finalize_construction (parent, name); }
      virtual ~SendAction () {}
      void impl_activate () override;
    };
    class ConnectionAction : public Action
    {
      public:
        ConnectionAction (ParentProcess* parent, const string& name) :
          Action (parent, name) { finalize_construction (parent, name); }
        virtual ~ConnectionAction () {}
        void impl_activate () override { ((RemoteProc*)get_parent ())->connection ();}
    };
    class ReceiveAction : public Action, public ExternalSource
    {
    public:
      ReceiveAction (ParentProcess* parent, const string& name) :
        Action (parent, name), ExternalSource (name) { finalize_construction (parent, name); }
      virtual ~ReceiveAction () {}
      void impl_activate () override;
      void impl_deactivate () override;
      virtual bool pre_activate () override {
        if (get_parent () != nullptr && !get_parent ()->somehow_activating () )
          return false;
        set_activation_state(ACTIVATING);
        return true;
      }
      void post_activate () override {

      }
      private:
        void run () override;
    };


  /*** RemoteProc Class ***/

  public:
     RemoteProc (ParentProcess* parent, const string& name,
      const string& addr="224.1.2.3", int port = 2010);

    virtual ~RemoteProc ();

    typedef map<string, RemoteProperty*> dist_map_t;

    FatChildProcess* find_child_impl (const string&) override;
    string& get_addr () { return _addr; }
    int get_port () { return _port; }
    void connection ();
  protected:
    void impl_activate () override;
    void impl_deactivate () override;
    void subscribe (const string& path);
    void subscribe_all ();
    void unsubscribe (const string& path);
    void unsubscribe_all ();
    dist_map_t get_send_props () { return _send_map; }
    SOCKET get_sock () { return _fd; }
    void connection_failure () { _con_status.set_value (false, true); }
    bool connected () { return _con_status.get_value (); }
  private:
    string _addr;
    int _port;
    SOCKET _fd;
    SendAction _send;
    ReceiveAction _receive;
    ConnectionAction _con;
    BoolProperty _con_status;
    Spike _con_req;
    Coupling _c_con_req;
    dist_map_t _send_map;
    vector<Coupling*> _props_c;
  };
}
