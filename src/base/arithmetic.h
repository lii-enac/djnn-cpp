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

namespace djnn
{
  using namespace std;

  class Adder : public BinaryOperator
  {
  private:
    class AdderAction : public BinaryOperatorAction
    {
    public:
      AdderAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
       AbstractProperty* result) :
      BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
      virtual ~AdderAction () {}
      void activate ()
      {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _result->set_value (((DoubleProperty*)_left)->get_value () + ((DoubleProperty*)_right)->get_value (), true);
      }
      void deactivate () {}
    };
  public:
    Adder (Process *p, const string &name, double l_val, double r_val);
    virtual ~Adder () {}
  protected:
    void serialize (const string& type) override;  
  };

  class Subtractor : public BinaryOperator
  {
  private:
    class SubtractorAction : public BinaryOperatorAction
    {
    public:
      SubtractorAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
        AbstractProperty* result) :
      BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
      virtual ~SubtractorAction () {}
      void activate ()
      {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _result->set_value (((DoubleProperty*)_left)->get_value () - ((DoubleProperty*)_right)->get_value (), true);
      }
      void deactivate () {}
    };
  public:
    Subtractor (Process *p, const string &name, double l_val, double r_val);
    virtual
    ~Subtractor () {}
  protected:
    void serialize (const string& type) override; 
  };

  class Multiplier : public BinaryOperator
  {
  private:
    class MultiplierAction : public BinaryOperatorAction
    {
    public:
      MultiplierAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
        AbstractProperty* result) :
      BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
      virtual ~MultiplierAction () {}
      void activate ()
      {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
          return;
        _result->set_value (((DoubleProperty*)_left)->get_value () * ((DoubleProperty*)_right)->get_value (), true);
      }
      void deactivate () {}
    };
  public:
    Multiplier (Process *p, const string &name, double l_val, double r_val);
    virtual ~Multiplier () {}
  protected:
    void serialize (const string& type) override;
  };

  class Divider : public BinaryOperator
  {
  private:
    class DividerAction : public BinaryOperatorAction
    {
    public:
      DividerAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
       AbstractProperty* result) :
      BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
      virtual ~DividerAction () {}
      void activate ()
      {
        //if (_parent->get_state () > activated)
        if (!_parent->somehow_activating ())
         return;
       double r = ((DoubleProperty*)_right)->get_value ();
       if (r == 0) {
        return;
      }
      _result->set_value (((DoubleProperty*)_left)->get_value () /r, true);
    }
    void deactivate () {}
  };
  public:
  Divider (Process *p, const string &name, double l_val, double r_val);
  virtual ~Divider () {}
  protected:
    void serialize (const string& type) override;
  };

class Modulo : public BinaryOperator
{
private:
  class ModuloAction : public BinaryOperatorAction
  {
  public:
    ModuloAction (Process* parent, string name, AbstractProperty* left, AbstractProperty* right,
      AbstractProperty* result) :
    BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
    virtual ~ModuloAction () {}
    void activate ()
    {
      //if (_parent->get_state () > activated)
      if (!_parent->somehow_activating ())
       return;
     int l = ((IntProperty*) _left)->get_value ();
     int r = ((IntProperty*) _right)->get_value ();
     if (r == 0) {
      cerr << "Warning: modulo per 0\n";
      return;
    }
    _result->set_value (l % r, true);
  }
  void deactivate () {}
};
public:
  Modulo (Process *p, const string &name, int l_val, int r_val);
  virtual ~Modulo () {}
protected:
    void serialize (const string& type) override;
};

class AscendingComparator : public BinaryOperator
{
private:
  class AscendingComparatorAction : public BinaryOperatorAction
  {
  public:
    AscendingComparatorAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
     AbstractProperty* result) :
    BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
    virtual ~AscendingComparatorAction () {}
    void activate ()
    {
      //if (_parent->get_state () > activated)
      if (!_parent->somehow_activating ())
        return;
      _result->set_value (((DoubleProperty*)_left)->get_value () <= ((DoubleProperty*)_right)->get_value (), true);
    }
    void deactivate () {}
  };
public:
  AscendingComparator (Process *p, const string &name, double l_val, double r_val);
  virtual ~AscendingComparator () {}
protected:
    void serialize (const string& type) override;
};

class StrictAscendingComparator : public BinaryOperator
{
private:
  class StrictAscendingComparatorAction : public BinaryOperatorAction
  {
  public:
    StrictAscendingComparatorAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
     AbstractProperty* result) :
    BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
    virtual ~StrictAscendingComparatorAction () {}
    void activate ()
    {
      //if (_parent->get_state () > activated)
      if (!_parent->somehow_activating ())
        return;
      _result->set_value (((DoubleProperty*)_left)->get_value () < ((DoubleProperty*)_right)->get_value (), true);
    }
    void deactivate () {}
  };
public:
  StrictAscendingComparator (Process *p, const string &name, double l_val, double r_val);
  virtual ~StrictAscendingComparator () {}
protected:
    void serialize (const string& type) override;
};

class EqualityComparator : public BinaryOperator
{
private:
  class EqualityComparatorAction : public BinaryOperatorAction
  {
  public:
    EqualityComparatorAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
      AbstractProperty* result) :
    BinaryOperatorAction (parent, name, left, right, result) { Process::finalize (); }
    virtual ~EqualityComparatorAction () {}
    void activate ()
    {
      //if (_parent->get_state () > activated)
      if (!_parent->somehow_activating ())
        return;
      _result->set_value (((DoubleProperty*)_left)->get_value () == ((DoubleProperty*)_right)->get_value (), true);
    }
    void deactivate () {}
  };
public:
  EqualityComparator (Process *p, const string &name, double l_val, double r_val);
  virtual ~EqualityComparator () {}
protected:
  void serialize (const string& type) override;
};

class SignInverter : public UnaryOperator
{
private:
  class SignInverterAction : public UnaryOperatorAction
  {
  public:
    SignInverterAction (Process* parent, const string &name, AbstractProperty* input, AbstractProperty* output) :
    UnaryOperatorAction (parent, name, input, output) { Process::finalize (); }
    virtual ~SignInverterAction () {}
    void activate ()
    {
      //if (_parent->get_state () > activated)
      if (!_parent->somehow_activating ())
        return;
      _output->set_value (-(((DoubleProperty*)_input)->get_value ()), true);
    }
    void deactivate () {}
  };
public:
  SignInverter (Process *p, const string &name, double i_val);
  virtual ~SignInverter () {}
protected:
  void serialize (const string& type) override;
};

class Previous : public UnaryOperator
{
private:
  class PreviousAction : public UnaryOperatorAction
  {
  public:
    PreviousAction (Process* parent, const string &name, AbstractProperty* input, AbstractProperty* output, double init_val) :
    UnaryOperatorAction (parent, name, input, output), _prev (init_val) { Process::finalize (); }
    virtual ~PreviousAction () {}
    void activate ()
    {
      //if (_parent->get_state () > activated)
      if (!_parent->somehow_activating ())
        return;

      _output->set_value (_prev, true);
      _prev = ((DoubleProperty*)_input)->get_value ();
    }
    void deactivate () {}
  private:
    double _prev;
  };
public:
  Previous (Process *p, const string &name, double i_val);
  virtual ~Previous () {}
protected:
  void serialize (const string& type) override;
};

class Incr : public Process
{
public:
  Incr (Process *parent, string name, bool is_model);
  Incr (bool is_model);
  void activate () override;
  void deactivate () override {}
  virtual ~Incr ();
private:
  int
  init_incr (bool isModel);
  AbstractProperty *_delta, *_state;
protected:
  void serialize (const string& type) override;
};

class AdderAccumulator : public Process
{
private:
  class AdderAccumulatorAction : public Process
  {
  public:
    AdderAccumulatorAction (Process* parent, const string &name, AbstractProperty* input,
      AbstractProperty* clamp_min, AbstractProperty* clamp_max,
      AbstractProperty* result);
    void activate () override;
    void deactivate () override {}
  private:
    AbstractProperty  *_input, *_clamp_min, *_clamp_max, *_result;
  };
public:
  AdderAccumulator (Process* parent, const string &name, double input, double clamp_min, double clamp_max);
  virtual ~AdderAccumulator ();
  void activate () override;
  void deactivate () override;
protected:
  void serialize (const string& type) override;
private:
  AbstractProperty *_input, *_clamp_min, *_clamp_max, *_result;
  Coupling *_c_input;
  Process *_action;
};

}
