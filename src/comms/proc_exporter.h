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

#include "../core/tree/remote_property.h"
#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/execution/graph.h"
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
      SendAction (ParentProcess* parent, const std::string& name) :
        Action (parent, name) { finalize_construction (parent, name); }
      virtual ~SendAction () {}
      void impl_activate () override;
    };
    class BuildSendAction : public Action
    {
    public:
      BuildSendAction (ParentProcess* parent, const std::string& name) :
        Action (parent, name), _src (nullptr) { finalize_construction (parent, name); }
      virtual ~BuildSendAction () {}
      void impl_activate () override;
      void set_activation_source (CoreProcess *src) override { _src = src; }
      CoreProcess* get_activation_source () override { return _src; }
      private:
        CoreProcess *_src;
    };
    public:
      Receiver (ParentProcess* parent, const std::string& name, int fd, CoreProcess* tree);
      virtual ~Receiver () {}
      void impl_activate () override;
      void impl_deactivate () override;
      void find_and_set_value (std::string &path, std::string &value);
      void subscribe (const std::string& path);
      void unsubscribe (const std::string& path);
      void unsubscribe_all ();
      typedef std::map<CoreProcess*, std::string> dist_map_t;

    protected:
      std::string get_path (CoreProcess *s);
      std::vector<CoreProcess*> get_senders () { return _buff_senders; }
      void clear_senders () { _buff_senders.clear (); }
      void add_sender (CoreProcess *sender) { _buff_senders.push_back (sender); }
      int get_sock () { return _fd; }
    private:
      void run () override;
      int _fd;
      SendAction _send;
      BuildSendAction _build_send;
      CoreProcess* _tree;

      dist_map_t _senders_map;
      std::vector<Coupling*> _props_c;
      std::vector<CoreProcess*> _buff_senders;
  };

  class ProcExporter : public FatProcess, public ExternalSource
  {

  /*** ProcExporter Class ***/

  public:
    ProcExporter (ParentProcess* parent, const std::string& name, CoreProcess *tree, int port);
    virtual ~ProcExporter ();
    int get_sock () { return _fd; }
    bool connection ();

  protected:
    void impl_activate () override;
    void impl_deactivate () override;


  private:
    int _fd;
    int _port;
    CoreProcess* _tree;
    std::vector<Receiver*> _recvs;
    void run () override;
  };
}
