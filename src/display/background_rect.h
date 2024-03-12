/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2021)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "core/tree/component.h"
#include "display/pickui.h"
#include "display/ui.h"

namespace djnn {

class Window;

class BackgroundRect : public Component, public PickUI {
  public:
    BackgroundRect (CoreProcess* parent, const string&);
    ~BackgroundRect ();

    CoreProcess* find_child_impl (const string&) override;
    void         set_mouse_local_coords (double x, double y, bool is_move) override {};

  protected:
    Window* get_frame () { return (Window*)get_parent (); }
};
} // namespace djnn