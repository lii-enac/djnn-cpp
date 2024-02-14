/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019-2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once
#include "core/property/int_property.h"
#include "core/property/text_property.h"
#include "display-dev.h"

namespace djnn {
void p_init_display ();
void p_clear_display ();

class GUIKeyboard : public FatProcess
{
  public:
    GUIKeyboard (CoreProcess* parent, const string& name);
    ~GUIKeyboard ();
    void          impl_activate () {}
    void          impl_deactivate () {}
    IntProperty*  key_pressed () { return _key_pressed; }
    IntProperty*  key_released () { return _key_released; }
    TextProperty* key_pressed_text () { return _key_pressed_text; }
    TextProperty* key_released_text () { return _key_released_text; }

  private:
    TextProperty* _key_pressed_text;
    IntProperty*  _key_pressed;
    TextProperty* _key_released_text;
    IntProperty*  _key_released;
};
} // namespace djnn
