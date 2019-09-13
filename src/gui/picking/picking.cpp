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
#include <assert.h>

#define DBG std::cerr << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;

namespace djnn
{

  Picking::Picking (Window *win) :
      _win (win), _catching_shape (nullptr), _hover (nullptr), _mouse_released (true)
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
  Picking::set_local_coords (AbstractGShape* s, Touch *t, double x, double y, bool is_move)
  {
    Homography *h = dynamic_cast<Homography*> (s->inverted_matrix ());
    double loc_x = h->raw_props.m11 * x + h->raw_props.m12 * y + h->raw_props.m13 + h->raw_props.m14
        - s->origin_x ()->get_value ();
    double loc_y = h->raw_props.m21 * x + h->raw_props.m22 * y + h->raw_props.m23 + h->raw_props.m24
        - s->origin_y ()->get_value ();
    if (t != nullptr) {
      if (is_move) {
        t->set_local_move_x (loc_x);
        t->set_local_move_y (loc_y);

      } else {
        t->set_local_init_x (loc_x);
        t->set_local_init_y (loc_y);
      }
    } else {
      s->get_ui()->mouse_local_move_x->set_value (loc_x, true);
      s->get_ui()->mouse_local_move_y->set_value (loc_y, true);
    }
    if (is_move) {
      s->get_ui()->local_move_x->set_value (loc_x, true);
      s->get_ui()->local_move_y->set_value (loc_y, true);
    } else {
      s->get_ui()->local_press_x->set_value (loc_x, true);
      s->get_ui()->local_press_y->set_value (loc_y, true);
    }
  }

  void
  Picking::object_deactivated (AbstractGShape* gobj)
  {
    /* Reset _catching_shape to nullptr if this object has been removed from picking_view */
    if (_catching_shape == gobj) {
      _catching_shape = nullptr;
    }

     /* Reset _hover to nullptr if this object has been removed from picking_view */
    if (_hover == gobj) {
      _hover = nullptr;
    }

    /* reset all _active_touches[x]->init_shape ans _active_touches[x]->current_shape */
    for ( auto iter = _active_touches.begin(); iter != _active_touches.end(); ++iter) 
    {
      Touch *t = iter->second;
      assert(t);
      if (t->init_shape () == gobj)
        t->set_init_shape (nullptr);
      if (t->current_shape () == gobj)
        t->set_current_shape (nullptr);
    }
  }

  bool
  Picking::genericEnterLeave (AbstractGShape* picked) {
    auto s = picked;
    bool exec_ = false;
    if (s) {
      if (s != _hover) {
        if (_hover != nullptr) {
        _hover->get_ui()->leave->notify_activation ();
        _hover->get_ui()->mouse_leave->notify_activation ();
        }
        s->get_ui ()->enter->notify_activation ();
        s->get_ui ()->mouse_enter->notify_activation ();
        _hover = s;
        exec_ = true;
      }    
    } else if (_hover != nullptr) {
        _hover->get_ui ()->leave->notify_activation ();
        _hover->get_ui ()->mouse_leave->notify_activation ();
        _hover = nullptr;
        exec_ = true;
    }
    return exec_;
  }

  bool
  Picking::genericCheckShapeAfterDraw (double x, double y)
  {
    if (_mouse_released && mouse_tracking == 0)
      return false;
    bool exec_ = false;
    AbstractGShape *s = this->pick (x, y);

    if (s) {
      double cur_move_x = s->get_ui ()->move_x->get_value ();
      double cur_move_y = s->get_ui ()->move_y->get_value ();
      if (s == _catching_shape) {
        if (cur_move_x == x && cur_move_y == y)
          return exec_;
        else {
          set_local_coords (s, nullptr, x, y, true);
        }
      } 
      s->get_ui ()->move->notify_activation ();
      s->get_ui ()->mouse_move->notify_activation ();
    }

    exec_ |= genericEnterLeave(s);

    return exec_;
  }

  void
  Picking::common_press (double x, double y , AbstractGShape *s)
  {
    s->get_ui()->press_x->set_value (x, true);
    s->get_ui()->press_y->set_value (y, true);
    s->get_ui()->move_x->set_value (x, true);
    s->get_ui()->move_y->set_value (y, true);
    if (s != _hover)
      s->get_ui()->enter->notify_activation ();
    s->get_ui()->press->notify_activation ();
    _catching_shape = s;
    _hover = s;
  }

  bool
  Picking::genericMousePress (double x, double y, mouse_button button)
  {
    _mouse_released = false;
    bool exec_ = false;
    _win->press_x ()->set_value (x, true);
    _win->press_y ()->set_value (y, true);
    _win->move_x ()->set_value (x, true);
    _win->move_y ()->set_value (y, true);
    _win->press ()->notify_activation ();

    AbstractGShape *s = this->pick (x, y);
    if (s != nullptr) {
      if (s != _hover)
        s->get_ui()->mouse_enter->notify_activation ();
      common_press (x, y, s);
      s->get_ui()->mouse_press_x->set_value (x, true);
      s->get_ui()->mouse_press_y->set_value (y, true);
      s->get_ui()->mouse_move_x->set_value (x, true);
      s->get_ui()->mouse_move_y->set_value (y, true);
      set_local_coords (s, nullptr, x, y, false);
      s->get_ui()->mouse_press->notify_activation ();;

      exec_ = true;
    }

    switch (button)
      {
      case BUTTON_LEFT:
        ((GUIMouse*) GenericMouse)->left ()->press ()->activate ();
        break;
      case BUTTON_RIGHT:
        ((GUIMouse*) GenericMouse)->right ()->press ()->activate ();
        break;
      case BUTTON_MIDDLE:
        ((GUIMouse*) GenericMouse)->middle ()->press ()->activate ();
        break;
      default:
        ((GUIMouse*) GenericMouse)->left ()->press ()->activate ();
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
      if (t->init_shape () != nullptr) {
        t->init_shape ()->get_ui()->touches->remove_child (t);
        t->leave ();
      }
      _active_touches.erase (it);
      Graph::instance().add_process_to_delete (t);
    }
    t = new Touch (_win->touches (), to_string (id), id, x, y, pressure);
    _active_touches[id] = t;

    AbstractGShape *s = this->pick (x, y);
    if (s != nullptr) {
      common_press (x, y, s);
      t->set_init_shape (s);
      t->set_current_shape (s);
      set_local_coords (s, t, x, y, false);
      s->get_ui()->touches->add_child (t, to_string (id));
      t->enter ();
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
    exec_ |= genericEnterLeave(s);
    if (s) {
      if (x != old_x) {
        s->get_ui()->move_x->set_value (x, true);
        s->get_ui()->mouse_move_x->set_value (x, true);
      }
      if (y != old_y) {
        s->get_ui()->move_y->set_value (y, true);
        s->get_ui()->mouse_move_y->set_value (y, true);
      }
      set_local_coords (s, nullptr, x, y, true);
      s->get_ui()->move->notify_activation ();
      s->get_ui()->mouse_move->notify_activation ();
      exec_ = true;
    }
    if (_catching_shape != nullptr && _catching_shape != s) {
      if (x != old_x) {
        _catching_shape->get_ui()->move_x->set_value (x, true);
        _catching_shape->get_ui()->mouse_move_x->set_value (x, true);
      }
      if (y != old_y) {
        _catching_shape->get_ui()->move_y->set_value (y, true);
        _catching_shape->get_ui()->mouse_move_y->set_value (y, true);
      }
      set_local_coords (_catching_shape, nullptr, x, y, true);
      _catching_shape->get_ui()->move->notify_activation ();
      _catching_shape->get_ui()->mouse_move->notify_activation ();
      exec_ = true;
    }

    ((GUIMouse*)GenericMouse)->x ()->set_value (x, true);
    ((GUIMouse*)GenericMouse)->y ()->set_value (y, true);
    ((GUIMouse*)GenericMouse)->move ()->activate ();
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
      t->set_move_x (x);
      t->set_move_y (y);
      t->set_pressure (pressure);
      t->set_id (id);
      AbstractGShape *s = this->pick (x, y);
      AbstractGShape *cur_shape = t->current_shape ();
      AbstractGShape *init_shape = t->init_shape ();
      if (s == nullptr && cur_shape != nullptr) {
        if (cur_shape != init_shape)
          cur_shape->get_ui ()->touches->remove_child (t);
        t->set_current_shape (nullptr);
        t->leave ();
      } else if (s != nullptr && s != cur_shape) {
        if (cur_shape != nullptr && cur_shape != init_shape)
          cur_shape->get_ui ()->touches->remove_child (t);
        s->get_ui ()->touches->add_child (t, to_string (id));
        t->set_current_shape (s);
        t->enter ();
        s->get_ui ()->move_x->set_value (x, true);
        s->get_ui ()->move_y->set_value (y, true);
        s->get_ui ()->move->notify_activation ();
        set_local_coords (s, t, x, y, true);
      }
      t->get_move ()->notify_activation ();
      genericEnterLeave (s);
    } else {
      genericTouchPress (x, y, id, pressure);
    }
    return true;
  }

  bool
  Picking::genericMouseRelease (double x, double y, mouse_button button)
  {
    _mouse_released = true;
    bool exec_ = false;
    AbstractGShape *s = this->pick (x, y);
    if (s) {
      s->get_ui ()->release->notify_activation ();
      s->get_ui ()->mouse_release->notify_activation ();
      if (mouse_tracking == 0) {
        s->get_ui ()->leave->notify_activation ();
        s->get_ui ()->mouse_leave->notify_activation ();
      }
      exec_ = true;
    }
    if (_catching_shape && _catching_shape != s) {
      _catching_shape->get_ui ()->release->notify_activation ();
      _catching_shape->get_ui ()->mouse_release->notify_activation ();
      exec_ = true;
    }

    if (mouse_tracking == 0) {
      _hover = nullptr;
    }
    _catching_shape = nullptr;

    //exec_ |= genericEnterLeave(s);

    switch (button)
      {
      case BUTTON_LEFT:
        ((GUIMouse*) GenericMouse)->left ()->release ()->activate ();
        break;
      case BUTTON_RIGHT:
        ((GUIMouse*) GenericMouse)->right ()->release ()->activate ();
        break;
      case BUTTON_MIDDLE:
        ((GUIMouse*) GenericMouse)->middle ()->release ()->activate ();
        break;
      default:
        ((GUIMouse*) GenericMouse)->left ()->release ()->activate ();
      }
    if (_win->release ()->has_coupling ()) {
      _win->release ()->notify_activation ();
      exec_ = true;
    }
    if (((GUIMouse*) GenericMouse)->left ()->release ()->has_coupling ()
        || ((GUIMouse*) GenericMouse)->right ()->release ()->has_coupling ()
        || ((GUIMouse*) GenericMouse)->middle ()->release ()->has_coupling ()) {
      exec_ = true;
    }
    return exec_;
  }

  bool
  Picking::genericTouchRelease (double x, double y, int id, float pressure)
  {
    AbstractGShape *s = this->pick (x, y);
    map<int, Touch*>::iterator it = _active_touches.find (id);
    Touch *t;
    if (it != _active_touches.end ()) {
      t = it->second;
      t->set_move_x (x);
      t->set_move_y (y);
      t->set_pressure (pressure);
      t->set_id (id);
      AbstractGShape *current_shape = t->current_shape ();
      AbstractGShape *init_shape = t->init_shape ();
      if (init_shape != nullptr) {
        init_shape->get_ui()->touches->remove_child (t);
        set_local_coords (init_shape, t, x, y, true);
      }
      if (current_shape != nullptr) {
        current_shape->get_ui()->touches->remove_child (t);
      }
      if (current_shape || init_shape)
        t->leave ();
      t->set_init_shape (nullptr);
      t->set_current_shape (nullptr);

      _win->touches ()->remove_child (t);
      _active_touches.erase (it);
      Graph::instance().add_process_to_delete (t);
    }
    if (s) {
      s->get_ui ()->release->notify_activation ();
      s->get_ui ()->leave->notify_activation ();
    }
    if (_catching_shape && _catching_shape != s) {
      _catching_shape->get_ui ()->release->notify_activation ();
    }
    _hover = nullptr;
    _catching_shape = nullptr;
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
    ((GUIMouse*)GenericMouse)->wheel ()->activate ();
    if (((GUIMouse*)GenericMouse)->wheel ()->has_coupling() || ((GUIMouse*)GenericMouse)->dx ()->has_coupling() || ((GUIMouse*)GenericMouse)->dy ()->has_coupling()) {
      exec_ = true;
     }
    ((GUIMouse*)GenericMouse)->dx ()->set_value (0, false);
    ((GUIMouse*)GenericMouse)->dy ()->set_value (0, false);
    return exec_;
  }

} /* namespace djnn */
