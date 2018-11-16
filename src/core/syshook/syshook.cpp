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
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "main_loop.h"
#include "syshook.h"
#include <mutex>

#include <chrono>
#include <iostream>

#include <QThread.h>
#include <QMutex.h>

#define DBG_MUTEX 0

namespace djnn
{
  //typedef std::mutex mutex_t;
  typedef QMutex mutex_t;

  static mutex_t* global_mutex;
  //thread_local bool _please_stop;
  
  void
  get_exclusive_access (const char * debug)
  {
#if DBG_MUTEX
    //std::cerr << debug << " priority:" << QThread::currentThread()->priority() << std::flush;
#endif
    global_mutex->lock ();
#if DBG_MUTEX
    std::cerr << " GOT " << debug << " priority:" << QThread::currentThread()->priority() << std::endl << std::flush;
#endif
  }

  void
  release_exclusive_access (const char * debug)
  {
#if DBG_MUTEX
    //std::cerr << debug << std::flush;
#endif
    global_mutex->unlock ();
#if DBG_MUTEX
    //std::cerr << " ROL " << debug << std::endl << std::flush;
#endif
  }

  ExternalSource * MainLoop::another_source_wants_to_be_mainloop = nullptr;

  std::shared_ptr<MainLoop> MainLoop::_instance;
  std::once_flag MainLoop::onceFlag;
  MainLoop&
  MainLoop::instance ()
  {
    std::call_once (MainLoop::onceFlag, [] () {
      global_mutex = new mutex_t ();      
      _instance.reset(new MainLoop);
    });

    return *(_instance.get ());
  }
}
