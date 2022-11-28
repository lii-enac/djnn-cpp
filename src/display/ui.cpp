/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2021)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "core/tree/list.h"
#include "display/ui.h"

#include "core/core-dev.h" //  add/remove edge

#include <math.h>
#include <float.h>

namespace djnn
{
  UI::UI (ParentProcess* parent_, FatProcess *f) : parent (parent_)
  {
    pickable = new BoolProperty (parent, "pickable", true);
    //FatProcess *update = UpdateDrawing::instance ()->get_damaged ();
    //cpick = new CouplingWithData (pickable, ACTIVATION, update, ACTIVATION, nullptr);
    //if (f != nullptr)
    //  cpick->enable (f);
    FatProcess *update = f;
    if (update) update = dynamic_cast<Window*>(f)->damaged ();
    cpick = new CouplingWithData (pickable, ACTIVATION, update, ACTIVATION);
    if (f != nullptr) cpick->enable ();

    _press = new Spike (parent, "press");
    _move = new Spike (parent, "move");
    _left = new Component (parent, "left");
    _left_press = new Spike (_left, "press");
    _left_release = new Spike (_left, "release");
    _right = new Component (parent, "right");
    _right_press = new Spike (_right, "press");
    _right_release = new Spike (_right, "release");
    _middle = new Component (parent, "middle");
    _middle_press = new Spike (_middle, "press");
    _middle_release = new Spike (_middle, "release");
    _release = new Spike (parent, "release");
    _enter = new Spike (parent, "enter");
    _leave = new Spike (parent, "leave");
    _mouse = new Component (parent, "mouse");
    _wheel = new Component (parent, "wheel");
    _mouse_press = new Spike (_mouse, "press");
    _mouse_release = new Spike (_mouse, "release");
    _mouse_move  = new Spike (_mouse, "move");
    _mouse_enter = new Spike (_mouse, "enter");
    _mouse_leave = new Spike (_mouse, "leave");
    _mouse_wheel = new Spike (_mouse, "wheel");

    graph_add_edge (_enter, _leave); // make sur enter event are process before leave event
    graph_add_edge (_mouse_enter, _mouse_leave); // make sur enter event are process before leave event

    _touches = new List (parent, "touches");
    _touches->set_activation_state (ACTIVATED);
    _mouse->set_activation_state(ACTIVATED);
    _left->set_activation_state (ACTIVATED);
    _right->set_activation_state (ACTIVATED);
    _middle->set_activation_state (ACTIVATED);
    _wheel->set_activation_state (ACTIVATED);

    _move_x = new DoubleProperty (nullptr, "move_x", 0);
    _move_y = new DoubleProperty (nullptr, "move_y", 0);
    _press_x = new DoubleProperty (nullptr, "press_x", 0);
    _press_y = new DoubleProperty (nullptr, "press_y", 0);

    _wheel_dx = new DoubleProperty (_wheel, "dx", 0);
    _wheel_dy = new DoubleProperty (_wheel, "dy", 0);
    _wheel_x = new DoubleProperty (_wheel, "x", 0);
    _wheel_y = new DoubleProperty (_wheel, "y", 0);
    _local_wheel_x = new DoubleProperty (_wheel, "local_wheel_x", 0);
    _local_wheel_y = new DoubleProperty (_wheel, "local_wheel_y", 0);
    
    _local_move_x = new DoubleProperty (nullptr, "local_move_x", 0);
    _local_move_y = new DoubleProperty (nullptr, "local_move_y", 0);
    _local_press_x = new DoubleProperty (nullptr, "local_press_x", 0);
    _local_press_y = new DoubleProperty (nullptr, "local_press_y", 0);

    _move->add_symbol ("x", _move_x);
    _move->add_symbol ("y", _move_y);
    _move->add_symbol ("local_x", _local_move_x);
    _move->add_symbol ("local_y", _local_move_y);

    _press->add_symbol ("x", _press_x);
    _press->add_symbol ("y", _press_y);
    _press->add_symbol ("local_x", _local_press_x);
    _press->add_symbol ("local_y", _local_press_y);

    _mouse_press_x = new DoubleProperty (nullptr, "mouse_press_x", 0);
    _mouse_press_y = new DoubleProperty (nullptr, "mouse_press_y", 0);
    _mouse_move_x = new DoubleProperty (nullptr, "mouse_move_x", 0);
    _mouse_move_y = new DoubleProperty (nullptr, "mouse_move_y", 0);
    _mouse_wheel_x = new DoubleProperty (nullptr, "mouse_wheel_x", 0);
    _mouse_wheel_y = new DoubleProperty (nullptr, "mouse_wheel_y", 0);

    _mouse_local_press_x = new DoubleProperty (nullptr, "mouse_local_press_x", 0);
    _mouse_local_press_y = new DoubleProperty (nullptr, "mouse_local_press_y", 0);
    _mouse_local_move_x = new DoubleProperty (nullptr, "mouse_local_move_x", 0);
    _mouse_local_move_y = new DoubleProperty (nullptr, "mouse_local_move_y", 0);
    _mouse_local_wheel_x = new DoubleProperty (nullptr, "mouse_local_wheel_x", 0);
    _mouse_local_wheel_y = new DoubleProperty (nullptr, "mouse_local_wheel_y", 0);

    _mouse_move->add_symbol ("x", _mouse_move_x);
    _mouse_move->add_symbol ("y", _mouse_move_y);
    _mouse_move->add_symbol ("local_x", _mouse_local_move_x);
    _mouse_move->add_symbol ("local_y", _mouse_local_move_y);

    _mouse_press->add_symbol ("x", _mouse_press_x);
    _mouse_press->add_symbol ("y", _mouse_press_y);
    _mouse_press->add_symbol ("local_x", _mouse_local_press_x);
    _mouse_press->add_symbol ("local_y", _mouse_local_press_y);

    _mouse_wheel->add_symbol ("x", _mouse_wheel_x);
    _mouse_wheel->add_symbol ("y", _mouse_wheel_y);
    _mouse_wheel->add_symbol ("local_x", _mouse_local_wheel_x);
    _mouse_wheel->add_symbol ("local_y", _mouse_local_wheel_y);
  }

  UI::~UI()
  {
    _mouse_wheel->remove_symbol ("local_y");
    _mouse_wheel->remove_symbol ("local_x");
    _mouse_wheel->remove_symbol ("y");
    _mouse_wheel->remove_symbol ("x");
    
    _mouse_press->remove_symbol ("local_y");
    _mouse_press->remove_symbol ("local_x");
    _mouse_press->remove_symbol ("y");
    _mouse_press->remove_symbol ("x");
    
    _mouse_move->remove_symbol ("local_y");
    _mouse_move->remove_symbol ("local_x");
    _mouse_move->remove_symbol ("y");
    _mouse_move->remove_symbol ("x");
    
    delete _mouse_local_wheel_y;
    delete _mouse_local_wheel_x;
    delete _mouse_local_move_y;
    delete _mouse_local_move_x;
    delete _mouse_local_press_y;
    delete _mouse_local_press_x;

    delete _mouse_wheel_y;
    delete _mouse_wheel_x;
    delete _mouse_move_y;
    delete _mouse_move_x;
    delete _mouse_press_y;
    delete _mouse_press_x;

    _press->remove_symbol ("local_y");
    _press->remove_symbol ("local_x");
    _press->remove_symbol ("y");
    _press->remove_symbol ("x");

    _move->remove_symbol ("local_y");
    _move->remove_symbol ("local_x");
    _move->remove_symbol ("y");
    _move->remove_symbol ("x");
        
    delete _local_press_y;
    delete _local_press_x;
    delete _local_move_y;
    delete _local_move_x;

    _wheel->remove_child (_local_wheel_y);
    _wheel->remove_child (_local_wheel_x);
    delete _local_wheel_y;
    delete _local_wheel_x;
    _wheel->remove_child (_wheel_y);
    _wheel->remove_child (_wheel_x);
    delete _wheel_y;
    delete _wheel_x;
    _wheel->remove_child (_wheel_dy);
    _wheel->remove_child (_wheel_dx);
    delete _wheel_dy; 
    delete _wheel_dx;
    
    delete _press_y;
    delete _press_x;
    delete _move_y;
    delete _move_x;
    
    _wheel->set_activation_state (DEACTIVATED);
    _middle->set_activation_state (DEACTIVATED);
    _right->set_activation_state (DEACTIVATED);
    _left->set_activation_state (DEACTIVATED);
    _mouse->set_activation_state(DEACTIVATED);
    _touches->set_activation_state (DEACTIVATED);
    
    parent->remove_child (_touches);
    /* HACK 
    * clear _children of the (djnn:List) touches before
    * deleting it (touches), to avoid ~Container to delete touches inside "touches list"
    * which are also in a _active_touches list
    */
    ((List*) _touches)->clear ();
    delete _touches;

    graph_remove_edge (_mouse_enter, _mouse_leave);
    graph_remove_edge (_enter, _leave);

    _mouse->remove_child (_mouse_wheel);
    delete _mouse_wheel;
    _mouse->remove_child (_mouse_leave);
    delete _mouse_leave;
    _mouse->remove_child (_mouse_enter);
    delete _mouse_enter;
    _mouse->remove_child (_mouse_move);
    delete _mouse_move;
    _mouse->remove_child (_mouse_release);
    delete _mouse_release;
    _mouse->remove_child (_mouse_press);
    delete _mouse_press;
    parent->remove_child (_wheel);
    delete _wheel;
    parent->remove_child (_mouse);
    delete _mouse;
    parent->remove_child (_leave);
    delete _leave;
    parent->remove_child (_enter);
    delete _enter;
    parent->remove_child (_release);
    delete _release;
    _middle->remove_child (_middle_release);
    delete _middle_release;
    _middle->remove_child (_middle_press);
    delete _middle_press;
    parent->remove_child (_middle);
    delete _middle;
    _right->remove_child (_right_release);
    delete _right_release;
    _right->remove_child (_right_press);
    delete _right_press;
    parent->remove_child (_right);
    delete _right;
    _left->remove_child (_left_release);
    delete _left_release;
    _left->remove_child (_left_press);
    delete _left_press;
    parent->remove_child (_left);
    delete _left;
    parent->remove_child (_move);
    delete _move;
    parent->remove_child (_press);
    delete _press;
    
    delete cpick;
    parent->remove_child (pickable);
    delete pickable; 
  }
} /* namespace djnn */
