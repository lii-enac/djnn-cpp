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

#include "core/tree/remote_property.h"
#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/tree/bool_property.h"
#include "core/tree/spike.h"

#include "exec_env/global_mutex.h"
#include "exec_env/external_source.h"

namespace djnn
{

  /*** Receiver Class ***/

  class Receiver : public FatProcess, public ExternalSource
  {
    class SendAction : public Action
    {
    public:
      SendAction (ParentProcess* parent, const string& name) :
        Action (parent, name) { finalize_construction (parent, name); }
      virtual ~SendAction () {}
      void impl_activate () override;
    };
    class BuildSendAction : public Action
    {
    public:
      BuildSendAction (ParentProcess* parent, const string& name) :
        Action (parent, name), _src (nullptr) { finalize_construction (parent, name); }
      virtual ~BuildSendAction () {}
      void impl_activate () override;
      void set_activation_source (CoreProcess *src) override { _src = src; }
      CoreProcess* get_activation_source () override { return _src; }
      private:
        CoreProcess *_src;
    };
    public:
      Receiver (ParentProcess* parent, const string& name, SOCKET fd, CoreProcess* tree);
      virtual ~Receiver () {}
      void impl_activate () override;
      void impl_deactivate () override;
      void find_and_set_value (string &path, string &value);
      void subscribe (const string& path);
      void unsubscribe (const string& path);
      void unsubscribe_all ();
      typedef map<CoreProcess*, string> dist_map_t;

    protected:
      string get_path (CoreProcess *s);
      vector<CoreProcess*> get_senders () { return _buff_senders; }
      void clear_senders () { _buff_senders.clear (); }
      void add_sender (CoreProcess *sender) { _buff_senders.push_back (sender); }
      SOCKET get_sock () { return _fd; }
    private:
      void run () override;
      SOCKET _fd;
      SendAction _send;
      BuildSendAction _build_send;
      CoreProcess* _tree;

      dist_map_t _senders_map;
      vector<Coupling*> _props_c;
      vector<CoreProcess*> _buff_senders;
  };

  class ProcExporter : public FatProcess, public ExternalSource
  {

  /*** ProcExporter Class ***/

  public:
    ProcExporter (ParentProcess* parent, const string& name, CoreProcess *tree, int port);
    virtual ~ProcExporter ();
    SOCKET get_sock () { return _fd; }
    bool connection ();

  protected:
    void impl_activate () override;
    void impl_deactivate () override;


  private:
    SOCKET _fd;
    int _port;
    CoreProcess* _tree;
    vector<Receiver*> _recvs;
    void run () override;
  };
}
