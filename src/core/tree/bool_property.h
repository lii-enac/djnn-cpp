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
  using namespace std;

  class AbstractBoolProperty : public AbstractProperty {
  public:
    AbstractBoolProperty () : AbstractProperty () {};
    AbstractBoolProperty (Process* p, const string &name, int notify_mask=notify_none) : AbstractProperty (p, name, notify_mask) { Process::finalize (); };
    virtual ~AbstractBoolProperty ();
    virtual int get_prop_type () override { return Boolean; }

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

    bool get_value () { return get_ref_value(); };
  protected:
    virtual bool& get_ref_value() = 0;
    Process *_true, *_false;
  };

  class BoolProperty : public AbstractBoolProperty {
  public:
    BoolProperty (bool v) : AbstractBoolProperty (), value(v) { init_BoolProperty (); }
    BoolProperty (Process* p, const string &name, bool v) : AbstractBoolProperty (p, name), value(v) { init_BoolProperty (); }
    void serialize (const string& format) override;
    Process* clone () override;
  protected:
    virtual bool& get_ref_value() override { return value; }
  private:
    bool value;
    void init_BoolProperty ();
  };  

  class BoolPropertyProxy : public AbstractBoolProperty {
  public:
    BoolPropertyProxy (bool &v) : AbstractBoolProperty (), value(v) { init_BoolProperty (); }
    BoolPropertyProxy (Process* p, const string &name, bool &v, int notify_mask=notify_none) : AbstractBoolProperty (p, name, notify_mask), value(v) { init_BoolProperty (); }
    void serialize (const string& format) override;
    Process* clone () override;
  protected:
    virtual bool& get_ref_value() override { return value; }
  private:
    bool& value;
    void init_BoolProperty ();
  };

}
