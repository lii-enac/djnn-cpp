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

  class AbstractTextProperty : public AbstractProperty {
  public:
    AbstractTextProperty (ParentProcess* parent, const djnn::string& name, int notify_mask=notify_none) : AbstractProperty (parent, name, notify_mask) { finalize_construction (parent, name); };
    virtual int get_prop_type () const override { return String; }
    //virtual process_type_e get_process_type () const override { return TEXT_PROPERTY_T; }

    // AbstractProperty interface
    void set_value (int newValue, bool propagate) override;
    void set_value (double v, bool propagate) override;
    void set_value (bool v, bool propagate) override;
    void set_value (CoreProcess* v, bool propagate) override;
    void set_value (const djnn::string& v, bool propagate) override;
    void set_value (const char* v, bool propagate) override { set_value(djnn::string(v), propagate); };
    double get_double_value () override;
    double get_double_value () const override;
    djnn::string get_string_value () override { return get_ref_value (); }
    djnn::string get_string_value () const override { return get_ref_value (); }
    djnn::string& get_value () { return get_ref_value(); };
  protected:
    virtual djnn::string& get_ref_value() = 0;
    virtual const djnn::string& get_ref_value() const= 0;

  public:
#ifndef DJNN_NO_DEBUG
    void dump (int level=0) override;
#endif
#ifndef DJNN_NO_SERIALIZE
    void serialize (const djnn::string& format) override;
#endif
  };

  class TextProperty : public AbstractTextProperty {
  public:
    TextProperty (ParentProcess* parent, const djnn::string& name, djnn::string v) : AbstractTextProperty (parent, name), value(v) { }
    FatProcess* clone () override;
  protected:
    virtual djnn::string& get_ref_value() override { return value; }
    virtual const djnn::string& get_ref_value() const override { return value; }
  private:
    djnn::string value;
  };

  class TextPropertyProxy : public AbstractTextProperty {
  public:
    TextPropertyProxy (ParentProcess* parent, const djnn::string& name, djnn::string &v, int notify_mask=notify_none) : AbstractTextProperty (parent, name, notify_mask), value(v) { }
    FatProcess* clone () override;
  protected:
    virtual djnn::string& get_ref_value() override { return value; }
    virtual const djnn::string& get_ref_value() const override { return value; }
  private:
    djnn::string& value;
  };


  djnn::string toString(const AbstractProperty& x);
  djnn::string& toString(CoreProcess* p);

  djnn::string& getString (CoreProcess* p);
  void setString (CoreProcess* p, djnn::string &v);
}

