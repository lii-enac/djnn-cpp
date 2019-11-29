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
 *
 */

#pragma once

#include <libudev.h>


#include "display/display-priv.h"
#include "core/syshook/unix/iofd.h"

namespace djnn {

  class DRMConnector : public Process {
  public:
    DRMConnector (Process *p, const string& name, int id, bool connected);
    ~DRMConnector ();
    void impl_activate () {}
    void impl_deactivate () {}
    void set_display (Process* display) { _display = display; }
  private:
    Process* _display;
    int _id;
  };

  class DRMDevice : public Process {
  public:
    DRMDevice (Process *p, const string& name, int fd, int min_width, int max_width, int min_height, int max_height);
    ~DRMDevice ();
    void impl_activate () {}
    void impl_deactivate () {}
    int fd () { return _fd; }
  private:
    int _fd;
  };

  class DRMUdev {
    private:
      class DRMUdevAction: public Action {
        public:
          DRMUdevAction (DRMUdev* udev) :
            Action (), _udev (udev) {}
          virtual ~DRMUdevAction () {}
          void impl_activate ()
          {
            _udev->handle_udev_msg ();
          }
          void impl_deactivate () {}
        private:
          DRMUdev* _udev;
      };
    public:
      DRMUdev ();
      ~DRMUdev ();
      void handle_udev_msg ();
      void handle_drm_device (const char*, struct udev_device*);
      void update_drm_device (const char*, struct udev_device*);
    private:
      IOFD* _udev_iofd;
      struct udev *_udev_connection;
      struct udev_monitor *_udev_mon;
      DRMUdevAction *_action;
      Coupling *_readable_cpl;
    };
}
