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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "core/enums.h"
#include "core/ontology/process.h"

namespace djnn {

class SuperAbstractProperty : public FatProcess {
  public:
    SuperAbstractProperty (CoreProcess* parent, const string& name, unsigned int notify_mask = notify_none)
        : FatProcess (name),
          _notify_mask (notify_mask) {}
    // virtual ~SuperAbstractProperty () override {}
    virtual process_type_e  get_process_type () const override { return PROPERTY_T; }
    virtual property_type_e get_property_type () const = 0;
    bool                    is_activable () const {
        return get_parent () == nullptr || get_parent ()->somehow_activating ();
    }
    void notify_parent () {
        if (_notify_mask != notify_none)
            if (auto* p = get_parent ())
                p->notify_change (_notify_mask);
    }
    unsigned int get_notify_mask () const { return _notify_mask; }

  protected:
    void         post_activate () override { post_activate_auto_deactivate (); }
    void         impl_activate () override {}
    void         impl_deactivate () override {}
    unsigned int _notify_mask;
};

// for simple property such as int, double, string, ref...
class AbstractSimpleProperty : public SuperAbstractProperty {
  public:
    AbstractSimpleProperty (CoreProcess* parent, const string& name, unsigned int notify_mask = notify_none)
        : SuperAbstractProperty (parent, name, notify_mask) {}
    virtual void   set_value (int v, bool propagate)           = 0;
    virtual void   set_value (double v, bool propagate)        = 0;
    virtual void   set_value (bool v, bool propagate)          = 0;
    virtual void   set_value (CoreProcess* v, bool propagate)  = 0;
    virtual void   set_value (const string& v, bool propagate) = 0;
    virtual void   set_value (const char* v, bool propagate)   = 0;
    virtual double get_double_value ()                         = 0;
    virtual double get_double_value () const                   = 0;
    virtual string get_string_value ()                         = 0;
    virtual string get_string_value () const                   = 0;
    explicit       operator bool () const { return get_double_value () != 0; } // added for the management of ternary operator in Smala
};

inline double
get_property_value (const AbstractSimpleProperty* ap) {
    return ap->get_double_value ();
}

// using AbstractProperty = SuperAbstractProperty; // should be this
using AbstractProperty = AbstractSimpleProperty; // but use this instead, to avoid breaking legacy apps

// inline
// double
// get_property_value(const CoreProcess* fcp) {
//   auto * ap = dynamic_cast<const AbstractProperty*>(fcp);
//   if (!ap) {
//     //warning (fcp, "not a property"); // FIXME warn or do something
//     return -1;
//   }
//   return ap->get_double_value();
// }
} // namespace djnn
