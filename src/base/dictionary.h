/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/tree/ref_property.h"
#include "core/tree/spike.h"

#include <map>

namespace djnn {

  class Dictionary: public Process {
    class FindAction : public Action
    {
    public:
      FindAction (Process* parent, const std::string& name) :
        Action (parent, name) {};
      virtual ~FindAction () {};
      void impl_activate () override;
      void impl_deactivate () override {};
    };
    class AddEntryAction : public Action
    {
    public:
      AddEntryAction (Process* parent, const std::string& name) :
        Action (parent, name) {};
      virtual ~AddEntryAction () {};
      void impl_activate () override;
      void impl_deactivate () override {};
    };
    class DelEntryAction : public Action
    {
    public:
      DelEntryAction (Process* parent, const std::string& name) :
        Action (parent, name) {};
      virtual ~DelEntryAction () {};
      void impl_activate () override;
      void impl_deactivate () override {};
    };
  public:
    Dictionary (Process* parent, const std::string& name);
    virtual ~Dictionary ();
    void impl_activate () override;
    void impl_deactivate () override;
    void add_entry (Process* key, Process *value);
    Process* get_value (Process* key);
    void del_entry (Process* key);
    void set_value (Process* value) { _value.set_value (value, true); }
    std::map<Process*,Process*>& get_map () { return _map; }
    RefProperty* key () { return &_key; }
    RefProperty* value () { return &_value; }
  private:
    RefProperty _key, _value;
    Spike _add, _del;
    FindAction _find_action;
    AddEntryAction _add_action;
    DelEntryAction _del_action;
    Coupling _c_add, _c_del, _c_find;
    std::map<Process*,Process*> _map;
  };
}
