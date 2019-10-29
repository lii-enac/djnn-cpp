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

#include <functional>

namespace djnn
{
  using namespace std;

#if NEW_OP

  typedef NewBinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, std::plus<double>, double, double> AdderAction;
  typedef NewBinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, std::plus<double>, double, double> NewAdder;

  typedef NewBinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, std::minus<double>, double, double> SubtractorAction;
  typedef NewBinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, std::minus<double>, double, double> NewSubtractor;

  typedef NewBinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, std::multiplies<double>, double, double> MultiplierAction;
  typedef NewBinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, std::multiplies<double>, double, double> NewMultiplier;

  typedef NewBinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, std::divides<double>, double, double> DividerAction;
  typedef NewBinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, std::divides<double>, double, double> NewDivider;

  typedef NewBinaryOperatorAction<IntProperty,    IntProperty,    IntProperty,    std::modulus<int>, int, int> ModuloAction;
  typedef NewBinaryOperator      <IntProperty,    IntProperty,    IntProperty,    std::modulus<int>, int, int> NewModulo;

  typedef NewBinaryOperatorAction<DoubleProperty, DoubleProperty, BoolProperty,   std::greater<double>, double, double> StrictAscendingComparatorAction;
  typedef NewBinaryOperator      <DoubleProperty, DoubleProperty, BoolProperty,   std::greater<double>, double, double> NewStrictAscendingComparator;

  typedef NewBinaryOperatorAction<DoubleProperty, DoubleProperty, BoolProperty,   std::greater_equal<double>, double, double> AscendingComparatorAction;
  typedef NewBinaryOperator      <DoubleProperty, DoubleProperty, BoolProperty,   std::greater_equal<double>, double, double> NewAscendingComparator;

  typedef NewBinaryOperatorAction<DoubleProperty, DoubleProperty, BoolProperty,   std::equal_to<double>, double, double> EqualityComparatorAction;
  typedef NewBinaryOperator      <DoubleProperty, DoubleProperty, BoolProperty,   std::equal_to<double>, double, double> NewEqualityComparator;

#else

  class OldAdder : public BinaryOperator
  {
  private:
    class AdderAction : public BinaryOperatorAction
    {
    public:
      AdderAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
       AbstractProperty* result) :
      BinaryOperatorAction (parent, name, left, right, result) { Process::finalize_construction (parent); }
      virtual ~AdderAction () {}
      void impl_activate ()
      {
        _result->set_value (((DoubleProperty*)_left)->get_value () + ((DoubleProperty*)_right)->get_value (), true);
      }
      void impl_deactivate () {}
    };
  public:
    OldAdder (Process *p, const string &name, double l_val, double r_val);
    virtual ~OldAdder () {}
  protected:
    void serialize (const string& type) override;  
  };

  class OldSubtractor : public BinaryOperator
  {
  private:
    class SubtractorAction : public BinaryOperatorAction
    {
    public:
      SubtractorAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
        AbstractProperty* result) :
      BinaryOperatorAction (parent, name, left, right, result) { Process::finalize_construction (parent); }
      virtual ~SubtractorAction () {}
      void impl_activate ()
      {
        _result->set_value (((DoubleProperty*)_left)->get_value () - ((DoubleProperty*)_right)->get_value (), true);
      }
      void impl_deactivate () {}
    };
  public:
    OldSubtractor (Process *p, const string &name, double l_val, double r_val);
    virtual
    ~OldSubtractor () {}
  protected:
    void serialize (const string& type) override; 
  };

  class OldMultiplier : public BinaryOperator
  {
  private:
    class MultiplierAction : public BinaryOperatorAction
    {
    public:
      MultiplierAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
        AbstractProperty* result) :
      BinaryOperatorAction (parent, name, left, right, result) { Process::finalize_construction (parent); }
      virtual ~MultiplierAction () {}
      void impl_activate ()
      {
        _result->set_value (((DoubleProperty*)_left)->get_value () * ((DoubleProperty*)_right)->get_value (), true);
      }
      void impl_deactivate () {}
    };
  public:
    OldMultiplier (Process *p, const string &name, double l_val, double r_val);
    virtual ~OldMultiplier () {}
  protected:
    void serialize (const string& type) override;
  };

  class OldDivider : public BinaryOperator
  {
  private:
    class DividerAction : public BinaryOperatorAction
    {
    public:
      DividerAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
       AbstractProperty* result) :
      BinaryOperatorAction (parent, name, left, right, result) { Process::finalize_construction (parent); }
      virtual ~DividerAction () {}
      void impl_activate ()
      {
       double r = ((DoubleProperty*)_right)->get_value ();
       if (r == 0) {
        return;
      }
      _result->set_value (((DoubleProperty*)_left)->get_value () /r, true);
    }
    void impl_deactivate () {}
  };
  public:
  OldDivider (Process *p, const string &name, double l_val, double r_val);
  virtual ~OldDivider () {}
  protected:
    void serialize (const string& type) override;
  };

class OldModulo : public BinaryOperator
{
private:
  class ModuloAction : public BinaryOperatorAction
  {
  public:
    ModuloAction (Process* parent, string name, AbstractProperty* left, AbstractProperty* right,
      AbstractProperty* result) :
    BinaryOperatorAction (parent, name, left, right, result) { Process::finalize_construction (parent); }
    virtual ~ModuloAction () {}
    void impl_activate ()
    {
     int l = ((IntProperty*) _left)->get_value ();
     int r = ((IntProperty*) _right)->get_value ();
     if (r == 0) {
      //cerr << "Warning: modulo per 0\n";
      return;
    }
    _result->set_value (l % r, true);
  }
  void impl_deactivate () {}
};
public:
  OldModulo (Process *p, const string &name, int l_val, int r_val);
  virtual ~OldModulo () {}
protected:
    void serialize (const string& type) override;
};

class OldAscendingComparator : public BinaryOperator
{
private:
  class AscendingComparatorAction : public BinaryOperatorAction
  {
  public:
    AscendingComparatorAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
     AbstractProperty* result) :
    BinaryOperatorAction (parent, name, left, right, result) { Process::finalize_construction (parent); }
    virtual ~AscendingComparatorAction () {}
    void impl_activate ()
    {
      _result->set_value (((DoubleProperty*)_left)->get_value () <= ((DoubleProperty*)_right)->get_value (), true);
    }
    void impl_deactivate () {}
  };
public:
  OldAscendingComparator (Process *p, const string &name, double l_val, double r_val);
  virtual ~OldAscendingComparator () {}
protected:
    void serialize (const string& type) override;
};

class OldStrictAscendingComparator : public BinaryOperator
{
private:
  class StrictAscendingComparatorAction : public BinaryOperatorAction
  {
  public:
    StrictAscendingComparatorAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
     AbstractProperty* result) :
    BinaryOperatorAction (parent, name, left, right, result) { Process::finalize_construction (parent); }
    virtual ~StrictAscendingComparatorAction () {}
    void impl_activate ()
    {

      _result->set_value (((DoubleProperty*)_left)->get_value () < ((DoubleProperty*)_right)->get_value (), true);
    }
    void impl_deactivate () {}
  };
public:
  OldStrictAscendingComparator (Process *p, const string &name, double l_val, double r_val);
  virtual ~OldStrictAscendingComparator () {}
protected:
    void serialize (const string& type) override;
};

class OldEqualityComparator : public BinaryOperator
{
private:
  class EqualityComparatorAction : public BinaryOperatorAction
  {
  public:
    EqualityComparatorAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
      AbstractProperty* result) :
    BinaryOperatorAction (parent, name, left, right, result) { Process::finalize_construction (parent); }
    virtual ~EqualityComparatorAction () {}
    void impl_activate ()
    {
      _result->set_value (((DoubleProperty*)_left)->get_value () == ((DoubleProperty*)_right)->get_value (), true);
    }
    void impl_deactivate () {}
  };
public:
  OldEqualityComparator (Process *p, const string &name, double l_val, double r_val);
  virtual ~OldEqualityComparator () {}
protected:
  void serialize (const string& type) override;
};

#endif

class SignInverter : public UnaryOperator
{
private:
  class SignInverterAction : public UnaryOperatorAction
  {
  public:
    SignInverterAction (Process* parent, const string &name, AbstractProperty* input, AbstractProperty* output) :
    UnaryOperatorAction (parent, name, input, output) { Process::finalize_construction (parent); }
    virtual ~SignInverterAction () {}
    void impl_activate ()
    {
      _output->set_value (-(((DoubleProperty*)_input)->get_value ()), true);
    }
    void impl_deactivate () {}
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
    UnaryOperatorAction (parent, name, input, output), _prev (init_val) { Process::finalize_construction (parent); }
    virtual ~PreviousAction () {}
    void impl_activate ()
    {
      _output->set_value (_prev, true);
      _prev = ((DoubleProperty*)_input)->get_value ();
    }
    void impl_deactivate () {}
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
  void impl_activate () override;
  void impl_deactivate () override {}
  void post_activate () override { notify_activation (); set_activation_state (DEACTIVATED); }
  virtual ~Incr ();
private:
  int
  init_incr (bool isModel);
  AbstractProperty *_delta, *_state;
protected:
  void set_parent (Process* p) override;
  void serialize (const string& type) override;
};

class AdderAccumulator : public Process
{
private:
  class AdderAccumulatorAction : public Action
  {
  public:
    AdderAccumulatorAction (Process* parent, const string &name, AbstractProperty* input,
      AbstractProperty* clamp_min, AbstractProperty* clamp_max,
      AbstractProperty* result);
    void impl_activate () override;
    void impl_deactivate () override {}
  private:
    AbstractProperty  *_input, *_clamp_min, *_clamp_max, *_result;
  };
public:
  AdderAccumulator (Process* parent, const string &name, double input, double clamp_min, double clamp_max);
  virtual ~AdderAccumulator ();
  void impl_activate () override;
  void impl_deactivate () override;
protected:
  void set_parent (Process* p) override;
  void serialize (const string& type) override;
private:
  AbstractProperty *_input, *_clamp_min, *_clamp_max, *_result;
  Coupling *_c_input;
  Process *_action;
};

#if NEW_OP
  typedef NewAdder Adder;
  typedef NewSubtractor Subtractor;
  typedef NewMultiplier Multiplier;
  typedef NewDivider Divider;
  typedef NewModulo Modulo;
  typedef NewAscendingComparator AscendingComparator;
  typedef NewStrictAscendingComparator StrictAscendingComparator;
  typedef NewEqualityComparator EqualityComparator;
#else
  typedef OldAdder Adder;
  typedef OldSubtractor Subtractor;
  typedef OldMultiplier Multiplier;
  typedef OldDivider Divider;
  typedef OldModulo Modulo;
  typedef OldAscendingComparator AscendingComparator;
  typedef OldStrictAscendingComparator StrictAscendingComparator;
  typedef OldEqualityComparator EqualityComparator;
#endif
}
