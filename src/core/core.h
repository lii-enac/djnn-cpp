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
 *		Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once


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
#include "control/exit.h"
#include "serializer/serializer.h"
#include "syshook/timer.h"
#include "tree/abstract_property.h"
#include "tree/blank.h"
#include "tree/bool_property.h"
#include "tree/int_property.h"
#include "tree/double_property.h"
#include "tree/text_property.h"
#include "tree/ref_property.h"
#include "tree/set.h"
#include "utils/error.h"


namespace djnn {

  class StructureObserver {
  public:
    StructureObserver () {};
    virtual ~StructureObserver () {};
    virtual void add_container (Process *cont) = 0;
    virtual void remove_container (Process *cont) = 0;
    virtual void add_child_to_container (Process *cont, Process *c, int index) = 0;
    virtual void add_child_at (Process *cont, Process *c, int neighboor_index, int spec, int new_index) = 0;
    virtual void move_child_to (Process *cont, Process *c, int neighboor_index, int spec, int new_index) = 0;
    virtual void remove_child_from_container (Process *cont, Process *c) = 0;
    virtual void swap_children (Process *cont, int i, int j) = 0;
    virtual void set_child (Process *cont, Process *child, int i) = 0;
  };
  extern std::vector<string> loadedModules;
  extern std::vector<StructureObserver*> structure_observer_list;
  void init_core ();
  void clear_core ();

}
