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
  //using namespace std;

  class AbstractDoubleProperty : public AbstractProperty {
  public:
    AbstractDoubleProperty (FatProcess* parent, const std::string& name, int notify_mask=notify_none) : AbstractProperty (parent, name, notify_mask) { finalize_construction (parent, name); };
    virtual int get_prop_type () const override { return Double; }
    //virtual process_type_e get_process_type () const override { return DOUBLE_PROPERTY_T; }

    // AbstractProperty interface
    void set_value (int newValue, bool propagate) override;
    void set_value (double v, bool propagate) override;
    void set_value (bool v, bool propagate) override;
    void set_value (FatProcess* v, bool propagate) override;
    void set_value (const std::string& v, bool propagate) override;
    void set_value (const char* v, bool propagate) override { set_value(std::string(v), propagate);};
    double get_double_value () override { return get_ref_value(); }
    double get_double_value () const override { return get_ref_value(); }
    std::string get_string_value () override { return std::to_string (get_ref_value ()); }
    std::string get_string_value () const override { return std::to_string (get_ref_value ()); }

    double get_value () { return get_ref_value(); };
  protected:
    virtual double& get_ref_value() = 0;
    virtual const double& get_ref_value() const = 0;

#ifndef DJNN_NO_DEBUG
    void dump (int level=0) override;
#endif
  };

  class DoubleProperty : public AbstractDoubleProperty {
  public:
    DoubleProperty (FatProcess* parent, const std::string& name, double v) : AbstractDoubleProperty (parent, name), value(v) { }
    FatProcess* clone () override;
  protected:
    virtual double& get_ref_value() override { return value; }
    virtual const double& get_ref_value() const override { return value; }
  private:
    double value;

#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

  class DoublePropertyProxy : public AbstractDoubleProperty {
  public:
    DoublePropertyProxy (FatProcess* parent, const std::string& name, double &v, int notify_mask=notify_none) : AbstractDoubleProperty (parent, name, notify_mask), value(v) { }
    FatProcess* clone () override;
  protected:
    virtual double& get_ref_value() override { return value; }
    virtual const double& get_ref_value() const override { return value; }
  private:
    double& value;

#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

  double getDouble (FatProcess *p);
  void setDouble (FatProcess *p, double v);
}
