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

#include "../core/tree/process.h"
#include "../core/tree/ref_property.h"
#include "../core/tree/spike.h"
#include "../core/control/coupling.h"
#include <map>

namespace djnn {
  using namespace std;

  class Dictionary: public Process {
    class FindAction : public Action
    {
    public:
      FindAction (Process* p, const string &n) :
        Action (p, n) {};
      virtual ~FindAction () {};
      void activate () override;
      void deactivate () override {};
    };
    class AddEntryAction : public Action
    {
    public:
      AddEntryAction (Process* p, const string &n) :
        Action (p, n) {};
      virtual ~AddEntryAction () {};
      void activate () override;
      void deactivate () override {};
    };
    class DelEntryAction : public Action
    {
    public:
      DelEntryAction (Process* p, const string &n) :
        Action (p, n) {};
      virtual ~DelEntryAction () {};
      void activate () override;
      void deactivate () override {};
    };
  public:
    Dictionary (Process* parent, const string &name);
    virtual ~Dictionary ();
    void activate () override;
    void deactivate () override;
    void add_entry (Process* key, Process *value);
    Process* get_value (Process* key);
    void del_entry (Process* key);
    void set_value (Process* value) { _value->set_value (value, true); }
    map<Process*,Process*>& get_map () { return _map; }
    RefProperty* key () { return _key; }
    RefProperty* value () { return _value; }
  private:
    RefProperty* _key, *_value;
    Spike *_add, *_del;
    Coupling *_c_add, *_c_del, *_c_find;
    FindAction* _find_action;
    AddEntryAction* _add_action;
    DelEntryAction* _del_action;
    map<Process*,Process*> _map;
  };
}
