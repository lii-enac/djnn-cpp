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

#include "abstract_gobj.h"

#include "../core/tree/double_property.h"
#include "../core/tree/process.h"
#include <iostream>

namespace djnn
{

  class Touch : public Process
  {
  public:
    Touch (Process *p, const std::string &n);
    Touch ();
    virtual ~Touch ();
  private:
    DoubleProperty *_x,* _y;
  };

  class AbstractGShape : public AbstractGObj
  {
  public:
    AbstractGShape (Process *p, const std::string& n);
    AbstractGShape ();
    virtual ~AbstractGShape ();
    Process* press () { return _press; }
    Process* move () { return _move; }
    Process* release () { return _release; }
    Process* enter () { return _enter; }
    Process* leave () { return _leave; }
    Process* touches () { return _touches; }
    DoubleProperty* x () { return _x; }
    DoubleProperty* y () { return _y; }
    
  private:
    void init_mouse_ui ();
    Process* _press;
    Process* _move;
    Process* _release;
    Process* _enter;
    Process* _leave;
    Process* _touches;
    DoubleProperty *_x;
    DoubleProperty *_y;
  };

  inline bool is_pickable (AbstractGShape * s) {
      return s->press ()->has_coupling () || s->x ()->has_coupling () || s->y ()->has_coupling ()
        || s->move ()->has_coupling () || s->release ()->has_coupling () || s->enter ()->has_coupling ()
        || s->leave ()->has_coupling ();
  }

} /* namespace djnn */
