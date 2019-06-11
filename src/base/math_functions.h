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

  class Exp : public UnaryOperator
  {
  private:
    class ExpAction : public UnaryOperatorAction
    {
    public:
      ExpAction (Process* parent, const string &name, AbstractProperty* input, AbstractProperty* output) :
        UnaryOperatorAction (parent, name, input, output) { Process::finalize (); }
      virtual ~ExpAction () {}
      void activate ()
      {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _output->set_value (exp (((DoubleProperty*)_input)->get_value ()), true);
      }
      void deactivate () {}
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
          UnaryOperatorAction (parent, name, input, output) { Process::finalize (); }
        virtual ~LogAction () {}
        void activate ()
        {
          //if (_parent->get_state () > activated)
          if (!_parent->somehow_activating ())
            return;
          _output->set_value (log (((DoubleProperty*)_input)->get_value ()), true);
        }
        void deactivate () {}
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
          UnaryOperatorAction (parent, name, input, output) { Process::finalize (); }
        virtual ~Log10Action () {}
        void activate ()
        {
          //if (_parent->get_state () > activated)
          if (!_parent->somehow_activating ())
            return;
          _output->set_value (log10 (((DoubleProperty*)_input)->get_value ()), true);
        }
        void deactivate () {}
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
          BinaryOperatorAction (p, n, base, exp, result) { Process::finalize (); }
        virtual ~PowAction () {}
        void activate () {
          //if (_parent->get_state () > activated)
          if (!_parent->somehow_activating ())
            return;
          _result->set_value (pow (((DoubleProperty*) _left)->get_value (), ((DoubleProperty*) _right)->get_value ()), true);
        }
         void deactivate () {}
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
          UnaryOperatorAction (parent, name, input, output) { Process::finalize (); }
        virtual ~SqrtAction () {}
        void activate ()
        {
          //if (_parent->get_state () > activated)
          if (!_parent->somehow_activating ())
            return;
          _output->set_value (sqrt (((DoubleProperty*)_input)->get_value ()), true);
        }
        void deactivate () {}
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
          UnaryOperatorAction (parent, name, input, output) { Process::finalize (); }
        virtual ~AbsAction () {}
        void activate ()
        {
          //if (_parent->get_state () > activated)
          if (!_parent->somehow_activating ())
            return;
          _output->set_value (abs (((DoubleProperty*)_input)->get_value ()), true);
        }
        void deactivate () {}
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
          BinaryOperatorAction (p, n, min, input, result) { Process::finalize (); }
        virtual ~MinAction () {}
        void activate () {
          //if (_parent->get_state () > activated)
          if (!_parent->somehow_activating ())
            return;
          double min = ((DoubleProperty*) _left)->get_value ();
          double input = ((DoubleProperty*) _right)->get_value ();
          _result->set_value ((min <= input ? input : min), true);
        }
         void deactivate () {}
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
          BinaryOperatorAction (p, n, max, input, result) { Process::finalize (); }
        virtual ~MaxAction () {}
        void activate () {
          //if (_parent->get_state () > activated)
          if (!_parent->somehow_activating ())
            return;
          double max = ((DoubleProperty*) _left)->get_value ();
          double input = ((DoubleProperty*) _right)->get_value ();
          _result->set_value ((max >= input ? input : max), true);
        }
         void deactivate () {}
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
      void activate () override {
        double max = ((DoubleProperty*) _max)->get_value ();
        double min = ((DoubleProperty*) _min)->get_value ();
        double input = ((DoubleProperty*) _input)->get_value ();
        double r = input < min ? min : (input > max ? max : input);
        _result->set_value (r, true);
      }
      void deactivate () override {}
    private:
      AbstractProperty *_min;
      AbstractProperty *_max;
      AbstractProperty *_input;
      AbstractProperty *_result;
    };
  public:
    BoundedValue (Process *p, const string &n, double min, double max, double init_val);
    void activate () override { _c_min->enable(); _c_max->enable (); _c_input->enable (); _action->activation (); };
    void deactivate () override { _c_min->disable (); _c_max->disable (); _c_input->disable (); _action->deactivation ();};
    virtual ~BoundedValue ();
    void serialize (const string& type) override;
  protected:
    AbstractProperty  *_min, *_max, *_input, *_result;
    Coupling *_c_min, *_c_max, *_c_input;
    Process *_action;
  };
}
