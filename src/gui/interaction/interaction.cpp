/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "interaction.h"

#include "core/execution/graph.h"
#include "core/tree/list.h"
#include "gui/transformation/transformations.h"



#include <cmath>
#include <iostream>

#define DBG std::cerr << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;

#ifndef M_PI
#define M_PI 3.141592653
#endif

namespace djnn
{
  ScaleRotateTranslate::ScaleRotateTranslate(Process *parent, const std::string &name, Process* shape, Process* matrix)
  : Process (name)
  {
    if (shape == nullptr || matrix == nullptr) {
      error (this, "Shape or Matrix cannot be null in ScaleRotateTranslate constructor");
      return;
    }
    _shape = shape;
    _matrix = dynamic_cast<AbstractHomography*> (matrix);
    if (!_matrix) {
      error (this, "Wrong type of Process for matrix provided to ScaleRotateTranslate constructor");
      return;
    }
    _touches = shape->find_component ("touches");
    if (!_touches) {
      error (this, "Wrong interface for Shape provided to ScaleRotateTranslate constructor");
      return;
    }

    _added = _touches->find_component ("$added");
    _removed = _touches->find_component ("$removed");
    _add_touch_action = new AddTouchAction (this, "add_touch_action");
    _remove_touch_action = new RemoveTouchAction (this, "remove_touch_action");
    _t_move1 = new TouchMoveAction (this, "touch_move1_action");
    _c_on_add = new Coupling (_added, ACTIVATION, _add_touch_action, ACTIVATION, true);
    _c_on_del = new Coupling (_removed, ACTIVATION, _remove_touch_action, ACTIVATION, true);
    _update_action = new ScaleRotateTranslateAction (this, "srt_action");
    _c_move = new Coupling (_t_move1, ACTIVATION, _update_action, ACTIVATION);
    Graph::instance().add_edge (_t_move1, _update_action);

    Process::finalize_construction (parent, name);
  }

  ScaleRotateTranslate::~ScaleRotateTranslate ()
  {

    Graph::instance().remove_edge (_t_move1, _update_action);
    delete _c_move;

    std::map<int, TouchAlive*>::iterator it;
    for ( it = touches.begin (); it != touches.end (); ++it ) {
      delete it->second;
      touches.erase (it);
    }

    delete _update_action;
    
    delete _c_on_del;
    delete _c_on_add;

    delete _t_move1;
    
    delete _remove_touch_action;
    delete _add_touch_action;
  }

  void
  ScaleRotateTranslate::impl_activate ()
  {
    _c_on_add->enable ();
    _c_on_del->enable ();
    _c_move->enable ();
  }

  void
  ScaleRotateTranslate::impl_deactivate ()
  {
    _c_on_add->disable ();
    _c_on_del->disable ();
    _c_move->disable ();
  }

  void
  ScaleRotateTranslate::add_touch ()
  {
    Touch *t = (Touch*) getRef (_added);
    touches[t->get_id ()] = new TouchAlive (new Point (t->get_move_x (), t->get_move_y ()), 
                                            new Point (t->get_move_x (), t->get_move_y ()),
                                            new Coupling (t->get_move (), ACTIVATION, _t_move1, ACTIVATION, t, true));
  }

  void
  ScaleRotateTranslate::remove_touch ()
  {
    Touch *t = (Touch*) getRef (_removed);
    std::map<int, TouchAlive*>::iterator it = touches.find (t->get_id ());
    if (it != touches.end ()) {
      delete it->second;
      touches.erase (it);
    } else {
      std::cout << "touch not found on remove\n";
    }
  }

  void
  ScaleRotateTranslate::touch_move (Touch *t)
  {
    std::map<int, TouchAlive*>::iterator it = touches.find (t->get_id ());
    if (it != touches.end ()) {
      TouchAlive *ta = it->second;
      ta->_last_pt->set_values (ta->_new_pt);
      ta->_new_pt->set_values (t->get_move_x (), t->get_move_y ());
    } else {
      std::cout << "touch not found on move\n";
    }
  }

  void
  ScaleRotateTranslate::update ()
  {
    double ds, dr, dx, dy;
    if (estimateTSR (touches, &dx, &dy, &ds, &dr)) {
       double scale = sqrt (dr*dr + ds*ds);
      _matrix->leftRotate (atan2 (dr, ds) * 180/M_PI);
      _matrix->leftScale (scale, scale);
      _matrix->leftTranslate (dx, dy);
    }
  }

} /* namespace djnn */
