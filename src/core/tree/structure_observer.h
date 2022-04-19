#pragma once

#include "core/ontology/process.h"

namespace djnn {

  class FatProcess;
  //class FatChildProcess;

  class StructureObserver {
  public:
    StructureObserver () {};
    virtual ~StructureObserver () {};
    virtual void add_container (FatProcess *cont) = 0;
    virtual void remove_container (FatProcess *cont) = 0;
    virtual void add_child_to_container (FatProcess *cont, FatChildProcess *c, int index) = 0;
    virtual void add_child_at (FatProcess *cont, FatChildProcess *c, int neighboor_index, child_position_e spec, int new_index) = 0;
    virtual void move_child_to (FatProcess *cont, FatChildProcess *c, int neighboor_index, child_position_e spec, int new_index) = 0;
    virtual void remove_child_from_container (FatProcess *cont, FatChildProcess *c) = 0;
    virtual void swap_children (FatProcess *cont, int i, int j) = 0;
    virtual void set_child (FatProcess *cont, FatChildProcess *child, int i) = 0;
  };

  extern vector<StructureObserver*> structure_observer_list;
}
