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

#include <sys/select.h>

namespace djnn {

  IOFD::IOFD(int readfd)
  : _readfd(readfd)
  {
    _readable = shared_ptr<Process> (new Spike (this, "readable"));
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
    set_please_stop (true);
  }

  void
  IOFD::run ()
  {
    while (!get_please_stop ()) {
      fd_set reads;
      FD_ZERO (&reads);
      FD_SET (_readfd, &reads);

      select(_readfd+1, &reads, nullptr, nullptr, nullptr); // blocking call

      djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
      if (!get_please_stop ()) {
        _readable.get()->activation (); // propagating
        Graph::instance ().exec (); // executing
      }
      djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
    }
  }
}
