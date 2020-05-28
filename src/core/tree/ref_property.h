/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "abstract_property.h"

#include <string>

namespace djnn {
  //using namespace std;

  class RefProperty: public AbstractProperty {
  public:
    RefProperty (FatProcess* parent, const std::string& name, CoreProcess* v, unsigned int nm = notify_none) 
    : AbstractProperty (parent, name, nm), _value (v) { finalize_construction (parent, name); }
    virtual int get_prop_type () const override { return Reference; }
    //virtual process_type_e get_process_type () const override { return REF_PROPERTY_T; }

    CoreProcess* get_value () { return _value; }
    void set_value (int newValue, bool propagate) override;
    void set_value (double v, bool propagate) override;
    void set_value (bool v, bool propagate) override;
    void set_value (FatProcess* v, bool propagate) override;
    void set_value (CoreProcess* v, bool propagate); // FIXME
    void set_value (const std::string& v, bool propagate) override;
    void set_value (const char* v, bool propagate) override { set_value(std::string(v), propagate);}
    double get_double_value () override;
    double get_double_value () const override;
    std::string get_string_value () override;
    std::string get_string_value () const override;
    FatChildProcess* find_child (const std::string& path) override;
    RefProperty* clone () override;

  private:
    CoreProcess* _value;

  public:
#ifndef DJNN_NO_DEBUG
    void dump (int level=0) override;
#endif
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
};

  CoreProcess* getRef (CoreProcess *p);
  void setRef (CoreProcess *p, CoreProcess *v);
}
