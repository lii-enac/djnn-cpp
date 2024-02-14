#include "core/utils/algorithm.h"

#include "qt_display.h"
#include "qt_window.h"

#include <QtCore>

#define MACRO(lowkey,capkey) extern const int DJN_Key_ ## lowkey = Qt::Key_ ## lowkey;

namespace djnn {

  #include "display/const_keys.h"
  MACRO (Print, PRINT)
  MACRO (Shift, SHIFT)
  MACRO (Control, CONTROL)
  MACRO (Meta, META)
  MACRO (Alt, ALT)
  MACRO (AltGr, ALTGR)
  MACRO (NumLock, NUMLOCK)
  MACRO (Exclam, EXCLAM)
  MACRO (Apostrophe, APOSTROPHE)
  MACRO (ParenLeft, PARENLEFT)
  MACRO (ParenRight, PARENRIGHT)
  MACRO (Equal, EQUAL)

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

  void QtDisplayBackend::add_window (QtWindow* win) { _windows.push_back (win); }
  void QtDisplayBackend::remove_window (QtWindow* win) { _windows.erase (djnnstl::remove (_windows.begin (), _windows.end (), win), _windows.end ()); }

}

