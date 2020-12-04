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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/tree/bool_property.h"
#include "core/tree/int_property.h"
#include "core/tree/double_property.h"
#include "core/serializer/serializer.h"

// #include <string> // pch

namespace djnn {

  void
  init_binary_couplings (FatProcess& _left, FatProcess& _right, FatProcess& _result, Action& _action, Coupling& _c_left, Coupling& _c_right);
  void
  uninit_binary_couplings (FatProcess* this_, FatProcess& _left, FatProcess& _right, FatProcess& _result, Action& _action, Coupling& _c_left, Coupling& _c_right);

  template <typename Left, typename Right, typename Result, typename BinaryFunction, typename Left_init, typename Right_init>
  class BinaryOperator;

  template <typename Left, typename Right, typename Result, typename BinaryFunction, typename Left_init, typename Right_init>
  class BinaryOperatorAction : public Action
  {
  public:
    typedef BinaryOperator<Left, Right, Result, BinaryFunction, Left_init, Right_init> BinOperator;

    BinaryOperatorAction (ParentProcess* parent, const std::string& name, BinOperator& binop) : Action(parent,name), _binop(binop) {
      finalize_construction (parent, name);
    }
    virtual ~BinaryOperatorAction () {};
    void impl_activate () override {
      _binop._result.set_value (BinaryFunction()(_binop._left.get_value (), _binop._right.get_value ()), true);
    }
  protected:
    BinOperator& _binop;
  };

  template <typename Function>
  struct name_info {
    static const char left[], right[], serialize[];
  };

  class BinaryOperatorCommon : public FatProcess {
    public:
    BinaryOperatorCommon (const std::string& name) : FatProcess(name) {}
    virtual ~BinaryOperatorCommon () {}
  };

  template <typename Left, typename Right, typename Result, typename BinaryFunction, typename Left_init, typename Right_init>
  class BinaryOperator : public BinaryOperatorCommon// Process
  {
  public:
    typedef BinaryOperatorAction<Left, Right, Result, BinaryFunction, Left_init, Right_init> Action;

    BinaryOperator (ParentProcess* parent, const std::string& name, const Left_init& l_val, const Right_init& r_val)
    : BinaryOperatorCommon (name),
      _left(this, name_info<BinaryFunction>::left, l_val),
      _right(this, name_info<BinaryFunction>::right, r_val),
      _result(this, "result", BinaryFunction()(l_val,r_val)),
      _action(this, name+"action", *this),
      _c_left(&_left, ACTIVATION, &_action, ACTIVATION),
      _c_right(&_right, ACTIVATION, &_action, ACTIVATION)
    {
      init_binary_couplings(_left, _right, _result, _action, _c_left, _c_right);
      finalize_construction (parent, name);
    }
    virtual ~BinaryOperator () {
      uninit_binary_couplings(this, _left, _right, _result, _action, _c_left, _c_right);
    }
    void impl_activate () override { _c_left.enable(); _c_right.enable (); _action.activate (); }
    void impl_deactivate () override { _c_left.disable (); _c_right.disable (); _action.deactivate ();}

#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& type) override {
      AbstractSerializer::pre_serialize(this, type);
      AbstractSerializer::serializer->start ("base:" + std::string(name_info<BinaryFunction>::serialize));
      AbstractSerializer::serializer->text_attribute ("id", get_name ());
      AbstractSerializer::serializer->cpptype_attribute (std::string(name_info<BinaryFunction>::left), _left.get_value ());
      AbstractSerializer::serializer->cpptype_attribute (std::string(name_info<BinaryFunction>::right), _right.get_value ());
      AbstractSerializer::serializer->end ();
      AbstractSerializer::post_serialize(this);
    }
#endif
  protected:
    void set_parent (ParentProcess* parent) override {
      // in case of re-parenting remove edge dependency in graph
      if (get_parent ()) {
         remove_state_dependency (get_parent (), &_action);
      }
      add_state_dependency (parent, &_action);
      FatProcess::set_parent (parent);
    }
  public:
    Left _left;
    Right _right;
    Result _result;
    Action _action;
    Coupling _c_left, _c_right;
  };




  void
  init_unary_couplings (FatProcess& _input, FatProcess& _output, Action& _action, Coupling& _coupling);
  void
  uninit_unary_couplings (FatProcess * this_, FatProcess& _input, FatProcess& _output, Action& _action, Coupling& _coupling);


  template <typename Input, typename Output, typename UnaryFunction, typename Input_init>
  class UnaryOperator;

  template <typename Input, typename Output, typename UnaryFunction, typename Input_init>
  class UnaryOperatorAction : public Action
  {
  public:
    typedef UnaryOperator<Input, Output, UnaryFunction, Input_init> UnOperator;

    UnaryOperatorAction (UnOperator& unop) : _unop(unop) {}
    UnaryOperatorAction (ParentProcess* parent, const std::string& name, UnOperator& unop) : Action(parent,name), _unop(unop) {
      finalize_construction (parent, name);
    }
    virtual ~UnaryOperatorAction () {};
    void impl_activate () override {
      _unop._output.set_value (UnaryFunction()(_unop._input.get_value ()), true);
      //UnaryFunction()(_unop.input, _unop.output);
    }
  protected:
    UnOperator& _unop;
  };

  class UnaryOperatorCommon : public FatProcess {
    public:
    UnaryOperatorCommon (const std::string& name) : FatProcess(name) {}
    virtual ~UnaryOperatorCommon () {}
  };

  template <typename Input, typename Output, typename UnaryFunction, typename Input_init>
  class UnaryOperator : public UnaryOperatorCommon // Process
  {
  public:
    typedef UnaryOperatorAction<Input, Output, UnaryFunction, Input_init> Action;

    UnaryOperator (ParentProcess* parent, const std::string& name, const Input_init& i_val)
    : UnaryOperatorCommon (name),
      _input(this, "input", i_val),
      _output(this, "output", UnaryFunction()(i_val)),
      _action(this, "action", *this),
      _coupling(&_input, ACTIVATION, &_action, ACTIVATION)
    {
      init_unary_couplings(_input, _output, _action, _coupling);
      finalize_construction (parent, name);
    }
    virtual ~UnaryOperator () {
      uninit_unary_couplings(this, _input, _output, _action, _coupling);
    }
    void impl_activate () override { _coupling.enable (); _action.activate (); }
    void impl_deactivate () override { _coupling.disable (); _action.deactivate ();}

#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& type) override {
      AbstractSerializer::pre_serialize(this, type);
      AbstractSerializer::serializer->start ("base:" + std::string(name_info<UnaryFunction>::serialize));
      AbstractSerializer::serializer->text_attribute ("id", get_name ());
      AbstractSerializer::serializer->cpptype_attribute ("input", _input.get_value ());
      AbstractSerializer::serializer->end ();
      AbstractSerializer::post_serialize(this);
    }
#endif

  protected:
    void set_parent (ParentProcess* parent) override {
      // in case of re-parenting remove edge dependency in graph
      if (get_parent ()) {
         remove_state_dependency (get_parent (), &_action);
      }
      add_state_dependency (parent, &_action);
      FatProcess::set_parent (parent);
    }
  public:
    Input _input;
    Output _output;
    Action _action;
    Coupling _coupling;
  };

}
