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

#include "gui.h"
#include "abstract_gobj.h"
#include "backend.h"
#include "../core/syshook/main_loop.h"

#define _PERF_TEST 0
#if _PERF_TEST
static int __nb_Drawing_object = 0;
#endif

namespace djnn
{

  static bool __module_initialized = false;
  Process *DrawingRefreshManager;
  GUIStructureObserver * gui_structure_observer;


  void 
  GUIStructureHolder::add_gui_child (Process *c, int index) {

#if _PERF_TEST
   __nb_Drawing_object++;
#endif
   _children.push_back (std::pair<Process*, int> (c, index)); 
 }


 void 
 GUIStructureHolder::remove_gui_child (Process *c) { 

#if _PERF_TEST
  /* substract __nb_Drawing_object only if 'c' was a GOBJ (was in GUIStructureHolder::_children) */
  std::vector<std::pair<Process*, int>>::iterator new_end = std::remove_if (_children.begin (), _children.end (), [c](std::pair<Process*, int> p){return p.first == c;});
  if (new_end != _children.end ()) {
    _children.erase (new_end, _children.end ()); 
    __nb_Drawing_object--;
  } 
#else
  _children.erase (std::remove_if (_children.begin (), _children.end (), [c](std::pair<Process*, int> p){return p.first == c;}), _children.end ()); 
#endif
 }

 void 
 GUIStructureHolder::draw () {

  ComponentObserver::instance ().start_draw ();
  for (auto p: _children) {
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
  GUIStructureHolder::add_gui_child_at (Process *c, int index)
  {
    int sz = _children.size ();
    for (int i = 0; i < sz; i++) {
      if (_children.at (i).second == index)
       _children.insert (_children.begin () + i, std::pair<Process*,int>(c, index));
    }
  }

  void
  GUIStructureHolder::move_child_to (Process *c, int index)
  {
    _children.erase (std::remove_if (_children.begin (), _children.end (), [c](std::pair<Process*, int> p){return p.first == c;}), _children.end ());
    add_gui_child_at (c, index);
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
  GUIStructureObserver::add_child_at (Process *cont, Process *c, int index)
  {
    //AbstractGObj *obj = dynamic_cast<AbstractGObj*> (c);
    std::map<Process*, GUIStructureHolder*>::iterator it_cont = _structure_map.find (cont);
    if (c->get_cpnt_type () == GOBJ_T) {
      if (it_cont != _structure_map.end ()) {
        it_cont->second->add_gui_child_at (c, index);
      }
    } else if (c->get_cpnt_type () == WINDOW_T) {
      Window *w = dynamic_cast<Window*> (c);
      w->set_holder (it_cont->second);
    } else if (c->get_cpnt_type () == COMPONENT_T || c->get_cpnt_type () == FSM_T || c->get_cpnt_type () == SWITCH_T) {
      add_container (c);
      if (it_cont != _structure_map.end ())
        it_cont->second->add_gui_child_at (c, index);
    }
  }

  void
  GUIStructureObserver::move_child_to (Process *cont, Process *c, int index)
  {
    std::map<Process*, GUIStructureHolder*>::iterator it_cont = _structure_map.find (cont);
    if (c->get_cpnt_type () == GOBJ_T) {
      if (it_cont != _structure_map.end ()) {
        it_cont->second->move_child_to (c, index);
      }
    } else if (c->get_cpnt_type () == WINDOW_T) {
      Window *w = dynamic_cast<Window*> (c);
      w->set_holder (it_cont->second);
    } else if (c->get_cpnt_type () == COMPONENT_T || c->get_cpnt_type () == FSM_T || c->get_cpnt_type () == SWITCH_T) {
      add_container (c);
      if (it_cont != _structure_map.end ())
        it_cont->second->move_child_to (c, index);
    }
  }

  void
  GUIStructureObserver::remove_child_from_container (Process *cont, Process *c)
  {
    std::map<Process*, GUIStructureHolder*>::iterator it_cont = _structure_map.find (cont);
    if (it_cont != _structure_map.end ())
      it_cont->second->remove_gui_child (c);
  }

  void
  init_gui ()
  {

    if (__module_initialized == false) {

      __module_initialized = true;

      djnn::loadedModules.push_back ("gui");

      Backend::init ();
      UpdateDrawing::init ();
      DrawingRefreshManager = UpdateDrawing::instance ();
      MainLoop::instance ().add_background_process (DrawingRefreshManager);
      init_svg_parser ();
      gui_structure_observer = new GUIStructureObserver ();
      structure_observer_list.push_back (gui_structure_observer);
    }
  }

  void
  clear_gui ()
  {

    __module_initialized = false;
    UpdateDrawing::clear ();

  }

} /* namespace Smala */
