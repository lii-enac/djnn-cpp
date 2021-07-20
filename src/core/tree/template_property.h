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
 *      Stephane Conversy <mathieu.stephane.conversy@enac.fr>
 *
 */

#include "abstract_property.h"

namespace djnn {

    template <typename X>
    struct TemplatePropertyType {
        static PropertyType type;
    };

    template <typename X>
    class AbstractTemplateProperty : public AbstractProperty {
    public:
        AbstractTemplateProperty (Process* parent, const string& name, int notify_mask=notify_none)
        : AbstractProperty (parent, name, notify_mask) { finalize_construction (parent, name); }
        virtual int get_prop_type () const override { return TemplatePropertyType<X>.type; }
        void set_value (const X& v, bool propagate) override {
            get_ref_value() = X;
            if (is_activable () && propagate) {
                notify_activation ();
                notify_parent ();
            }
        }
        X& get_value () { return get_ref_value(); }
        const X& get_value () const { return get_ref_value(); }

    protected:
        virtual X& get_ref_value() = 0;
        virtual const X& get_ref_value() const = 0;
    };

    template <typename X>
    class TemplateProperty : public AbstractTemplateProperty<X> {
    public:
        TemplateProperty (Process* parent, const string& name, int v)
        : AbstractTemplateProperty (parent, name), value(v) {}
        Process* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
    protected:
        virtual X& get_ref_value() override { return value; }
        virtual const X& get_ref_value() const override { return value; }
    private:
        X value;

    public:
        #ifndef DJNN_NO_SERIALIZE
        void serialize (const string& format) override;
        #endif
    };

    template <typename X>
    class TemplatePropertyProxy : public AbstractTemplateProperty<X> {
    public:
        IntPropertyProxy (Process* parent, const string& name, X &v, int notify_mask=notify_none)
        : AbstractTemplateProperty (parent, name, notify_mask), value(v) {}
        Process* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
    protected:
        virtual X& get_ref_value() override { return value; }
        virtual const X& get_ref_value() const override { return value; }
    private:
        X& value;

    public:
        #ifndef DJNN_NO_SERIALIZE
        void serialize (const string& format) override;
        #endif
    };
}
