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
      CosineAction (Process *p, const string &n, AbstractProperty* input, AbstractProperty* output) :
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (); }
      virtual ~CosineAction () {}
      void activate () {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _output->set_value (cos (((DoubleProperty*) _input)->get_value ()), true);
      }
      void deactivate () {}
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
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (); }
      virtual ~SineAction () {}
      void activate () {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _output->set_value (sin (((DoubleProperty*) _input)->get_value ()), true);
      }
      void deactivate () {}
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
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (); }
      virtual ~TangentAction () {}
      void activate () {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _output->set_value (tan (((DoubleProperty*) _input)->get_value ()), true);
      }
      void deactivate () {}
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
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (); }
      virtual ~ArcCosineAction () {}
      void activate () {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _output->set_value (acos (((DoubleProperty*) _input)->get_value ()), true);
      }
      void deactivate () {}
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
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (); }
      virtual ~ArcSineAction () {}
      void activate () {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _output->set_value (asin (((DoubleProperty*) _input)->get_value ()), true);
      }
      void deactivate () {}
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
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (); }
      virtual ~ArcTangentAction () {}
      void activate () {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _output->set_value (atan (((DoubleProperty*) _input)->get_value ()), true);
      }
      void deactivate () {}
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
          BinaryOperatorAction (p, n, y, x, result) { Process::finalize_construction (); }
        virtual ~ArcTangent2Action () {}
        void activate () {
          //if (_parent->get_state () > activated)
          if (!_parent->somehow_activating ())
            return;
          _result->set_value (atan2 (((DoubleProperty*) _left)->get_value (), ((DoubleProperty*) _right)->get_value ()), true);
        }
        void deactivate () {}
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
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (); }
      virtual ~HyperbolicCosineAction () {}
      void activate () {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _output->set_value (cosh (((DoubleProperty*) _input)->get_value ()), true);
      }
      void deactivate () {}
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
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (); }
      virtual ~HyperbolicSineAction () {}
      void activate () {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _output->set_value (sinh (((DoubleProperty*) _input)->get_value ()), true);
      }
      void deactivate () {}
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
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (); }
      virtual ~HyperbolicTangentAction () {}
      void activate () {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _output->set_value (tanh (((DoubleProperty*) _input)->get_value ()), true);
      }
      void deactivate () {}
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
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (); }
      virtual ~HyperbolicArcCosineAction () {}
      void activate () {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _output->set_value (acosh (((DoubleProperty*) _input)->get_value ()), true);
      }
      void deactivate () {}
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
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (); }
      virtual ~HyperbolicArcSineAction () {}
      void activate () {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _output->set_value (asinh (((DoubleProperty*) _input)->get_value ()), true);
      }
      void deactivate () {}
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
        UnaryOperatorAction (p, n, input, output) { Process::finalize_construction (); }
      virtual ~HyperbolicArcTangentAction () {}
      void activate () {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _output->set_value (atanh (((DoubleProperty*) _input)->get_value ()), true);
      }
      void deactivate () {}
    };
  public:
    HyperbolicArcTangent (Process *p, const string &n, double in_val);
    virtual ~HyperbolicArcTangent () {}
    void serialize (const string& type) override;
  };
}
