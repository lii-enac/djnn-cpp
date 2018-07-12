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
#include <linux/input.h>
#include <libevdev/libevdev.h>

#include "../input-priv.h"
#include "../../core/syshook/unix/iofd.h"

namespace djnn {
  enum dev_type {
    MOUSE, KEYBOARD, TOUCH_PANEL
  };

  class LinuxDevice : public Process {
  public:
    LinuxDevice (Process *p, const string& n, dev_type type) : Process (p, n), _type (type) {}
    ~LinuxDevice () {}
    virtual void handle_event (struct input_event *ev) = 0;
    dev_type type () { return _type; }
  private:
    dev_type _type;
  };

  unique_ptr<LinuxDevice> map_device (const struct libevdev *_dev, const string &n);

  class Evdev {
    private:
    class EvdevAction: public Process {
      public:
        EvdevAction (Evdev* evdev) :
        Process (), _evdev (evdev) {}
        virtual ~EvdevAction () {}
        void activate ()
        {
          _evdev->handle_evdev_msg ();
        }
        void deactivate () {}
      private:
        Evdev* _evdev;
    };
  public:
    Evdev (const char* node);
    ~Evdev ();
    Process* action () { return _action.get (); }
    void handle_evdev_msg ();
  private:
    shared_ptr<Process> _action;
    unique_ptr<IOFD> _iofd;
    unique_ptr<LinuxDevice> _djn_dev;
    unique_ptr<Coupling> _readable_cpl;
    string _name;
    struct libevdev *_dev;
    int _fd;
    bool _aborted;
  };

	class Udev {
  private:
    class UdevAction: public Process {
      public:
        UdevAction (Udev* udev) :
        Process (), _udev (udev) {}
        virtual ~UdevAction () {}
        void activate ()
        {
          _udev->handle_udev_msg ();
        }
        void deactivate () {}
      private:
        Udev* _udev;
    };
  public:
    Udev ();
    ~Udev ();
    void handle_udev_msg ();
    void handle_input_device (struct udev_device*);
  private:
		IOFD* _udev_iofd;
    struct udev *_udev_connection;
    struct udev_monitor *_udev_mon;
    map<string, Evdev*> _sysname_to_dev;
    shared_ptr<UdevAction> _action;
    unique_ptr<Coupling> _readable_cpl;
	};

  class LinuxMouse : public LinuxDevice
  {
  public:
    LinuxMouse (Process *p, const string &n, const struct libevdev *dev);
    ~LinuxMouse ();
    void mouse_btn_event (const char* name, int val);
    void activate () override {}
    void deactivate () override {}
    void handle_event (struct input_event *ev) override;
  private:
    unique_ptr<Spike> _move, _btn, _press, _release, _wheel;
    unique_ptr<IntProperty> _move_dx, _move_dy, _wheel_dx, _wheel_dy;
    unique_ptr<TextProperty> _btn_name;
  };
}
