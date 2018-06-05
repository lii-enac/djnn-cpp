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

#include "process.h"

namespace djnn {
  using namespace std;

  class AbstractProperty : public Process
  {
  public:
    AbstractProperty (Process* parent, const string &name) : Process (parent, name) { _type = Integer; _cpnt_type = PROPERTY; };
    AbstractProperty () : Process () { _type = Integer; _cpnt_type = PROPERTY; };
    virtual ~AbstractProperty () {};
    PropertyType type () { return _type; }
    bool is_activable () {
      return get_parent () == 0 || get_parent ()->get_state () < deactivating;
    }
    virtual void set_value (int v, bool propagate) = 0;
    virtual void set_value (double v, bool propagate) = 0;
    virtual void set_value (bool v, bool propagate) = 0;
    virtual void set_value (Process* v, bool propagate) = 0;
    virtual void set_value (const string &v, bool propagate) = 0;
  protected:
    PropertyType _type;
    void post_activate () { _activation_state = deactivated; };
    void activate () {};
    void deactivate () {};
    void deactivation () {};
  };

}
