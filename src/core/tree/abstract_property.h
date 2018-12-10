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

#include "process.h"

namespace djnn {
  using namespace std;

  class AbstractProperty : public Process
  {
  public:
    AbstractProperty (Process* parent, const string &name, int nm=notify_none) : Process (parent, name), _notify_mask (nm) {}
    AbstractProperty () : Process (), _notify_mask (notify_none) {}
    virtual ~AbstractProperty () {};
    virtual int get_cpnt_type () override { return PROPERTY_T; }
    virtual int get_prop_type () = 0;
    bool is_activable () {
      return get_parent () == 0 || get_parent ()->somehow_activating ();
    }
    void notify_parent () { if (_notify_mask != notify_none && _parent) _parent->notify_change (_notify_mask ); }
    virtual void set_value (int v, bool propagate) = 0;
    virtual void set_value (double v, bool propagate) = 0;
    virtual void set_value (bool v, bool propagate) = 0;
    virtual void set_value (Process* v, bool propagate) = 0;
    virtual void set_value (const string &v, bool propagate) = 0;
    virtual void set_value (const char* v, bool propagate) = 0;
    virtual double get_double_value () = 0;
  protected:
    void post_activate () override { set_deactivated (); }
    void activate () override {}
    void deactivate () override {}
    void deactivation () override {}
    int _notify_mask;
  };

}
