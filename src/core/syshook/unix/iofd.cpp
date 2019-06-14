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

#include "../syshook.h"
#include "../../utils/error.h"
#include "../../execution/graph.h"

#include <sys/select.h>

namespace djnn {

  IOFD::IOFD(int readfd)
  : _readfd(readfd)
  {
    _readable = new Spike (this, "readable");
  }

  void
  IOFD::activate ()
  {
    please_stop ();
    start_thread ();
  }

  void
  IOFD::deactivate ()
  {
    please_stop ();
  }

  void
  IOFD::run ()
  {
    set_please_stop (false);
    try {
      while (!get_please_stop ()) {
        fd_set reads;
        FD_ZERO (&reads);
        FD_SET (_readfd, &reads);
        int ret = select(_readfd+1, &reads, nullptr, nullptr, nullptr); // blocking call
        if (ret == -1) {
          warning (nullptr, "error reading fd");
          return;
        }
        djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
        if (!get_please_stop ()) {
          _readable->activation (); // propagating
         GRAPH_EXEC; // executing
        }
        djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
      }
    } catch (exception& e) {
      warning (nullptr, e.what());
    }
  }
}
