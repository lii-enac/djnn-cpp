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

#include "../error.h"
#include "abstract_property.h"
#include <string>

namespace djnn {
  using namespace std;

  class TextProperty: public AbstractProperty {
  public:
    TextProperty (string v) : AbstractProperty (), value (v) {};
    TextProperty (Process*  p, const string &name, const string &v) : AbstractProperty (p, name), value (v.c_str ()) { Process::finalize (); };
    virtual int get_prop_type () override { return String; }
    void activate () override {} ;
    void deactivate () override {};
    string& get_value () { return value; };
    void set_value (int newValue, bool propagate) override;
    void set_value (double v, bool propagate) override;
    void set_value (bool v, bool propagate) override;
    void set_value (Process* v, bool propagate) override;
    void set_value (const string &v, bool propagate) override;
    void set_value (const char* v, bool propagate) override { set_value(string(v), propagate);};
    double get_double_value () override {
      try {
        double r = stof (value);
        return r;
      }
      catch (const std::invalid_argument& ia) {
        warning (this, "undefined double value for text property");
        return 0;
      }
    }
    void dump (int level=0) override;
    void serialize (const string& format) override;
    Process* clone () override;
  private:
    string value;
  };

}
