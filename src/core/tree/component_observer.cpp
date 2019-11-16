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

#include <atomic>
#include <algorithm>

#include <iostream>
#define DBG std::cerr << __PRETTY_FUNCTION__ << " " << __FILE__ << ":" << __LINE__ << std::endl;

namespace djnn
{

  ComponentObserver* ComponentObserver::_instance;
  //std::once_flag ComponentObserver::onceFlag;

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
    static std::atomic_flag onceFlag = ATOMIC_FLAG_INIT;
    if(!onceFlag.test_and_set()) {
      _instance = new ComponentObserver();
    }

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
