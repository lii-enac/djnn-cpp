/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019-2024)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include <cmath>

#include "interaction.h"

#include "core/tree/list.h"
#include "core/utils/error.h"
#include "gui/transformation/homography.h"

#include "core/utils/iostream.h"

namespace djnn {

static const char*
print_state (State s)
{
    switch (s) {
    case START:
        return "START";
        break;
    case WAITHYST:
        return "WAITHYST";
        break;
    case WAITHYST2:
        return "WAITHYST2";
        break;
    case DRAGGING:
        return "DRAGGING";
        break;
    case RRR:
        return "RRR";
        break;
    case RESIZING:
        return "RESIZING";
        break;
    default:
        return "UNKNOWN STATE";
        break;
    }
}

template <typename K, typename V>
V GetFirstNotId(std::map<K, V>& m, const K& id) {
    for (auto& pair : m) {
        if (pair.first != id) {
            return pair.second; // V is already a pointer
        }
    }
    return nullptr; // Return nullptr if no such element is found
}

static void
print_matrix (const std::string& s, Matrix4x4 m)
{
    std::cerr << s << ":" << std::endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cerr << m[i][j] << " ";
        }
        std::cerr << std::endl;
    }
    std::cerr << std::endl;
}

static void
print_matrix (const std::string& s, AbstractHomography* m)
{
    std::cerr << s << ":" << std::endl;
    std::cerr << m->m11 ()->get_value () << "  " << m->m12 ()->get_value () << "  " << m->m13 ()->get_value () << "  " << m->m14 ()->get_value () << std::endl;
    std::cerr << m->m21 ()->get_value () << "  " << m->m22 ()->get_value () << "  " << m->m23 ()->get_value () << "  " << m->m24 ()->get_value () << std::endl;
    std::cerr << m->m31 ()->get_value () << "  " << m->m32 ()->get_value () << "  " << m->m33 ()->get_value () << "  " << m->m34 ()->get_value () << std::endl;
    std::cerr << m->m41 ()->get_value () << "  " << m->m42 ()->get_value () << "  " << m->m43 ()->get_value () << "  " << m->m44 ()->get_value () << std::endl;
    std::cerr << std::endl;
}

static void
LeftRotateByVector (AbstractHomography* matrix, Vector u, Vector v, Point center)
{

    // Compute transform matrix for shape
    Matrix4x4 t (matrix->m11 ()->get_value (), matrix->m12 ()->get_value (), matrix->m13 ()->get_value (), matrix->m14 ()->get_value (),
                 matrix->m21 ()->get_value (), matrix->m22 ()->get_value (), matrix->m23 ()->get_value (), matrix->m24 ()->get_value (),
                 matrix->m31 ()->get_value (), matrix->m32 ()->get_value (), matrix->m33 ()->get_value (), matrix->m34 ()->get_value (),
                 matrix->m41 ()->get_value (), matrix->m42 ()->get_value (), matrix->m43 ()->get_value (), matrix->m44 ()->get_value ());

    double uvnorm = u.norm () * v.norm ();
    double cosa   = Vector::scalar_product (u, v) / uvnorm;
    double sina   = Vector::cross_product (u, v) / uvnorm;

    Matrix4x4 rot;
    rot.set_sin_cos (-sina, cosa);

    t.left_translate_by (-center.x (), -center.y ());
    t.left_multiply_by (rot);
    t.left_translate_by (center.x (), center.y ());

    matrix->m11 ()->set_value (t[0][0], true);
    matrix->m12 ()->set_value (t[0][1], true);
    // matrix->m13 ()->set_value (t[0][2], true); //
    matrix->m14 ()->set_value (t[0][3], true);

    matrix->m21 ()->set_value (t[1][0], true);
    matrix->m22 ()->set_value (t[1][1], true);
    // matrix->m23 ()->set_value (t[1][2], true); //
    matrix->m24 ()->set_value (t[1][3], true);

    // matrix->m31 ()->set_value (t[2][0], true); //
    // matrix->m32 ()->set_value (t[2][1], true); //
    // matrix->m33 ()->set_value (t[2][2], true); //
    // matrix->m34 ()->set_value (t[2][3], true); //
    // matrix->m41 ()->set_value (t[3][0], true); //
    // matrix->m42 ()->set_value (t[3][1], true); //
    // matrix->m43 ()->set_value (t[3][2], true); //
    // matrix->m44 ()->set_value (t[3][3], true); //

    // std::cerr << "final: --- " << center.x () << " - " << center.y () << std::endl;
    // print_matrix ("matrix final", matrix);
}

static void
leftScale (AbstractHomography* matrix, double scale, Point center)
{

    Matrix4x4 t (matrix->m11 ()->get_value (), matrix->m12 ()->get_value (), matrix->m13 ()->get_value (), matrix->m14 ()->get_value (),
                 matrix->m21 ()->get_value (), matrix->m22 ()->get_value (), matrix->m23 ()->get_value (), matrix->m24 ()->get_value (),
                 matrix->m31 ()->get_value (), matrix->m32 ()->get_value (), matrix->m33 ()->get_value (), matrix->m34 ()->get_value (),
                 matrix->m41 ()->get_value (), matrix->m42 ()->get_value (), matrix->m43 ()->get_value (), matrix->m44 ()->get_value ());

    Matrix4x4 scaleMatrix;
    scaleMatrix[0][0] = scale;
    scaleMatrix[1][1] = scale;

    t.left_translate_by (-center.x (), -center.y ());
    t.left_multiply_by (scaleMatrix);
    t.left_translate_by (center.x (), center.y ());

    matrix->m11 ()->set_value (t[0][0], true);
    matrix->m12 ()->set_value (t[0][1], true);
    // matrix->m13 ()->set_value (t[0][2], true); //
    matrix->m14 ()->set_value (t[0][3], true);

    matrix->m21 ()->set_value (t[1][0], true);
    matrix->m22 ()->set_value (t[1][1], true);
    // matrix->m23 ()->set_value (t[1][2], true); //
    matrix->m24 ()->set_value (t[1][3], true);

    // matrix->m31 ()->set_value (t[2][0], true); //
    // matrix->m32 ()->set_value (t[2][1], true); //
    // matrix->m33 ()->set_value (t[2][2], true); //
    // matrix->m34 ()->set_value (t[2][3], true); //

    // matrix->m41 ()->set_value (t[3][0], true); //
    // matrix->m42 ()->set_value (t[3][1], true); //
    // matrix->m43 ()->set_value (t[3][2], true); //
    // matrix->m44 ()->set_value (t[3][3], true); //
}

static bool
computeRRR (AbstractHomography* matrix, Point& fixedPoint, Point& oldPoint, Point& newPoint)
{

    if (oldPoint.equals (newPoint) == true) {
        return false;
    }
    if (fixedPoint.equals (newPoint) == true) {
        return false;
    }

    Vector u = Point::minus (newPoint, fixedPoint);
    Vector v = Point::minus (oldPoint, fixedPoint);
    LeftRotateByVector (matrix, u, v, fixedPoint);

    // Calcul de l'échelle entre u et v
    double dscale = u.norm () / v.norm ();

    // Appliquer la mise à l'échelle à la matrice
    leftScale (matrix, dscale, fixedPoint);

    return true;
}

static void
computeResizing (AbstractHomography* matrix, Point& fixedPoint, Point& oldPoint, Point& newPoint)
{
    double dscale = Point::distance (fixedPoint, newPoint) / Point::distance (fixedPoint, oldPoint);
    leftScale (matrix, dscale, fixedPoint);
}

AbstractRRR::AbstractRRR (CoreProcess* parent, const string& name, CoreProcess* shape, CoreProcess* matrix)
    : FatProcess (name),
      _current_FSM_state (START),
      _distHyst (1000), // square distance
      _shape (nullptr), _touches_shape (nullptr),
      _added (nullptr), _removed (nullptr), _matrix (nullptr)
{
    if (shape == nullptr || matrix == nullptr) {
        error (this, "Shape or Matrix cannot be null in RRR_2T constructor");
        return;
    }
    _shape  = shape;
    _matrix = dynamic_cast<AbstractHomography*> (matrix);
    if (!_matrix) {
        error (this, "Wrong type of FatProcess for matrix provided to RRR_2T constructor");
        return;
    }
    _touches_shape = dynamic_cast<FatProcess*> (_shape->find_child_impl ("touches"));
    if (!_touches_shape) {
        error (this, "Wrong interface for Shape provided to RRR_2T constructor");
        return;
    }

    _press_touch_action   = new AddTouchAction (this, "add_touch_action");
    _release_touch_action = new RemoveTouchAction (this, "remove_touch_action");

    _added    = dynamic_cast<FatProcess*> (_touches_shape->find_child_impl ("$added"));
    _c_on_add = new Coupling (_added, ACTIVATION, _press_touch_action, ACTIVATION, true);

    _removed  = dynamic_cast<FatProcess*> (_touches_shape->find_child_impl ("$removed"));
    _c_on_del = new Coupling (_removed, ACTIVATION, _release_touch_action, ACTIVATION, true);

    finalize_construction (parent, name);
}

AbstractRRR::~AbstractRRR ()
{

    map<int, RRR_touch*>::iterator it = _touches_map_RRR.begin();
    while (it != _touches_map_RRR.end()) {
        RRR_touch* touch = it->second;
        it = _touches_map_RRR.erase(it); // keep track on the iterator
        delete touch;
    }

    delete _c_on_del;
    delete _c_on_add;

    delete _release_touch_action;
    delete _press_touch_action;
}

void
AbstractRRR::impl_activate ()
{
    _c_on_add->enable ();
    _c_on_del->enable ();
}

void
AbstractRRR::impl_deactivate ()
{
    _c_on_add->disable ();
    _c_on_del->disable ();
}

RRR_touch::RRR_touch (AbstractRRR* srt, Touch* t) //, Action* action)
    : last_pt (t->get_move_x (), t->get_move_y ()),
      new_pt (last_pt),
      t (t),
      _move_touch_action (srt, "_move_touch_action", this),

      _cpl (t->get_move (), ACTIVATION, &_move_touch_action, ACTIVATION, true)
{
    _cpl.enable ();
}

RRR_touch::~RRR_touch ()
{
    _cpl.disable ();
}

void
RRR_touch::MoveTouchAction::impl_activate ()
{
    ((AbstractRRR*)get_parent ())->on_move_touch_event (ta->t);
}

void
RRR_2T::on_press_touch_event ()
{

    Touch* added_touch = (Touch*)getRef (_added);

    // debug
    // std::cerr << "=====> on_press_touch_event - " << added_touch->get_id () << std::endl;

    auto new_touch_alive                     = new RRR_touch (this, added_touch);
    _touches_map_RRR[added_touch->get_id ()] = new_touch_alive;

    switch (_current_FSM_state) {
    case START:
        // action
        _lastPoint1     = new_touch_alive->new_pt;
        _lastPointHyst1 = _lastPoint1;
        _touchID1       = added_touch->get_id ();
        // goto
        _current_FSM_state = WAITHYST;
        break;
    case WAITHYST:
        // action
        _lastPoint2     = new_touch_alive->new_pt;
        _lastPointHyst2 = _lastPoint2;
        _touchID2       = added_touch->get_id ();
        // goto
        _current_FSM_state = WAITHYST2;
        break;
    case DRAGGING:
        _touchID2       = added_touch->get_id ();
        _lastPoint2     = new_touch_alive->new_pt;
        _lastPointHyst2 = _lastPoint2;

        _current_FSM_state = RRR;
        break;
    default:
        break;
    }

    // debug
    // std::cerr << "Current State: " << print_state (_current_FSM_state) << std::endl;
}

void
RRR_2T::on_release_touch_event ()
{
    Touch* t = (Touch*)getRef (_removed);

    // debug
    // std::cerr << "<===== on_release_touch_event - " << t->get_id () << std::endl;

    map<int, RRR_touch*>::iterator it = _touches_map_RRR.find (t->get_id ());
    if (it == _touches_map_RRR.end ()) {
        warning (this, "touch not found on release");
        return;
    }

    switch (_current_FSM_state) {
    case WAITHYST:
        if (t->get_id () == _touchID1) {
            _current_FSM_state = START;
        }
        break;
    case WAITHYST2:
        if ((t->get_id () == _touchID1) || (t->get_id () == _touchID2)) {
            if (t->get_id () == _touchID1) {
                _touchID1       = _touchID2;
                _lastPoint1     = _lastPoint2;
                _lastPointHyst1 = _lastPointHyst2;
            }
            _current_FSM_state = WAITHYST;
        }
        break;
    case DRAGGING:
        if (t->get_id () == _touchID1) {
            _current_FSM_state = START;
        }
        break;
    case RRR:
        if ((t->get_id () == _touchID1) || (t->get_id () == _touchID2)) {
            if (t->get_id () == _touchID1) {
                _touchID1   = _touchID2;
                _lastPoint1 = _lastPoint2;
            }
            _current_FSM_state = DRAGGING;
        }
        break;
    default:
        break;
    }

    // erase and delete
    RRR_touch* touch = it->second;
    _touches_map_RRR.erase (it);
    delete touch;

    // debug
    // std::cerr << "Current State: " << print_state (_current_FSM_state) << std::endl;
}

void
RRR_2T::on_move_touch_event (Touch* t)
{
    // debug
    // std::cerr << "- RRR_move_touch - " << t->get_id () << std::endl;

    map<int, RRR_touch*>::iterator it = _touches_map_RRR.find (t->get_id ());
    if (it == _touches_map_RRR.end ()) {
        warning (this, "touch not found on move");
        return;
    }

    // update touch alive
    RRR_touch* ta = it->second;
    ta->last_pt.set_values (ta->new_pt);
    ta->new_pt.set_values (t->get_move_x (), t->get_move_y ());

    // debug - To avoid having too much irrelevant information.
    // bool is_state_changed = false;

    bool guard = false;

    switch (_current_FSM_state) {
    case WAITHYST:
        // guard
        if (t->get_id () != _touchID1)
            return;
        _lastPointHyst1 = ta->new_pt;
        guard           = !(Point::distance_sq (_lastPoint1, _lastPointHyst1) < _distHyst);
        // action & goto
        if (guard) {
            _lastPoint1        = _lastPointHyst1;
            _current_FSM_state = DRAGGING;
            // is_state_changed = true;
        }
        break;
    case WAITHYST2:
        // guard

        if (t->get_id () == _touchID1) {
            _lastPointHyst1 = ta->new_pt;
            guard           = !(Point::distance_sq (_lastPoint1, _lastPointHyst1) < _distHyst);
        } else if (t->get_id () == _touchID2) {
            _lastPointHyst2 = ta->new_pt;
            guard           = !(Point::distance_sq (_lastPoint2, _lastPointHyst2) < _distHyst);
        }

        // action & goto
        if (guard) {
            _lastPoint1        = _lastPointHyst1;
            _lastPoint2        = _lastPointHyst2;
            _current_FSM_state = RRR;
            // is_state_changed = true;
        }
        break;
    case DRAGGING:
        // guard & action
        if (t->get_id () == _touchID1) {
            Point  newPoint = ta->new_pt;
            Vector v        = Point::minus (newPoint, _lastPoint1);
            _matrix->leftTranslate (v.dx (), v.dy ());
            _lastPoint1 = newPoint;
        }
        break;
    case RRR:
        // guard & action
        if ((t->get_id() == _touchID1) || (t->get_id() == _touchID2)) {
            Point &fixedPoint = (t->get_id() == _touchID1) ? _lastPoint2 : _lastPoint1;
            Point &oldPoint = (t->get_id() == _touchID1) ? _lastPoint1 : _lastPoint2;
            Point &newPoint = ta->new_pt;

            // Local copies of points for updating _lastPoint1 and _lastPoint2 after RRR is applied
            Point newPoint1 = _lastPoint1;
            Point newPoint2 = _lastPoint2;

            if (t->get_id() == _touchID1) {
                newPoint1 = newPoint;
            } else {
                newPoint2 = newPoint;
            }

            bool RRRapplied = computeRRR(_matrix, fixedPoint, oldPoint, newPoint);
            if (RRRapplied) {
                _lastPoint1 = newPoint1;
                _lastPoint2 = newPoint2;
            }
        }
        break;
    default:
        break;
    }

    // debug - To avoid having too much irrelevant information.
    //  if (is_state_changed)
    //      std::cerr << "Current State: " << print_state (_current_FSM_state) << std::endl;
}

void
RRR_MT::on_press_touch_event ()
{

    Touch* added_touch = (Touch*)getRef (_added);
    Point touch_point (added_touch->get_move_x (), added_touch->get_move_y ());

    // debug
    // std::cerr << "=====> on_press_touch_event - " << added_touch->get_id () << std::endl;

    RRR_touch* newTouch                      = new RRR_touch (this, added_touch);
    newTouch->new_pt                         = touch_point;
    _touches_map_RRR[added_touch->get_id ()] = newTouch;

    switch (_current_FSM_state) {
    case START:
        _touch1            = newTouch;
        _current_FSM_state = WAITHYST;
        break;

    case WAITHYST:
        _touch2            = newTouch;
        _current_FSM_state = WAITHYST2;
        break;

    case WAITHYST2:
    case RRR:
        _touch2 = newTouch;
        break;

    case DRAGGING:
        _touch2            = newTouch;
        _current_FSM_state = RRR;
        break;

    default:
        break;
    }

    //debug
    //std::cerr << "Current State: " << print_state (_current_FSM_state) << std::endl;
}

void
RRR_MT::on_release_touch_event ()
{
    Touch* t = (Touch*)getRef (_removed);

    // debug
    // std::cerr << "<===== on_release_touch_event - " << t->get_id () << std::endl;

    map<int, RRR_touch*>::iterator it = _touches_map_RRR.find (t->get_id ());
    if (it == _touches_map_RRR.end ()) {
        warning (this, "touch not found on release");
        return;
    }

    bool guard = false;
    RRR_touch* rrr_touch = it->second;

    switch (_current_FSM_state) {
    case WAITHYST:
        //case 1 = not the last touch
        
        //guard
        guard = (_touches_map_RRR.size () > 1) && (it != _touches_map_RRR.end ());

        // action and quit
        if (guard){
            _touches_map_RRR.erase (it);
            delete rrr_touch;
            _touch1 = GetFirstNotId (_touches_map_RRR, _touch2->t->get_id ()); //changed _touch1 reference
            break;
        }

        // case 2 : last touch 
        guard = (_touches_map_RRR.size () == 1) && (it->second->t->get_id () == _touch1->t->get_id ());

        // action and goto
        if (guard) {
            _touches_map_RRR.erase (it);
            delete rrr_touch;
            _touches_map_RRR.clear ();
            _current_FSM_state = START;
        }
        break;
    case WAITHYST2:
        if (it != _touches_map_RRR.end()) {
            // Case 1: More than two touches
            if (_touches_map_RRR.size() > 2) {

                // Erase the touch and update references if necessary
                if (it->second->t->get_id() == _touch1->t->get_id()) {
                    _touches_map_RRR.erase(it);
                    delete rrr_touch;
                    _touch1 = GetFirstNotId(_touches_map_RRR, _touch2->t->get_id());
                } else if (it->second->t->get_id() == _touch2->t->get_id()) {
                    _touches_map_RRR.erase(it);
                    delete rrr_touch;
                    _touch2 = GetFirstNotId(_touches_map_RRR, _touch1->t->get_id());
                } else {
                    _touches_map_RRR.erase(it);
                    delete rrr_touch;
                }
                break;
            } 

            // Case 2: Exactly two touches
            if (_touches_map_RRR.size() == 2) {

                // Erase the touch and update FSM state
                if (it->second->t->get_id() == _touch1->t->get_id()) {
                    _touches_map_RRR.erase(it);
                    delete rrr_touch;
                    _touch1 = _touch2;
                    _touch2 = nullptr;
                    _current_FSM_state = DRAGGING;
                } else if (it->second->t->get_id() == _touch2->t->get_id()) {
                    _touches_map_RRR.erase(it);
                    delete rrr_touch;
                    _touch2 = nullptr;
                    _current_FSM_state = DRAGGING;
                }
                break;
            }
        }
        break;
    case DRAGGING:
        // case 1 : _touch1
        guard = (_touches_map_RRR.size () > 1) && (it->second->t->get_id () == _touch1->t->get_id ());

        if (guard) {
            _touches_map_RRR.erase (it);
            delete rrr_touch;
            _touch1 = GetFirstNotId (_touches_map_RRR, _touch2->t->get_id ()); //changed _touch1 reference
            break;
        }

        // case 2 : another touch
        guard = (_touches_map_RRR.size () > 1) && (it != _touches_map_RRR.end ());

        if (guard) {
            _touches_map_RRR.erase (it);
            delete rrr_touch;
            // no reference to change
            break;
        }

        // case 3 : lasttouch
        guard = (_touches_map_RRR.size () == 1) && (it->second->t->get_id () == _touch1->t->get_id ());

        // action and goto
        if (guard) {
            _touches_map_RRR.erase (it);
            delete rrr_touch;
            _touches_map_RRR.clear ();
            _current_FSM_state = START;
        }
        break;
    case RRR:
        if (it != _touches_map_RRR.end()) {
            // Case 1: More than two touches
            if (_touches_map_RRR.size() > 2) {
                // Erase the touch and update references if necessary
                if (it->second->t->get_id() == _touch1->t->get_id()) {
                    _touches_map_RRR.erase(it);
                    delete rrr_touch;
                    _touch1 = GetFirstNotId(_touches_map_RRR, _touch2->t->get_id());
                } else if (it->second->t->get_id() == _touch2->t->get_id()) {
                    _touches_map_RRR.erase(it);
                    delete rrr_touch;
                    _touch2 = GetFirstNotId(_touches_map_RRR, _touch1->t->get_id());
                } else {
                    _touches_map_RRR.erase(it);
                    delete rrr_touch;
                }
                break;
            } 

            // Case 2: Exactly two touches
            if (_touches_map_RRR.size() == 2) {
                // Erase the touch and update FSM state
                if (it->second->t->get_id() == _touch1->t->get_id()) {
                    _touches_map_RRR.erase(it);
                    delete rrr_touch;
                    _touch1 = _touch2;
                    _touch2 = nullptr;
                    _current_FSM_state = DRAGGING;
                } else if (it->second->t->get_id() == _touch2->t->get_id()) {
                    _touches_map_RRR.erase(it);
                    delete rrr_touch;
                    _touch2 = nullptr;
                    _current_FSM_state = DRAGGING;
                }
                break;
            } 
        }
        break;
    default:
        break;
    }

    //debug
    //std::cerr << "Current State: " << print_state (_current_FSM_state) << std::endl;
}

void
RRR_MT::on_move_touch_event (Touch* touch)
{
    // debug
    // std::cerr << "- RRR_move_touch - " << t->get_id () << std::endl;

    //debug - To avoid having too much irrelevant information.
    //bool is_state_changed = false;

    bool guard = false;
    map<int, RRR_touch*>::iterator it;

    Point touch_point (touch->get_move_x (), touch->get_move_y ());

    switch (_current_FSM_state) {
    case WAITHYST:
        // guard
        guard = (touch->get_id () == _touch1->t->get_id ()) && (Point::distance_sq (_touch1->new_pt, touch_point) > _distHyst);

        // action & goto
        if (guard) {
           _touch1->new_pt = touch_point;
           _current_FSM_state = DRAGGING;
           //is_state_changed = true;
        }
        break;
    case WAITHYST2:
        // case 1 : _touch1
        guard = (touch->get_id () == _touch1->t->get_id ()) && (Point::distance_sq (_touch1->new_pt, touch_point) > _distHyst);

        if (guard) {
            _touch1->new_pt = touch_point ;
            _current_FSM_state = RRR;
            //is_state_changed = true;
            break;
        }
        
        // case 2 : _touch2
        guard = (touch->get_id () == _touch2->t->get_id ()) && (Point::distance_sq (_touch2->new_pt, touch_point) > _distHyst);

        if (guard) {
            _touch2->new_pt = touch_point ;
            _current_FSM_state = RRR;
            //is_state_changed = true;
            break;
        }

        // case 3 : another touch
        it = _touches_map_RRR.find (touch->get_id ());
        guard =  (it != _touches_map_RRR.end () &&  it->first != _touch1->t->get_id () && it->first != _touch2->t->get_id ());

        if (guard) {
            it->second->new_pt = touch_point;
        }
        break;
    case DRAGGING:
        // case 1 : touch is _touch1 
        // guard & action
        if (touch->get_id () == _touch1->t->get_id()) {
            Vector v = Point::minus (touch_point, _touch1->new_pt);
            _matrix->leftTranslate (v.dx (), v.dy());
            _touch1->new_pt = touch_point;
            break;
        }

        // case 2 : it's another known touch that moving : we update it
        it = _touches_map_RRR.find (touch->get_id ());
        if (it != _touches_map_RRR.end ()) {
            it->second->new_pt = touch_point;
        }

        break;
    case RRR:
        // case 1 : _touch1
        guard = (touch->get_id () == _touch1->t->get_id ());

        if (guard) {
            bool RRRapplied = computeRRR (_matrix, _touch2->new_pt, _touch1->new_pt, touch_point);
            if (RRRapplied)
                _touch1->new_pt = touch_point;
            break;
        }

        // case 2 : _touch2
        guard = (touch->get_id () == _touch2->t->get_id ());

        if (guard) {
            bool RRRapplied = computeRRR (_matrix, _touch1->new_pt, _touch2->new_pt, touch_point);
            if (RRRapplied)
                _touch2->new_pt = touch_point;
            break;
        }

        // case 3 : other touches
        it = _touches_map_RRR.find (touch->get_id ());
        if (it != _touches_map_RRR.end ()) {
            it->second->new_pt = touch_point;
        }
        break;
    default:
        break;
    }

    // debug - To avoid having too much irrelevant information.
    // if (is_state_changed)
    //     std::cerr << "Current State: " << print_state (_current_FSM_state) << std::endl;
}

void
RR_T::on_press_touch_event ()
{

    Touch* added_touch = (Touch*)getRef (_added);

    // debug
    // std::cerr << "=====> on_press_touch_event - " << added_touch->get_id () << std::endl;

    auto new_touch_alive                     = new RRR_touch (this, added_touch);
    _touches_map_RRR[added_touch->get_id ()] = new_touch_alive;

    switch (_current_FSM_state) {
    case START:
        // action
        _lastPoint1     = new_touch_alive->new_pt;
        _lastPointHyst1 = _lastPoint1;
        _touchID1       = added_touch->get_id ();
        // goto
        _current_FSM_state = WAITHYST;
        break;
    case WAITHYST:
        // action
        _lastPoint2     = new_touch_alive->new_pt;
        _lastPointHyst2 = _lastPoint2;
        _touchID2       = added_touch->get_id ();
        // goto
        _current_FSM_state = WAITHYST2;
        break;
    case DRAGGING:
        _touchID2       = added_touch->get_id ();
        _lastPoint2     = new_touch_alive->new_pt;
        _lastPointHyst2 = _lastPoint2;

        _current_FSM_state = RESIZING;
        break;
    default:
        break;
    }

    // debug
    // std::cerr << "Current State: " << print_state (_current_FSM_state) << std::endl;
}

void
RR_T::on_release_touch_event ()
{
    Touch* t = (Touch*)getRef (_removed);

    // debug
    // std::cerr << "<===== on_release_touch_event - " << t->get_id () << std::endl;

    map<int, RRR_touch*>::iterator it = _touches_map_RRR.find (t->get_id ());
    if (it == _touches_map_RRR.end ()) {
        warning (this, "touch not found on release");
        return;
    }

    switch (_current_FSM_state) {
    case WAITHYST:
        if (t->get_id () == _touchID1) {
            _current_FSM_state = START;
        }
        break;
    case WAITHYST2:
        if ((t->get_id () == _touchID1) || (t->get_id () == _touchID2)) {
            if (t->get_id () == _touchID1) {
                _touchID1       = _touchID2;
                _lastPoint1     = _lastPoint2;
                _lastPointHyst1 = _lastPointHyst2;
            }
            _current_FSM_state = WAITHYST;
        }
        break;
    case DRAGGING:
        if (t->get_id () == _touchID1) {
            _current_FSM_state = START;
        }
        break;
    case RESIZING:
        if ((t->get_id () == _touchID1) || (t->get_id () == _touchID2)) {
            if (t->get_id () == _touchID1) {
                _touchID1   = _touchID2;
                _lastPoint1 = _lastPoint2;
            }
            _current_FSM_state = DRAGGING;
        }
        break;
    default:
        break;
    }

    // erase and delete
    RRR_touch* touch = it->second;
    _touches_map_RRR.erase (it);
    delete touch;

    // debug
    // std::cerr << "Current State: " << print_state (_current_FSM_state) << std::endl;
}

void
RR_T::on_move_touch_event (Touch* t)
{
    // debug
    // std::cerr << "- RRR_move_touch - " << t->get_id () << std::endl;

    map<int, RRR_touch*>::iterator it = _touches_map_RRR.find (t->get_id ());
    if (it == _touches_map_RRR.end ()) {
        warning (this, "touch not found on move");
        return;
    }

    // update touch alive
    RRR_touch* ta = it->second;
    ta->last_pt.set_values (ta->new_pt);
    ta->new_pt.set_values (t->get_move_x (), t->get_move_y ());

    // debug - To avoid having too much irrelevant information.
    // bool is_state_changed = false;

    bool guard = false;

    switch (_current_FSM_state) {
    case WAITHYST:
        // guard
        if (t->get_id () != _touchID1)
            return;
        _lastPointHyst1 = ta->new_pt;
        guard           = !(Point::distance_sq (_lastPoint1, _lastPointHyst1) < _distHyst);
        // action & goto
        if (guard) {
            _lastPoint1        = _lastPointHyst1;
            _current_FSM_state = DRAGGING;
            // is_state_changed = true;
        }
        break;
    case WAITHYST2:
        // guard

        if (t->get_id () == _touchID1) {
            _lastPointHyst1 = ta->new_pt;
            guard           = !(Point::distance_sq (_lastPoint1, _lastPointHyst1) < _distHyst);
        } else if (t->get_id () == _touchID2) {
            _lastPointHyst2 = ta->new_pt;
            guard           = !(Point::distance_sq (_lastPoint2, _lastPointHyst2) < _distHyst);
        }

        // action & goto
        if (guard) {
            _lastPoint1        = _lastPointHyst1;
            _lastPoint2        = _lastPointHyst2;
            _current_FSM_state = RESIZING;
            // is_state_changed = true;
        }
        break;
    case DRAGGING:
        // guard & action
        if (t->get_id () == _touchID1) {
            Point  newPoint = ta->new_pt;
            Vector v        = Point::minus (newPoint, _lastPoint1);
            _matrix->leftTranslate (v.dx (), v.dy ());
            _lastPoint1 = newPoint;
        }
        break;
    case RESIZING:
        // guard & action
        if ((t->get_id() == _touchID1) || (t->get_id() == _touchID2)) {
            Point &fixedPoint = (t->get_id() == _touchID1) ? _lastPoint2 : _lastPoint1;
            Point &oldPoint = (t->get_id() == _touchID1) ? _lastPoint1 : _lastPoint2;
            Point &newPoint = ta->new_pt;

            computeResizing(_matrix, fixedPoint, oldPoint, newPoint);

            if (t->get_id() == _touchID1) {
                _lastPoint1 = newPoint;
            } else {
                _lastPoint2 = newPoint;
            }
        }
        break;
    default:
        break;
    }

    // debug - To avoid having too much irrelevant information.
    //  if (is_state_changed)
    //      std::cerr << "Current State: " << print_state (_current_FSM_state) << std::endl;
}

} /* namespace djnn */
