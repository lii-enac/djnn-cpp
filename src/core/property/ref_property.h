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

namespace djnn {

class AbstractRefProperty : public AbstractSimpleProperty { // for GET_CHILD_VALUE to work...
  public:
    AbstractRefProperty (CoreProcess* parent, const string& name, unsigned int nm = notify_none)
        : AbstractSimpleProperty (parent, name, nm) {}

    virtual CoreProcess* get_value () = 0;
};

class RefProperty : public AbstractRefProperty {
  public:
    RefProperty (CoreProcess* parent, const string& name, CoreProcess* v, unsigned int nm = notify_none)
        : AbstractRefProperty (parent, name, nm), _value (v) { finalize_construction (parent, name); }
    virtual property_type_e get_property_type () const override { return Reference; }
    // virtual process_type_e get_process_type () const override { return REF_PROPERTY_T; }

    CoreProcess* get_value () override { return _value; }
    void         set_value (int newValue, bool propagate) override;
    void         set_value (double v, bool propagate) override;
    void         set_value (bool v, bool propagate) override;
    void         set_value (CoreProcess* v, bool propagate) override;
    void         set_value (const string& v, bool propagate) override;
    void         set_value (const char* v, bool propagate) override { set_value (string (v), propagate); }
    double       get_double_value () override;
    double       get_double_value () const override;
    string       get_string_value () override;
    string       get_string_value () const override;
    RefProperty* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;

  protected:
    CoreProcess* find_child_impl (const string& path) override;

  private:
    CoreProcess* _value;

  public:
#ifndef DJNN_NO_DEBUG
    void dump (int level = 0) override;
#endif
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
};

CoreProcess* getRef (CoreProcess* p);
void         setRef (CoreProcess* p, CoreProcess* v);
} // namespace djnn
