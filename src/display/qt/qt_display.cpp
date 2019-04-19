#include "qt_display.h"
#include "qt_window.h"

namespace djnn {

  QtDisplayBackend* __instance;

  QtDisplayBackend*
  QtDisplayBackend::instance()
  {
  	return __instance;
  }

  void
  QtDisplayBackend::slot_for_about_to_block()
  {
    for (auto w : _windows) {
     w->check_for_update ();
    }
  }

}

