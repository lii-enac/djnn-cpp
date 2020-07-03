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

  class AbstractTextProperty : public AbstractProperty {
  public:
    AbstractTextProperty (ParentProcess* parent, const std::string& name, int notify_mask=notify_none) : AbstractProperty (parent, name, notify_mask) { finalize_construction (parent, name); };
    virtual int get_prop_type () const override { return String; }
    //virtual process_type_e get_process_type () const override { return TEXT_PROPERTY_T; }

    // AbstractProperty interface
    void set_value (int newValue, bool propagate) override;
    void set_value (double v, bool propagate) override;
    void set_value (bool v, bool propagate) override;
    void set_value (CoreProcess* v, bool propagate) override;
    void set_value (const std::string& v, bool propagate) override;
    void set_value (const char* v, bool propagate) override { set_value(std::string(v), propagate); };
    double get_double_value () override;
    double get_double_value () const override;
    std::string get_string_value () override { return get_ref_value (); }
    std::string get_string_value () const override { return get_ref_value (); }
    std::string& get_value () { return get_ref_value(); };
  protected:
    virtual std::string& get_ref_value() = 0;
    virtual const std::string& get_ref_value() const= 0;

  public:
#ifndef DJNN_NO_DEBUG
    void dump (int level=0) override;
#endif
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

  class TextProperty : public AbstractTextProperty {
  public:
    TextProperty (ParentProcess* parent, const std::string& name, std::string v) : AbstractTextProperty (parent, name), value(v) { }
    FatProcess* clone () override;
  protected:
    virtual std::string& get_ref_value() override { return value; }
    virtual const std::string& get_ref_value() const override { return value; }
  private:
    std::string value;
  };

  class TextPropertyProxy : public AbstractTextProperty {
  public:
    TextPropertyProxy (ParentProcess* parent, const std::string& name, std::string &v, int notify_mask=notify_none) : AbstractTextProperty (parent, name, notify_mask), value(v) { }
    FatProcess* clone () override;
  protected:
    virtual std::string& get_ref_value() override { return value; }
    virtual const std::string& get_ref_value() const override { return value; }
  private:
    std::string& value;
  };


  std::string toString(const AbstractProperty& x);
  std::string& toString(FatProcess* p);

  std::string& getString (FatProcess* p);
  void setString (FatProcess* p, std::string &v);
}

