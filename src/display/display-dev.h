/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

// #include "abstract_display.h"

namespace djnn {

void init_display ();
void clear_display ();

extern int mouse_tracking;
extern int full_screen;
extern int hide_pointer;

typedef enum mouse_button {
    BUTTON_LEFT,
    BUTTON_MIDDLE,
    BUTTON_RIGHT
} mouse_button;

typedef enum stylus_type {
    STYLUS_PEN,
    STYLUS_ERASER
} stylus_type;

class CoreProcess;

extern CoreProcess* GPUs;
extern CoreProcess* Displays;
extern CoreProcess* GenericKeyboard;
extern CoreProcess* DrawingRefreshManager;

class AbstractDisplay;
class DisplayBackend {
  public:
    static AbstractDisplay* instance ();
    static void             init ();
    static void             clear ();

  private:
    // class Impl;
    class Impl {
      public:
        // SDLDisplayBackend* backend;
        AbstractDisplay* backend;
    };
    static Impl* _instance;
};

} // namespace djnn

#define MACRO(lowkey, capkey) extern const int DJN_Key_##lowkey;

namespace djnn {

#include "const_keys.h"
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

} // namespace djnn
