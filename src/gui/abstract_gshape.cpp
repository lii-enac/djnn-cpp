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

#include "abstract_gshape.h"
#include "../core/tree/spike.h"

namespace djnn
{

  void
  AbstractGShape::init_mouse_ui ()
  {
    _press = new Spike (this, "press");
    _release = new Spike (this, "release");
    _move = new Spike (this, "move");
    _enter = new Spike (this, "enter");
    _leave = new Spike (this, "leave");
    _x = new DoubleProperty (0);
    _y = new DoubleProperty (0);
    _press->add_symbol ("x", _x);
    _press->add_symbol ("y", _y);
    _move->add_symbol ("x", _x);
    _move->add_symbol ("y", _y);
  }

  AbstractGShape::AbstractGShape () :
      AbstractGObj ()
  {
    init_mouse_ui ();
  }

  AbstractGShape::AbstractGShape (Process *p, const std::string& n) :
      AbstractGObj (p, n)
  {
    init_mouse_ui ();
  }

  AbstractGShape::~AbstractGShape ()
  {
    delete _x;
    delete _y;
    delete _press;
    delete _move;
    delete _release;
    delete _enter;
    delete _leave;
  }

} /* namespace djnn */
