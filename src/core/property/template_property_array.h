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

#include "template_property.h"
#include "abstract_property.h"
#include "s_to_p.h"

namespace djnn {

  using DoubleArray = vector<double>;
  //using DoubleArray = TemplateProperty<double_array>*;

  const DoubleArray
  operator+ (const DoubleArray &l, const DoubleArray &r) {
    vector<double> res;
    res.resize (l.size ());
    if (l.size () == r.size ()) {
      for (int i = 0; i < l.size (); i++) {
        double v = l.at (i) + r.at (i);
        res[i] = l.at (i) + r.at (i);
      }
    }
    return res;
  }

  const DoubleArray
  operator- (const DoubleArray &l, const DoubleArray &r) {
    vector<double> res;
    res.resize (l.size ());
    if (l.size () == r.size ()) {
      for (int i = 0; i < l.size (); i++) {
        double v = l.at (i) + r.at (i);
        res[i] = l.at (i) - r.at (i);
      }
    }
    return res;
  }

  const DoubleArray
  operator* (const DoubleArray &l, const DoubleArray &r) {
    vector<double> res;
    res.resize (l.size ());
    if (l.size () == r.size ()) {
      for (int i = 0; i < l.size (); i++) {
        double v = l.at (i) + r.at (i);
        res[i] = l.at (i) * r.at (i);
      }
    }
    return res;
  }

  const DoubleArray
  operator/ (const DoubleArray &l, const DoubleArray &r) {
    vector<double> res;
    res.resize (l.size ());
    if (l.size () == r.size ()) {
      for (int i = 0; i < l.size (); i++) {
        double v = l.at (i) + r.at (i);
        res[i] = l.at (i) / r.at (i);
      }
    }
    return res;
  }

  void
  set_value (TemplateProperty<DoubleArray>* array, int at, double v, int propagate) {
    if (array->get_value().size () > at) array->get_value().at (at) = v;
    if (array->is_activable () && propagate) {
      array->notify_activation ();
      array->notify_parent ();
    }
  }
};
