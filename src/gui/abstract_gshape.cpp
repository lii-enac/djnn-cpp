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

#include "abstract_gshape.h"
#include "../core/tree/spike.h"
#include "../core/tree/set.h"

namespace djnn
{
  vector<string> AbstractGShape::_ui =
    { "press", "release", "move", "enter", "leave", "touches" };

  Process*
  SVGHolder::find_component (const string &path)
  {
    Process *p = Container::find_component (path);
    if (p == nullptr && _gobj != nullptr)
      p = _gobj->find_component (path);
    return p;
  }

  Touch::Touch (Process *p, const string &n) :
      Process (p, n), _shape (nullptr)
  {
    _x = new DoubleProperty (this, "x", 0);
    _y = new DoubleProperty (this, "y", 0);
    _activation_state = activated;
    Process::finalize ();
  }

  Touch::Touch () :
      Process (), _shape (nullptr)
  {
    _x = new DoubleProperty (this, "x", 0);
    _y = new DoubleProperty (this, "y", 0);
    _activation_state = activated;
  }

  Touch::~Touch ()
  {
  }

  void
  AbstractGShape::init_mouse_ui ()
  {
    Spike* press = new Spike (this, "press");
    new Spike (this, "release");
    Spike* move = new Spike (this, "move");
    new Spike (this, "enter");
    new Spike (this, "leave");
    Process* s = new Set (this, "touches");
    s->set_activation_flag (activated);
    DoubleProperty* x = new DoubleProperty (0);
    DoubleProperty* y = new DoubleProperty (0);
    press->add_symbol ("x", x);
    press->add_symbol ("y", y);
    move->add_symbol ("x", x);
    move->add_symbol ("y", y);
    _has_ui = true;
  }

  AbstractGShape::AbstractGShape () :
      AbstractGObj (), _has_ui (false)
  {
  }

  AbstractGShape::AbstractGShape (Process *p, const std::string& n) :
      AbstractGObj (p, n), _has_ui (false)
  {
  }

  Process*
  AbstractGShape::find_component (const string &path)
  {
    if (_has_ui)
      return Process::find_component (path);
    else {
      size_t found = path.find_first_of ('/');
      string key = path;
      if (found != string::npos) {
        key = path.substr (0, found);
      }
      vector<string>::iterator it = _ui.begin ();
      found = false;
      while (!found && it != _ui.end ()) {
        if (key.compare (*it) == 0) {
          found = true;
          init_mouse_ui ();
        }
        it++;
      }
      return Process::find_component (path);
    }
  }

  AbstractGShape::~AbstractGShape ()
  {
    if (_has_ui) {
      Process* press = find_component ("press");
      Process* release = find_component ("release");
      Process* move = find_component ("move");
      Process* enter = find_component ("enter");
      Process* leave = find_component ("leave");
      Process* touches = find_component ("touches");
      Process* x = find_component ("move/x");
      Process* y = find_component ("move/y");
      press->remove_symbol ("x");
      press->remove_symbol ("y");
      move->remove_symbol ("x");
      move->remove_symbol ("y");
      delete press;
      delete release;
      delete move;
      delete enter;
      delete leave;
      delete x;
      delete y;
      delete touches;
    }
  }

} /* namespace djnn */
