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
#include "exec_env/main_loop.h"
#include "display/window.h"
#include "core/tree/component_observer.h"

#include <algorithm>
#include <boost/range/adaptor/reversed.hpp>

#include <iostream>
#include "utils/debug.h"

#define _PERF_TEST 0
#if _PERF_TEST
int __nb_Drawing_object = 0;
int __nb_Drawing_object_picking = 0;
#endif

namespace djnn
{

  static bool __module_initialized = false;
  
  Process *GenericMouse;
  GUIStructureObserver * gui_structure_observer;

  void
  GUIStructureHolder::add_gui_child (Process *c, int index)
  {
    _children.push_back (std::pair<Process*, int> (c, index));
  }

  void
  GUIStructureHolder::remove_gui_child (Process *c)
  {
    _children.erase (
        std::remove_if (_children.begin (), _children.end (), [c](std::pair<Process*, int> p) {return p.first == c;}),
        _children.end ());
  }

  void
  GUIStructureHolder::swap_children (int i, int j)
  {
    if (i < _children.size () && j < _children.size ()) {
      std::pair<Process*, int> buff = _children[j];
      _children[j] = _children[i];
      _children[i] = buff;
    }
  }

  void
  GUIStructureHolder::set_child (Process *child, int i)
  {
    if (i < _children.size ()) {
      for (auto p : _children) {
        if (p.second == i) {
          p.first = child;
          break;
        }
      }
    }
  }

  void
  GUIStructureHolder::draw ()
  {
    
#if _PERF_TEST
    /* reset  */
    __nb_Drawing_object = 0 ;
    __nb_Drawing_object_picking = 0 ;
#endif

    ComponentObserver::instance ().start_draw ();
    for (auto p : _children) {
      p.first->draw ();
    }
    ComponentObserver::instance ().end_draw ();

#if _PERF_TEST
    /* result  */
    cerr << "\033[1;31m" << endl;
    cerr << "NB DRAWING OBJS: " << __nb_Drawing_object << endl;
    cerr << "NB DRAWING OBJS PICKING VIEW: " << __nb_Drawing_object_picking << endl;
    cerr << "\033[0m";
#endif
  }

  AbstractGShape* 
  GUIStructureHolder::pick_analytical (PickAnalyticalContext& pac)
  {
    //ComponentObserver::instance ().start_draw ();
    //for (auto p : boost::adaptors::reverse(_children)) {
    AbstractGShape * picked = nullptr;
    for (auto p : _children) {
      AbstractGShape * picked_ = p.first->pick_analytical (pac);
      if(picked_) picked = picked_;
    }
    //ComponentObserver::instance ().end_draw ();
    return picked;
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
      _structure_map.insert (std::pair<Process*, GUIStructureHolder*> (cont, holder));
    }
  }

  void
  GUIStructureObserver::remove_container (Process *cont)
  {
    std::map<Process*, GUIStructureHolder*>::iterator it_cont = _structure_map.find (cont);
    if (it_cont != _structure_map.end ()) {
      delete it_cont->second;
      _structure_map.erase (it_cont);
      
    }
  }

  void
  GUIStructureObserver::add_child_to_container (Process *cont, Process *c, int index)
  {
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
    cont->update_drawing ();
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
    cont->update_drawing ();
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
    cont->update_drawing ();
  }

  void
  GUIStructureObserver::remove_child_from_container (Process *cont, Process *c)
  {
    std::map<Process*, GUIStructureHolder*>::iterator it_cont = _structure_map.find (cont);
    if (it_cont != _structure_map.end ())
      it_cont->second->remove_gui_child (c);
    cont->update_drawing ();
  }

  void
  GUIStructureObserver::swap_children (Process *cont, int  i, int j)
  {
    std::map<Process*, GUIStructureHolder*>::iterator it_cont = _structure_map.find (cont);
    if (it_cont != _structure_map.end ())
      it_cont->second->swap_children (i, j);
    cont->update_drawing ();
  }

  void
  GUIStructureObserver::set_child (Process *cont, Process *child, int  i)
  {
    std::map<Process*, GUIStructureHolder*>::iterator it_cont = _structure_map.find (cont);
    if (it_cont != _structure_map.end ())
      it_cont->second->set_child (child, i);
    cont->update_drawing ();
  }

  GUIMouseButton::GUIMouseButton (Process *parent, const std::string& name)
  : Process (name)
  {
    _press = new Spike (this, "press");
    _release = new Spike (this, "release");
    Process::finalize_construction (parent, name);
  }

  GUIMouse::GUIMouse (Process *parent, const std::string& name)
  : Process (name)
  {
    _left = new GUIMouseButton (this, "left");
    _right = new GUIMouseButton (this, "right");
    _middle = new GUIMouseButton (this, "middle");
    _move = new Spike (this, "move");
    _wheel = new Spike (this, "wheel");
    _pos_x = new DoubleProperty (this, "x", 0);
    _pos_y = new DoubleProperty (this, "y", 0);
    _dx = new DoubleProperty (nullptr, "dx", 0);
    _dy = new DoubleProperty (nullptr, "dy", 0);
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
    bool found_display = false;
    for(auto it = djnn::loadedModules.rbegin(); it != djnn::loadedModules.rend(); ++it) {
      if (*it == "display"){
        found_display = true;
        continue;
      }
    }
    if (!found_display)
        error (nullptr, " module display has not been initialized before module GUI and it should");

    if (__module_initialized == false) {

      __module_initialized = true;

      djnn::loadedModules.push_back ("gui");

      Backend::init ();
      
      GenericMouse = new GUIMouse (nullptr, "GenericMouse");
      GenericMouse->activate ();
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
    clear_svg_parser ();

    p_clear_gui ();

    /* remove container from structure_observer_list */
    structure_observer_list.erase (
      remove (structure_observer_list.begin (), structure_observer_list.end (), gui_structure_observer), 
      structure_observer_list.end ()
    );
    /* and delete it */
    delete gui_structure_observer;
    delete GenericMouse;

    __module_initialized = false;
  }

} /* namespace Smala */
