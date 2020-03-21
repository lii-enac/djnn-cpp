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

#include <libudev.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

#include "display/display-priv.h"
#include "exec_env/unix/iofd.h"
#include "core/ontology/coupling.h"
#include "core/tree/int_property.h"
#include "core/tree/bool_property.h"

namespace djnn {
  typedef struct buff {
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    uint32_t size;
    uint32_t handle;
    uint8_t *map;
    uint32_t fb;
  } buff;

  class DRMConnector : public Process {
    private:
    class UpdatePosAction : public Action
    {
    public:
      UpdatePosAction (Process* parent, const std::string& name) : Action(parent, name) { Process::finalize_construction (parent, name); }
      virtual ~UpdatePosAction () {}
      void impl_activate () { ((DRMConnector*) get_parent())->update_pos (); }
      void impl_deactivate () {}
    };
  public:
    DRMConnector (Process *p, const std::string& name, int fd, drmModeRes *res, drmModeConnector *conn);
    ~DRMConnector ();
    void impl_activate () {}
    void impl_deactivate () {}
    int crtc () { return _crtc_id; }
    void set_crtc (int crtc) { _crtc_id = crtc; }
    void clean ();
    void update_pos ();
    void page_flip ();
    void vblank_event ();
    void init_connection (drmModeRes *res, drmModeConnector *drm_conn);
    bool is_waiting_vblank () { return _waiting_vblank; }
    bool is_connected () { return _connected.get_value (); }
    Process* get_vblank () { return &_vblank; }
    buff* get_next_buff ();
    int fd () { return _fd; }

  private:
    int get_crtc (drmModeRes *res, drmModeConnector *drm_conn);
    int init_fb (buff* b);
    void destroy (int i);
    int _fd;
    int _crtc_id;
    int _cur_buff;
    uint32_t _conn;
    bool _waiting_vblank;
    buff _buffs[2];
    drmModeModeInfo _mode;
    drmModeCrtc *_saved_crtc;
    IntProperty _width, _height, _x, _y;
    BoolProperty _connected;
    UpdatePosAction _pos_action;
    Spike _vblank;
    Coupling _c_update_pos_x, _c_update_pos_y;
  };

  extern  std::vector<DRMConnector*> conn_list;

  class DRMDevice : public Process {
    class VBlankAction : public Action
    {
    public:
      VBlankAction (Process* parent, const std::string& name) : Action(parent, name) { Process::finalize_construction (parent, name); }
      virtual ~VBlankAction () {}
      void impl_activate () { ((DRMDevice*) get_parent())->vblank (); }
      void impl_deactivate () {}
    };
  public:
    DRMDevice (Process *p, const std::string& name, int fd, int min_width, int max_width, int min_height, int max_height);
    ~DRMDevice ();
    void impl_activate () {}
    void impl_deactivate () {}
    int fd () { return _fd; }
    void vblank ();
  private:
    int _fd;
    IOFD _iofd;
    VBlankAction _vblank_action;
    Coupling _c_vblank;
  };

  class DRMUdev {
    private:
      class DRMUdevAction: public Action {
        public:
          DRMUdevAction (DRMUdev* udev) :
            Action (nullptr, "DRMUdevAction"), _udev (udev) {}
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
