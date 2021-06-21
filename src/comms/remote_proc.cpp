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


#include "remote_proc.h"

#include "core/utils/error.h"
#include "core/utils/to_string.h"

#include <stdio.h>
#include <string.h>


namespace djnn
{
  void
  RemoteProc::SendAction::impl_activate ()
  {
    RemoteProc *p = (RemoteProc*) get_parent ();
    if (!p->connected ())
      return;
    dist_map_t props = p->get_send_props ();
    std::string msg = "N";
    bool to_send = false;
    for (auto const &prop : props) {
      if (prop.second->send()) {
        msg += ":" + prop.first + "=" + prop.second->get_value ();
        prop.second->sent ();
        to_send = true;
      }
    }
    if (!to_send)
      return;
    int sz = msg.size ();
    msg = djnn::to_string(sz) + msg;
    if (send (p->get_sock (), msg.c_str (), strlen (msg.c_str ()), 0) < 0) {
      p->connection_failure ();
    }
  }

  void
  RemoteProc::ReceiveAction::impl_activate ()
  {
    RemoteProc* p = (RemoteProc*) get_parent ();
    if (p->connected ()) {
      p->subscribe_all ();
      start_thread ();
    }
  }

  void
  RemoteProc::ReceiveAction::impl_deactivate ()
  {
    RemoteProc* p = (RemoteProc*) get_parent ();
    p->unsubscribe_all ();
    please_stop ();
  }

  std::vector<std::string> // FIXME copy of vector
  tokenize (char* buff, int sz)
  {
    std::vector<std::string> res;
    std::string msg;
    for (int i = 0; i < sz; i++) {
      if (buff[i] == ':') {
        res.push_back (msg);
        msg = "";
      } else {
        msg += buff[i];
      }
    }
    res.push_back (msg);
    return res;
  }

  void
  RemoteProc::ReceiveAction::run ()
  {
    set_please_stop (false);
    djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
    RemoteProc* p = (RemoteProc*) get_parent ();
    djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
    while (!should_i_stop ()) {
      dist_map_t props = p->get_send_props ();
      char buff_sz[1];
      int sz;
      bool end_sz = false;
      std::string str_sz;
      while (!end_sz) {
        if (recvfrom (p->get_sock (), buff_sz, 1, 0, NULL, NULL) == SOCKET_ERROR) {
          goto fail;
        }
        else {
          if (should_i_stop ())
            return;
          str_sz += buff_sz[0];
        }
        char *endptr[1];
        sz = strtol(str_sz.c_str (), endptr, 0);
        if (strcmp (*endptr,"")) {
          end_sz = true;
        }
      }
      char buffer[sz];
      memset (buffer, 0, sz);
      buffer[0] = str_sz.back ();
      if (recvfrom (p->get_sock (), buffer + 1, sz - 1, 0, NULL, NULL) == SOCKET_ERROR) {
        goto fail;
      }
      if (should_i_stop ())
        return;
      std::vector<std::string> msg = tokenize (buffer, sz);

      djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
      for (auto s : msg) {
        std::size_t found = s.find ('=');
        if (found != std::string::npos) {
          dist_map_t::iterator it = props.find (s.substr(0,found));
          if (it != props.end ())
            it->second->set_incoming_value (s.substr (found + 1), true);
        }
      }
      if (!should_i_stop ()) {
        GRAPH_EXEC; // executing
      }
      djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
    }
    fail:
      p->connection_failure ();
      if (!should_i_stop ()) {
        djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
        GRAPH_EXEC; // executing
        djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
      }
  }

  void
  RemoteProc::subscribe (const std::string& path)
  {
    std::string msg = "S:" + path;
    int sz = msg.size ();
    msg = djnn::to_string(sz) + msg;
    if (send (_fd, msg.c_str (), strlen (msg.c_str ()), 0) == SOCKET_ERROR) {
      connection_failure ();
    }
  }

  void
  RemoteProc::subscribe_all ()
  {
    for (auto const &prop : _send_map) {
      subscribe (prop.first);
     }
  }

  void
  RemoteProc::unsubscribe_all ()
  {
    for (auto const &prop : _send_map) {
      unsubscribe (prop.first);
     }
  }

  void
  RemoteProc::unsubscribe (const std::string& path)
  {
    std::string msg = "U:" + path;
    int sz = msg.size ();
    msg = djnn::to_string(sz) + msg;
    if (send (_fd, msg.c_str (), strlen (msg.c_str ()), 0) == SOCKET_ERROR) {
      connection_failure ();
    }
  }

  RemoteProc::RemoteProc (ParentProcess *parent, const std::string &name, const std::string &addr, int port) :
      FatProcess (name), _addr (addr), _port (port), _fd (0), _send (this, "send"), _receive (this, "receive"),
      _con (this, "connection_action"), _con_status (this, "status", false), _con_req (this, "connect"),
      _c_con_req (&_con_req, ACTIVATION, &_con, ACTIVATION)
  {
    finalize_construction (parent, name);
  }

  RemoteProc::~RemoteProc ()
  {
    _receive.deactivate ();
    for (auto c: _props_c) {
      CoreProcess* src = c->get_src ();
      delete c;
      delete src;
    }
  }

  FatChildProcess*
  RemoteProc::find_child_impl (const std::string &path)
  {
    FatChildProcess* res = FatProcess::find_child_impl (path);
    if (res)
      return res;
    dist_map_t::iterator it = _send_map.find (path);
    if (it != _send_map.end ())
      return it->second;
    else {
      RemoteProperty *p = new RemoteProperty (nullptr, "", "");
      _send_map[path] = p;

      Coupling *c = new Coupling (p, ACTIVATION, &_send, ACTIVATION);
      if (is_activated ()) {
        c->enable ();
        subscribe (path);
      }
      _props_c.push_back (c);

      return p;
    }
  }

  void
  RemoteProc::impl_activate ()
  {
    connection ();
    for (auto c: _props_c)
      c->enable ();
    _c_con_req.enable ();
  }

  void
  RemoteProc::connection ()
  {
    if (_con_status.get_value () == true)
      return;

    _fd = socket_open_client (_addr.c_str (), _port);
    if (_fd<0) {
      _con_status.set_value (false, true);
    } else {
      _con_status.set_value (true, true);
      _receive.activate ();
    }
  }

  void
  RemoteProc::impl_deactivate ()
  {
    for (auto c: _props_c)
      c->disable ();
    _receive.deactivate ();
    _c_con_req.disable ();
    socket_close (_fd);
  }
}
