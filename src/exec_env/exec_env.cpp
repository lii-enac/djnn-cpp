/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "exec_env/exec_env.h"
#include "exec_env/global_mutex.h"
#include "exec_env/main_loop.h"
#include "exec_env/djnn_time_manager.h"

#include <iostream>
#include <sys/time.h>
#include <stdio.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#define DBG_MUTEX 0
#include <iostream>
#include "utils/debug.h"

namespace djnn
{

  static bool __module_initialized = false;
  Process* mainloop;

  void
  init_exec_env()
  {
    if (__module_initialized == false) {
      __module_initialized = true;
      djnn::loadedModules.push_back ("exec_env");
      init_global_mutex();
      mainloop = &MainLoop::instance ();
      //DjnnTimeManager::instance().activate ();
      MainLoop::instance ().add_external_source(&DjnnTimeManager::instance());
      setlocale(LC_NUMERIC, "C");
    }
    DjnnTimeManager::instance().cleanup ();
  }

  void
  clear_exec_env()
  {
  }

  void
  get_monotonic_time (struct timespec *ts)
  {
#ifdef __MACH__
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service (mach_host_self (), SYSTEM_CLOCK, &cclock);
    clock_get_time (cclock, &mts);
    mach_port_deallocate (mach_task_self(), cclock);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;
#else
#ifndef DJNN_USE_FREERTOS
    clock_gettime(CLOCK_MONOTONIC, ts);
#endif
#endif
  }

  static struct timespec before;
  static struct timespec after;
  static int init = 0;

  void
  t1 ()
  {
    get_monotonic_time (&before);
    init = 1;
  }

  double
  t2 (const std::string &msg, bool display_stderr)
  {
    if (!init)
      return 0.0;
    get_monotonic_time (&after);
    double elapsedTime = (after.tv_sec * 1000 + after.tv_nsec * 1e-6) - (before.tv_sec * 1000 + before.tv_nsec * 1e-6);
    if(display_stderr)
      std::cout << msg << " elapsedTime = " << elapsedTime << " ms" << std::endl;
    init = 0;
    return elapsedTime;
  }

}
