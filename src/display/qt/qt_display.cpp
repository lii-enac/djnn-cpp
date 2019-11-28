#include "qt_display.h"
#include "qt_window.h"

#include <QtCore>

#define MACRO(key) extern const int DJN_Key_ ## key = Qt::Key_ ## key;

namespace djnn {

  #include "display/const_keys.h"

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

