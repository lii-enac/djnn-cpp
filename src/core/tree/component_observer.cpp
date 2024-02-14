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

#include "core/utils/algorithm.h"
#include "component_observer.h"


#include "core/utils/remotery.h"

namespace djnn
{

  ComponentObserver ComponentObserver::_instance;

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
    return _instance;
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
//rmt_BeginCPUSample(start_draw, RMTSF_Aggregate);
    for (auto m : _draw_manager_list) {
      m->push ();
    }
//rmt_EndCPUSample();
  }

  void
  ComponentObserver::end_draw ()
  {
//rmt_BeginCPUSample(stop_draw, RMTSF_Aggregate);
    for (auto m : _draw_manager_list) {
      m->pop ();
    }
//rmt_EndCPUSample();
  }

  void
  ComponentObserver::add_context_manager (ContextManager* m)
  {
    _manager_list.push_back (m);
  }

  void
  ComponentObserver::remove_context_manager (ContextManager* m)
  {
    _manager_list.erase (djnnstl::remove (_manager_list.begin (), _manager_list.end (), m), _manager_list.end ());
  }

  void
  ComponentObserver::add_draw_context_manager (ContextManager* m)
  {
    _draw_manager_list.push_back (m);
  }

  void
  ComponentObserver::remove_draw_context_manager (ContextManager* m)
  {
    _draw_manager_list.erase (djnnstl::remove (_draw_manager_list.begin (), _draw_manager_list.end (), m),
                              _draw_manager_list.end ());
  }

} /* namespace djnn */
