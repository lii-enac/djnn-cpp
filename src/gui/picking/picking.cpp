/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2014-2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stéphane Chatty <chatty@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */
#include "color_picking.h"

namespace djnn
{

  Picking::Picking (Window *win) :
      _win (win), _cur_obj (nullptr)
  {

  }

  Picking::~Picking ()
  {
  }

  void
  Picking::init ()
  {
    if (!_color_map.empty ())
      _color_map.clear ();
  }

  bool
  Picking::genericMousePress(double x, double y, int button)
  {
    bool exec_ = false;
    _win->x ()->set_value (x, true);
    _win->y ()->set_value (y, true);
    _win->press ()->notify_activation ();
    AbstractGShape *s = this->pick (x, y);
    if (s != nullptr) {
      s->x ()->set_value (x, true);
      s->y ()->set_value (y, true);
      s->press ()->notify_activation ();
      exec_ = true;
    }
    if (_win->press ()->has_coupling () || _win->x ()->has_coupling () || _win->y ()->has_coupling ()) {
      exec_ = true;
    }
    return exec_;
  }

  
  bool
  Picking::genericMouseMove(double x, double y)
  {
    bool exec_ = false;
    _win->x ()->set_value (x, true);
    _win->y ()->set_value (y, true);
    _win->move ()->notify_activation ();
    AbstractGShape *s = this->pick (x, y);
    if (_win->move ()->has_coupling () || _win->x ()->has_coupling () || _win->y ()->has_coupling ()) {
      exec_ = true;
    }
    if (s) {
      s->x ()->set_value (x, true);
      s->y ()->set_value (y, true);
      if (s != _cur_obj) {
        if (_cur_obj != 0)
          _cur_obj->leave ()->notify_activation ();
        s->enter ()->notify_activation ();
        _cur_obj = s;
      }
      s->move ()->notify_activation ();
      exec_ = true;
    } else {
      if (_cur_obj != nullptr) {
        _cur_obj->leave ()->notify_activation ();
        _cur_obj = nullptr;
        exec_ = true;
      }
    }
    return exec_;
  }
  
  bool
  Picking::genericMouseRelease(double x, double y, int button)
  {
    bool exec_ = false;
    AbstractGShape *s = this->pick (x, y);
    if (s) {
      if (s != _cur_obj) {
        if (_cur_obj != nullptr)
          _cur_obj->leave ()->notify_activation ();
        s->enter ()->notify_activation ();
        _cur_obj = s;
      }
      s->release ()->notify_activation ();
      exec_ = true;
    } else {
      if (_cur_obj != nullptr) {
        _cur_obj->leave ()->notify_activation ();
        _cur_obj = 0;
        exec_ = true;
      }
    }
    if (_win->release ()->has_coupling ()) {
      _win->release ()->notify_activation ();
      exec_ = true;
    }
    return exec_;
  }

  bool
  Picking::genericMouseWheel(double x, double y)
  {
    bool exec_ = false;
    _win->wheel_dx ()->set_value (x, true);
    _win->wheel_dy ()->set_value (y, true);
    _win->wheel ()->notify_activation ();
    if (_win->wheel ()->has_coupling () || _win->wheel_dx ()->has_coupling ()
        || _win->wheel_dy ()->has_coupling ()) {
      exec_ = true;
    }
    return exec_;
  }

} /* namespace djnn */
