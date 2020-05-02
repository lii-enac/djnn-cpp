#pragma once

#include <vector>

namespace djnn {

  class FatProcess;

  class StructureObserver {
  public:
    StructureObserver () {};
    virtual ~StructureObserver () {};
    virtual void add_container (FatProcess *cont) = 0;
    virtual void remove_container (FatProcess *cont) = 0;
    virtual void add_child_to_container (FatProcess *cont, FatProcess *c, int index) = 0;
    virtual void add_child_at (FatProcess *cont, FatProcess *c, int neighboor_index, int spec, int new_index) = 0;
    virtual void move_child_to (FatProcess *cont, FatProcess *c, int neighboor_index, int spec, int new_index) = 0;
    virtual void remove_child_from_container (FatProcess *cont, FatProcess *c) = 0;
    virtual void swap_children (FatProcess *cont, int i, int j) = 0;
    virtual void set_child (FatProcess *cont, FatProcess *child, int i) = 0;
  };

  extern std::vector<StructureObserver*> structure_observer_list;
}
