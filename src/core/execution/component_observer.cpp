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

#include "component_observer.h"
#include <algorithm>
#include <iostream>

namespace djnn
{

  ComponentObserver* ComponentObserver::_instance;
  std::once_flag ComponentObserver::onceFlag;

  ComponentObserver::ComponentObserver ()
  {
  }

  ComponentObserver::~ComponentObserver ()
  {
    _manager_list.clear ();
  }

  ComponentObserver&
  ComponentObserver::instance ()
  {
    std::call_once (ComponentObserver::onceFlag, [] () {
      _instance = new ComponentObserver();
    });

    return *(_instance) ;
  }

   void
  ComponentObserver::start_component ()
  {
    for (auto m : _manager_list) {
      m->push ();
    }
  }

  void
  ComponentObserver::end_component ()
  {
    for (auto m : _manager_list) {
      m->pop ();
    }
  }

 void
  ComponentObserver::start_draw ()
  {
    for (auto m : _draw_manager_list) {
      m->push ();
    }
  }

  void
  ComponentObserver::end_draw ()
  {
    for (auto m : _draw_manager_list) {
      m->pop ();
    }
  }

  void
  ComponentObserver::add_context_manager (ContextManager* m)
  {
    _manager_list.push_back (m);
  }

  void
  ComponentObserver::remove_context_manager (ContextManager* m)
  {
    _manager_list.erase (std::remove (_manager_list.begin (), _manager_list.end (), m), _manager_list.end ());
  }

  void
  ComponentObserver::add_draw_context_manager (ContextManager* m)
  {
    _draw_manager_list.push_back (m);
  }

  void
  ComponentObserver::remove_draw_context_manager (ContextManager* m)
  {
    _draw_manager_list.erase (std::remove (_draw_manager_list.begin (), _draw_manager_list.end (), m),
                              _draw_manager_list.end ());
  }

} /* namespace djnn */
