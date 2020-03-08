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

#include "display/drm/drm_display.h"
#include "display/window.h"
#include "core/ontology/process.h"
#include "syshook/external_source.h"

#include <string>

namespace djnn {

  class DRMWindow : public WinImpl
  {
  public:
    DRMWindow (Window *win, const std::string &title, double x, double y, double w, double h);
    virtual ~DRMWindow ();
    buff* get_next_buff ();
    void page_flip ();
    void set_cursor (const string &path, int hotX, int hotY) {}
    Process* get_vblank () {
      if (_conn == nullptr)
        return nullptr;
      else
        return _conn->get_vblank ();
    }
    bool is_waiting_vblank () {
      if (_conn != nullptr)
        return _conn->is_waiting_vblank ();
      else {
        error (nullptr, "Bad DRMWindow initialization");
        return 0;
      }
    }
    int set_dpy_connection ();
   protected:
    djnn::Window* _window;
    DRMConnector* _conn;
    bool is_activated;
  };

}
