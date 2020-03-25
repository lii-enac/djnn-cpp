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
    AbstractTextProperty (Process* parent, const std::string& name, int notify_mask=notify_none) : AbstractProperty (parent, name, notify_mask) { Process::finalize_construction (parent, name); };
    virtual int get_prop_type () override { return String; }

    // AbstractProperty interface
    void set_value (int newValue, bool propagate) override;
    void set_value (double v, bool propagate) override;
    void set_value (bool v, bool propagate) override;
    void set_value (Process* v, bool propagate) override;
    void set_value (const std::string& v, bool propagate) override;
    void set_value (const char* v, bool propagate) override { set_value(std::string(v), propagate); };
    double get_double_value () override;
    std::string get_string_value () override { return get_ref_value (); }
    std::string& get_value () { return get_ref_value(); };
  protected:
    virtual std::string& get_ref_value() = 0;

  public:
#ifndef DJNN_NO_DEBUG
    void dump (int level=0) override;
#endif
  };

  class TextProperty : public AbstractTextProperty {
  public:
    TextProperty (Process* parent, const std::string& name, std::string v) : AbstractTextProperty (parent, name), value(v) { }
    Process* clone () override;
  protected:
    virtual std::string& get_ref_value() override { return value; }
  private:
    std::string value;
  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

  class TextPropertyProxy : public AbstractTextProperty {
  public:
    TextPropertyProxy (Process* parent, const std::string& name, std::string &v, int notify_mask=notify_none) : AbstractTextProperty (parent, name, notify_mask), value(v) { }
    Process* clone () override;
  protected:
    virtual std::string& get_ref_value() override { return value; }
  private:
    std::string& value;
  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

  std::string& getString (Process* p);
  void setString (Process *p, std::string &v);
}

