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

//#include <utility> // pair
#include "containers.h"

namespace djnn {
  class CoreProcess;
  class FatProcess;
  class RefProperty;
  class Coupling;
 
  typedef pair<RefProperty*, string> ref_info_t;
  ref_info_t check_for_ref (FatProcess* src, const string& spec);
  string __to_string(long x);
  string __to_string(void *x);

#if defined (GPERF_VERSION) && GPERF_VERSION >= 31
typedef size_t gperf_t;
#else
typedef unsigned gperf_t;
#endif

  const string& get_parent_name (const CoreProcess *p, int up);
  const string get_hierarchy_name (const CoreProcess *p, int up=-1);

  string cpp_demangle (char const * name);

  // for gen_prop.py
  void enable (Coupling* c, CoreProcess* dst);
  void remove_edge (Coupling *c);
  void disable (Coupling *c);

#ifndef DJNN_NO_DEBUG
  /* DEBUG OPTION - FOR djnn */

  /* 
    diplayed infos :
    - a new frame with interactive object beside (right) the main frame
  */
  extern int _DEBUG_SEE_COLOR_PICKING_VIEW;

  /* 
    debug to detect cycle in graph and display informations
  */
  extern int _DEBUG_GRAPH_CYCLE_DETECT;

  /*
    explicitly accept cycles in graph without displaying information
  */
  extern int _AUTHORIZE_CYCLE;

  /*
    diplayed infos (YELLOW) : 
    - display order in which component are activated and deactivated by GRAPH_EXEC
 */
  extern int _DEBUG_SEE_ACTIVATION_SEQUENCE;
  extern int _DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_TIME_US;
  extern int _DEBUG_SEE_ACTIVATION_SEQUENCE_ONLY_TARGETED;

  /*
    displayed info : 
    - when paintevent is receiv and when Layer (Pixmap) is recompute
    - when Layer (Pixmap) is recompute only
  */
  extern int _DEBUG_SEE_RECOMPUTE_PIXMAP_AND_PAINTEVENT;
  extern int _DEBUG_SEE_RECOMPUTE_PIXMAP_ONLY;

  /*
    displayed info :
    - level 1: display execution graph infos on coupling in the warnings
    - level 2: display the tree structure of component's destruction
  */

  extern int _DEBUG_SEE_COMPONENTS_DESTRUCTION_INFO_LEVEL;

  /* 
    disable touch event  (especially for macos trackpad)
  */

  extern int _DEBUG_NO_TOUCH_EVENT;

#else

  const int
    _DEBUG_SEE_COLOR_PICKING_VIEW=0,
    _DEBUG_GRAPH_CYCLE_DETECT=0,
    _DEBUG_SEE_ACTIVATION_SEQUENCE=0,
    _DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_TIME_US=1000,
    _DEBUG_SEE_ACTIVATION_SEQUENCE_ONLY_TARGETED=1,
    _AUTHORIZE_CYCLE=0,
    _DEBUG_SEE_RECOMPUTE_PIXMAP_AND_PAINTEVENT = 0,
    _DEBUG_SEE_RECOMPUTE_PIXMAP_ONLY=0,
    _DEBUG_SEE_COMPONENTS_DESTRUCTION_INFO_LEVEL=0,
    _DEBUG_NO_TOUCH_EVENT=0
    ;
#endif
} 
