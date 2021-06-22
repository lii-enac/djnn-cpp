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

#include <sys/event.h>
#include <fcntl.h>
#include "files/darwin/darwin_directory_observer.h"
#include "core/utils/error.h"
#include "files/files.h"

#include <iostream>

namespace djnn
{
  DirectoryObserverData*
  p_init_directory_watcher (const djnn::string& path)
  {
    int kq = kqueue ();
    if (kq == -1) {
      warning (nullptr, "Unable to init directory watcher");
      return nullptr;
    }
    int fd = open(path.c_str (), O_RDONLY);
    if (fd == -1) {
      warning (nullptr, djnn::string ("Unable to find directory " + path));
      return nullptr;
    }
    struct kevent ev;
    EV_SET (&ev, fd, EVFILT_VNODE, EV_ADD | EV_ENABLE | EV_CLEAR, NOTE_WRITE, 0, 0);
    if (kevent(kq, &ev, 1, NULL, 0, NULL) == -1) {
      warning (nullptr, "Unable to init directory watcher");
      return nullptr;
    }
    return new DirectoryObserverData {fd, kq};
  }

  djn_dir_event
  p_run_directory_watcher (DirectoryObserverData *data)
  {
    struct kevent ev;
    if (kevent (data->_kq, NULL, 0, &ev, 1, NULL) != -1) {
      switch (ev.fflags) {
        case NOTE_WRITE:
          return DJN_CHANGE;
          break;
        case NOTE_DELETE:
          return DJN_DELETE;
          break;
        case  NOTE_RENAME:
          return DJN_RENAME;
        break;
        default:
          return DJN_UNKNOWN;
    ;
      }
    }
    return DJN_ERROR;
  }
}
