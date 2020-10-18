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

  class Set;

  class SetIterator : public FatProcess
  {
  public:
    SetIterator (ParentProcess* parent, const std::string& name, CoreProcess *set, CoreProcess *action, bool model = true);
    virtual ~SetIterator () {}
    virtual process_type_e get_process_type () const override { return CONTAINER_T; }
    void impl_activate () override;
    void impl_deactivate () override {};
    void post_activate () override;
  private:
    Set *_set;
    CoreProcess *_action;
  };

  class Set : public FatProcess
  {
  public:
    Set (ParentProcess* parent, const std::string& name);
    void add_child (FatChildProcess* c, const std::string& name) override;
    void remove_child (FatChildProcess* c) override;
    void remove_child (const std::string& name) override;
    FatChildProcess* find_child_impl (const std::string& path) override;
    void impl_activate () override;
    void impl_deactivate () override;
    virtual ~Set ();
  private:
    RefProperty _added, _removed;
    IntProperty _size;

  public:
#ifndef DJNN_NO_DEBUG
    void dump (int level=0) override;
#endif
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };
}
