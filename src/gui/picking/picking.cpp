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
  Picking::genericMousePress (double x, double y, int button)
  {
    bool exec_ = false;
    _win->x ()->set_value (x, true);
    _win->y ()->set_value (y, true);
    _win->press ()->notify_activation ();
    AbstractGShape *s = this->pick (x, y);
    if (s != nullptr) {
      ((DoubleProperty*) s->find_component ("move/x"))->set_value (x, true);
      ((DoubleProperty*) s->find_component ("move/y"))->set_value (y, true);
      s->find_component ("press")->notify_activation ();
      exec_ = true;
    }
    if (_win->press ()->has_coupling () || _win->x ()->has_coupling () || _win->y ()->has_coupling ()) {
      exec_ = true;
    }
    return exec_;
  }

  bool
  Picking::genericTouchPress (double x, double y, int id)
  {
    map<int, Touch*>::iterator it = _active_touches.find (id);
    Touch *t;
    if (it != _active_touches.end ()) {
      t = it->second;
      _win->touches ()->remove_child (t);
      if (t->shape () != nullptr) {
        t->shape ()->find_component ("touches")->remove_child (t);
      }
      _active_touches.erase (it);
      delete t;
    }
    t = new Touch (_win->touches (), to_string (id));
    _active_touches[id] = t;
    t->set_x (x);
    t->set_y (y);
    AbstractGShape *s = this->pick (x, y);
    if (s != nullptr) {
      t->set_shape (s);
      s->find_component ("touches")->add_child (t, to_string (id));
    }
    return true;
  }

  bool
  Picking::genericMouseMove (double x, double y)
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
      ((DoubleProperty*) s->find_component ("move/x"))->set_value (x, true);
      ((DoubleProperty*) s->find_component ("move/y"))->set_value (y, true);
      if (s != _cur_obj) {
        if (_cur_obj != 0)
          _cur_obj->find_component ("leave")->notify_activation ();
        s->find_component ("enter")->notify_activation ();
        _cur_obj = s;
      }
      s->find_component ("move")->notify_activation ();
      exec_ = true;
    } else {
      if (_cur_obj != nullptr) {
        _cur_obj->find_component ("leave")->notify_activation ();
        _cur_obj = nullptr;
        exec_ = true;
      }
    }
    return exec_;
  }

  bool
  Picking::genericTouchMove (double x, double y, int id)
  {
    map<int, Touch*>::iterator it = _active_touches.find (id);
    Touch *t;
    if (it != _active_touches.end ()) {
      t = it->second;
      t->set_x (x);
      t->set_y (y);
      AbstractGShape *s = this->pick (x, y);
      AbstractGShape *t_shape = t->shape ();
      if (s == nullptr && t_shape != nullptr) {
        t_shape->find_component ("touches")->remove_child (t);
        t->set_shape (nullptr);
      } else if (s != nullptr) {
        if (t_shape == nullptr) {
          s->find_component ("touches")->add_child (t, to_string (id));
          t->set_shape (s);
        } else if (s != t_shape) {
          t_shape->find_component ("touches")->remove_child (t);
          s->find_component ("touches")->add_child (t, to_string (id));
          t->set_shape (s);
        }
      }
    } else {
      genericTouchPress (x, y, id);
    }
    return true;
  }

  bool
  Picking::genericMouseRelease (double x, double y, int button)
  {
    bool exec_ = false;
    AbstractGShape *s = this->pick (x, y);
    if (s) {
      if (s != _cur_obj) {
        if (_cur_obj != nullptr)
          _cur_obj->find_component ("leave")->notify_activation ();
        s->find_component ("enter")->notify_activation ();
        _cur_obj = s;
      }
      s->find_component ("release")->notify_activation ();
      exec_ = true;
    } else {
      if (_cur_obj != nullptr) {
        _cur_obj->find_component ("leave")->notify_activation ();
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
  Picking::genericTouchRelease (double x, double y, int id)
  {
    map<int, Touch*>::iterator it = _active_touches.find (id);
    Touch *t;
    if (it != _active_touches.end ()) {
      t = it->second;
      t->set_x (x);
      t->set_y (y);
      AbstractGShape *t_shape = t->shape ();
      if (t_shape != nullptr) {
        t_shape->find_component ("touches")->remove_child (t);
      }
      _win->touches ()->remove_child (t);
      _active_touches.erase (it);
      delete t;
    }
    return true;
  }

  bool
  Picking::genericMouseWheel (double x, double y)
  {
    bool exec_ = false;
    _win->wheel_dx ()->set_value (x, true);
    _win->wheel_dy ()->set_value (y, true);
    _win->wheel ()->notify_activation ();
    if (_win->wheel ()->has_coupling () || _win->wheel_dx ()->has_coupling () || _win->wheel_dy ()->has_coupling ()) {
      exec_ = true;
    }
    return exec_;
  }

} /* namespace djnn */
