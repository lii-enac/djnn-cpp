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
#include "core/control/action.h"
#include "core/ontology/coupling.h"
#include "core/tree/int_property.h"
#include "core/tree/bool_property.h"
#include "core/control/action.h"

// dbg
//#include <__iostream>
//#include "utils/debug.h"

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

  class DRMConnector : public FatProcess {
    private:
    class UpdatePosAction : public Action
    {
    public:
      UpdatePosAction (ParentProcess* parent, const std::string& name) : Action(parent, name) { finalize_construction (parent, name); }
      virtual ~UpdatePosAction () {}
      void impl_activate () { ((DRMConnector*) get_parent())->update_pos (); }
    };
  public:
    DRMConnector (ParentProcess* parent, const std::string& name, int fd, drmModeRes *res, drmModeConnector *conn);
    ~DRMConnector ();
    void impl_activate () {}
    void impl_deactivate () {}
    int get_crtc () { return _crtc_id; }
    void set_crtc (int crtc) { _crtc_id = crtc; }
    void clean ();
    void update_pos ();
    void update_pos_fb (uint32_t fb);
    void flip_page ();
    void flip_page_fb (uint32_t fb);
    void handle_vblank ();
    void init_connection (drmModeRes *res, drmModeConnector *drm_conn);
    int init_connection_crtc(uint32_t fb);
    bool is_waiting_for_vblank () const { return _waiting_vblank; }
    bool is_connected () { return _connected.get_value (); }
    FatProcess* get_vblank () { return &_vblank; }
    buff* get_next_buff ();
    int get_fd () { return _fd; }
    int get_cur_buff_id () { return _cur_buff; }
    int get_next_buff_id () { return _cur_buff^1; }

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

  class DRMDevice : public FatProcess {
    class VBlankAction : public Action
    {
    public:
      VBlankAction (ParentProcess* parent, const std::string& name) : Action(parent, name) { finalize_construction (parent, name); }
      virtual ~VBlankAction () {}
      void impl_activate () { static_cast<DRMDevice*>(get_parent())->read_vblank (); }
    };
  public:
    DRMDevice (ParentProcess* parent, const std::string& name, int fd, int min_width, int max_width, int min_height, int max_height);
    ~DRMDevice ();
    void impl_activate () {}
    void impl_deactivate () {}
    int get_fd () { return _fd; }
    void read_vblank ();
  private:
    int _fd;
    IOFD _iofd;
    VBlankAction _read_vblank_action;
    Coupling _c_read_vblank;
  };

  class DRMUdev {
    private:
      class DRMUdevAction : public Action {
        public:
          DRMUdevAction (DRMUdev* udev) :
            Action (nullptr, "DRMUdevAction"), _udev (udev) {}
          virtual ~DRMUdevAction () {}
          void impl_activate ()
          {
            _udev->handle_udev_msg ();
          }
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
