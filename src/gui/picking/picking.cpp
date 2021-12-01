/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2014-2021)
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

#include "core/utils/to_string.h"
#include "color_picking.h"
#include "gui/transformation/homography.h"
#include "display/ui.h"
#include "core/core-dev.h"

#include <assert.h>
namespace djnn
{
  extern int mouse_tracking; // in display
  extern FatProcess* GenericMouse; // in gui

  Picking::Picking (Window *win) :
      _win (win), _cought_shape (nullptr), _hovered_shape (nullptr), _mouse_released (true)
  {
    // FIXME: uniformiser l'API
    //_win->set_picking_view(this);
  }

  Picking::~Picking ()
  {
    //DBG;
  }

  void
  Picking::init ()
  {
  }

  void
  Picking::object_deactivated (PickUI* picked_shape)
  {
    /* Reset _catched_shape to nullptr if this object has been removed from picking_view */
    if (_cought_shape == picked_shape) {
      _cought_shape = nullptr;
    }

     /* Reset _hovered_shape to nullptr if this object has been removed from picking_view */
    if (_hovered_shape == picked_shape) {
      _hovered_shape = nullptr;
    }

    /* reset all _active_touches[x]->init_shape ans _active_touches[x]->current_shape */
    for ( auto iter = _active_touches.begin(); iter != _active_touches.end(); ++iter) 
    {
      Touch *t = iter->second;
      assert(t);
      if (t->init_shape () == picked_shape)
        t->set_init_shape (nullptr);
      if (t->current_shape () == picked_shape)
        t->set_current_shape (nullptr);
    }
  }

  bool
  Picking::genericEnterLeave (PickUI* picked) 
  {

    auto s = picked;

    if (s) {
      if (s != _hovered_shape) {
        if (_hovered_shape != nullptr) {
          FatProcess* p1 = _hovered_shape->ui ()->leave ();
          FatProcess* p2 = _hovered_shape->ui ()->mouse_leave ();
          p1->schedule_activation ();
          p2->schedule_activation ();
          //cut the event synthesis when enter/leave happened
          GRAPH_EXEC;
        }
        s->ui ()->enter ()->schedule_activation ();
        s->ui ()->mouse_enter ()->schedule_activation ();
        
        /* new _hovered_shape */
        _hovered_shape = s;
        //cut the event synthesis when enter/leave happened
        GRAPH_EXEC;
      }    
    } 
    else if (_hovered_shape != nullptr) {
        FatProcess* p1 = _hovered_shape->ui ()->leave ();
        FatProcess* p2 = _hovered_shape->ui ()->mouse_leave ();
        p1->schedule_activation ();
        p2->schedule_activation ();
        
        /* reset */
        _hovered_shape = nullptr;

        //cut the event synthesis when enter/leave happened
        GRAPH_EXEC;
    }
    
    return false;
  }

  bool
  Picking::genericCheckShapeAfterDraw (double x, double y)
  {

    /* release happend and no mouse tracking */
    if (_mouse_released && mouse_tracking == 0)
      return false;

    /* shape */
    bool exec_ = false;
    PickUI *s = this->pick (x, y);
    if (s) {
      double cur_move_x = s->ui ()->move_x ()->get_value ();
      double cur_move_y = s->ui ()->move_y ()->get_value ();

      if (s == _cought_shape) {
        if (cur_move_x == x && cur_move_y == y)
          return exec_; /* return false */
        else {
            s->set_mouse_local_coords (x, y, true);
        }
      } 
      s->ui ()->move ()->schedule_activation ();
      s->ui ()->mouse_move ()->schedule_activation ();
    }

    exec_ |= genericEnterLeave(s);

    return exec_;
  }

  void
  Picking::common_press_setting (double x, double y , PickUI *s)
  {
    s->ui()->press_x ()->set_value (x, true);
    s->ui()->press_y ()->set_value (y, true);
    s->ui()->move_x ()->set_value (x, true);
    s->ui()->move_y ()->set_value (y, true);
  }

  void
  Picking::common_press_notify (PickUI *s)
  {
    if (s != _hovered_shape)
      s->ui()->enter ()->schedule_activation ();
    s->ui()->press ()->schedule_activation ();

    /* reset _hovered_shape and _catched_shape */
    _cought_shape = s;
    _hovered_shape = s;
  }

  bool
  Picking::genericMousePress (double x, double y, mouse_button button)
  {
    _mouse_released = false;
    bool exec_ = false;

    /* windows setting */
    _win->press_x ()->set_value (x, true);
    _win->press_y ()->set_value (y, true);
    // _win->move_x ()->set_value (x, true);
    // _win->move_y ()->set_value (y, true);
    

    /* shape */ 
    PickUI *s = this->pick (x, y);
    //std::cerr << s << " " << typeid(s).name() << std::endl;
    if (s != nullptr) {
     
      /* setting */
      common_press_setting (x, y, s);
      s->ui()->mouse_press_x ()->set_value (x, true);
      s->ui()->mouse_press_y ()->set_value (y, true);
      // s->ui()->mouse_move_x->set_value (x, true);
      // s->ui()->mouse_move_y->set_value (y, true);
      s->set_mouse_local_coords (x, y, false);

      /* event */
      if (s != _hovered_shape)
        s->ui()->mouse_enter ()->schedule_activation ();
      s->ui()->mouse_press ()->schedule_activation ();
      common_press_notify (s);

      exec_ = true;
    }

    /* button */
    switch (button)
      {
      case BUTTON_LEFT:
        ((GUIMouse*) GenericMouse)->left ()->press ()->activate ();
        if (s != nullptr) {
          s->ui()->left_press ()->activate ();
        }
        break;
      case BUTTON_RIGHT:
        ((GUIMouse*) GenericMouse)->right ()->press ()->activate ();
        if (s != nullptr) {
          s->ui()->right_press ()->activate ();
        }
        break;
      case BUTTON_MIDDLE:
        ((GUIMouse*) GenericMouse)->middle ()->press ()->activate ();
        if (s != nullptr) {
          s->ui()->middle_press ()->activate ();
        }
        break;
      default:
        ((GUIMouse*) GenericMouse)->left ()->press ()->activate ();
      }
    if (((GUIMouse*)GenericMouse)->left ()->press ()->has_coupling () || ((GUIMouse*)GenericMouse)->right ()->press ()->has_coupling ()
        || ((GUIMouse*)GenericMouse)->middle ()->press ()->has_coupling ()) {
      exec_ = true;
    }

    /* windows event schedule event After shape event*/
    _win->press ()->schedule_activation ();
    if (_win->press ()->has_coupling () || _win->press_x ()->has_coupling () || _win->press_y ()->has_coupling ()) {
      exec_ = true;
    }

    return exec_;
  }

  bool
  Picking::genericTouchPress (double x, double y, int id, float pressure)
  {
    /* touch management */
    map<int, Touch*>::iterator it = _active_touches.find (id);
    Touch *t;
    if (it != _active_touches.end ()) {
      t = it->second;
      _win->touches ()->remove_child (t);
      if (t->init_shape () != nullptr) {
        t->init_shape ()->ui()->touches ()->remove_child (t);
        t->leave ();
      }
      _active_touches.erase (it);
      t->schedule_delete ();
    }
    t = new Touch (_win->touches (), djnn::to_string (id), id, x, y, pressure);
    _active_touches[id] = t;

    /* picking/shape management */
    PickUI *s = this->pick (x, y);
    if (s != nullptr) {
      common_press_setting (x, y, s);
      t->set_init_shape (s);
      t->set_current_shape (s);
      t->set_touch_local_coords (s, x, y, false);
      common_press_notify (s);
      /* press event on touch is replace by touches/$added */
      s->ui()->touches ()->add_child (t, djnn::to_string (id));
      t->enter ();
    }
    return true;
  }

  bool
  Picking::genericMouseMove (double x, double y)
  {
    bool exec_ = false;

    /* windows setting */
    double old_x = _win->move_x ()->get_value ();
    double old_y = _win->move_y ()->get_value ();
    if (x != old_x)
      _win->move_x ()->set_value (x, true);
    if (y != old_y)
      _win->move_y ()->set_value (y, true);
    

    /* shape */
    PickUI *s = this->pick (x, y);
    exec_ |= genericEnterLeave(s);
    if (s) {

      /* setting */
      if (x != old_x) {
        s->ui()->move_x ()->set_value (x, true);
        s->ui()->mouse_move_x ()->set_value (x, true);
      }
      if (y != old_y) {
        s->ui()->move_y ()->set_value (y, true);
        s->ui()->mouse_move_y ()->set_value (y, true);
      }
      s->set_mouse_local_coords (x, y, true); 

      /* event */
      s->ui()->move ()->schedule_activation ();
      s->ui()->mouse_move ()->schedule_activation ();

      exec_ = true;
    }

    /* _catched_shape */
    if (_cought_shape != nullptr && _cought_shape != s) {

       /* setting */
      if (x != old_x) {
        DoubleProperty* p1 = _cought_shape->ui()->move_x ();
        DoubleProperty* p2 = _cought_shape->ui()->mouse_move_x ();
        p1->set_value (x, true);
        p2->set_value (x, true);
      }
      if (y != old_y) {
        DoubleProperty* p1 = _cought_shape->ui()->move_y ();
        DoubleProperty* p2 = _cought_shape->ui()->mouse_move_y ();
        p1->set_value (y, true);
        p2->set_value (y, true);
      }
      _cought_shape->set_mouse_local_coords (x, y, true);

       /* event */
      FatProcess* p1 = _cought_shape->ui ()->move ();
      FatProcess* p2 = _cought_shape->ui ()->mouse_move ();
      p1->schedule_activation ();
      p2->schedule_activation ();

      exec_ = true;
    }

    /* generic mouse setting */
    ((GUIMouse*)GenericMouse)->x ()->set_value (x, true);
    ((GUIMouse*)GenericMouse)->y ()->set_value (y, true);
    /* generic mouse event */
    ((GUIMouse*)GenericMouse)->move ()->activate ();
    if (((GUIMouse*)GenericMouse)->move ()->has_coupling() || ((GUIMouse*)GenericMouse)->x ()->has_coupling() || ((GUIMouse*)GenericMouse)->y ()->has_coupling()) {
      exec_ = true;
    }

    /* windows event schedule event After shape event*/
    _win->move ()->schedule_activation ();
    if (_win->move ()->has_coupling () || _win->move_x ()->has_coupling () || _win->move_y ()->has_coupling ()) {
      exec_ = true;
    }

    return exec_;
  }

  bool
  Picking::genericTouchMove (double x, double y, int id, float pressure)
  {
    map<int, Touch*>::iterator it = _active_touches.find (id);
    Touch *t;
    /* touch exist */
    if (it != _active_touches.end ()) {
      t = it->second;

      /* setting */
      t->set_move_x (x);
      t->set_move_y (y);
      t->set_pressure (pressure);
      t->set_id (id);

      /* shape */
      PickUI* s = this->pick (x, y);
      PickUI* cur_shape = t->current_shape ();
      PickUI* init_shape = t->init_shape ();
      if (s == nullptr && cur_shape != nullptr) {
        if (cur_shape != init_shape)
          cur_shape->ui ()->touches ()->remove_child (t);
        t->set_current_shape (nullptr);
        /* touch event */
        t->leave ();
      } 
      else if (s != nullptr && s != cur_shape) {
        if (cur_shape != nullptr && cur_shape != init_shape)
          cur_shape->ui ()->touches ()->remove_child (t);
        s->ui ()->touches ()->add_child (t, djnn::to_string (id));
        t->set_current_shape (s);
        /* setting */
        s->ui ()->move_x ()->set_value (x, true);
        s->ui ()->move_y ()->set_value (y, true);
        t->set_touch_local_coords (s, x, y, true);
        /* touch and shape event */
        s->ui ()->move ()->schedule_activation ();
        t->enter ();
        
      }

      /* touch event */
      t->get_move ()->schedule_activation ();
      genericEnterLeave (s);

    } 
    /* touch do not exist = press */
    else {
      genericTouchPress (x, y, id, pressure);
    }

    return true;
  }

  bool
  Picking::genericMouseRelease (double x, double y, mouse_button button)
  {
    _mouse_released = true;
    bool exec_ = false;

    /* shape */
    PickUI *s = this->pick (x, y);
    if (s) {
      /* event */

      s->ui ()->release ()->schedule_activation ();
      s->ui ()->mouse_release ()->schedule_activation ();
      /* event if no mouse tracking */
      if (mouse_tracking == 0) {
        s->ui ()->leave ()->schedule_activation ();
        s->ui ()->mouse_leave ()->schedule_activation ();
      }

      exec_ = true;
    }

    /* reset _hovered_shape if no mouse tracking */
    if (mouse_tracking == 0) {
      _hovered_shape = nullptr;
    }

    /* reset _catched_shape */
    if (_cought_shape && _cought_shape != s) {
      FatProcess* p1 = _cought_shape->ui ()->release ();
      FatProcess* p2 = _cought_shape->ui ()->mouse_release ();
      p1->schedule_activation ();
      p2->schedule_activation ();
      exec_ = true;
    }
    _cought_shape = nullptr;

    /* button */
    switch (button)
      {
      case BUTTON_LEFT:
        ((GUIMouse*) GenericMouse)->left ()->release ()->activate ();
        if (s != nullptr) {
          s->ui()->left_release ()->activate ();
        }
        break;
      case BUTTON_RIGHT:
        ((GUIMouse*) GenericMouse)->right ()->release ()->activate ();
        if (s != nullptr) {
          s->ui()->right_release ()->activate ();
        }
        break;
      case BUTTON_MIDDLE:
        ((GUIMouse*) GenericMouse)->middle ()->release ()->activate ();
        if (s != nullptr) {
          s->ui()->middle_release ()->activate ();
        }
        break;
      default:
        ((GUIMouse*) GenericMouse)->left ()->release ()->activate ();
      }
    if (((GUIMouse*) GenericMouse)->left ()->release ()->has_coupling ()
        || ((GUIMouse*) GenericMouse)->right ()->release ()->has_coupling ()
        || ((GUIMouse*) GenericMouse)->middle ()->release ()->has_coupling ()) {
      exec_ = true;
    }

    /* windows event schedule event After shape event*/
    if (_win->release ()->has_coupling ()) {
      _win->release ()->schedule_activation ();
      exec_ = true;
    }

    return exec_;
  }

  bool
  Picking::genericTouchRelease (double x, double y, int id, float pressure)
  {
    
    PickUI *s = this->pick (x, y);
    map<int, Touch*>::iterator it = _active_touches.find (id);
    Touch *t;
    /* touch exist */
    if (it != _active_touches.end ()) {
      t = it->second;

      /* setting */
      t->set_move_x (x);
      t->set_move_y (y);
      t->set_pressure (pressure);
      t->set_id (id);

      /* shape */
      PickUI* current_shape = t->current_shape ();
      PickUI* init_shape = t->init_shape ();
      if (init_shape != nullptr) {
        init_shape->ui()->touches ()->remove_child (t);
        t->set_touch_local_coords (init_shape, x, y, true);
      }
      if (current_shape != nullptr) {
        current_shape->ui()->touches ()->remove_child (t);
      }

      /* touch event */
      if (current_shape || init_shape)
        t->leave ();

      t->set_init_shape (nullptr);
      t->set_current_shape (nullptr);

      /* remove touch from list */
      _win->touches ()->remove_child (t);
      _active_touches.erase (it);

      /* delay touch delete */
      t->schedule_delete ();
    }

    /* common shape event */
    if (s) {
      s->ui ()->release ()->schedule_activation ();
      s->ui ()->leave ()->schedule_activation ();
    }

    /* reset _catched_shape */
    if (_cought_shape && _cought_shape != s) {
      _cought_shape->ui ()->release ()->schedule_activation ();
    }
    _cought_shape = nullptr;

    /* reset _hovered_shape */
    _hovered_shape = nullptr;
    
    return true;
  }

  bool
  Picking::genericMouseWheel (double x, double y)
  {
    bool exec_ = false;

    /* window setting */
    _win->wheel_dx ()->set_value (x, true);
    _win->wheel_dy ()->set_value (y, true);
    
    /* generic mouse setting */
    ((GUIMouse*)GenericMouse)->dx ()->set_value (x, true);
    ((GUIMouse*)GenericMouse)->dy ()->set_value (y, true);
    /* generic mouse event */
    ((GUIMouse*)GenericMouse)->wheel ()->schedule_activation ();
    if (((GUIMouse*)GenericMouse)->wheel ()->has_coupling() || ((GUIMouse*)GenericMouse)->dx ()->has_coupling() || ((GUIMouse*)GenericMouse)->dy ()->has_coupling()) {
      exec_ = true;
     }

    /* windows event schedule event After shape event*/
    _win->wheel ()->schedule_activation ();
    if (_win->wheel ()->has_coupling () || _win->wheel_dx ()->has_coupling () || _win->wheel_dy ()->has_coupling ()) {
      exec_ = true;
    }

    return exec_;
  }

} /* namespace djnn */
