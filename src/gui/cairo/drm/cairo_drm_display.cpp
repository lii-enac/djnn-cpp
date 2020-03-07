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

#include "display/display.h"
#include "display/abstract_display.h"

#include "display/drm/drm_window.h"
#include "gui/cairo/drm/cairo_drm_window.h"
#include <mutex> // std::call_once
#include <iostream>

namespace djnn {

	class CairoDRMDisplayBackend : public AbstractDisplay {
  public:
    static CairoDRMDisplayBackend* instance ();
    WinImpl* create_window (Window *win, const std::string& title,  double x, double y, double w, double h);

    struct Impl;
    Impl *impl;

    static std::shared_ptr<CairoDRMDisplayBackend> _instance;
    static std::once_flag onceFlag;
  };

  std::shared_ptr<CairoDRMDisplayBackend> CairoDRMDisplayBackend::_instance;
  std::once_flag CairoDRMDisplayBackend::onceFlag;

  WinImpl*
  CairoDRMDisplayBackend::create_window (Window *win, const std::string& title,  double x, double y, double w, double h)
  {
    DRMWindow *implwin = new CairoDRMWindow (win, title, x, y, w, h);
    set_window (win);
    return implwin;
  }

  CairoDRMDisplayBackend*
  CairoDRMDisplayBackend::instance ()
  {
    std::call_once (CairoDRMDisplayBackend::onceFlag, [] () {
      _instance.reset(new CairoDRMDisplayBackend);
    });

    return _instance.get ();
  }

  void
  DisplayBackend::init ()
  {
    if (_instance != nullptr)
      return;
    _instance = new Impl ();
    _instance->backend = CairoDRMDisplayBackend::instance ();
  }

	void
  p_init_p_display ()
  {
    DisplayBackend::init ();
  }
}
