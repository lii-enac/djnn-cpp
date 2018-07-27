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

#include "operators.h"
#include <iostream>
#include <cmath>

namespace djnn
{
  using namespace std;

  class Cosine : public UnaryOperator
  {
  private:
    class CosineAction : public UnaryOperatorAction
    {
    public:
      CosineAction (Process *p, const string &n, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize (); }
      virtual ~CosineAction () {}
      void activate () {
        if (_parent->get_state () > activated)
                  return;
        _output.get()->set_value (cos (((DoubleProperty*) _input.get())->get_value ()), true);
      }
      void deactivate () {}
    };
  public:
    Cosine (Process *p, const string &n, double in_val);
    virtual ~Cosine () {}
  };

  class Sine : public UnaryOperator
  {
  private:
    class SineAction : public UnaryOperatorAction
    {
    public:
      SineAction (Process *p, const string &n, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize (); }
      virtual ~SineAction () {}
      void activate () {
        if (_parent->get_state () > activated)
                  return;
        _output.get()->set_value (sin (((DoubleProperty*) _input.get())->get_value ()), true);
      }
      void deactivate () {}
    };
  public:
    Sine (Process *p, const string &n, double in_val);
    virtual ~Sine () {}
  };

  class Tangent : public UnaryOperator
  {
  private:
    class TangentAction : public UnaryOperatorAction
    {
    public:
      TangentAction (Process *p, const string &n, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize (); }
      virtual ~TangentAction () {}
      void activate () {
        if (_parent->get_state () > activated)
                  return;
        _output.get()->set_value (tan (((DoubleProperty*) _input.get())->get_value ()), true);
      }
      void deactivate () {}
    };
  public:
    Tangent (Process *p, const string &n, double in_val);
    virtual ~Tangent () {}
  };

  class ArcCosine : public UnaryOperator
  {
  private:
    class ArcCosineAction : public UnaryOperatorAction
    {
    public:
      ArcCosineAction (Process *p, const string &n, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize (); }
      virtual ~ArcCosineAction () {}
      void activate () {
        if (_parent->get_state () > activated)
                  return;
        _output.get()->set_value (acos (((DoubleProperty*) _input.get())->get_value ()), true);
      }
      void deactivate () {}
    };
  public:
    ArcCosine (Process *p, const string &n, double in_val);
    virtual ~ArcCosine () {}
  };

  class ArcSine : public UnaryOperator
  {
  private:
    class ArcSineAction : public UnaryOperatorAction
    {
    public:
      ArcSineAction (Process *p, const string &n, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize (); }
      virtual ~ArcSineAction () {}
      void activate () {
        if (_parent->get_state () > activated)
                  return;
        _output.get()->set_value (asin (((DoubleProperty*) _input.get())->get_value ()), true);
      }
      void deactivate () {}
    };
  public:
    ArcSine (Process *p, const string &n, double in_val);
    virtual ~ArcSine () {}
  };

  class ArcTangent : public UnaryOperator
  {
  private:
    class ArcTangentAction : public UnaryOperatorAction
    {
    public:
      ArcTangentAction (Process *p, const string &n, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize (); }
      virtual ~ArcTangentAction () {}
      void activate () {
        if (_parent->get_state () > activated)
                  return;
        _output.get()->set_value (atan (((DoubleProperty*) _input.get())->get_value ()), true);
      }
      void deactivate () {}
    };
  public:
    ArcTangent (Process *p, const string &n, double in_val);
    virtual ~ArcTangent () {}
  };

  class ArcTangent2 : public BinaryOperator
    {
    private:
      class ArcTangent2Action : public BinaryOperatorAction
      {
      public:
        ArcTangent2Action (Process *p, const string &n, shared_ptr<AbstractProperty> y, shared_ptr<AbstractProperty> x, shared_ptr<AbstractProperty> result) :
          BinaryOperatorAction (p, n, y, x, result) { Process::finalize (); }
        virtual ~ArcTangent2Action () {}
        void activate () {
          if (_parent->get_state () > activated)
                    return;
          _result.get()->set_value (atan2 (((DoubleProperty*) _left.get())->get_value (), ((DoubleProperty*) _right.get())->get_value ()), true);
        }
        void deactivate () {}
      };
    public:
      ArcTangent2 (Process *p, const string &n, double y_val, double x_val);
      virtual ~ArcTangent2 () {}
    };

  class HyperbolicCosine : public UnaryOperator
  {
  private:
    class HyperbolicCosineAction : public UnaryOperatorAction
    {
    public:
      HyperbolicCosineAction (Process *p, const string &n, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize (); }
      virtual ~HyperbolicCosineAction () {}
      void activate () {
        if (_parent->get_state () > activated)
                  return;
        _output.get()->set_value (cosh (((DoubleProperty*) _input.get())->get_value ()), true);
      }
      void deactivate () {}
    };
  public:
    HyperbolicCosine (Process *p, const string &n, double in_val);
    virtual ~HyperbolicCosine () {}
  };

  class HyperbolicSine : public UnaryOperator
  {
  private:
    class HyperbolicSineAction : public UnaryOperatorAction
    {
    public:
      HyperbolicSineAction (Process *p, const string &n, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize (); }
      virtual ~HyperbolicSineAction () {}
      void activate () {
        if (_parent->get_state () > activated)
                  return;
        _output.get()->set_value (sinh (((DoubleProperty*) _input.get())->get_value ()), true);
      }
      void deactivate () {}
    };
  public:
    HyperbolicSine (Process *p, const string &n, double in_val);
    virtual ~HyperbolicSine () {}
  };

  class HyperbolicTangent : public UnaryOperator
  {
  private:
    class HyperbolicTangentAction : public UnaryOperatorAction
    {
    public:
      HyperbolicTangentAction (Process *p, const string &n, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize (); }
      virtual ~HyperbolicTangentAction () {}
      void activate () {
        if (_parent->get_state () > activated)
                  return;
        _output.get()->set_value (tanh (((DoubleProperty*) _input.get())->get_value ()), true);
      }
      void deactivate () {}
    };
  public:
    HyperbolicTangent (Process *p, const string &n, double in_val);
    virtual ~HyperbolicTangent () {}
  };

  class HyperbolicArcCosine : public UnaryOperator
  {
  private:
    class HyperbolicArcCosineAction : public UnaryOperatorAction
    {
    public:
      HyperbolicArcCosineAction (Process *p, const string &n, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize (); }
      virtual ~HyperbolicArcCosineAction () {}
      void activate () {
        if (_parent->get_state () > activated)
                  return;
        _output.get()->set_value (acosh (((DoubleProperty*) _input.get())->get_value ()), true);
      }
      void deactivate () {}
    };
  public:
    HyperbolicArcCosine (Process *p, const string &n, double in_val);
    virtual ~HyperbolicArcCosine () {}
  };

  class HyperbolicArcSine : public UnaryOperator
  {
  private:
    class HyperbolicArcSineAction : public UnaryOperatorAction
    {
    public:
      HyperbolicArcSineAction (Process *p, const string &n, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize (); }
      virtual ~HyperbolicArcSineAction () {}
      void activate () {
        if (_parent->get_state () > activated)
                  return;
        _output.get()->set_value (asinh (((DoubleProperty*) _input.get())->get_value ()), true);
      }
      void deactivate () {}
    };
  public:
    HyperbolicArcSine (Process *p, const string &n, double in_val);
    virtual ~HyperbolicArcSine () {}
  };

  class HyperbolicArcTangent : public UnaryOperator
  {
  private:
    class HyperbolicArcTangentAction : public UnaryOperatorAction
    {
    public:
      HyperbolicArcTangentAction (Process *p, const string &n, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize (); }
      virtual ~HyperbolicArcTangentAction () {}
      void activate () {
        if (_parent->get_state () > activated)
                  return;
        _output.get()->set_value (atanh (((DoubleProperty*) _input.get())->get_value ()), true);
      }
      void deactivate () {}
    };
  public:
    HyperbolicArcTangent (Process *p, const string &n, double in_val);
    virtual ~HyperbolicArcTangent () {}
  };
}
