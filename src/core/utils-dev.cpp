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

#include "utils-dev.h"
#include <iostream>
#include <sys/time.h>
#include <stdio.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

namespace djnn {
  void get_monotonic_time(struct timespec *ts) {
  #ifdef __MACH__
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_MONOTONIC, ts);
#endif
  }

static struct timespec before;
static struct timespec after;
static int init = 0;

  void t1 () {
    get_monotonic_time(&before);
    init = 1;
  }

  void t2 (const std::string &msg) {
    if (!init)
      return;
    get_monotonic_time (&after);
    double elapsedTime = after.tv_sec - before.tv_sec;
    elapsedTime += after.tv_nsec - before.tv_nsec;
    elapsedTime *= 1e-9;
    std::cout << msg << " elapsedTime = " << elapsedTime << std::endl;
    init = 0;
  }
}
