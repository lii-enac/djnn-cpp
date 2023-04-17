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

//#include <cmath>

extern "C" {
  #if __MINGW64__
  #define noexcept
  #endif
  double exp (double) noexcept;
  double log (double) noexcept;
  double log10 (double) noexcept;
  double sqrt (double) noexcept;
  double fabs (double) noexcept;
  double pow (double,double) noexcept;
  #if __MINGW64__
  #undef noexcept
  #endif
}

#include "operators.h"

namespace djnn
{

  template <typename T> struct my_exp { T operator() (T t1) { return ::exp(t1);} };
  template <> const char name_info<my_exp<double>>::left[];
  template <> const char name_info<my_exp<double>>::right[];
  template <> const char name_info<my_exp<double>>::serialize[];
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  my_exp<double>, double> ExpAction;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  my_exp<double>, double> Exp;

  template <typename T> struct my_log { T operator() (T t1) { return ::log(t1);} };
  template <> const char name_info<my_log<double>>::left[];
  template <> const char name_info<my_log<double>>::right[];
  template <> const char name_info<my_log<double>>::serialize[];
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  my_log<double>, double> LogAction;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  my_log<double>, double> Log;

  template <typename T> struct my_log10 { T operator() (T t1) { return ::log10(t1);} };
  template <> const char name_info<my_log10<double>>::left[];
  template <> const char name_info<my_log10<double>>::right[];
  template <> const char name_info<my_log10<double>>::serialize[];
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  my_log<double>, double> Log10Action;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  my_log<double>, double> Log10;

  template <typename T> struct my_sqrt { T operator() (T t1) { return ::sqrt(t1);} };
  template <> const char name_info<my_sqrt<double>>::left[];
  template <> const char name_info<my_sqrt<double>>::right[];
  template <> const char name_info<my_sqrt<double>>::serialize[];
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  my_sqrt<double>, double> SqrtAction;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  my_sqrt<double>, double> Sqrt;

  template <typename T> struct my_abs { T operator() (T t1) { return ::fabs(t1);} };
  template <> const char name_info<my_abs<double>>::left[];
  template <> const char name_info<my_abs<double>>::right[];
  template <> const char name_info<my_abs<double>>::serialize[];
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  my_abs<double>, double> AbsAction;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  my_abs<double>, double> Abs;


  template <typename T> struct my_pow { T operator() (T t1, T t2) { return ::pow(t1, t2);} };
  template <> const char name_info<my_pow<double>>::left[];
  template <> const char name_info<my_pow<double>>::right[];
  template <> const char name_info<my_pow<double>>::serialize[];
  typedef BinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, my_pow<double>, double, double> PowAction;
  typedef BinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, my_pow<double>, double, double> Pow;

  #define DJNN_MIN___(x,y) ((x)<(y)?(x):(y))
  template <typename T> struct my_min { T operator() (T t1, T t2) { return DJNN_MIN___(t1, t2);} };
  template <> const char name_info<my_min<double>>::left[];
  template <> const char name_info<my_min<double>>::right[];
  template <> const char name_info<my_min<double>>::serialize[];
  typedef BinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, my_min<double>, double, double> MinAction;
  typedef BinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, my_min<double>, double, double> Min;
  #undef DJNN_MIN___

  #define DJNN_MAX___(x,y) ((x)>(y)?(x):(y))
  template <typename T> struct my_max { T operator() (T t1, T t2) { return DJNN_MAX___(t1, t2);} };
  template <> const char name_info<my_max<double>>::left[];
  template <> const char name_info<my_max<double>>::right[];
  template <> const char name_info<my_max<double>>::serialize[];
  typedef BinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, my_max<double>, double, double> MaxAction;
  typedef BinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, my_max<double>, double, double> Max;
  #undef DJNN_MAX___

  template <typename T> struct my_clamp_min { T operator() (T t1, T t2) { return t2 < t1 ? t1 : t2;} };
  template <> const char name_info<my_clamp_min<double>>::left[];
  template <> const char name_info<my_clamp_min<double>>::right[];
  template <> const char name_info<my_clamp_min<double>>::serialize[];
  typedef BinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, my_clamp_min<double>, double, double> ClampMinAction;
  typedef BinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, my_clamp_min<double>, double, double> ClampMin;

  template <typename T> struct my_clamp_max { T operator() (T t1, T t2) { return t2 > t1 ? t1 : t2;} };
  template <> const char name_info<my_clamp_max<double>>::left[];
  template <> const char name_info<my_clamp_max<double>>::right[];
  template <> const char name_info<my_clamp_max<double>>::serialize[];
  typedef BinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, my_clamp_max<double>, double, double> ClampMaxAction;
  typedef BinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, my_clamp_max<double>, double, double> ClampMax;



  class BoundedValue : public FatProcess
  {
  private:
    class BoundedValueAction : public Action
    {
    public:
      BoundedValueAction (CoreProcess* parent, const string& name, BoundedValue& bv) : Action (parent, name), _bv(bv) {}
    virtual ~BoundedValueAction () {}
      void impl_activate () override {
        double max = _bv._max.get_value ();
        double min = _bv._min.get_value ();
        double input = _bv._input.get_value ();
        double r = input < min ? min : (input > max ? max : input);
        _bv._result.set_value (r, true);
      }
    private:
      BoundedValue& _bv;
    };
  public:
    BoundedValue (CoreProcess* parent, const string& name, double min, double max, double init_val);
    void impl_activate () override { _c_min.enable(); _c_max.enable (); _c_input.enable (); _action.activate (); };
    void impl_deactivate () override { _c_min.disable (); _c_max.disable (); _c_input.disable (); _action.deactivate ();};
    virtual ~BoundedValue ();
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const string& format) override;
#endif
  protected:
    void set_parent (CoreProcess* parent) override;
    DoubleProperty _min, _max, _input, _result;
    BoundedValueAction _action;
    Coupling _c_min, _c_max, _c_input;
  };

}
