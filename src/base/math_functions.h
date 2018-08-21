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
      ExpAction (Process* parent, const string &name, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
        UnaryOperatorAction (parent, name, input, output) { Process::finalize (); }
      virtual ~ExpAction () {}
      void activate ()
      {
        if (_parent->get_state () > activated)
          return;
        _output.get()->set_value (exp (((DoubleProperty*)_input.get())->get_value ()), true);
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
        LogAction (Process* parent, const string &name, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
          UnaryOperatorAction (parent, name, input, output) { Process::finalize (); }
        virtual ~LogAction () {}
        void activate ()
        {
          if (_parent->get_state () > activated)
            return;
          _output.get()->set_value (log (((DoubleProperty*)_input.get())->get_value ()), true);
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
        Log10Action (Process* parent, const string &name, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
          UnaryOperatorAction (parent, name, input, output) { Process::finalize (); }
        virtual ~Log10Action () {}
        void activate ()
        {
          if (_parent->get_state () > activated)
            return;
          _output.get()->set_value (log10 (((DoubleProperty*)_input.get())->get_value ()), true);
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
        PowAction (Process *p, const string &n, shared_ptr<AbstractProperty> base, shared_ptr<AbstractProperty> exp, shared_ptr<AbstractProperty> result) :
          BinaryOperatorAction (p, n, base, exp, result) { Process::finalize (); }
        virtual ~PowAction () {}
        void activate () {
          if (_parent->get_state () > activated)
            return;
          _result.get()->set_value (pow (((DoubleProperty*) _left.get())->get_value (), ((DoubleProperty*) _right.get())->get_value ()), true);
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
        SqrtAction (Process* parent, const string &name, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
          UnaryOperatorAction (parent, name, input, output) { Process::finalize (); }
        virtual ~SqrtAction () {}
        void activate ()
        {
          if (_parent->get_state () > activated)
            return;
          _output.get()->set_value (sqrt (((DoubleProperty*)_input.get())->get_value ()), true);
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
        AbsAction (Process* parent, const string &name, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
          UnaryOperatorAction (parent, name, input, output) { Process::finalize (); }
        virtual ~AbsAction () {}
        void activate ()
        {
          if (_parent->get_state () > activated)
            return;
          _output.get()->set_value (abs (((DoubleProperty*)_input.get())->get_value ()), true);
        }
        void deactivate () {}
      };
    public:
      Abs (Process *p, const string &name, double i_val);
      virtual ~Abs () {}
      void serialize (const string& type) override;
  };
}
