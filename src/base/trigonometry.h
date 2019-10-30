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

namespace djnn
{
  using namespace std;

#if NEW_OP
  template <typename T> struct my_cos { T operator() (T t1) { return ::cos(t1);} };
  template <> std::string serialize_info<my_cos<double>>::name;
  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  my_cos<double>, double> CosAction;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  my_cos<double>, double> Cosine;

  template <typename T> struct my_sin { T operator() (T t1) { return ::sin(t1);} };
  template <> std::string serialize_info<my_sin<double>>::name;
  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  my_sin<double>, double> SinAction;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  my_sin<double>, double> Sine;

  template <typename T> struct my_tan { T operator() (T t1) { return ::tan(t1);} };
  template <> std::string serialize_info<my_tan<double>>::name;
  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  my_tan<double>, double> TanAction;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  my_tan<double>, double> Tangent;

  template <typename T> struct my_acos { T operator() (T t1) { return ::acos(t1);} };
  template <> std::string serialize_info<my_acos<double>>::name;
  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  my_acos<double>, double> ArcCosAction;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  my_acos<double>, double> ArcCosine;

  template <typename T> struct my_asin { T operator() (T t1) { return ::asin(t1);} };
  template <> std::string serialize_info<my_asin<double>>::name;
  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  my_asin<double>, double> ArcSinAction;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  my_asin<double>, double> ArcSine;

  template <typename T> struct my_atan { T operator() (T t1) { return ::atan(t1);} };
  template <> std::string serialize_info<my_atan<double>>::name;
  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  my_atan<double>, double> ArcTanAction;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  my_atan<double>, double> ArcTangent;

  template <typename T> struct my_atan2 { T operator() (T t1, T t2) { return ::atan2(t1, t2);} };
  template <> std::string serialize_info<my_atan2<double>>::name;
  typedef NewBinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, my_atan2<double>, double, double> ArcTan2Action;
  typedef NewBinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, my_atan2<double>, double, double> ArcTangent2;

  template <typename T> struct my_cosh { T operator() (T t1) { return ::cosh(t1);} };
  template <> std::string serialize_info<my_cosh<double>>::name;
  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  my_cosh<double>, double> HyperbolicCosAction;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  my_cosh<double>, double> HyperbolicCosine;

  template <typename T> struct my_sinh { T operator() (T t1) { return ::asin(t1);} };
  template <> std::string serialize_info<my_sinh<double>>::name;
  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  my_sinh<double>, double> HyperbolicSinAction;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  my_sinh<double>, double> HyperbolicSine;

  template <typename T> struct my_tanh { T operator() (T t1) { return ::atan(t1);} };
  template <> std::string serialize_info<my_tanh<double>>::name;
  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  my_tanh<double>, double> HyperbolicTanAction;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  my_tanh<double>, double> HyperbolicTangent;

  template <typename T> struct my_acosh { T operator() (T t1) { return ::acosh(t1);} };
  template <> std::string serialize_info<my_acosh<double>>::name;
  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  my_acosh<double>, double> HyperbolicArcCosAction;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  my_acosh<double>, double> HyperbolicArcCosine;

  template <typename T> struct my_asinh { T operator() (T t1) { return ::asin(t1);} };
  template <> std::string serialize_info<my_asinh<double>>::name;
  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  my_asinh<double>, double> HyperbolicArcSinAction;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  my_asinh<double>, double> HyperbolicArcSine;

  template <typename T> struct my_atanh { T operator() (T t1) { return ::atan(t1);} };
  template <> std::string serialize_info<my_atanh<double>>::name;
  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  my_atanh<double>, double> HyperbolicArcTanAction;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  my_atanh<double>, double> HyperbolicArcTangent;

#else

  class Cosine : public UnaryOperator
  {
  private:
    class CosineAction : public UnaryOperatorAction
    {
    public:
      CosineAction (Process *p, const string &n, AbstractProperty* input, AbstractProperty* output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (p); }
      virtual ~CosineAction () {}
      void impl_activate () {
        _output->set_value (cos (((DoubleProperty*) _input)->get_value ()), true);
      }
      void impl_deactivate () {}
    };
  public:
    Cosine (Process *p, const string &n, double in_val);
    virtual ~Cosine () {}
    void serialize (const string& type) override;
  };

  class Sine : public UnaryOperator
  {
  private:
    class SineAction : public UnaryOperatorAction
    {
    public:
      SineAction (Process *p, const string &n, AbstractProperty* input, AbstractProperty* output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (p); }
      virtual ~SineAction () {}
      void impl_activate () {
        _output->set_value (sin (((DoubleProperty*) _input)->get_value ()), true);
      }
      void impl_deactivate () {}
    };
  public:
    Sine (Process *p, const string &n, double in_val);
    virtual ~Sine () {}
    void serialize (const string& type) override;
  };

  class Tangent : public UnaryOperator
  {
  private:
    class TangentAction : public UnaryOperatorAction
    {
    public:
      TangentAction (Process *p, const string &n, AbstractProperty* input, AbstractProperty* output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (p); }
      virtual ~TangentAction () {}
      void impl_activate () {
        _output->set_value (tan (((DoubleProperty*) _input)->get_value ()), true);
      }
      void impl_deactivate () {}
    };
  public:
    Tangent (Process *p, const string &n, double in_val);
    virtual ~Tangent () {}
    void serialize (const string& type) override;
  };

  class ArcCosine : public UnaryOperator
  {
  private:
    class ArcCosineAction : public UnaryOperatorAction
    {
    public:
      ArcCosineAction (Process *p, const string &n, AbstractProperty* input, AbstractProperty* output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (p); }
      virtual ~ArcCosineAction () {}
      void impl_activate () {
        _output->set_value (acos (((DoubleProperty*) _input)->get_value ()), true);
      }
      void impl_deactivate () {}
    };
  public:
    ArcCosine (Process *p, const string &n, double in_val);
    virtual ~ArcCosine () {}
    void serialize (const string& type) override;
  };

  class ArcSine : public UnaryOperator
  {
  private:
    class ArcSineAction : public UnaryOperatorAction
    {
    public:
      ArcSineAction (Process *p, const string &n, AbstractProperty* input, AbstractProperty* output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (p); }
      virtual ~ArcSineAction () {}
      void impl_activate () {
        _output->set_value (asin (((DoubleProperty*) _input)->get_value ()), true);
      }
      void impl_deactivate () {}
    };
  public:
    ArcSine (Process *p, const string &n, double in_val);
    virtual ~ArcSine () {}
    void serialize (const string& type) override;
  };

  class ArcTangent : public UnaryOperator
  {
  private:
    class ArcTangentAction : public UnaryOperatorAction
    {
    public:
      ArcTangentAction (Process *p, const string &n, AbstractProperty* input, AbstractProperty* output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (p); }
      virtual ~ArcTangentAction () {}
      void impl_activate () {
        _output->set_value (atan (((DoubleProperty*) _input)->get_value ()), true);
      }
      void impl_deactivate () {}
    };
  public:
    ArcTangent (Process *p, const string &n, double in_val);
    virtual ~ArcTangent () {}
    void serialize (const string& type) override;
  };

  class ArcTangent2 : public BinaryOperator
    {
    private:
      class ArcTangent2Action : public BinaryOperatorAction
      {
      public:
        ArcTangent2Action (Process *p, const string &n, AbstractProperty* y, AbstractProperty* x, AbstractProperty* result) :
          BinaryOperatorAction (p, n, y, x, result) { Process::finalize_construction (p); }
        virtual ~ArcTangent2Action () {}
        void impl_activate () {
          _result->set_value (atan2 (((DoubleProperty*) _left)->get_value (), ((DoubleProperty*) _right)->get_value ()), true);
        }
        void impl_deactivate () {}
      };
    public:
      ArcTangent2 (Process *p, const string &n, double y_val, double x_val);
      virtual ~ArcTangent2 () {}
      void serialize (const string& type) override;
    };

  class HyperbolicCosine : public UnaryOperator
  {
  private:
    class HyperbolicCosineAction : public UnaryOperatorAction
    {
    public:
      HyperbolicCosineAction (Process *p, const string &n, AbstractProperty* input, AbstractProperty* output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (p); }
      virtual ~HyperbolicCosineAction () {}
      void impl_activate () {
        _output->set_value (cosh (((DoubleProperty*) _input)->get_value ()), true);
      }
      void impl_deactivate () {}
    };
  public:
    HyperbolicCosine (Process *p, const string &n, double in_val);
    virtual ~HyperbolicCosine () {}
    void serialize (const string& type) override;
  };

  class HyperbolicSine : public UnaryOperator
  {
  private:
    class HyperbolicSineAction : public UnaryOperatorAction
    {
    public:
      HyperbolicSineAction (Process *p, const string &n, AbstractProperty* input, AbstractProperty* output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (p); }
      virtual ~HyperbolicSineAction () {}
      void impl_activate () {
        _output->set_value (sinh (((DoubleProperty*) _input)->get_value ()), true);
      }
      void impl_deactivate () {}
    };
  public:
    HyperbolicSine (Process *p, const string &n, double in_val);
    virtual ~HyperbolicSine () {}
    void serialize (const string& type) override;
  };

  class HyperbolicTangent : public UnaryOperator
  {
  private:
    class HyperbolicTangentAction : public UnaryOperatorAction
    {
    public:
      HyperbolicTangentAction (Process *p, const string &n, AbstractProperty* input, AbstractProperty* output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (p); }
      virtual ~HyperbolicTangentAction () {}
      void impl_activate () {
        _output->set_value (tanh (((DoubleProperty*) _input)->get_value ()), true);
      }
      void impl_deactivate () {}
    };
  public:
    HyperbolicTangent (Process *p, const string &n, double in_val);
    virtual ~HyperbolicTangent () {}
    void serialize (const string& type) override;
  };

  class HyperbolicArcCosine : public UnaryOperator
  {
  private:
    class HyperbolicArcCosineAction : public UnaryOperatorAction
    {
    public:
      HyperbolicArcCosineAction (Process *p, const string &n, AbstractProperty* input, AbstractProperty* output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (p); }
      virtual ~HyperbolicArcCosineAction () {}
      void impl_activate () {
        _output->set_value (acosh (((DoubleProperty*) _input)->get_value ()), true);
      }
      void impl_deactivate () {}
    };
  public:
    HyperbolicArcCosine (Process *p, const string &n, double in_val);
    virtual ~HyperbolicArcCosine () {}
    void serialize (const string& type) override;
  };

  class HyperbolicArcSine : public UnaryOperator
  {
  private:
    class HyperbolicArcSineAction : public UnaryOperatorAction
    {
    public:
      HyperbolicArcSineAction (Process *p, const string &n, AbstractProperty* input, AbstractProperty* output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (p); }
      virtual ~HyperbolicArcSineAction () {}
      void impl_activate () {
        _output->set_value (asinh (((DoubleProperty*) _input)->get_value ()), true);
      }
      void impl_deactivate () {}
    };
  public:
    HyperbolicArcSine (Process *p, const string &n, double in_val);
    virtual ~HyperbolicArcSine () {}
    void serialize (const string& type) override;
  };

  class HyperbolicArcTangent : public UnaryOperator
  {
  private:
    class HyperbolicArcTangentAction : public UnaryOperatorAction
    {
    public:
      HyperbolicArcTangentAction (Process *p, const string &n, AbstractProperty* input, AbstractProperty* output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (p); }
      virtual ~HyperbolicArcTangentAction () {}
      void impl_activate () {
        _output->set_value (atanh (((DoubleProperty*) _input)->get_value ()), true);
      }
      void impl_deactivate () {}
    };
  public:
    HyperbolicArcTangent (Process *p, const string &n, double in_val);
    virtual ~HyperbolicArcTangent () {}
    void serialize (const string& type) override;
  };
#endif
}
