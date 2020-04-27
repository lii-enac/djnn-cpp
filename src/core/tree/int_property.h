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

namespace djnn {
  //using namespace std;

  class AbstractIntProperty : public AbstractProperty {
  public:
    AbstractIntProperty (Process* parent, const std::string& name, int notify_mask=notify_none) : AbstractProperty (parent, name, notify_mask) { finalize_construction (parent, name); };
    virtual int get_prop_type () const override { return Integer; }
    //virtual process_type_e get_process_type () const override { return INT_PROPERTY_T; }

    // AbstractProperty interface
    void set_value (int newValue, bool propagate) override;
    void set_value (double v, bool propagate) override;
    void set_value (bool v, bool propagate) override;
    void set_value (Process* v, bool propagate) override;
    void set_value (const std::string& v, bool propagate) override;
    void set_value (const char* v, bool propagate) override { set_value(std::string(v), propagate);};
    double get_double_value () override { return get_ref_value(); }
    double get_double_value () const override { return get_ref_value(); }
    std::string get_string_value () override { return std::to_string (get_ref_value ()); }
    std::string get_string_value () const override { return std::to_string (get_ref_value ()); }

    int get_value () { return get_ref_value(); };
  protected:
    virtual int& get_ref_value() = 0;
    virtual const int& get_ref_value() const = 0;

  public:
#ifndef DJNN_NO_DEBUG
    void dump (int level=0) override;
#endif
  };

  class IntProperty : public AbstractIntProperty {
  public:
    IntProperty (Process* parent, const std::string& name, int v) : AbstractIntProperty (parent, name), value(v) { }
    Process* clone () override;
  protected:
    virtual int& get_ref_value() override { return value; }
    virtual const int& get_ref_value() const override { return value; }
  private:
    int value;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

  class IntPropertyProxy : public AbstractIntProperty {
  public:
    IntPropertyProxy (Process* parent, const std::string& name, int &v, int notify_mask=notify_none) : AbstractIntProperty (parent, name, notify_mask), value(v) { }
    Process* clone () override;
  protected:
    virtual int& get_ref_value() override { return value; }
    virtual const int& get_ref_value() const override { return value; }
  private:
    int& value;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

  int getInt (Process *p);
  void setInt (Process *p, int v);

}
