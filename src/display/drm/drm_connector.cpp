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

#include "display/drm/drm_display.h"
#include "core/tree/bool_property.h"
#include "core/tree/int_property.h"
#include "core/tree/set.h"
#include "core/ontology/coupling.h"
#include "core/utils/error.h"
#include "core/execution/graph.h"

#include <string.h>
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#ifndef DEBUG
#define DEBUG 0
#endif

#include <iostream>
#include "utils/debug.h"

using namespace std;

namespace djnn {

  std::vector<DRMConnector*> conn_list;

  void
  p_clear_display () {
    for (auto d: conn_list) {
      delete d;
    }
  }

  int
  DRMConnector::get_crtc (drmModeRes *res, drmModeConnector *drm_conn)
  {
    drmModeEncoder *enc = nullptr;
    int i, j;
    int crtc;
    if (drm_conn->encoder_id)
      enc = drmModeGetEncoder (_fd, drm_conn->encoder_id);
    if (enc) {
      if (enc->crtc_id) {
        crtc = enc->crtc_id;
        /* check that crtc is not already in use by a previously initialized connector */
        for (auto c: conn_list) {
          if (c->get_crtc() == crtc) {
            crtc = -1;
            break;
          }
        }
        if (crtc >= 0) {
          drmModeFreeEncoder (enc);
          _crtc_id = crtc;
          return 1;
        }
      }
      drmModeFreeEncoder (enc);
    }
    for (i = 0; i < drm_conn->count_encoders; ++i) {
      enc = drmModeGetEncoder (_fd, drm_conn->encoders[i]);
      if (!enc)
        continue;
      for (j = 0; j < res->count_crtcs; ++j) {
        if (!(enc->possible_crtcs & (1 << j)))
          continue;
        crtc = res->crtcs[j];
        for (auto c: conn_list) {
          if (c->get_crtc() == crtc) {
            crtc = -1;
            break;
          }
        }
        if (crtc >= 0) {
          drmModeFreeEncoder (enc);
          _crtc_id = crtc;
          return 1;
        }
      }
      drmModeFreeEncoder (enc);
    }
    return 0;
  }

  int
  DRMConnector::init_fb (buff* b )
  {
    struct drm_mode_create_dumb creq;
    struct drm_mode_destroy_dumb dreq;
    struct drm_mode_map_dumb mreq;
    int success = 0;

    memset (&creq, 0, sizeof(creq));
    creq.width = b->width;
    creq.height = b->height;
    creq.bpp = 32;
    success = drmIoctl (_fd, DRM_IOCTL_MODE_CREATE_DUMB, &creq);
    if (success < 0) {
      cerr << "failed to create framebuffer\n";
      return -1;
    }
    b->stride = creq.pitch;
    b->size = creq.size;
    b->handle = creq.handle;
    success = drmModeAddFB (_fd, b->width, b->height, 24, 32, b->stride,
      b->handle, &b->fb);
    if (success < 0) {
      cerr << "failed to create framebuffer\n";
      goto err_destroy;
    }
    memset (&mreq, 0, sizeof (mreq));
    mreq.handle = b->handle;
    success = drmIoctl (_fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq);
    if (success < 0) {
      cerr << "failed to create framebuffer\n";
      goto err_fb;
    }
    b->map = (uint8_t*) mmap (0, b->size, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, mreq.offset);
    if (b->map == MAP_FAILED) {
      cerr << "failed to create framebuffer\n";
      goto err_fb;
    }
    memset (b->map, 0, b->size);
    return 1;

    err_fb:
      drmModeRmFB (_fd, b->fb);
    err_destroy:
      memset (b->map, 0, b->size);
      dreq.handle = b->handle;
      drmIoctl (_fd, DRM_IOCTL_MODE_DESTROY_DUMB, &dreq);
      return -1;
  }

  void
  DRMConnector::init_connection (drmModeRes *res, drmModeConnector *drm_conn)
  {
    memcpy(&_mode, &drm_conn->modes[0], sizeof(_mode));
    _buffs[0].width = drm_conn->modes[0].hdisplay;
    _buffs[1].width = drm_conn->modes[0].hdisplay;
    _buffs[0].height = drm_conn->modes[0].vdisplay;
    _buffs[1].height = drm_conn->modes[0].vdisplay;
    _width.set_value (drm_conn->modes[0].hdisplay, false);
    _height.set_value (drm_conn->modes[0].vdisplay, false);
#if DEBUG
    cout << "available modes:\n";
    for (int i = 0; i < drm_conn->count_modes; i++) {
      cout << "\thdisplay: "  << drm_conn->modes[i].hdisplay << endl;
      cout << "\tvdisplay: "  << drm_conn->modes[i].vdisplay << endl;
      cout << "\thsync: " << drm_conn->modes[i].hsync_start << " " << drm_conn->modes[i].hsync_end << endl;
      cout << "\tvsync: " << drm_conn->modes[i].vsync_start << " " << drm_conn->modes[i].vsync_end << endl;
      cout << "\tclock: " << drm_conn->modes[i].clock << endl;
      cout << "\tvrefresh: " << drm_conn->modes[i].vrefresh << endl;
    }
#endif

    int ret = get_crtc(res, drm_conn);
    if (!ret) {
      error (this, string("no valid crtc for connector ") + to_string(
      drm_conn->connector_id));
    }

    ret = init_fb(&_buffs[0]);
    ret &= init_fb(&_buffs[1]);
    if (!ret) {
      error (this, string("Cannot create framebuffer for connector ") + to_string(
        drm_conn->connector_id));
    }

    _saved_crtc = drmModeGetCrtc (_fd, _crtc_id);
    buff *b = &_buffs[0];
    ret = init_connection_crtc(b->fb);
    if (ret) {
      error (this, string("failed to set mode for connector ") + to_string(
        drm_conn->connector_id));
    }
  }

  int
  DRMConnector::init_connection_crtc(uint32_t fb)
  {
    return drmModeSetCrtc (_fd, _crtc_id, fb, 0, 0, &_conn, 1, &_mode);
  }

  DRMConnector::DRMConnector (ParentProcess* parent, const std::string& name, int fd, drmModeRes *res, drmModeConnector *drm_conn) : FatProcess (name),
     _fd (fd), _crtc_id (-1), _cur_buff (0), _conn (drm_conn->connector_id), _waiting_vblank (false),  _saved_crtc (nullptr),
    _width (this, "width", 0), _height (this, "height", 0), _x (this, "x", 0), _y (this, "y", 0), _connected (this, "connected", false),
    _pos_action (this, "pos_action"), _vblank (this, "_vblank"),
    _c_update_pos_x (&_x, ACTIVATION, &_pos_action, ACTIVATION),
    _c_update_pos_y (&_y, ACTIVATION, &_pos_action, ACTIVATION)

  {
    bool connected = drm_conn->connection == DRM_MODE_CONNECTED;
    _connected.set_value (connected, false);

#if DEBUG
    cout << "New connector found " << name;
    if (connected)
      cout << " STATUS: connected\n";
    else
      cout << " STATUS: not connected\n";
#endif
    if (connected) {
      init_connection (res, drm_conn);
    }

    Graph::instance ().add_edge (&_x, &_pos_action);
    Graph::instance ().add_edge (&_y, &_pos_action);
    _c_update_pos_x.enable ();
    _c_update_pos_y.enable ();
    conn_list.push_back (this);
    finalize_construction (parent, name);
    set_activation_state (ACTIVATED);
  }

  buff*
  DRMConnector::get_next_buff ()
  {
    return &_buffs[_cur_buff^1];
  }

  void
  DRMConnector::flip_page ()
  {
    buff* b = &_buffs[_cur_buff^1];
    /*drmModePageFlip (_fd, _crtc_id, b->fb, DRM_MODE_PAGE_FLIP_EVENT, this);
    _cur_buff ^= 1;
    _waiting_vblank = true;*/
    flip_page_fb(b->fb);
  }

  void
  DRMConnector::flip_page_fb (uint32_t fb)
  {
    //DBG;
    //buff* b = &_buffs[_cur_buff^1];
    drmModePageFlip (_fd, _crtc_id, fb, DRM_MODE_PAGE_FLIP_EVENT, this);
    _cur_buff ^= 1;
    _waiting_vblank = true;
  }

  void
  DRMConnector::handle_vblank ()
  {
    //DBG;
    _vblank.activate ();
    _waiting_vblank = false;
  }

  void
  DRMConnector::update_pos () {
    buff *b = &_buffs[0];
    int ret = drmModeSetCrtc (_fd, _crtc_id, b->fb, _x.get_value (), _y.get_value (), &_conn, 1, &_mode);
    if (ret) {
      error (this, "failed to set crtc for connector");
    }
  }

  void
  DRMConnector::update_pos_fb (uint32_t fb) {
    //buff *b = &_buffs[0];
    int ret = drmModeSetCrtc (_fd, _crtc_id, fb, _x.get_value (), _y.get_value (), &_conn, 1, &_mode);
    if (ret) {
      error (this, "failed to set crtc for connector");
    }
  }

  void
  DRMConnector::destroy (int i) {
    struct drm_mode_destroy_dumb dreq;
    buff* b = &_buffs[i];
    munmap (b->map, b->size);
    drmModeRmFB (_fd, b->fb);
    memset (&dreq, 0, sizeof (dreq));
    dreq.handle = b->handle;
    drmIoctl (_fd, DRM_IOCTL_MODE_DESTROY_DUMB, &dreq);
  }

  void
  DRMConnector::clean ()
  {
    if (!_saved_crtc)
      return;
    drmModeSetCrtc (_fd, _saved_crtc->crtc_id,
                         _saved_crtc->buffer_id,
                         _saved_crtc->x,
                         _saved_crtc->y,
                         &_conn,
                         1,
                         &_saved_crtc->mode);
    drmModeFreeCrtc (_saved_crtc);
    destroy (1);
    destroy (0);
  }

  DRMConnector::~DRMConnector ()
  {
    clean ();
    Graph::instance ().remove_edge (&_x, &_pos_action);
    Graph::instance ().remove_edge (&_y, &_pos_action);
    delete find_child ("connected");
  }
}
