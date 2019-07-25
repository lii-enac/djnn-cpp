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
#include "../../core/tree/list.h"
#include "../transformation/transformations.h"
#include <cmath>
#include <iostream>

#define DBG std::cerr << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;

namespace djnn
{
  ScaleRotateTranslate::ScaleRotateTranslate(Process *p, const string &n, Process* shape, Process* matrix) : Process (p, n), _state (START)
  {
    if (shape == nullptr || matrix == nullptr) {
      error (this, "Shape or Matrix cannot be null in ScaleRotateTranslate constructor");
    }
    _shape = shape;
    _matrix = dynamic_cast<AbstractHomography*> (matrix);
    if (!_matrix) {
      error (this, "Wrong type of Process for matrix provided to ScaleRotateTranslate constructor");
    }
    _touches = shape->find_component ("touches");
    if (!_touches) {
      error (this, "Wrong interface for Shape provided to ScaleRotateTranslate constructor");
    }

    _added = _touches->find_component ("$added");
    _removed = _touches->find_component ("$removed");
    _add_touch_action = new AddTouchAction (this, "add_touch_action");
    _remove_touch_action = new RemoveTouchAction (this, "remove_touch_action");
    _t_move1 = new TouchMoveAction (this, "touch_move1_action");
    _t_move2 = new TouchMoveAction (this, "touch_move2_action");
    _c_on_add = new Coupling (_added, ACTIVATION, _add_touch_action, ACTIVATION, true);
    _c_on_del = new Coupling (_removed, ACTIVATION, _remove_touch_action, ACTIVATION, true);
    _update_action = new ScaleRotateTranslateAction (this, "srt_action");
    _c_move_t1 = new Coupling (_t_move1, ACTIVATION, _update_action, ACTIVATION);
    _c_move_t2 = new Coupling (_t_move2, ACTIVATION, _update_action, ACTIVATION);

    Graph::instance().add_edge (_t_move1, _update_action);
    Graph::instance().add_edge (_t_move2, _update_action);

    _c_move_c1 = _c_move_c2 = nullptr;

    _last_point1 = new Point();
    _last_point2 = new Point();
    _new_point1 = new Point();
    _new_point2 = new Point();
    _last_point_hyst_1 = new Point ();
    _last_point_hyst_2 = new Point ();

    Process::finalize_construction ();
  }

  ScaleRotateTranslate::~ScaleRotateTranslate ()
  {
    Graph::instance().remove_edge (_t_move1, _update_action);
    Graph::instance().remove_edge (_t_move2, _update_action);

    delete _c_on_del;
    delete _c_on_add;
    delete _c_move_c1;
    delete _c_move_c2;
    delete _c_move_t1;
    delete _c_move_t2;

    delete _update_action;
    delete _add_touch_action;
    delete _remove_touch_action;
    delete _t_move1;
    delete _t_move2;

    delete _last_point1;
    delete _last_point2;
    delete _new_point1;
    delete _new_point2;
    delete _last_point_hyst_1;
    delete _last_point_hyst_2;
  }

  void
  ScaleRotateTranslate::impl_activate ()
  {
    _c_on_add->enable ();
    _c_on_del->enable ();
    if (_c_move_c1)
      _c_move_c1->enable ();
    if (_c_move_c2)
      _c_move_c2->enable ();
  }

  void
  ScaleRotateTranslate::impl_deactivate ()
  {
    _c_on_add->disable ();
    _c_on_del->disable ();
    if (_c_move_c1)
      _c_move_c1->disable ();
    if (_c_move_c2)
      _c_move_c2->disable ();
  }



  void
  ScaleRotateTranslate::add_touch ()
  {
    Touch *t = (Touch*) getRef (_added);
    switch (_state){
      case START:
        _last_point1->set_values (t->get_move_x (), t->get_move_y ());
        _new_point1->set_values (_last_point1);
        _cursor1_id = t->get_id ();
        if (_c_move_c1 == nullptr)
          _c_move_c1 = new Coupling (t->get_move (), ACTIVATION, _t_move1, ACTIVATION, t, true);
        else
          _c_move_c1->change_source (t->get_move (), t);
        _c_move_c1->enable ();
        change_state (DRAGGING);
        break;
      case DRAGGING:
        _last_point2->set_values (t->get_move_x (), t->get_move_y ());
        _new_point2->set_values (_last_point2);
        _cursor2_id = t->get_id ();
        if (_c_move_c2 == nullptr)
          _c_move_c2 = new Coupling (t->get_move (), ACTIVATION, _t_move2, ACTIVATION, t, true);
        else
          _c_move_c2->change_source (t->get_move (), t);
        _c_move_c2->enable ();
        change_state (RRR);
        break;
      default:;
    }
  }

  void
  ScaleRotateTranslate::remove_touch ()
  {
    Touch *t = (Touch*) getRef (_removed);
    switch (_state) {
      case DRAGGING:
        if (t->get_id () == _cursor1_id) {
          change_state (START);
          _c_move_c1->change_source (nullptr);
        }
        break;
      case RRR:
        if (_cursor1_id == t->get_id ()) {
          _cursor1_id = _cursor2_id;
          _last_point1->set_values (_last_point2);
          _new_point1->set_values (_new_point2);
          _c_move_c1->change_source (_c_move_c2->get_src (), _c_move_c2->get_data ());
          _c_move_c1->enable ();
        }
        _c_move_c2->disable ();
        _c_move_c2->change_source (nullptr);
        change_state (DRAGGING);
        break;
      default:;
    }
  }

  void
  ScaleRotateTranslate::touch_move (Touch *t)
  {
    switch (_state)
      {
      case DRAGGING:
        {
          if (t->get_id () != _cursor1_id) { return; }
          _new_point1->set_values (t->get_move_x (), t->get_move_y ());
          Vector v = Point::minus (*_new_point1, *_last_point1);
          _matrix->leftTranslate (v.dx (), v.dy ());
          _last_point1->set_values (*_new_point1);
          break;
        }
      case RRR:
        {
          if (t->get_id () == _cursor1_id) {
          _new_point1->set_values (t->get_move_x (), t->get_move_y ());
          }
          else if (t->get_id () == _cursor2_id) {
            _new_point2->set_values (t->get_move_x (), t->get_move_y ());
          }
          break;
        }
      default:
        ;
      }
  }

  void
  ScaleRotateTranslate::update ()
  {
    if (_state != RRR || (_last_point1->equals (*_new_point1) && _last_point2->equals (*_new_point2))) {
        return;
	}
	vector< pair<double, double> > old_pts = { {_last_point1->x (), _last_point1->y ()},
		                     {_last_point2->x (), _last_point2->y ()} };

	vector< pair<double, double> > new_pts = { {_new_point1->x (), _new_point1->y ()},
		                     {_new_point2->x (), _new_point2->y ()} };
    double ds, dr, dx, dy;
    estimateTSR (old_pts, new_pts, &dx, &dy, &ds, &dr);

    _matrix->leftTranslate (dx, dy);
    _matrix->leftRotate (atan2 (dr, ds) * 180/M_PI);
    double scale = sqrt (dr*dr + ds*ds);
    _matrix->leftScale (scale, scale);
    _last_point1->set_values (_new_point1);
    _last_point2->set_values (_new_point2);
  }
} /* namespace djnn */
