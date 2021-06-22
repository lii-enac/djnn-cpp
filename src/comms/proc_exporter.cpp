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


#include "proc_exporter.h"

#include "core/utils/error.h"
#include "core/utils/to_string.h"

#include <stdio.h>
#include <string.h>

namespace djnn
{
  void
  Receiver::SendAction::impl_activate ()
  {
    Receiver *p = (Receiver*) get_parent();
    vector<CoreProcess*> senders = p->get_senders();
    string msg;
    string start = "";
    for (auto s : senders) {
      string path = p->get_path(s);
      string value;
      AbstractProperty *ap = dynamic_cast<AbstractProperty*>(s);
      if (ap)
        value = ap->get_string_value();
      else {
        value = s->somehow_activating () ? "ACTIVATION" : "DEACTIVATION";
      }
      msg += start + path + "=" + value;
      start = ":";
    }
    p->clear_senders();
    int sz = msg.size();
    msg = djnn::to_string(sz) + msg;
    if (send(p->get_sock(), msg.c_str(), strlen(msg.c_str()), 0) == SOCKET_ERROR) {
      warning (this, "Failed to send message: " + msg);
    }
  }

  void
  Receiver::BuildSendAction::impl_activate ()
  {
    Receiver *p = (Receiver*) get_parent ();
    p->add_sender (_src);
  }

  Receiver::Receiver (ParentProcess* parent, const string& name, SOCKET fd, CoreProcess* tree) : FatProcess (name), ExternalSource (name),
      _fd (fd),_send (this, "send"), _build_send (this, "build_send"), _tree (tree)
  {
  }

  void
  Receiver::impl_activate ()
  {
    start_thread ();
  }

  void
  Receiver::impl_deactivate ()
  {
    please_stop ();
    socket_close (_fd);
  }

  void
  Receiver::find_and_set_value (string &path, string &value)
  {
    CoreProcess *cp = _tree->find_child (path);
    if (cp) {
      AbstractProperty *ap = dynamic_cast<AbstractProperty*> (cp);
      if (ap)
        ap->set_value (value, true);
      else {
        if (value == "ACTIVATION")
          cp->set_activation_flag (ACTIVATION);
        else if (value == "DEACTIVATION")
          cp->set_activation_flag (DEACTIVATION);
      }
    }
  }

  void
  Receiver::subscribe (const string &path)
  {
    CoreProcess *cp = _tree->find_child (path);
    if (!cp)
      return;
    _senders_map[cp] = path;
    Coupling *c = new Coupling (cp, ACTIVATION, &_send, ACTIVATION);
    Coupling *c2 = new Coupling (cp, ACTIVATION, &_build_send, ACTIVATION, true);
    c->enable ();
    _props_c.push_back (c);
    _props_c.push_back (c2);
    AbstractProperty *ap = dynamic_cast<AbstractProperty*> (cp);
    /* if not a property we may also be interested by the deactivation */
    if (!ap) {
      c = new Coupling (cp, DEACTIVATION, &_send, ACTIVATION);
      c2 = new Coupling (cp, DEACTIVATION, &_build_send, ACTIVATION, true);
      c->enable ();
      _props_c.push_back (c);
      _props_c.push_back (c2);
    }
  }

  void
  Receiver::unsubscribe (const string &path)
  {
    CoreProcess *cp = _tree->find_child (path);
    bool found = false;
    if (!cp)
      return;
    vector<Coupling*>::iterator  it;
    for (it = _props_c.begin (); it != _props_c.end ();) {
      if ((*it)->get_src () == cp) {
        if (!found) {
          found = true;
        }
        delete *it;
        it = _props_c.erase (it);
      } else {
        ++it;
      }
    }
  }

  void
  Receiver::unsubscribe_all ()
  {
    for (auto const &s : _senders_map) {
      unsubscribe (s.second);
    }
  }

  string
  Receiver::get_path (CoreProcess *s)
  {
    string res = "";
    dist_map_t::iterator it = _senders_map.find (s);
    if (it != _senders_map.end ())
      res = it->second;
    return res;
  }

  extern vector<string>
  tokenize (char* buff, int sz);

  void
  Receiver::run ()
  {
    set_please_stop (false);
    while (!should_i_stop ()) {
      char buff_sz[1];
      int sz;
      bool end_sz = false;
      string str_sz;
      while (!end_sz) {
        if (recvfrom (_fd, buff_sz, 1, 0, NULL, NULL) == SOCKET_ERROR) {
          goto fail;
        }
        else
          str_sz += buff_sz[0];
        char *endptr[1];
        sz = strtol(str_sz.c_str (), endptr, 0);
        if (strcmp (*endptr,"")) {
          end_sz = true;
        }
      }
      char buffer[sz];
      memset (buffer, 0, sz);
      buffer[0] = str_sz.back ();
      if (recvfrom (_fd, buffer + 1, sz - 1, 0, NULL, NULL) == SOCKET_ERROR) {
        goto fail;
      }
      vector < string > msg = tokenize (buffer, sz);
      djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
      if (msg[0] == "N") {
        for (size_t i = 1; i < msg.size (); ++i) {
          std::size_t found = msg[i].find ('=');
          if (found != string::npos) {
            string path = msg[i].substr (0, found);
            string value = msg[i].substr (found + 1);
            find_and_set_value (path, value);
          }
        }
        if (!should_i_stop ()) {
          GRAPH_EXEC; // executing
        }

      } else if (msg[0] == "S") {
        for (size_t i = 1; i < msg.size (); ++i) {
          subscribe (msg[i]);
        }
      } else if (msg[0] == "U") {
        for (size_t i = 1; i < msg.size (); ++i) {
          unsubscribe (msg[i]);
        }
      } else {
        warning (this, "unknown msg: " + msg[1]);
      }
      djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
    }
    fail:
      djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
      unsubscribe_all ();
      this->schedule_delete ();
      if (!should_i_stop ()) {
        GRAPH_EXEC; // executing
      }
      djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
  }

  ProcExporter::ProcExporter (ParentProcess *parent, const string &name, CoreProcess *tree, int port) :
      FatProcess (name), ExternalSource (name), _fd (0), _port (port), _tree (tree)
  {
    finalize_construction (parent, name);
  }

  ProcExporter::~ProcExporter ()
  {
    if (somehow_activating ()) {
      please_stop ();
      for (auto r: _recvs) {
        r->deactivate ();
        delete r;
      }
      socket_close (_fd);
    }
  }

  void
  ProcExporter::impl_activate ()
  {
    if (connection ())
      start_thread ();
  }

  void
  ProcExporter::impl_deactivate ()
  {
    please_stop ();
    for (auto r: _recvs) {
      r->deactivate ();
      delete r;
    }
    socket_close (_fd);
  }

  bool
  ProcExporter::connection ()
  {
    _fd = socket_open_server (_port);
    return _fd > 0;
  }

  void
  ProcExporter::run ()
  {
    set_please_stop (false);
    struct sockaddr_in cli_addr;
    socklen_t clilen;

    listen(_fd, 5);
    clilen = sizeof (cli_addr);
    while (!should_i_stop ()) {
      SOCKET newsockfd = accept (_fd, (struct sockaddr*) &cli_addr, &clilen);
      Receiver* recv = new Receiver (nullptr, "", newsockfd, _tree);
      _recvs.push_back (recv);
      djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
      recv->activate ();
      djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
    }
  }
}
