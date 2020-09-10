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
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "iofd.h"

#include "exec_env/global_mutex.h"
#include "exec_env/main_loop.h"
#include "core/utils/error.h"
#include "core/execution/graph.h"

#include <sys/select.h>

// dbg
// #include <iostream>
// #include "utils/debug.h"
// #include "utils/utils-dev.h"
//#include "utils/ext/debugbreak.h"

namespace djnn {

  IOFD::IOFD(ParentProcess* parent, const std::string& name, int readfd)
  : FatProcess (name),
  ExternalSource(name),
  _readfd(readfd),
  _readable (this, "readable")
  {
    //MainLoop::instance().add_external_source(this);
    finalize_construction(parent, name);
    //"IOFD"+std::to_string(readfd)
  }

  IOFD::~IOFD()
  {
    //MainLoop::instance().remove_external_source(this);
  }

  void
  IOFD::impl_activate ()
  {
    start_thread ();
  }

  void
  IOFD::impl_deactivate ()
  {
    please_stop ();
  }

  void
  IOFD::run ()
  {
    set_please_stop (false);
    // try {
      while (!should_i_stop ()) {
        fd_set reads;
        FD_ZERO (&reads);
        FD_SET (_readfd, &reads);
        int ret = select(_readfd+1, &reads, nullptr, nullptr, nullptr); // blocking call
        if (ret == -1) {
          warning (nullptr, "error reading fd");
          return;
        }
        if (should_i_stop()) {
          return;
        }
        djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
        if (!should_i_stop ()) {
          _readable.activate (); // propagating
          GRAPH_EXEC; // executing
        }
        djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
      }
    // } catch (exception& e) {
    //   warning (nullptr, e.what());
    // }
  }
}
