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

class AbstractTextProperty : public AbstractSimpleProperty {
  public:
    AbstractTextProperty (CoreProcess* parent, const string& name, int notify_mask = notify_none)
        : AbstractSimpleProperty (parent, name, notify_mask) { finalize_construction (parent, name); };
    virtual property_type_e get_property_type () const override { return String; }
    // virtual process_type_e get_process_type () const override { return TEXT_PROPERTY_T; }

    // AbstractSimpleProperty interface
    void          set_value (int newValue, bool propagate) override;
    void          set_value (double v, bool propagate) override;
    void          set_value (bool v, bool propagate) override;
    void          set_value (CoreProcess* v, bool propagate) override;
    void          set_value (const string& v, bool propagate) override;
    void          set_value (const char* v, bool propagate) override { set_value (string (v), propagate); };
    double        get_double_value () override;
    double        get_double_value () const override;
    string        get_string_value () override { return get_ref_value (); }
    string        get_string_value () const override { return get_ref_value (); }
    string&       get_value () { return get_ref_value (); }
    const string& get_value () const { return get_ref_value (); }

  protected:
    virtual string&       get_ref_value ()       = 0;
    virtual const string& get_ref_value () const = 0;

  public:
#ifndef DJNN_NO_DEBUG
    void dump (int level = 0) override;
#endif
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
};

class TextProperty : public AbstractTextProperty {
  public:
    TextProperty (CoreProcess* parent, const string& name, const string& v)
        : AbstractTextProperty (parent, name), value (v) {}
    FatProcess* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;

  protected:
    virtual string&       get_ref_value () override { return value; }
    virtual const string& get_ref_value () const override { return value; }

  private:
    string value;
};

class TextPropertyProxy : public AbstractTextProperty {
  public:
    TextPropertyProxy (CoreProcess* parent, const string& name, string& v, int notify_mask = notify_none)
        : AbstractTextProperty (parent, name, notify_mask), value (v) {}
    FatProcess* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;

  protected:
    virtual string&       get_ref_value () override { return value; }
    virtual const string& get_ref_value () const override { return value; }

  private:
    string& value;
};

djnnstl::string  toString (const AbstractSimpleProperty& x);
djnnstl::string& toString (CoreProcess* p);

djnnstl::string& getString (CoreProcess* p);
void             setString (CoreProcess* p, djnnstl::string& v);
} // namespace djnn
