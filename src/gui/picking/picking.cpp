/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2014-2019)
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
#include "../transformation/transformations.h"

#define DBG std::cerr << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;

namespace djnn
{

  Picking::Picking (Window *win) :
      _win (win), _cur_obj (nullptr)
  {
    // FIXME: uniformiser l'API
    //_win->set_picking_view(this);
  }

  Picking::~Picking ()
  {
  }

  void
  Picking::init ()
  {
  }

  void
  Picking::set_local_coords (AbstractGShape* s, Touch *t, double x, double y)
  {
    Homography *h = dynamic_cast<Homography*> (s->inverted_matrix ());
    double loc_x = h->raw_props.m11 * x + h->raw_props.m12 * y + h->raw_props.m13 + h->raw_props.m14
        - s->origin_x ()->get_value ();
    double loc_y = h->raw_props.m21 * x + h->raw_props.m22 * y + h->raw_props.m23 + h->raw_props.m24
        - s->origin_y ()->get_value ();
    if (t != nullptr) {
      t->set_local_x (loc_x);
      t->set_local_y (loc_y);
    } else {
      ((DoubleProperty*) s->find_component ("move/local_x"))->set_value (loc_x, true);
      ((DoubleProperty*) s->find_component ("move/local_y"))->set_value (loc_y, true);
    }
  }

  void
  Picking::object_deleted (AbstractGShape* gobj)
  {
    // Reset _cur_obj to nullptr if this object has been removed from picking_view
    //DBG;
    if (_cur_obj == gobj) {
      //DBG;
      _cur_obj = nullptr;
    }
  }

  bool
  Picking::genericEnterLeave (AbstractGShape* picked) {
    auto s = picked;
    bool exec_ = false;
    if (s) {
      if (s != _cur_obj) {
        if (_cur_obj != nullptr)
          _cur_obj->find_component ("leave")->notify_activation ();
        s->find_component ("enter")->notify_activation ();
        _cur_obj = s;
        exec_ = true;
      }    
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
  Picking::genericCheckShapeAfterDraw (double x, double y)
  {
    bool exec_ = false;
    AbstractGShape *s = this->pick (x, y);

    if (s) {
      double cur_move_x = ((DoubleProperty*) s->find_component ("move/x"))->get_value ();
      double cur_move_y = ((DoubleProperty*) s->find_component ("move/y"))->get_value ();
      if (s == _cur_obj) {
        if (cur_move_x == x && cur_move_y == y)
          return exec_;
        else {
          set_local_coords (s, nullptr, x, y);
        }
      } 
      s->find_component ("move")->notify_activation ();
    }

    exec_ |= genericEnterLeave(s);

    return exec_;
  }

  bool
  Picking::genericMousePress (double x, double y, mouse_button button)
  {
    bool exec_ = false;
    _win->press_x ()->set_value (x, true);
    _win->press_y ()->set_value (y, true);
    _win->move_x ()->set_value (x, true);
    _win->move_y ()->set_value (y, true);
    _win->press ()->notify_activation ();

    AbstractGShape *s = this->pick (x, y);
    if (s != nullptr) {
      ((DoubleProperty*) s->find_component ("press/x"))->set_value (x, true);
      ((DoubleProperty*) s->find_component ("press/y"))->set_value (y, true);
      ((DoubleProperty*) s->find_component ("move/x"))->set_value (x, true);
      ((DoubleProperty*) s->find_component ("move/y"))->set_value (y, true);
      set_local_coords (s, nullptr, x, y);
      s->find_component ("press")->notify_activation ();
      exec_ = true;
    }

    //exec_ |= genericEnterLeave(s);

    switch (button)
      {
      case BUTTON_LEFT:
        ((GUIMouse*)GenericMouse)->left ()->press ()->activation ();
        break;
      case BUTTON_RIGHT:
	((GUIMouse*)GenericMouse)->right ()->press ()->activation ();
        break;
      case BUTTON_MIDDLE:
	((GUIMouse*)GenericMouse)->middle ()->press ()->activation ();
        break;
      default:
	((GUIMouse*)GenericMouse)->left ()->press ()->activation ();
      }
    if (_win->press ()->has_coupling () || _win->press_x ()->has_coupling () || _win->press_y ()->has_coupling ()) {
      exec_ = true;
    }
    if (((GUIMouse*)GenericMouse)->left ()->press ()->has_coupling () || ((GUIMouse*)GenericMouse)->right ()->press ()->has_coupling ()
        || ((GUIMouse*)GenericMouse)->middle ()->press ()->has_coupling ()) {
      exec_ = true;
    }
    return exec_;
  }

  bool
  Picking::genericTouchPress (double x, double y, int id, float pressure)
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
      Graph::instance().add_process_to_delete (t);
      //delete t;
    }
    t = new Touch (_win->touches (), to_string (id), x, y, pressure);
    _active_touches[id] = t;
    t->set_x (x);
    t->set_y (y);
    t->set_pressure (pressure);
    t->set_id (id);

    AbstractGShape *s = this->pick (x, y);
    if (s != nullptr) {
      t->set_shape (s);
      set_local_coords (s, t, x, y);
      s->find_component ("touches")->add_child (t, to_string (id));
    }
    return true;
  }

  bool
  Picking::genericMouseMove (double x, double y)
  {
    bool exec_ = false;

    double old_x = _win->move_x ()->get_value ();
    double old_y = _win->move_y ()->get_value ();
    if (x != old_x)
      _win->move_x ()->set_value (x, true);
    if (y != old_y)
      _win->move_y ()->set_value (y, true);
    _win->move ()->notify_activation ();
    if (_win->move ()->has_coupling () || _win->move_x ()->has_coupling () || _win->move_y ()->has_coupling ()) {
      exec_ = true;
    }

    AbstractGShape *s = this->pick (x, y);
    if (s) {
      if (x != old_x)
        ((DoubleProperty*) s->find_component ("move/x"))->set_value (x, true);
      if (y != old_y)
        ((DoubleProperty*) s->find_component ("move/y"))->set_value (y, true);
      set_local_coords (s, nullptr, x, y);
      s->find_component ("move")->notify_activation ();
      exec_ = true;
    }

    exec_ |= genericEnterLeave(s);

    ((GUIMouse*)GenericMouse)->x ()->set_value (x, true);
    ((GUIMouse*)GenericMouse)->y ()->set_value (y, true);
    ((GUIMouse*)GenericMouse)->move ()->activation ();
    if (((GUIMouse*)GenericMouse)->move ()->has_coupling() || ((GUIMouse*)GenericMouse)->x ()->has_coupling() || ((GUIMouse*)GenericMouse)->y ()->has_coupling()) {
      exec_ = true;
    }
    return exec_;
  }

  bool
  Picking::genericTouchMove (double x, double y, int id, float pressure)
  {
    map<int, Touch*>::iterator it = _active_touches.find (id);
    Touch *t;
    if (it != _active_touches.end ()) {
      t = it->second;
      t->set_x (x);
      t->set_y (y);
      t->set_pressure (pressure);
      t->set_id (id);
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
        set_local_coords (s, t, x, y);
      }
    } else {
      genericTouchPress (x, y, id, pressure);
    }
    return true;
  }

  bool
  Picking::genericMouseRelease (double x, double y, mouse_button button)
  {
    bool exec_ = false;
    AbstractGShape *s = this->pick (x, y);
    if (s) {
      s->find_component ("release")->notify_activation ();
      exec_ = true;
    }

    //exec_ |= genericEnterLeave(s);

    switch (button)
      {
      case BUTTON_LEFT:
	((GUIMouse*)GenericMouse)->left ()->release ()->activation ();
        break;
      case BUTTON_RIGHT:
	((GUIMouse*)GenericMouse)->right ()->release ()->activation ();
        break;
      case BUTTON_MIDDLE:
	((GUIMouse*)GenericMouse)->middle ()->release ()->activation ();
        break;
      default:
	((GUIMouse*)GenericMouse)->left ()->release ()->activation ();
      }
    if (_win->release ()->has_coupling ()) {
      _win->release ()->notify_activation ();
      exec_ = true;
    }
    if (((GUIMouse*)GenericMouse)->left ()->release ()->has_coupling () || ((GUIMouse*)GenericMouse)->right ()->release ()->has_coupling ()
        || ((GUIMouse*)GenericMouse)->middle ()->release ()->has_coupling ()) {
      exec_ = true;
    }
    return exec_;
  }

  bool
  Picking::genericTouchRelease (double x, double y, int id, float pressure)
  {
    map<int, Touch*>::iterator it = _active_touches.find (id);
    Touch *t;
    if (it != _active_touches.end ()) {
      t = it->second;
      t->set_x (x);
      t->set_y (y);
      t->set_pressure (pressure);
      t->set_id (id);
      AbstractGShape *t_shape = t->shape ();
      if (t_shape != nullptr) {
        t_shape->find_component ("touches")->remove_child (t);
        set_local_coords (t_shape, t, x, y);
      }
      _win->touches ()->remove_child (t);
      _active_touches.erase (it);
      Graph::instance().add_process_to_delete (t);
      //delete t;
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
    _win->wheel_dx ()->set_value (0, false);
    _win->wheel_dy ()->set_value (0, false);
    ((GUIMouse*)GenericMouse)->dx ()->set_value (x, true);
    ((GUIMouse*)GenericMouse)->dy ()->set_value (y, true);
    ((GUIMouse*)GenericMouse)->wheel ()->activation ();
    if (((GUIMouse*)GenericMouse)->wheel ()->has_coupling() || ((GUIMouse*)GenericMouse)->dx ()->has_coupling() || ((GUIMouse*)GenericMouse)->dy ()->has_coupling()) {
      exec_ = true;
     }
    ((GUIMouse*)GenericMouse)->dx ()->set_value (0, false);
    ((GUIMouse*)GenericMouse)->dy ()->set_value (0, false);
    return exec_;
  }

} /* namespace djnn */
