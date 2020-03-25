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
#include "spike.h"

namespace djnn {
  //using namespace std;

  class AbstractBoolProperty : public AbstractProperty {
  public:
    AbstractBoolProperty (Process* parent, const std::string& name, int notify_mask=notify_none);
    virtual ~AbstractBoolProperty ();
    virtual int get_prop_type () override { return Boolean; }

    // AbstractProperty interface
    void set_value (int newValue, bool propagate) override;
    void set_value (double v, bool propagate) override;
    void set_value (bool v, bool propagate) override;
    void set_value (Process* v, bool propagate) override;
    void set_value (const std::string& v, bool propagate) override;
    void set_value (const char* v, bool propagate) override { set_value(std::string(v), propagate);};
    double get_double_value () override { return get_ref_value(); }
    std::string get_string_value () override { return std::to_string (get_ref_value ()); }
    bool get_value () { return get_ref_value(); };

  protected:
    virtual bool& get_ref_value() = 0;
    Spike _true, _false;

  public:
#ifndef DJNN_NO_DEBUG
    void dump (int level=0) override;
#endif
  };

  class BoolProperty : public AbstractBoolProperty {
  public:
    BoolProperty (Process* parent, const std::string& name, bool v) : AbstractBoolProperty (parent, name), value(v) { Process::finalize_construction (parent, name); }    
    Process* clone () override;
  protected:
    virtual bool& get_ref_value() override { return value; }
  private:
    bool value;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };  

  class BoolPropertyProxy : public AbstractBoolProperty {
  public:
    BoolPropertyProxy (Process* parent, const std::string& name, bool &v, int notify_mask=notify_none) : AbstractBoolProperty (parent, name, notify_mask), value(v) { Process::finalize_construction (parent, name); }
    Process* clone () override;
  protected:
    virtual bool& get_ref_value() override { return value; }
  private:
    bool& value;
  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

  bool getBool (Process *p);
  void setBool (Process *p, bool v);
}
