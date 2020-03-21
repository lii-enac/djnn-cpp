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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "component.h"
#include "ref_property.h"
#include "int_property.h"

namespace djnn {
  //using namespace std;

  class SetIterator : public Process
  {
  public:
    SetIterator (Process *parent, const std::string& name, Process *set, Process *action, bool model = true);
    virtual ~SetIterator () {}
    virtual process_type_e get_cpnt_type () const override { return COMPONENT_T; }
    void impl_activate () override;
    void impl_deactivate () override {};
    void post_activate () override;
  private:
    Process *_set, *_action;
  };

  class Set : public Process
  {
  public:
    Set (Process *parent, const std::string& name);
    void add_child (Process* c, const std::string& name) override;
    void remove_child (Process* c) override;
    void remove_child (const std::string& name) override;
    Process* find_component (const std::string& path) override;
    void impl_activate () override;
    void impl_deactivate () override;
    virtual ~Set ();
    void serialize (const std::string& type) override;
    void dump(int level=0) override;
  private:
    RefProperty _added, _removed;
    IntProperty _size;
  };
}
