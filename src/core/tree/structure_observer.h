#pragma once

#include <vector>

namespace djnn {

  class Process;

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

  extern std::vector<StructureObserver*> structure_observer_list;
}
