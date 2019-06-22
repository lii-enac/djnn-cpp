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
 *
 */

#pragma once

#include "abstract_property.h"
#include "../utils/error.h"

#include <string>

namespace djnn {
  using namespace std;

  class RefProperty: public AbstractProperty {
  public:
    RefProperty (Process* v) : AbstractProperty (), value (v) {};
    RefProperty (Process* p, const string &name, Process* v, int nm = notify_none) : AbstractProperty (p, name, nm), value (v) { Process::finalize_construction (); };
    virtual int get_prop_type () override { return Reference; }
    Process* get_value () { return value; };
    void set_value (int newValue, bool propagate) override;
    void set_value (double v, bool propagate) override;
    void set_value (bool v, bool propagate) override;
    void set_value (Process* v, bool propagate) override;
    void set_value (const string &v, bool propagate) override;
    void set_value (const char* v, bool propagate) override { set_value(string(v), propagate);};
    double get_double_value () override { warning (this, "undefined double value for ref property"); return 0; }
    string get_string_value () override;
    Process* find_component (const string &path) override;
    void dump (int level=0) override;
    void serialize (const string& format) override;
    Process* clone () override;
  private:
    Process* value;
};

  Process* getRef (Process *p);
  void setRef (Process *p, Process *v);
}
