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

#if 1

namespace djnn {
  class AbstractIntProperty : public AbstractSimpleProperty {
  public:
    AbstractIntProperty (CoreProcess* parent, const string& name, int notify_mask=notify_none) : AbstractSimpleProperty (parent, name, notify_mask) { finalize_construction (parent, name); };
    virtual int get_prop_type () const override { return Integer; }
    //virtual process_type_e get_process_type () const override { return INT_PROPERTY_T; }

    // AbstractSimpleProperty interface
    void set_value (int newValue, bool propagate) override;
    void set_value (double v, bool propagate) override;
    void set_value (bool v, bool propagate) override;
    void set_value (CoreProcess* v, bool propagate) override;
    void set_value (const string& v, bool propagate) override;
    void set_value (const char* v, bool propagate) override { set_value(string(v), propagate);};
    double get_double_value () override { return get_ref_value(); }
    double get_double_value () const override { return get_ref_value(); }
    string get_string_value () override { return djnn::to_string (get_ref_value ()); }
    string get_string_value () const override { return djnn::to_string (get_ref_value ()); }

    int get_value () const { return get_ref_value(); };
  protected:
    virtual int& get_ref_value() = 0;
    virtual const int& get_ref_value() const = 0;

  public:
#ifndef DJNN_NO_DEBUG
    void dump (int level=0) override;
#endif
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
  };

  class IntProperty : public AbstractIntProperty {
  public:
    IntProperty (CoreProcess* parent, const string& name, int v) : AbstractIntProperty (parent, name), value(v) { }
    FatProcess* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
  protected:
    virtual int& get_ref_value() override { return value; }
    virtual const int& get_ref_value() const override { return value; }
  private:
    int value;
  };

  class IntPropertyProxy : public AbstractIntProperty {
  public:
    IntPropertyProxy (CoreProcess* parent, const string& name, int &v, int notify_mask=notify_none) : AbstractIntProperty (parent, name, notify_mask), value(v) { }
    FatProcess* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
  protected:
    virtual int& get_ref_value() override { return value; }
    virtual const int& get_ref_value() const override { return value; }
  private:
    int& value;
  };

  int getInt (CoreProcess *p);
  void setInt (CoreProcess* parent, int v);
}

#else

#include "template_property.h"
namespace djnn {

  template <>
  struct TemplatePropertyType<int> {
      static const PropertyType type = Integer;
  };

  using AbstractIntProperty = AbstractTemplateProperty<int>;
  using IntProperty = TemplateProperty<int>;
  using IntPropertyProxy = TemplatePropertyProxy<int>;

  int getInt (CoreProcess *p);
  void setInt (CoreProcess* parent, int v);
}


#endif
