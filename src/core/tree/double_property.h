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

#include "abstract_property.h"

#include <string>

namespace djnn {
  using namespace std;

  class AbstractDoubleProperty : public AbstractProperty {
  public:
    AbstractDoubleProperty () : AbstractProperty () {};
    AbstractDoubleProperty (Process* p, const string &name, int notify_mask=notify_none) : AbstractProperty (p, name, notify_mask) { Process::finalize_construction (); };
    virtual int get_prop_type () override { return Double; }

    // AbstractProperty interface
    void set_value (int newValue, bool propagate) override;
    void set_value (double v, bool propagate) override;
    void set_value (bool v, bool propagate) override;
    void set_value (Process* v, bool propagate) override;
    void set_value (const string &v, bool propagate) override;
    void set_value (const char* v, bool propagate) override { set_value(string(v), propagate);};
    double get_double_value () override { return get_ref_value(); }
    string get_string_value () override { return std::to_string (get_ref_value ()); }
    void dump (int level=0) override;

    double get_value () { return get_ref_value(); };
  protected:
    virtual double& get_ref_value() = 0;
  };

  class DoubleProperty : public AbstractDoubleProperty {
  public:
    DoubleProperty (double v) : AbstractDoubleProperty (), value(v) { }
    DoubleProperty (Process* p, const string &name, double v) : AbstractDoubleProperty (p, name), value(v) { }
    void serialize (const string& format) override;
    Process* clone () override;
  protected:
    virtual double& get_ref_value() override { return value; }
  private:
    double value;
  };

  class DoublePropertyProxy : public AbstractDoubleProperty {
  public:
    DoublePropertyProxy (double& v) : AbstractDoubleProperty (), value(v) { }
    DoublePropertyProxy (Process* p, const string &name, double &v, int notify_mask=notify_none) : AbstractDoubleProperty (p, name, notify_mask), value(v) { }
    void serialize (const string& format) override;
    Process* clone () override;
  protected:
    virtual double& get_ref_value() override { return value; }
  private:
    double& value;
  };

  double getDouble (Process *p);
  void setDouble (Process *p, double v);
}
