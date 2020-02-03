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

namespace djnn
{
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

  pair<RefProperty*, string>
  check_for_ref (Process* src, const string &spec)
  {
    /* spec is empty */
    if (spec.empty ()) 
      return pair<RefProperty*, string> (nullptr, spec);
    
    size_t found = spec.find ("$value");
    /* we found §value */
    if (found != string::npos) {
      
      Process *prop = src;
      
      /* $value is not the first element, eg : foo/bar/$value */
      if (found > 0) 
        prop = src->find_component (spec.substr (0, found - 1));
      
      /* prop == src (not null) OR the new element find by find_component */
      if (!prop) 
        return pair<RefProperty*, string> (nullptr, spec);


      RefProperty *ref = dynamic_cast<RefProperty*> (prop);
      /* ref is a refproperty */
      if (ref != nullptr ) {
        string new_spec = "";
        /* spec is more than juste $value, eg : boo/bar/$value/toto */
        if ( spec.size () > 6) {
          new_spec = spec.substr (found + 6);
          if (new_spec.at (0) == '/') 
            new_spec = new_spec.substr (1);
        }

        return pair<RefProperty*, string> (ref, new_spec);
       }
     }

     /* we did not found $value */
     return pair<RefProperty*, string> (nullptr, spec);
   }

  const string&
  get_parent_name (Process *p, int up)
  {
    while(up && p) { p=p->get_parent(); --up; }
    if(p) return p->get_name();
    else return Process::default_name;
  }

  const string
  get_hierarchy_name (Process *p, int up)
  {
    string res;
    while(up && p) { res = p->get_name()+"/"+res; p=p->get_parent(); --up; }
    return res;
  }
 }
