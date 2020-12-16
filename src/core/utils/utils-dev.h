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


namespace djnn {
  class CoreProcess;
  class FatProcess;
  class RefProperty;
 
  typedef std::pair<RefProperty*, std::string> ref_info_t;
  ref_info_t check_for_ref (FatProcess* src, const std::string& spec);
  std::string __to_string(long x);
  std::string __to_string(void *x);

#if defined (GPERF_VERSION) && GPERF_VERSION >= 31
typedef size_t gperf_t;
#else
typedef unsigned gperf_t;
#endif

  const std::string& get_parent_name (const CoreProcess *p, int up);
  const std::string get_hierarchy_name (const CoreProcess *p, int up=-1);
}
