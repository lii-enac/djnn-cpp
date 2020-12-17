/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */


#include "easing_func.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/serializer/serializer.h"
// #include <vector> // pch
#include <cmath>

#define PI           3.14159265358979323846

namespace djnn
{
  /* The list of easing function and their implementation is taken from https://easings.net */
  static double
  in_sine (double x) {
    return 1 - cos((x * PI) / 2);
  }

  static double
  out_sine (double x) {
    return sin((x * PI) / 2);
  }

  static double
  in_out_sine (double x) {
    return -(cos(PI * x) - 1) / 2;
  }

  static double
  in_cubic (double x) {
    return x * x * x;
  }

  static double
  out_cubic (double x) {
    return 1 - pow (1 - x, 3);
  }

  static double
  in_out_cubic (double x) {
    return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
  }

  static double
  in_quint (double x) {
    return x * x * x * x * x;
  }

  static double
  out_quint (double x) {
    return 1 - pow(1 - x, 5);
  }

  static double
  in_out_quint (double x) {
    return x < 0.5 ? 16 * x * x * x * x * x : 1 - pow(-2 * x + 2, 5) / 2;
  }

  static double
  in_circ (double x) {
    return 1 - sqrt(1 - pow(x, 2));
  }

  static double
  out_circ (double x) {
    return sqrt(1 - pow(x - 1, 2));
  }

  static double
  in_out_circ (double x) {
    return x < 0.5
      ? (1 - sqrt(1 - pow(2 * x, 2))) / 2
      : (sqrt(1 - pow(-2 * x + 2, 2)) + 1) / 2;
  }

  static double
  in_elastic (double x) {
    double c4 = (2 * PI) / 3;

    return x == 0
      ? 0
      : x == 1
      ? 1
      : -pow(2, 10 * x - 10) * sin((x * 10 - 10.75) * c4);
  }

  static double
  out_elastic (double x) {
    double c4 = (2 * PI) / 3;

    return x == 0
      ? 0
      : x == 1
      ? 1
      : pow(2, -10 * x) * sin((x * 10 - 0.75) * c4) + 1;
  }

  static double
  in_out_elastic (double x) {
    double c5 = (2 * PI) / 4.5;

    return x == 0
      ? 0
      : x == 1
      ? 1
      : x < 0.5
      ? -(pow(2, 20 * x - 10) * sin((20 * x - 11.125) * c5)) / 2
      : (pow(2, -20 * x + 10) * sin((20 * x - 11.125) * c5)) / 2 + 1;
  }

  static double
  in_quad (double x) {
    return x * x;
  }

  static double
  out_quad (double x) {
    return 1 - (1 - x) * (1 - x);
  }

  static double
  in_out_quad (double x) {
    return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
  }

  static double
  in_quart (double x) {
    return x * x * x * x;
  }

  static double
  out_quart (double x) {
    return 1 - pow(1 - x, 4);
  }

  static double
  in_out_quart (double x) {
    return x < 0.5 ? 8 * x * x * x * x : 1 - pow(-2 * x + 2, 4) / 2;
  }

  static double
  in_expo (double x) {
    return x == 0 ? 0 : pow(2, 10 * x - 10);
  }

  static double
  out_expo (double x) {
    return x == 1 ? 1 : 1 - pow(2, -10 * x);
  }

  static double
  in_out_expo (double x) {
    return x == 0
      ? 0
      : x == 1
      ? 1
      : x < 0.5 ? pow(2, 20 * x - 10) / 2
      : (2 - pow(2, -20 * x + 10)) / 2;
  }

  static double
  in_back (double x) {
    double c1 = 1.70158;
    double c3 = c1 + 1;

    return c3 * x * x * x - c1 * x * x;
  }

  static double
  out_back (double x) {
    double c1 = 1.70158;
    double c3 = c1 + 1;

    return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
  }

  static double
  in_out_back (double x) {
    double c1 = 1.70158;
    double c2 = c1 * 1.525;

    return x < 0.5
      ? (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
      : (pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
  }

  static double
  out_bounce (double x) {
    double n1 = 7.5625;
    double d1 = 2.75;
    if (x < (1 / d1)) {
      return   n1 * x * x;
    } else if (x < (2 / d1)) {
      x = x - (1.5 / d1);
      return n1 * x * x + 0.75;
    } else if (x < (2.5 / d1)) {
       x = x - (2.25 / d1);
       return n1 * x * x + 0.9375;
    } else {
      x = x - (2.625 / d1);
      return n1 * x * x + 0.984375;
    }
  }

  static double
  in_bounce (double x) {
    return 1 - out_bounce(1 - x);
  }

  static double
  in_out_bounce (double x) {
    return x < 0.5
      ? (1 - out_bounce(1 - 2 * x)) / 2
      : (1 + out_bounce(2 * x - 1)) / 2;
  }

  std::vector<easing_function*> func_list = { in_sine, out_sine, in_out_sine,
    in_cubic, out_cubic, in_out_cubic,
    in_quint, out_quint, in_out_quint,
    in_circ, out_circ, in_out_circ,
    in_elastic, out_elastic, in_out_elastic,
    in_quad, out_quad, in_out_quad,
    in_quart, out_quart, in_out_quart,
    in_expo, out_expo, in_out_expo,
    in_back, out_back, in_out_back,
    in_bounce, out_bounce, in_out_bounce};

  EasingGenerator::EasingGenerator (ParentProcess* parent, const std::string& name, int func) :
        FatProcess (name),
        _func_selector (this, "selector", func),
        _in (this, "input", 0),
        _out (this, "output", 0),
        _act (this, "action"),
        _c_in (&_in, ACTIVATION, &_act, ACTIVATION)
  {
    graph_add_edge (&_in, &_act);
    graph_add_edge (&_act, &_out);
    finalize_construction (parent, name);
  }

  EasingGenerator::~EasingGenerator () {
    graph_remove_edge (&_in, &_act);
    graph_remove_edge (&_act, &_out);
  }

  void
  EasingGenerator::impl_activate () {
    _c_in.enable ();
  }

  void
  EasingGenerator::impl_deactivate () {
    _c_in.disable ();
  }

  void
  EasingGenerator::compute () {
    double in = _in.get_value ();
    in = in < 0 ? 0 : in > 1 ? 1 : in;
    _out.set_value (func_list[_func_selector.get_value()](in), true);
  }

#ifndef DJNN_NO_SERIALIZE
  void
  EasingGenerator::serialize (const std::string& type)
  {

  }
#endif

}
