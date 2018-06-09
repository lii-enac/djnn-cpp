/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "backend.h"
#include "window.h"
#include "style/style.h"
#include "shapes/shapes.h"
#include "transformation/transformations.h"

namespace djnn {

extern int mouse_tracking;

void init_gui ();
void init_svg_parser ();
void clear_gui ();
}
