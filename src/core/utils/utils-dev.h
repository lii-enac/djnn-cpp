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

#pragma once

#include "core/tree/ref_property.h"

#include <time.h>

namespace djnn {
  void get_monotonic_time (struct timespec *ts);
  void t1 ();
  double t2 (const std::string &msg = "");
  pair<RefProperty*, string> check_for_ref (Process* src, const string &spec);

#if defined (GPERF_VERSION) && GPERF_VERSION >= 31
typedef size_t gperf_t;
#else
typedef unsigned gperf_t;
#endif
}
