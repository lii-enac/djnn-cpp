/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

#include "operators.h"

#include <functional>
#include <math.h> // necessary for smala programs

namespace djnn
{
  template <> const char name_info<std::plus<double>>::left[];
  template <> const char name_info<std::plus<double>>::right[];
  template <> const char name_info<std::plus<double>>::serialize[];
  typedef BinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, std::plus<double>, double, double> AdderAction;
  typedef BinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, std::plus<double>, double, double> Adder;

  template <> const char name_info<std::minus<double>>::left[];
  template <> const char name_info<std::minus<double>>::right[];
  template <> const char name_info<std::minus<double>>::serialize[];
  typedef BinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, std::minus<double>, double, double> SubtractorAction;
  typedef BinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, std::minus<double>, double, double> Subtractor;

  template <> const char name_info<std::multiplies<double>>::left[];
  template <> const char name_info<std::multiplies<double>>::right[];
  template <> const char name_info<std::multiplies<double>>::serialize[];
  typedef BinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, std::multiplies<double>, double, double> MultiplierAction;
  typedef BinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, std::multiplies<double>, double, double> Multiplier;

  template <> const char name_info<std::divides<double>>::left[];
  template <> const char name_info<std::divides<double>>::right[];
  template <> const char name_info<std::divides<double>>::serialize[];
  typedef BinaryOperatorAction<DoubleProperty, DoubleProperty, DoubleProperty, std::divides<double>, double, double> DividerAction;
  typedef BinaryOperator      <DoubleProperty, DoubleProperty, DoubleProperty, std::divides<double>, double, double> Divider;

  template <> const char name_info<std::modulus<int>>::left[];
  template <> const char name_info<std::modulus<int>>::right[];
  template <> const char name_info<std::modulus<int>>::serialize[];
  typedef BinaryOperatorAction<IntProperty,    IntProperty,    IntProperty,    std::modulus<int>, int, int> ModuloAction;
  typedef BinaryOperator      <IntProperty,    IntProperty,    IntProperty,    std::modulus<int>, int, int> Modulo;

  template <> const char name_info<std::greater<double>>::left[];
  template <> const char name_info<std::greater<double>>::right[];
  template <> const char name_info<std::greater<double>>::serialize[];
  typedef BinaryOperatorAction<DoubleProperty, DoubleProperty, BoolProperty,   std::greater<double>, double, double> StrictAscendingComparatorAction;
  typedef BinaryOperator      <DoubleProperty, DoubleProperty, BoolProperty,   std::greater<double>, double, double> StrictAscendingComparator;

  template <> const char name_info<std::greater_equal<double>>::left[];
  template <> const char name_info<std::greater_equal<double>>::right[];
  template <> const char name_info<std::greater_equal<double>>::serialize[];
  typedef BinaryOperatorAction<DoubleProperty, DoubleProperty, BoolProperty,   std::greater_equal<double>, double, double> AscendingComparatorAction;
  typedef BinaryOperator      <DoubleProperty, DoubleProperty, BoolProperty,   std::greater_equal<double>, double, double> AscendingComparator;

  template <> const char name_info<std::equal_to<double>>::left[];
  template <> const char name_info<std::equal_to<double>>::right[];
  template <> const char name_info<std::equal_to<double>>::serialize[];
  typedef BinaryOperatorAction<DoubleProperty, DoubleProperty, BoolProperty,   std::equal_to<double>, double, double> EqualityComparatorAction;
  typedef BinaryOperator      <DoubleProperty, DoubleProperty, BoolProperty,   std::equal_to<double>, double, double> EqualityComparator;

  template <> const char name_info<std::negate<double>>::left[];
  template <> const char name_info<std::negate<double>>::right[];
  template <> const char name_info<std::negate<double>>::serialize[];
  typedef UnaryOperatorAction<DoubleProperty, DoubleProperty,  std::negate<double>, double> SignInverterAction;
  typedef UnaryOperator      <DoubleProperty, DoubleProperty,  std::negate<double>, double> SignInverter;


  class Incr : public FatProcess
  {
  private:
    class StepAction : public Action
      {
      public:
        StepAction (ParentProcess* parent, const string& name) : Action (parent, name) { finalize_construction (parent, name); }
        virtual ~StepAction () {}
        void impl_activate ()  { ((Incr*)get_parent ())->step (); }
      };
  public:
    Incr (ParentProcess* parent, const string& name, bool is_model);
    bool pre_activate () override;
    void impl_activate () override;
    void impl_deactivate () override;
    void step ();
    //void post_activate () override { post_activate_auto_deactivate (); }
    virtual ~Incr ();
  private:
    int init_incr (bool isModel);
    DoubleProperty _delta, _state;
    Spike _step;
    StepAction _action;
    Coupling _c_step;
    bool _is_model;
  protected:
    void set_parent (ParentProcess* parent) override;
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const string& format) override;
#endif
  };

  class AdderAccumulator : public FatProcess
  {
  private:
    class AdderAccumulatorAction : public Action
    {
    public:
      AdderAccumulatorAction (ParentProcess* parent, const string& name, AdderAccumulator& aa);
      void impl_activate () override;
    private:
      AdderAccumulator& _aa;
    };
  public:
    AdderAccumulator (ParentProcess* parent, const string& name, double input, double clamp_min, double clamp_max);
    virtual ~AdderAccumulator ();
    void impl_activate () override;
    void impl_deactivate () override;
  protected:
    void set_parent (ParentProcess* parent) override;
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const string& format) override;
#endif
  private:
    friend class AdderAccumulatorAction;
    DoubleProperty _input, _clamp_min, _clamp_max, _result;
    AdderAccumulatorAction _action;
    Coupling _c_input;
  };
}
