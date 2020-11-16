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

#include "core/ontology/process.h"

namespace djnn {

  enum PropertyType {
    Boolean,
    Integer,
    Double,
    String,
    Reference,
    Dist
  };

  class AbstractProperty : public FatProcess
  {
  public:
    AbstractProperty (ParentProcess* parent, const std::string& name, unsigned int nm=notify_none) :
      FatProcess (name),
      _notify_mask (nm) {}
    virtual ~AbstractProperty () override {}
    virtual process_type_e get_process_type () const override { return PROPERTY_T; }
    virtual int get_prop_type () const = 0;
    bool is_activable () {
      return get_parent () == nullptr || get_parent ()->somehow_activating ();
    }
    void notify_parent () { if (_notify_mask != notify_none && get_parent ()) get_parent ()->notify_change (_notify_mask ); }
    virtual void set_value (int v, bool propagate) = 0;
    virtual void set_value (double v, bool propagate) = 0;
    virtual void set_value (bool v, bool propagate) = 0;
    virtual void set_value (CoreProcess* v, bool propagate) = 0;
    virtual void set_value (const std::string& v, bool propagate) = 0;
    virtual void set_value (const char* v, bool propagate) = 0;
    virtual double get_double_value () = 0;
    virtual double get_double_value () const = 0;
    virtual std::string get_string_value () = 0;
    virtual std::string get_string_value () const = 0;
    explicit operator bool () const { return get_double_value () != 0; } // added for the management of ternary operator in Smala
  protected:
    void post_activate () override { post_activate_auto_deactivate (); }
    void impl_activate () override {}
    void impl_deactivate () override {}
    unsigned int _notify_mask;
  };
}

#include "core/tree/operators.h"
