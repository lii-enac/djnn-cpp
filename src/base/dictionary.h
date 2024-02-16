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

#include "core/control/action.h"
#include "core/control/spike.h"
#include "core/ontology/coupling.h"
#include "core/ontology/process.h"
#include "core/property/ref_property.h"

namespace djnn {

class Dictionary : public FatProcess {
    class FindAction : public Action {
      public:
        FindAction (CoreProcess* parent, const string& name)
            : Action (parent, name){};
        virtual ~FindAction (){};
        void impl_activate () override;
    };
    class AddEntryAction : public Action {
      public:
        AddEntryAction (CoreProcess* parent, const string& name)
            : Action (parent, name){};
        virtual ~AddEntryAction (){};
        void impl_activate () override;
    };
    class DelEntryAction : public Action {
      public:
        DelEntryAction (CoreProcess* parent, const string& name)
            : Action (parent, name){};
        virtual ~DelEntryAction (){};
        void impl_activate () override;
    };

  public:
    Dictionary (CoreProcess* parent, const string& name);
    virtual ~Dictionary ();
    void                                    impl_activate () override;
    void                                    impl_deactivate () override;
    void                                    add_entry (CoreProcess* key, CoreProcess* value);
    CoreProcess*                            get_value (CoreProcess* key);
    void                                    del_entry (CoreProcess* key);
    void                                    set_value (CoreProcess* value) { _value.set_value (value, true); }
    typedef map<CoreProcess*, CoreProcess*> map_t;
    map_t&                                  get_map () { return _map; }
    RefProperty*                            key () { return &_key; }
    RefProperty*                            value () { return &_value; }

  private:
    RefProperty    _key, _value;
    Spike          _add, _del;
    FindAction     _find_action;
    AddEntryAction _add_action;
    DelEntryAction _del_action;
    Coupling       _c_add, _c_del, _c_find, _c_add_find;
    map_t          _map;
};
} // namespace djnn
