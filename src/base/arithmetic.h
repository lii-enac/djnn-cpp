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

namespace djnn
{
  using namespace std;

  class Adder : public BinaryOperator
  {
  private:
    class AdderAction : public BinaryOperatorAction
    {
    public:
      AdderAction (Process* parent, const string &name, shared_ptr<AbstractProperty> left, shared_ptr<AbstractProperty> right,
                   shared_ptr<AbstractProperty> result) :
		  BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
      virtual ~AdderAction () {}
      void activate ()
      {
        if (_parent->get_state () > activated)
          return;
        _result.get()->set_value (((DoubleProperty*)_left.get())->get_value () + ((DoubleProperty*)_right.get())->get_value (), true);
      }
      void deactivate () {}
    };
  public:
    Adder (Process *p, const string &name, double l_val, double r_val);
    virtual ~Adder () {}
  };

  class Subtractor : public BinaryOperator
  {
  private:
    class SubtractorAction : public BinaryOperatorAction
    {
    public:
      SubtractorAction (Process* parent, const string &name, shared_ptr<AbstractProperty> left, shared_ptr<AbstractProperty> right,
                        shared_ptr<AbstractProperty> result) :
	  BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
      virtual ~SubtractorAction () {}
      void activate ()
      {
        if (_parent->get_state () > activated)
          return;
        _result.get()->set_value (((DoubleProperty*)_left.get())->get_value () - ((DoubleProperty*)_right.get())->get_value (), true);
      }
      void deactivate () {}
    };
  public:
    Subtractor (Process *p, const string &name, double l_val, double r_val);
    virtual
    ~Subtractor () {}
  };

  class Multiplier : public BinaryOperator
  {
    private:
      class MultiplierAction : public BinaryOperatorAction
      {
      public:
        MultiplierAction (Process* parent, const string &name, shared_ptr<AbstractProperty> left, shared_ptr<AbstractProperty> right,
                          shared_ptr<AbstractProperty> result) :
  	    BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
        virtual ~MultiplierAction () {}
        void activate ()
        {
          if (_parent->get_state () > activated)
            return;
          _result.get()->set_value (((DoubleProperty*)_left.get())->get_value () * ((DoubleProperty*)_right.get())->get_value (), true);
        }
        void deactivate () {}
      };
    public:
      Multiplier (Process *p, const string &name, double l_val, double r_val);
      virtual ~Multiplier () {}
  };

  class Divider : public BinaryOperator
  {
    private:
      class DividerAction : public BinaryOperatorAction
      {
      public:
        DividerAction (Process* parent, const string &name, shared_ptr<AbstractProperty> left, shared_ptr<AbstractProperty> right,
                       shared_ptr<AbstractProperty> result) :
        BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
        virtual ~DividerAction () {}
        void activate ()
        {
     	  if (_parent->get_state () > activated)
    	    return;
     	  double r = ((DoubleProperty*)_right.get())->get_value ();
     	  if (r == 0) {
     	    return;
     	   }
    	  _result.get()->set_value (((DoubleProperty*)_left.get())->get_value () /r, true);
        }
        void deactivate () {}
      };
    public:
      Divider (Process *p, const string &name, double l_val, double r_val);
      virtual ~Divider () {}
  };

  class Modulo : public BinaryOperator
  {
    private:
      class ModuloAction : public BinaryOperatorAction
      {
      public:
        ModuloAction (Process* parent, string name, shared_ptr<AbstractProperty> left, shared_ptr<AbstractProperty> right,
                      shared_ptr<AbstractProperty> result) :
        BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
        virtual ~ModuloAction () {}
        void activate ()
        {
     	  if (_parent->get_state () > activated)
    	    return;
     	  int l = ((IntProperty*) _left.get())->get_value ();
     	  int r = ((IntProperty*) _right.get())->get_value ();
     	  if (r == 0) {
     	    cerr << "Warning: modulo per 0\n";
     	    return;
     	   }
    	  _result.get()->set_value (l % r, true);
        }
        void deactivate () {}
      };
    public:
      Modulo (Process *p, const string &name, int l_val, int r_val);
      virtual ~Modulo () {}
  };

  class AscendingComparator : public BinaryOperator
    {
    private:
      class AscendingComparatorAction : public BinaryOperatorAction
      {
      public:
        AscendingComparatorAction (Process* parent, const string &name, shared_ptr<AbstractProperty> left, shared_ptr<AbstractProperty> right,
                                   shared_ptr<AbstractProperty> result) :
                                     BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
        virtual ~AscendingComparatorAction () {}
        void activate ()
        {
          if (_parent->get_state () > activated)
            return;
          _result.get()->set_value (((DoubleProperty*)_left.get())->get_value () <= ((DoubleProperty*)_right.get())->get_value (), true);
        }
        void deactivate () {}
      };
    public:
      AscendingComparator (Process *p, const string &name, double l_val, double r_val);
      virtual ~AscendingComparator () {}
    };

    class StrictAscendingComparator : public BinaryOperator
      {
      private:
        class StrictAscendingComparatorAction : public BinaryOperatorAction
        {
        public:
          StrictAscendingComparatorAction (Process* parent, const string &name, shared_ptr<AbstractProperty> left, shared_ptr<AbstractProperty> right,
                                           shared_ptr<AbstractProperty> result) :
                                             BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
          virtual ~StrictAscendingComparatorAction () {}
          void activate ()
          {
            if (_parent->get_state () > activated)
              return;
            _result.get()->set_value (((DoubleProperty*)_left.get())->get_value () < ((DoubleProperty*)_right.get())->get_value (), true);
          }
          void deactivate () {}
        };
      public:
        StrictAscendingComparator (Process *p, const string &name, double l_val, double r_val);
        virtual ~StrictAscendingComparator () {}
      };

    class EqualityComparator : public BinaryOperator
      {
      private:
        class EqualityComparatorAction : public BinaryOperatorAction
        {
        public:
          EqualityComparatorAction (Process* parent, const string &name, shared_ptr<AbstractProperty> left, shared_ptr<AbstractProperty> right,
                                    shared_ptr<AbstractProperty> result) :
                                      BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
          virtual ~EqualityComparatorAction () {}
          void activate ()
          {
            if (_parent->get_state () > activated)
              return;
            _result.get()->set_value (((DoubleProperty*)_left.get())->get_value () == ((DoubleProperty*)_right.get())->get_value (), true);
          }
          void deactivate () {}
        };
      public:
        EqualityComparator (Process *p, const string &name, double l_val, double r_val);
        virtual ~EqualityComparator () {}
      };

  class SignInverter : public UnaryOperator
  {
  private:
    class SignInverterAction : public UnaryOperatorAction
    {
    public:
      SignInverterAction (Process* parent, const string &name, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
        UnaryOperatorAction (parent, name, input, output) { Process::finalize (); }
      virtual ~SignInverterAction () {}
      void activate ()
      {
        if (_parent->get_state () > activated)
          return;
        _output.get()->set_value (-(((DoubleProperty*)_input.get())->get_value ()), true);
      }
      void deactivate () {}
    };
  public:
    SignInverter (Process *p, const string &name, double i_val);
    virtual ~SignInverter () {}
  };

  class Previous : public UnaryOperator
    {
    private:
      class PreviousAction : public UnaryOperatorAction
      {
      public:
        PreviousAction (Process* parent, const string &name, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output, double init_val) :
          UnaryOperatorAction (parent, name, input, output), _prev (init_val) { Process::finalize (); }
        virtual ~PreviousAction () {}
        void activate ()
        {
          if (_parent->get_state () > activated)
            return;

          _output.get()->set_value (_prev, true);
          _prev = ((DoubleProperty*)_input.get())->get_value ();
        }
        void deactivate () {}
      private:
        double _prev;
      };
    public:
      Previous (Process *p, const string &name, double i_val);
      virtual ~Previous () {}
    };

  class Incr : public Process
  {
  public:
    Incr (Process *parent, string name, bool is_model);
    Incr (bool is_model);
    void activate ();
    void deactivate () {}
    virtual ~Incr ();
  private:
    int
    init_incr (bool isModel);
    shared_ptr<AbstractProperty> _delta, _state;
  };

  class AdderAccumulator : public Process
  {
  private:
    class AdderAccumulatorAction : public Process
    {
    public:
      AdderAccumulatorAction (Process* parent, const string &name, shared_ptr<AbstractProperty> input,
                              shared_ptr<AbstractProperty> clamp_min, shared_ptr<AbstractProperty> clamp_max,
                              shared_ptr<AbstractProperty> result);
      void activate () override;
      void deactivate () override {}
    private:
      shared_ptr<AbstractProperty> _input, _clamp_min, _clamp_max, _result;
    };
  public:
    AdderAccumulator (Process* parent, const string &name, double input, double clamp_min, double clamp_max);
    virtual ~AdderAccumulator ();
    void activate () override;
    void deactivate () override;
  private:
    shared_ptr<AbstractProperty> _input, _clamp_min, _clamp_max, _result;
    unique_ptr<Coupling> _c_input;
    unique_ptr<Process> _action;
  };

}
