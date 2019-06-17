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

#include "gui-priv.h"
#include "abstract_gobj.h"
#include "backend.h"
#include "../core/syshook/main_loop.h"
#include "../display/window.h"

#include <algorithm>


#include <iostream>
#define __FL__ " " __FILE__ ":" << __LINE__ << std::endl;
#define __EFL__ std::cerr << __FUNCTION__ << " " << __FILE__ ":" << __LINE__ << std::endl;
#define DBG __EFL__

#define _PERF_TEST 0
#if _PERF_TEST
static int __nb_Drawing_object = 0;
#endif

namespace djnn
{

  static bool __module_initialized = false;
  Process *DrawingRefreshManager;
  Process *GenericMouse;
  GUIStructureObserver * gui_structure_observer;

  void
  GUIStructureHolder::add_gui_child (Process *c, int index)
  {

#if _PERF_TEST
    __nb_Drawing_object++;
#endif
    _children.push_back (std::pair<Process*, int> (c, index));
  }

  void
  GUIStructureHolder::remove_gui_child (Process *c)
  {

#if _PERF_TEST
    /* substract __nb_Drawing_object only if 'c' was a GOBJ (was in GUIStructureHolder::_children) */
    std::vector<std::pair<Process*, int>>::iterator new_end = std::remove_if (_children.begin (), _children.end (), [c](std::pair<Process*, int> p) {return p.first == c;});
    if (new_end != _children.end ()) {
      _children.erase (new_end, _children.end ());
      __nb_Drawing_object--;
    }
#else
    _children.erase (
        std::remove_if (_children.begin (), _children.end (), [c](std::pair<Process*, int> p) {return p.first == c;}),
        _children.end ());
#endif
  }

  void
  GUIStructureHolder::draw ()
  {

    ComponentObserver::instance ().start_draw ();
    for (auto p : _children) {
      p.first->draw ();
    }
    ComponentObserver::instance ().end_draw ();

#if _PERF_TEST
    cerr << "\033[1;31m" << endl;
    cerr << "NB DRAWING OBJS: " << __nb_Drawing_object << endl;
    cerr << "\033[0m";
#endif
  }

  void
  GUIStructureHolder::add_gui_child_at (Process *c, int neighboor_index, int spec, int new_index)
  {
    int sz = _children.size ();
    switch (spec)
      {
      case LAST:
        _children.push_back (std::pair<Process*, int> (c, new_index));
        break;
      case FIRST:
        _children.insert (_children.begin (), std::pair<Process*, int> (c, new_index));
        break;
      case AFTER:
        for (int i = 0; i < sz - 1; i++) {
          if (_children.at (i).second == neighboor_index) {
            _children.insert (_children.begin () + i + 1, std::pair<Process*, int> (c, new_index));
            break;
          }
        }
        _children.push_back (std::pair<Process*, int> (c, new_index));
        break;
      case BEFORE:
        for (int i = 0; i < sz; i++) {
          if (_children.at (i).second == neighboor_index) {
            _children.insert (_children.begin () + i, std::pair<Process*, int> (c, new_index));
            break;
          }
        }
      }
    ((AbstractGObj*) c)->frame ()->update ();
  }

  void
  GUIStructureHolder::move_child_to (Process *c, int neighboor_index, int spec, int new_index)
  {
    _children.erase (
        std::remove_if (_children.begin (), _children.end (), [c](std::pair<Process*, int> p) {return p.first == c;}),
        _children.end ());
    add_gui_child_at (c, neighboor_index, spec, new_index);
  }

  void
  GUIStructureObserver::add_container (Process *cont)
  {
    std::map<Process*, GUIStructureHolder*>::iterator it_cont = _structure_map.find (cont);
    if (it_cont == _structure_map.end ()) {
      GUIStructureHolder *holder = new GUIStructureHolder ();
      /*    if (p) {
       std::map<Process*, GUIStructureHolder*>::iterator it = _structure_map.find (p);
       if (it != _structure_map.end ())
       it->second->add_gui_child (holder);
       }
       */
      _structure_map.insert (std::pair<Process*, GUIStructureHolder*> (cont, holder));
    }
  }

  void
  GUIStructureObserver::remove_container (Process *cont)
  {
    //std::map<Process*, GUIStructureHolder*>::iterator it_p = _structure_map.find (p);
    std::map<Process*, GUIStructureHolder*>::iterator it_cont = _structure_map.find (cont);
    if (it_cont != _structure_map.end ()) {
      //if (it_p != _structure_map.end ())
      //it_p->second->remove_gui_child (it_cont->second);
      GUIStructureHolder *holder = it_cont->second;
      _structure_map.erase (it_cont);
      delete holder;
    }
  }

  void
  GUIStructureObserver::add_child_to_container (Process *cont, Process *c, int index)
  {
    //AbstractGObj *obj = dynamic_cast<AbstractGObj*> (c);
    std::map<Process*, GUIStructureHolder*>::iterator it_cont = _structure_map.find (cont);
    if (c->get_cpnt_type () == GOBJ_T) {
      if (it_cont != _structure_map.end ())
        it_cont->second->add_gui_child (c, index);
    } else if (c->get_cpnt_type () == WINDOW_T) {
      Window *w = dynamic_cast<Window*> (c);
      w->set_holder (it_cont->second);
    } else if (c->get_cpnt_type () == COMPONENT_T || c->get_cpnt_type () == FSM_T || c->get_cpnt_type () == SWITCH_T) {
      add_container (c);
      if (it_cont != _structure_map.end ())
        it_cont->second->add_gui_child (c, index);
    }
  }

  void
  GUIStructureObserver::add_child_at (Process *cont, Process *c, int neighboor_index, int spec, int new_index)
  {
    //AbstractGObj *obj = dynamic_cast<AbstractGObj*> (c);
    std::map<Process*, GUIStructureHolder*>::iterator it_cont = _structure_map.find (cont);
    if (c->get_cpnt_type () == GOBJ_T) {
      if (it_cont != _structure_map.end ()) {
        it_cont->second->add_gui_child_at (c, neighboor_index, spec, new_index);
      }
    } else if (c->get_cpnt_type () == WINDOW_T) {
      Window *w = dynamic_cast<Window*> (c);
      w->set_holder (it_cont->second);
    } else if (c->get_cpnt_type () == COMPONENT_T || c->get_cpnt_type () == FSM_T || c->get_cpnt_type () == SWITCH_T) {
      add_container (c);
      if (it_cont != _structure_map.end ())
        it_cont->second->add_gui_child_at (c, neighboor_index, spec, new_index);
    }
  }

  void
  GUIStructureObserver::move_child_to (Process *cont, Process *c, int neighboor_index, int spec, int new_index)
  {
    std::map<Process*, GUIStructureHolder*>::iterator it_cont = _structure_map.find (cont);
    if (c->get_cpnt_type () == GOBJ_T) {
      if (it_cont != _structure_map.end ()) {
        it_cont->second->move_child_to (c, neighboor_index, spec, new_index);
      }
    } else if (c->get_cpnt_type () == WINDOW_T) {
      Window *w = dynamic_cast<Window*> (c);
      w->set_holder (it_cont->second);
    } else if (c->get_cpnt_type () == COMPONENT_T || c->get_cpnt_type () == FSM_T || c->get_cpnt_type () == SWITCH_T) {
      add_container (c);
      if (it_cont != _structure_map.end ())
        it_cont->second->move_child_to (c, neighboor_index, spec, new_index);
    }
  }

  void
  GUIStructureObserver::remove_child_from_container (Process *cont, Process *c)
  {
    std::map<Process*, GUIStructureHolder*>::iterator it_cont = _structure_map.find (cont);
    if (it_cont != _structure_map.end ())
      it_cont->second->remove_gui_child (c);
  }

  GUIMouseButton::GUIMouseButton (Process *p, const string &n) : Process (p, n)
  {
    _press = new Spike (this, "press");
    _release = new Spike (this, "release");
    Process::finalize_construction ();
  }

  GUIMouse::GUIMouse (Process *p, const string &n) : Process (p, n)
  {
    _left = new GUIMouseButton (this, "left");
    _right = new GUIMouseButton (this, "right");
    _middle = new GUIMouseButton (this, "middle");
    _move = new Spike (this, "move");
    _wheel = new Spike (this, "wheel");
    _pos_x = new DoubleProperty (this, "x", 0);
    _pos_y = new DoubleProperty (this, "y", 0);
    _dx = new DoubleProperty (0);
    _dy = new DoubleProperty (0);
    _wheel->add_symbol ("dx", _dx);
    _wheel->add_symbol ("dy", _dy);
  }

  GUIMouse::~GUIMouse ()
  {
    delete _left;
    delete _right;
    delete _middle;
    delete _move;
    delete _pos_x;
    delete _pos_y;
    _wheel->remove_symbol ("dx");
    _wheel->remove_symbol ("dy");
    delete _wheel;
    delete _dx;
    delete _dy;
  }

  extern void p_init_p_display ();

  void
  init_gui ()
  {

    if (__module_initialized == false) {

      __module_initialized = true;

      djnn::loadedModules.push_back ("gui");

      Backend::init ();
      UpdateDrawing::init ();
      DrawingRefreshManager = UpdateDrawing::instance ();
      GenericMouse = new GUIMouse (nullptr, "");
      GenericMouse->activation ();
      MainLoop::instance ().add_background_process (DrawingRefreshManager);
      init_svg_parser ();
      gui_structure_observer = new GUIStructureObserver ();
      structure_observer_list.push_back (gui_structure_observer);
      p_init_p_display ();
      p_init_gui ();
    }
  }

  void
  clear_gui ()
  {

    __module_initialized = false;
    UpdateDrawing::clear ();

  }

} /* namespace Smala */
