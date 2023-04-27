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
#include "core/utils/to_string.h"
#include "core/utils/containers.h"


namespace djnn {

  class AbstractDoubleProperty : public AbstractSimpleProperty {
  public:
    AbstractDoubleProperty (CoreProcess* parent, const string& name, int notify_mask=notify_none) : AbstractSimpleProperty (parent, name, notify_mask) { finalize_construction (parent, name); };
    virtual PropertyType get_prop_type () const override { return Double; }
    //virtual process_type_e get_process_type () const override { return DOUBLE_PROPERTY_T; }

    // AbstractSimpleProperty interface
    void set_value (int newValue, bool propagate) override;
    void set_value (double v, bool propagate) override;
    void set_value (bool v, bool propagate) override;
    void set_value (CoreProcess* v, bool propagate) override;
    void set_value (const string& v, bool propagate) override;
    void set_value (const char* v, bool propagate) override { set_value(string(v), propagate);};
    double get_double_value () override { return get_ref_value(); }
    double get_double_value () const override { return get_ref_value(); }
    string get_string_value () override { return djnnstl::to_string (get_ref_value ()); }
    string get_string_value () const override { return djnnstl::to_string (get_ref_value ()); }

    double get_value () const { return get_ref_value(); };
  protected:
    virtual double& get_ref_value() = 0;
    virtual const double& get_ref_value() const = 0;

  public:
#ifndef DJNN_NO_DEBUG
    void dump (int level=0) override;
#endif
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
  };

  class DoubleProperty : public AbstractDoubleProperty {
  public:
    DoubleProperty (CoreProcess* parent, const string& name, double v) : AbstractDoubleProperty (parent, name), value(v) { }
    DoubleProperty* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
  protected:
    virtual double& get_ref_value() override { return value; }
    virtual const double& get_ref_value() const override { return value; }
  private:
    double value;
  };

  class DoublePropertyProxy : public AbstractDoubleProperty {
  public:
    DoublePropertyProxy (CoreProcess* parent, const string& name, double &v, int notify_mask=notify_none) : AbstractDoubleProperty (parent, name, notify_mask), value(v) { }
    DoublePropertyProxy* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
  protected:
    virtual double& get_ref_value() override { return value; }
    virtual const double& get_ref_value() const override { return value; }
  private:
    double& value;
  };

  double getDouble (CoreProcess *p);
  void setDouble (CoreProcess* parent, double v);
}
