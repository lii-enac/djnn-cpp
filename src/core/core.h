/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *		  Mathieu Poirier <mathieu.poirier@enac.fr>
 *		  Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

#ifdef _WIN32
// TODO: windows hack - do better
#define NOGDI
#include <winsock2.h>
#endif

#include "ontology/process.h"
#include "execution/graph.h"
#include "control/binding.h"
#include "control/activator.h"
#include "control/synchronizer.h"
#include "tree/component.h"
#include "control/native_action.h"
#include "control/native_expression_action.h"
#include "tree/spike.h"
#include "control/assignment.h"
#include "tree/list.h"
#include "xml/xml.h"
#include "serializer/serializer.h"
#include "tree/abstract_property.h"
#include "tree/blank.h"
#include "tree/bool_property.h"
#include "tree/int_property.h"
#include "tree/double_property.h"
#include "tree/text_property.h"
#include "tree/ref_property.h"
#include "tree/remote_property.h"
#include "tree/operators.h"
#include "tree/set.h"

#include "core/core-dev.h"

// timer should be available in core, not only in exec_env
// include it here to prevent 'use exec_env in smala'
//#include "exec_env/timer.h"

namespace djnn {

  extern std::vector<std::string> loadedModules;
  //void module_loaded(const char* name);

  class StructureObserver;
  extern std::vector<StructureObserver*> structure_observer_list;
  void init_core ();
  void clear_core ();

  // Compatibility with djnn-test, smalac, cookbooks, legacy applications.
  //using Process = FatProcess; // the name "Process" should not be used inside djnn-cpp until the refactoring dust settles
  using Process = CoreProcess;

}
