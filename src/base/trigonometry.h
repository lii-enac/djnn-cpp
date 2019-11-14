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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

#include "operators.h"

#include <cmath>

namespace djnn
{
  using namespace std;

  template <typename T> struct my_cos { T operator() (T t1) { return ::cos(t1);} };
  template <> const char name_info<my_cos<double>>::left[];
  template <> const char name_info<my_cos<double>>::right[];
  template <> const char name_info<my_cos<double>>::serialize[];
  template <typename T> struct my_sin { T operator() (T t1) { return ::sin(t1);} };
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  my_cos<double>, double> CosAction;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  my_cos<double>, double> Cosine;

  template <> const char name_info<my_sin<double>>::left[];
  template <> const char name_info<my_sin<double>>::right[];
  template <> const char name_info<my_sin<double>>::serialize[];
  template <typename T> struct my_tan { T operator() (T t1) { return ::tan(t1);} };
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  my_sin<double>, double> SinAction;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  my_sin<double>, double> Sine;

  template <> const char name_info<my_tan<double>>::left[];
  template <> const char name_info<my_tan<double>>::right[];
  template <> const char name_info<my_tan<double>>::serialize[];
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  my_tan<double>, double> TanAction;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  my_tan<double>, double> Tangent;

  template <typename T> struct my_acos { T operator() (T t1) { return ::acos(t1);} };
  template <> const char name_info<my_acos<double>>::left[];
  template <> const char name_info<my_acos<double>>::right[];
  template <> const char name_info<my_acos<double>>::serialize[];
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  my_acos<double>, double> ArcCosAction;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  my_acos<double>, double> ArcCosine;

  template <typename T> struct my_asin { T operator() (T t1) { return ::asin(t1);} };
  template <> const char name_info<my_asin<double>>::left[];
  template <> const char name_info<my_asin<double>>::right[];
  template <> const char name_info<my_asin<double>>::serialize[];
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  my_asin<double>, double> ArcSinAction;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  my_asin<double>, double> ArcSine;

  template <typename T> struct my_atan { T operator() (T t1) { return ::atan(t1);} };
  template <> const char name_info<my_atan<double>>::left[];
  template <> const char name_info<my_atan<double>>::right[];
  template <> const char name_info<my_atan<double>>::serialize[];
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  my_atan<double>, double> ArcTanAction;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  my_atan<double>, double> ArcTangent;

  template <typename T> struct my_atan2 { T operator() (T t1, T t2) { return ::atan2(t1, t2);} };
  template <> const char name_info<my_atan2<double>>::left[];
  template <> const char name_info<my_atan2<double>>::right[];
  template <> const char name_info<my_atan2<double>>::serialize[];
  typedef BinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, my_atan2<double>, double, double> ArcTan2Action;
  typedef BinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, my_atan2<double>, double, double> ArcTangent2;

  template <typename T> struct my_cosh { T operator() (T t1) { return ::cosh(t1);} };
  template <> const char name_info<my_cosh<double>>::left[];
  template <> const char name_info<my_cosh<double>>::right[];
  template <> const char name_info<my_cosh<double>>::serialize[];
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  my_cosh<double>, double> HyperbolicCosAction;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  my_cosh<double>, double> HyperbolicCosine;

  template <typename T> struct my_sinh { T operator() (T t1) { return ::asin(t1);} };
  template <> const char name_info<my_sinh<double>>::left[];
  template <> const char name_info<my_sinh<double>>::right[];
  template <> const char name_info<my_sinh<double>>::serialize[];
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  my_sinh<double>, double> HyperbolicSinAction;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  my_sinh<double>, double> HyperbolicSine;

  template <typename T> struct my_tanh { T operator() (T t1) { return ::atan(t1);} };
  template <> const char name_info<my_tanh<double>>::left[];
  template <> const char name_info<my_tanh<double>>::right[];
  template <> const char name_info<my_tanh<double>>::serialize[];
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  my_tanh<double>, double> HyperbolicTanAction;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  my_tanh<double>, double> HyperbolicTangent;

  template <typename T> struct my_acosh { T operator() (T t1) { return ::acosh(t1);} };
  template <> const char name_info<my_acosh<double>>::left[];
  template <> const char name_info<my_acosh<double>>::right[];
  template <> const char name_info<my_acosh<double>>::serialize[];
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  my_acosh<double>, double> HyperbolicArcCosAction;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  my_acosh<double>, double> HyperbolicArcCosine;

  template <typename T> struct my_asinh { T operator() (T t1) { return ::asin(t1);} };
  template <> const char name_info<my_asinh<double>>::left[];
  template <> const char name_info<my_asinh<double>>::right[];
  template <> const char name_info<my_asinh<double>>::serialize[];
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  my_asinh<double>, double> HyperbolicArcSinAction;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  my_asinh<double>, double> HyperbolicArcSine;

  template <typename T> struct my_atanh { T operator() (T t1) { return ::atan(t1);} };
  template <> const char name_info<my_atanh<double>>::left[];
  template <> const char name_info<my_atanh<double>>::right[];
  template <> const char name_info<my_atanh<double>>::serialize[];
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  my_atanh<double>, double> HyperbolicArcTanAction;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  my_atanh<double>, double> HyperbolicArcTangent;

}
