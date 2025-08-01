#pragma once

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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "abstract_property.h"
#include "core/utils/error.h"
#include "s_to_p.h"
namespace djnn {

template <typename X>
struct TemplatePropertyType {
    static const property_type_e type = UserDefined; // by default
};

template <typename X>
class AbstractTemplateProperty : public SuperAbstractProperty {
  public:
    AbstractTemplateProperty (CoreProcess* parent, const string& name, int notify_mask = notify_none)
        : SuperAbstractProperty (parent, name, notify_mask) { finalize_construction (parent, name); }
    virtual property_type_e get_property_type () const override { return TemplatePropertyType<X>::type; }
    void                    my_set_value (const X& v, bool propagate) {
        get_ref_value () = v;
        if (is_activable () && propagate) {
            notify_activation ();
            notify_parent ();
        }
    }
    void set_value (const X& v, bool propagate) { my_set_value (v, propagate); }
    // virtual void set_value (int v, bool propagate) override {}
    // void set_value (double v, bool propagate) override = delete; // override { my_set_value(X(v), propagate); }
    // virtual void set_value (bool v, bool propagate) override {}
    // virtual void set_value (CoreProcess* v, bool propagate) override {}
    // virtual void set_value (const string& v, bool propagate) override {}
    // virtual void set_value (const char* v, bool propagate) override { set_value(string(v), propagate); }

    // double get_double_value () override { return 0; }
    // double get_double_value () const override { return 0; }
    // string get_string_value () override { return djnn::string (); }
    // string get_string_value () const override { return djnn::string (); }

    X&       get_value () { return get_ref_value (); }
    const X& get_value () const { return get_ref_value (); }

    using type = X;

  protected:
    virtual X&       get_ref_value ()       = 0;
    virtual const X& get_ref_value () const = 0;
};

template <class X>
inline const X&
get_property_value (const AbstractTemplateProperty<X>* ap) {
    return ap->get_value ();
}

template <typename X>
class TemplateProperty : public AbstractTemplateProperty<X> {
  public:
    TemplateProperty (CoreProcess* parent, const string& name, const X& v)
        : AbstractTemplateProperty<X> (parent, name), value (v) {}
    CoreProcess* impl_clone (djnnstl::map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override {
        error (this, "*PropertyProxy should not be cloned");
        return nullptr;
    }
    using type = X;

  protected:
    virtual X&       get_ref_value () override { return value; }
    virtual const X& get_ref_value () const override { return value; }

  private:
    X value;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override {}
#endif
};

template <typename X>
class TemplatePropertyProxy : public AbstractTemplateProperty<X> {
  public:
    TemplatePropertyProxy (CoreProcess* parent, const string& name, const X& v, int notify_mask = notify_none)
        : AbstractTemplateProperty<X> (parent, name, notify_mask), value (v) {}
    CoreProcess* impl_clone (djnnstl::map<const CoreProcess*, CoreProcess*>& origs_clones) const override {
        error (this, "*PropertyProxy should not be cloned");
        return nullptr;
    }
    using type = X;

  protected:
    virtual X&       get_ref_value () override { return value; }
    virtual const X& get_ref_value () const override { return value; }

  private:
    X& value;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override {}
#endif
};
} // namespace djnn
