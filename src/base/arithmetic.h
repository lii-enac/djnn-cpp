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

  template <> std::string serialize_info<std::plus<double>>::serialize;
  typedef NewBinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, std::plus<double>, double, double> AdderAction;
  typedef NewBinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, std::plus<double>, double, double> NewAdder;

  template <> std::string serialize_info<std::minus<double>>::serialize;
  typedef NewBinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, std::minus<double>, double, double> SubtractorAction;
  typedef NewBinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, std::minus<double>, double, double> NewSubtractor;

  template <> std::string serialize_info<std::multiplies<double>>::serialize;
  typedef NewBinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, std::multiplies<double>, double, double> MultiplierAction;
  typedef NewBinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, std::multiplies<double>, double, double> NewMultiplier;

  template <> std::string serialize_info<std::divides<double>>::serialize;
  typedef NewBinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, std::divides<double>, double, double> DividerAction;
  typedef NewBinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, std::divides<double>, double, double> NewDivider;

  template <> std::string serialize_info<std::modulus<int>>::serialize;
  typedef NewBinaryOperatorAction<IntProperty,    IntProperty,    IntProperty,    std::modulus<int>, int, int> ModuloAction;
  typedef NewBinaryOperator      <IntProperty,    IntProperty,    IntProperty,    std::modulus<int>, int, int> NewModulo;

  template <> std::string serialize_info<std::greater<double>>::serialize;
  typedef NewBinaryOperatorAction<DoubleProperty, DoubleProperty, BoolProperty,   std::greater<double>, double, double> StrictAscendingComparatorAction;
  typedef NewBinaryOperator      <DoubleProperty, DoubleProperty, BoolProperty,   std::greater<double>, double, double> NewStrictAscendingComparator;

  template <> std::string serialize_info<std::greater_equal<double>>::serialize;
  typedef NewBinaryOperatorAction<DoubleProperty, DoubleProperty, BoolProperty,   std::greater_equal<double>, double, double> AscendingComparatorAction;
  typedef NewBinaryOperator      <DoubleProperty, DoubleProperty, BoolProperty,   std::greater_equal<double>, double, double> NewAscendingComparator;

  template <> std::string serialize_info<std::equal_to<double>>::serialize;
  typedef NewBinaryOperatorAction<DoubleProperty, DoubleProperty, BoolProperty,   std::equal_to<double>, double, double> EqualityComparatorAction;
  typedef NewBinaryOperator      <DoubleProperty, DoubleProperty, BoolProperty,   std::equal_to<double>, double, double> NewEqualityComparator;

  template <> std::string serialize_info<std::negate<double>>::serialize;
  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  std::negate<double>, double> SignInverterAction;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  std::negate<double>, double> NewSignInverter;

  /*template<typename Input, typename Output>
  struct sign_inverter {
    void operator() (Input& input, Output& output) {
      output.set_value (-input.get_value (), true);
    }
  };

  typedef NewUnaryOperatorAction<DoubleProperty, DoubleProperty,  sign_inverter<DoubleProperty, DoubleProperty>, double> SignInverterAction;
  typedef NewUnaryOperator      <DoubleProperty, DoubleProperty,  sign_inverter<DoubleProperty, DoubleProperty>, double> NewSignInverter;*/


  class NewPrevious : public Process
  {
  private:
    class NewPreviousAction : public Action
    {
    public:
      NewPreviousAction (Process* parent, const string &name, NewPrevious& np, double init_val)
      : Action(parent, name),
      _np(np),
       _prev (init_val) { Process::finalize_construction (parent); }
      virtual ~NewPreviousAction () {}
      void impl_activate ()
      {
        _np._output.set_value (_prev, true);
        _prev = _np._input.get_value ();
      }
      void impl_deactivate () {}
    private:
      NewPrevious &_np;
      double _prev;
    };
  public:
    NewPrevious (Process *p, const string &name, double i_val);
    virtual ~NewPrevious () { uninit_unary_couplings(this, _input, _output, _action, _coupling); }
    void impl_activate () override { _coupling.enable (); _action.activate (); }
    void impl_deactivate () override { _coupling.disable (); _action.deactivate ();};
  protected:
    void serialize (const string& type) override;
    DoubleProperty _input;
    DoubleProperty _output;
    NewPreviousAction _action;
    Coupling _coupling;
  };

  class NewIncr : public Process
  {
  public:
    NewIncr (Process *parent, const string& name, bool is_model);
    NewIncr (bool is_model);
    void impl_activate () override;
    void impl_deactivate () override {}
    void post_activate () override { notify_activation (); set_activation_state (DEACTIVATED); }
    virtual ~NewIncr ();
  private:
    int init_incr (bool isModel);
    DoubleProperty _delta, _state;
  protected:
    void set_parent (Process* p) override;
    void serialize (const string& type) override;
  };

  class NewAdderAccumulator : public Process
  {
  private:
    class NewAdderAccumulatorAction : public Action
    {
    public:
      NewAdderAccumulatorAction (Process* parent, const string &name, NewAdderAccumulator& aa);
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      NewAdderAccumulator& _aa;
    };
  public:
    NewAdderAccumulator (Process* parent, const string &name, double input, double clamp_min, double clamp_max);
    virtual ~NewAdderAccumulator ();
    void impl_activate () override;
    void impl_deactivate () override;
  protected:
    void set_parent (Process* p) override;
    void serialize (const string& type) override;
  private:
    friend class NewAdderAccumulatorAction;
    DoubleProperty _input, _clamp_min, _clamp_max, _result;
    NewAdderAccumulatorAction _action;
    Coupling _c_input;
  };
  
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

class OldSignInverter : public UnaryOperator
{
private:
  class OldSignInverterAction : public UnaryOperatorAction
  {
  public:
    OldSignInverterAction (Process* parent, const string &name, AbstractProperty* input, AbstractProperty* output) :
    UnaryOperatorAction (parent, name, input, output) { Process::finalize_construction (parent); }
    virtual ~OldSignInverterAction () {}
    void impl_activate ()
    {
      _output->set_value (-(((DoubleProperty*)_input)->get_value ()), true);
    }
    void impl_deactivate () {}
  };
public:
  OldSignInverter (Process *p, const string &name, double i_val);
  virtual ~OldSignInverter () {}
protected:
  void serialize (const string& type) override;
};

class OldPrevious : public UnaryOperator
{
private:
  class OldPreviousAction : public UnaryOperatorAction
  {
  public:
    OldPreviousAction (Process* parent, const string &name, AbstractProperty* input, AbstractProperty* output, double init_val) :
    UnaryOperatorAction (parent, name, input, output), _prev (init_val) { Process::finalize_construction (parent); }
    virtual ~OldPreviousAction () {}
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
  OldPrevious (Process *p, const string &name, double i_val);
  virtual ~OldPrevious () {}
protected:
  void serialize (const string& type) override;
};

class OldIncr : public Process
{
public:
  OldIncr (Process *parent, string name, bool is_model);
  OldIncr (bool is_model);
  void impl_activate () override;
  void impl_deactivate () override {}
  void post_activate () override { notify_activation (); set_activation_state (DEACTIVATED); }
  virtual ~OldIncr ();
private:
  int
  init_incr (bool isModel);
  AbstractProperty *_delta, *_state;
protected:
  void set_parent (Process* p) override;
  void serialize (const string& type) override;
};

class OldAdderAccumulator : public Process
{
private:
  class OldAdderAccumulatorAction : public Action
  {
  public:
    OldAdderAccumulatorAction (Process* parent, const string &name, AbstractProperty* input,
      AbstractProperty* clamp_min, AbstractProperty* clamp_max,
      AbstractProperty* result);
    void impl_activate () override;
    void impl_deactivate () override {}
  private:
    AbstractProperty  *_input, *_clamp_min, *_clamp_max, *_result;
  };
public:
  OldAdderAccumulator (Process* parent, const string &name, double input, double clamp_min, double clamp_max);
  virtual ~OldAdderAccumulator ();
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
#endif

#if NEW_OP
  typedef NewAdder Adder;
  typedef NewSubtractor Subtractor;
  typedef NewMultiplier Multiplier;
  typedef NewDivider Divider;
  typedef NewModulo Modulo;
  typedef NewAscendingComparator AscendingComparator;
  typedef NewStrictAscendingComparator StrictAscendingComparator;
  typedef NewEqualityComparator EqualityComparator;
  typedef NewSignInverter SignInverter;
  typedef NewPrevious Previous;
  typedef NewIncr Incr;
  typedef NewAdderAccumulator AdderAccumulator;
#else
  typedef OldAdder Adder;
  typedef OldSubtractor Subtractor;
  typedef OldMultiplier Multiplier;
  typedef OldDivider Divider;
  typedef OldModulo Modulo;
  typedef OldAscendingComparator AscendingComparator;
  typedef OldStrictAscendingComparator StrictAscendingComparator;
  typedef OldEqualityComparator EqualityComparator;
  typedef OldSignInverter SignInverter;
  typedef OldPrevious Previous;
  typedef OldIncr Incr;
  typedef OldAdderAccumulator OldAdderAccumulator;
#endif
}
