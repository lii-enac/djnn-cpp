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

#include "operators.h"

#include <cmath>
#include <algorithm>

namespace djnn
{
  using namespace std;

#if NEW_OP
  template <typename T> struct my_exp { T operator() (T t1) { return ::exp(t1);} };
  template <> std::string serialize_info<my_exp<double>>::serialize;
  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  my_exp<double>, double> ExpAction;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  my_exp<double>, double> Exp;

  template <typename T> struct my_log { T operator() (T t1) { return ::log(t1);} };
  template <> std::string serialize_info<my_log<double>>::serialize;
  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  my_log<double>, double> LogAction;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  my_log<double>, double> Log;

  template <typename T> struct my_log10 { T operator() (T t1) { return ::log10(t1);} };
  template <> std::string serialize_info<my_log10<double>>::serialize;
  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  my_log<double>, double> Log10Action;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  my_log<double>, double> Log10;

  template <typename T> struct my_sqrt { T operator() (T t1) { return ::sqrt(t1);} };
  template <> std::string serialize_info<my_sqrt<double>>::serialize;
  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  my_sqrt<double>, double> SqrtAction;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  my_sqrt<double>, double> Sqrt;

  template <typename T> struct my_abs { T operator() (T t1) { return ::abs(t1);} };
  template <> std::string serialize_info<my_abs<double>>::serialize;
  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  my_abs<double>, double> AbsAction;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  my_abs<double>, double> Abs;


  template <typename T> struct my_pow { T operator() (T t1, T t2) { return ::pow(t1, t2);} };
  template <> std::string serialize_info<my_pow<double>>::serialize;
  typedef NewBinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, my_pow<double>, double, double> PowAction;
  typedef NewBinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, my_pow<double>, double, double> Pow;

  template <typename T> struct my_min { T operator() (T t1, T t2) { return std::min(t1, t2);} };
  template <> std::string serialize_info<my_min<double>>::serialize;
  typedef NewBinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, my_min<double>, double, double> MinAction;
  typedef NewBinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, my_min<double>, double, double> Min;

  template <typename T> struct my_max { T operator() (T t1, T t2) { return std::max(t1, t2);} };
  template <> std::string serialize_info<my_max<double>>::serialize;
  typedef NewBinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, my_max<double>, double, double> MaxAction;
  typedef NewBinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, my_max<double>, double, double> Max;

  class BoundedValue : public Process
  {
  private:
    class BoundedValueAction : public Action
    {
    public:
      BoundedValueAction (Process *p, const string &n, BoundedValue& bv) : Action (p, n), _bv(bv) {}
    virtual ~BoundedValueAction () {}
      void impl_activate () override {
        double max = _bv._max.get_value ();
        double min = _bv._min.get_value ();
        double input = _bv._input.get_value ();
        double r = input < min ? min : (input > max ? max : input);
        _bv._result.set_value (r, true);
      }
      void impl_deactivate () override {}
    private:
      BoundedValue& _bv;
    };
  public:
    BoundedValue (Process *p, const string &n, double min, double max, double init_val);
    void impl_activate () override { _c_min.enable(); _c_max.enable (); _c_input.enable (); _action.activate (); };
    void impl_deactivate () override { _c_min.disable (); _c_max.disable (); _c_input.disable (); _action.deactivate ();};
    virtual ~BoundedValue ();
    void serialize (const string& type) override;
  protected:
    void set_parent (Process* p) override;
    DoubleProperty _min, _max, _input, _result;
    BoundedValueAction _action;
    Coupling _c_min, _c_max, _c_input;
  };

#else

  class Exp : public UnaryOperator
  {
  private:
    class ExpAction : public UnaryOperatorAction
    {
    public:
      ExpAction (Process* parent, const string &name, AbstractProperty* input, AbstractProperty* output) :
        UnaryOperatorAction (parent, name, input, output) {}
      virtual ~ExpAction () {}
      void impl_activate ()
      {
        _output->set_value (exp (((DoubleProperty*)_input)->get_value ()), true);
      }
      void impl_deactivate () {}
    };
  public:
    Exp (Process *p, const string &name, double i_val);
    virtual ~Exp () {}
    void serialize (const string& type) override;
  };

  class Log : public UnaryOperator
  {
    private:
      class LogAction : public UnaryOperatorAction
      {
      public:
        LogAction (Process* parent, const string &name, AbstractProperty* input, AbstractProperty* output) :
          UnaryOperatorAction (parent, name, input, output) {}
        virtual ~LogAction () {}
        void impl_activate ()
        {
          _output->set_value (log (((DoubleProperty*)_input)->get_value ()), true);
        }
        void impl_deactivate () {}
      };
    public:
      Log (Process *p, const string &name, double i_val);
      virtual ~Log () {}
      void serialize (const string& type) override;
  };

  class Log10 : public UnaryOperator
  {
    private:
      class Log10Action : public UnaryOperatorAction
      {
      public:
        Log10Action (Process* parent, const string &name, AbstractProperty* input, AbstractProperty* output) :
          UnaryOperatorAction (parent, name, input, output) {}
        virtual ~Log10Action () {}
        void impl_activate ()
        {
          _output->set_value (log10 (((DoubleProperty*)_input)->get_value ()), true);
        }
        void impl_deactivate () {}
      };
    public:
      Log10 (Process *p, const string &name, double i_val);
      virtual ~Log10 () {}
      void serialize (const string& type) override;
  };

  class Pow : public BinaryOperator
  {
    private:
      class PowAction : public BinaryOperatorAction
      {
      public:
        PowAction (Process *p, const string &n, AbstractProperty* base, AbstractProperty* exp, AbstractProperty* result) :
          BinaryOperatorAction (p, n, base, exp, result) {}
        virtual ~PowAction () {}
        void impl_activate () {
          _result->set_value (pow (((DoubleProperty*) _left)->get_value (), ((DoubleProperty*) _right)->get_value ()), true);
        }
         void impl_deactivate () {}
       };
     public:
       Pow (Process *p, const string &n, double base, double exponent);
       virtual ~Pow () {}
       void serialize (const string& type) override;
   };

  class Sqrt : public UnaryOperator
  {
    private:
      class SqrtAction : public UnaryOperatorAction
      {
      public:
        SqrtAction (Process* parent, const string &name, AbstractProperty* input, AbstractProperty* output) :
          UnaryOperatorAction (parent, name, input, output) {}
        virtual ~SqrtAction () {}
        void impl_activate ()
        {
          _output->set_value (sqrt (((DoubleProperty*)_input)->get_value ()), true);
        }
        void impl_deactivate () {}
      };
    public:
      Sqrt (Process *p, const string &name, double i_val);
      virtual ~Sqrt () {}
      void serialize (const string& type) override;
  };

  class Abs : public UnaryOperator
  {
    private:
      class AbsAction : public UnaryOperatorAction
      {
      public:
        AbsAction (Process* parent, const string &name, AbstractProperty* input, AbstractProperty* output) :
          UnaryOperatorAction (parent, name, input, output) {}
        virtual ~AbsAction () {}
        void impl_activate ()
        {
          _output->set_value (abs (((DoubleProperty*)_input)->get_value ()), true);
        }
        void impl_deactivate () {}
      };
    public:
      Abs (Process *p, const string &name, double i_val);
      virtual ~Abs () {}
      void serialize (const string& type) override;
  };

  class Min : public BinaryOperator
  {
    private:
      class MinAction : public BinaryOperatorAction
      {
      public:
        MinAction (Process *p, const string &n, AbstractProperty* min, AbstractProperty* input, AbstractProperty* result) :
          BinaryOperatorAction (p, n, min, input, result) {}
        virtual ~MinAction () {}
        void impl_activate () {
          double min = ((DoubleProperty*) _left)->get_value ();
          double input = ((DoubleProperty*) _right)->get_value ();
          _result->set_value ((min <= input ? input : min), true);
        }
         void impl_deactivate () {}
       };
     public:
       Min (Process *p, const string &n, double min, double init_val);
       virtual ~Min () {}
       void serialize (const string& type) override;
   };

  class Max : public BinaryOperator
  {
    private:
      class MaxAction : public BinaryOperatorAction
      {
      public:
        MaxAction (Process *p, const string &n, AbstractProperty* max, AbstractProperty* input, AbstractProperty* result) :
          BinaryOperatorAction (p, n, max, input, result) {}
        virtual ~MaxAction () {}
        void impl_activate () {
          double max = ((DoubleProperty*) _left)->get_value ();
          double input = ((DoubleProperty*) _right)->get_value ();
          _result->set_value ((max >= input ? input : max), true);
        }
         void impl_deactivate () {}
       };
     public:
       Max (Process *p, const string &n, double max, double init_val);
       virtual ~Max () {}
       void serialize (const string& type) override;
   };

  class BoundedValue : public Process
  {
  private:
    class BoundedValueAction : public Action
    {
    public:
      BoundedValueAction (Process *p, const string &n, AbstractProperty* min,
                          AbstractProperty* max, AbstractProperty* input,
                          AbstractProperty* result) : Action (p, n), _min (min), _max (max), _input (input), _result (result) {}
    virtual ~BoundedValueAction () {}
      void impl_activate () override {
        double max = ((DoubleProperty*) _max)->get_value ();
        double min = ((DoubleProperty*) _min)->get_value ();
        double input = ((DoubleProperty*) _input)->get_value ();
        double r = input < min ? min : (input > max ? max : input);
        _result->set_value (r, true);
      }
      void impl_deactivate () override {}
    private:
      AbstractProperty *_min;
      AbstractProperty *_max;
      AbstractProperty *_input;
      AbstractProperty *_result;
    };
  public:
    BoundedValue (Process *p, const string &n, double min, double max, double init_val);
    void impl_activate () override { _c_min->enable(); _c_max->enable (); _c_input->enable (); _action->activate (); };
    void impl_deactivate () override { _c_min->disable (); _c_max->disable (); _c_input->disable (); _action->deactivate ();};
    virtual ~BoundedValue ();
    void serialize (const string& type) override;
  protected:
    void set_parent (Process* p) override;
    AbstractProperty  *_min, *_max, *_input, *_result;
    Coupling *_c_min, *_c_max, *_c_input;
    Process *_action;
  };
#endif

}
