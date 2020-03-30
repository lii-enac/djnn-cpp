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

//#include <time.h>

namespace djnn {
  
  typedef std::pair<RefProperty*, std::string> ref_info_t;
  ref_info_t check_for_ref (Process* src, const std::string& spec);
  std::string __to_string(long x);

#if defined (GPERF_VERSION) && GPERF_VERSION >= 31
typedef size_t gperf_t;
#else
typedef unsigned gperf_t;
#endif

  const std::string& get_parent_name (Process *p, int up);
  const std::string get_hierarchy_name (Process *p, int up=-1);
}
