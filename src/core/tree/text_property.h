/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

#include "core/utils/error.h"
#include "abstract_property.h"

#include <string>

namespace djnn {
  using namespace std;

  class AbstractTextProperty : public AbstractProperty {
  public:
    AbstractTextProperty (Process* parent, const string &name, int notify_mask=notify_none) : AbstractProperty (parent, name, notify_mask) { Process::finalize_construction (parent, name); };
    virtual int get_prop_type () override { return String; }

    // AbstractProperty interface
    void set_value (int newValue, bool propagate) override;
    void set_value (double v, bool propagate) override;
    void set_value (bool v, bool propagate) override;
    void set_value (Process* v, bool propagate) override;
    void set_value (const string &v, bool propagate) override;
    void set_value (const char* v, bool propagate) override { set_value(string(v), propagate); };
    double get_double_value () override {
      try {
        double r = stof (get_ref_value());
        return r;
      }
      catch (const std::invalid_argument& ia) {
        warning (this, "undefined double value for text property");
        return 0;
      }
    }
    string get_string_value () override { return get_ref_value (); }
    void dump (int level=0) override;

    string& get_value () { return get_ref_value(); };
  protected:
    virtual string& get_ref_value() = 0;
  };

  class TextProperty : public AbstractTextProperty {
  public:
    TextProperty (Process* parent, const string &name, string v) : AbstractTextProperty (parent, name), value(v) { }
    void serialize (const string& format) override;
    Process* clone () override;
  protected:
    virtual string& get_ref_value() override { return value; }
  private:
    string value;
  };

  class TextPropertyProxy : public AbstractTextProperty {
  public:
    TextPropertyProxy (Process* parent, const string &name, string &v, int notify_mask=notify_none) : AbstractTextProperty (parent, name, notify_mask), value(v) { }
    void serialize (const string& format) override;
    Process* clone () override;
  protected:
    virtual string& get_ref_value() override { return value; }
  private:
    string& value;
  };

  string& getString (Process* p);
  void setString (Process *p, string &v);
}
