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

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/tree/bool_property.h"
#include "core/tree/int_property.h"
#include "core/tree/double_property.h"
#include "core/serializer/serializer.h"

#include <string>

#define NEW_OP 1

namespace djnn {
  using namespace std;

#if NEW_OP

  void
  init_binary_couplings (Process& _left, Process& _right, Process& _result, Process& _action, Coupling& _c_left, Coupling& _c_right);
  void
  uninit_binary_couplings (Process* this_, Process& _left, Process& _right, Process& _result, Process& _action, Coupling& _c_left, Coupling& _c_right);

  template <typename Left, typename Right, typename Result, typename BinaryFunction, typename Left_init, typename Right_init>
  class NewBinaryOperator;

  template <typename Left, typename Right, typename Result, typename BinaryFunction, typename Left_init, typename Right_init>
  class NewBinaryOperatorAction : public Action
  {
  public:
    typedef NewBinaryOperator<Left, Right, Result, BinaryFunction, Left_init, Right_init> BinOperator;

    NewBinaryOperatorAction (BinOperator& binop) : _binop(binop) {}
    NewBinaryOperatorAction (Process* p, const string& n, BinOperator& binop) : Action(p,n), _binop(binop) {
      Process::finalize_construction (p);
    }
    virtual ~NewBinaryOperatorAction () {};
    void impl_activate () override {
      _binop._result.set_value (BinaryFunction()(_binop._left.get_value (), _binop._right.get_value ()), true);
    }
    void impl_deactivate () override {}
  protected:
    BinOperator& _binop;
  };

  template <typename Function>
  struct name_info {
    static const char left[], right[], serialize[];
  };

  template <typename Left, typename Right, typename Result, typename BinaryFunction, typename Left_init, typename Right_init>
  class NewBinaryOperator : public Process
  {
  public:
    typedef NewBinaryOperatorAction<Left, Right, Result, BinaryFunction, Left_init, Right_init> Action;

    NewBinaryOperator (Process *p, const string &n, const Left_init& l_val, const Right_init& r_val)
    : Process (n),
      _left(this, name_info<BinaryFunction>::left, l_val),
      _right(this, name_info<BinaryFunction>::right, r_val),
      _result(this, "result", BinaryFunction()(l_val,r_val)),
      _action(this, n+"action", *this),
      _c_left(&_left, ACTIVATION, &_action, ACTIVATION),
      _c_right(&_right, ACTIVATION, &_action, ACTIVATION)
    {
      init_binary_couplings(_left, _right, _result, _action, _c_left, _c_right);
      Process::finalize_construction (p);
    }
    virtual ~NewBinaryOperator () {
      uninit_binary_couplings(this, _left, _right, _result, _action, _c_left, _c_right);
    }
    void impl_activate () override { _c_left.enable(); _c_right.enable (); _action.activate (); }
    void impl_deactivate () override { _c_left.disable (); _c_right.disable (); _action.deactivate ();};
    void serialize (const string& type) override {
      AbstractSerializer::pre_serialize(this, type);
      AbstractSerializer::serializer->start ("base:" + std::string(name_info<BinaryFunction>::serialize));
      AbstractSerializer::serializer->text_attribute ("id", get_name ());
      AbstractSerializer::serializer->cpptype_attribute ("left", _left.get_value ());
      AbstractSerializer::serializer->cpptype_attribute ("right", _right.get_value ());
      AbstractSerializer::serializer->end ();
      AbstractSerializer::post_serialize(this);
    }

  protected:
    void set_parent (Process* p) override {
      // in case of re-parenting remove edge dependency in graph
      if (get_parent ()) {
         remove_state_dependency (get_parent (), &_action);
      }
      add_state_dependency (p, &_action);
      Process::set_parent (p);
    }
  public:
    Left _left;
    Right _right;
    Result _result;
    Action _action;
    Coupling _c_left, _c_right;
  };




  void
  init_unary_couplings (Process& _input, Process& _output, Process& _action, Coupling& _coupling);
  void
  uninit_unary_couplings (Process * this_, Process& _input, Process& _output, Process& _action, Coupling& _coupling);


  template <typename Input, typename Output, typename UnaryFunction, typename Input_init>
  class NewUnaryOperator;

  template <typename Input, typename Output, typename UnaryFunction, typename Input_init>
  class NewUnaryOperatorAction : public Action
  {
  public:
    typedef NewUnaryOperator<Input, Output, UnaryFunction, Input_init> UnOperator;

    NewUnaryOperatorAction (UnOperator& unop) : _unop(unop) {}
    NewUnaryOperatorAction (Process* p, const string& n, UnOperator& unop) : Action(p,n), _unop(unop) {
      Process::finalize_construction (p);
    }
    virtual ~NewUnaryOperatorAction () {};
    void impl_activate () override {
      _unop._output.set_value (UnaryFunction()(_unop._input.get_value ()), true);
      //UnaryFunction()(_unop.input, _unop.output);
    }
    void impl_deactivate () override {}
  protected:
    UnOperator& _unop;
  };

  template <typename Input, typename Output, typename UnaryFunction, typename Input_init>
  class NewUnaryOperator : public Process
  {
  public:
    typedef NewUnaryOperatorAction<Input, Output, UnaryFunction, Input_init> Action;

    NewUnaryOperator (Process *p, const string &n, const Input_init& i_val)
    : Process (n),
      _input(this, "input", i_val),
      _output(this, "output", UnaryFunction()(i_val)),
      _action(this, "action", *this),
      _coupling(&_input, ACTIVATION, &_action, ACTIVATION)
    {
      init_unary_couplings(_input, _output, _action, _coupling);
      Process::finalize_construction (p);
    }
    virtual ~NewUnaryOperator () {
      uninit_unary_couplings(this, _input, _output, _action, _coupling);
    }
    void impl_activate () override { _coupling.enable (); _action.activate (); }
    void impl_deactivate () override { _coupling.disable (); _action.deactivate ();};
    void serialize (const string& type) override {
      AbstractSerializer::pre_serialize(this, type);
      AbstractSerializer::serializer->start ("base:" + std::string(name_info<UnaryFunction>::serialize));
      AbstractSerializer::serializer->text_attribute ("id", get_name ());
      AbstractSerializer::serializer->cpptype_attribute ("input", _input.get_value ());
      AbstractSerializer::serializer->end ();
      AbstractSerializer::post_serialize(this);
    }

  protected:
    void set_parent (Process* p) override {
      // in case of re-parenting remove edge dependency in graph
      if (get_parent ()) {
         remove_state_dependency (get_parent (), &_action);
      }
      add_state_dependency (p, &_action);
      Process::set_parent (p);
    }
  public:
    Input _input;
    Output _output;
    Action _action;
    Coupling _coupling;
  };

#endif

  class BinaryOperatorAction : public Action
  {
  public:
    BinaryOperatorAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
                          AbstractProperty* result);
    virtual ~BinaryOperatorAction () {};
  protected:
    AbstractProperty* _left;
    AbstractProperty* _right;
    AbstractProperty* _result;
  };

  class BinaryOperator : public Process
  {
  public:
    BinaryOperator (Process *p, const string &n);
    void impl_activate () { _c_left->enable(); _c_right->enable (); _action->activate (); };
    void impl_deactivate () { _c_left->disable (); _c_right->disable (); _action->deactivate ();};
    virtual ~BinaryOperator ();
  protected:
    void init_couplings (Process* action);
    void set_parent (Process* p);
    AbstractProperty *_left, *_right, *_result;
    Coupling *_c_left, *_c_right;
    Process *_action;
  };

  class UnaryOperatorAction : public Action
  {
  public:
    UnaryOperatorAction (Process* parent, const string &name, AbstractProperty* input, AbstractProperty* output);
    virtual ~UnaryOperatorAction () {};
  protected:
    AbstractProperty* _input;
    AbstractProperty* _output;
  };

  class UnaryOperator : public Process
  {
  public:
    UnaryOperator (Process *p, const string &n);
    void impl_activate () { _c_input->enable (); _action->activate ();}
    void impl_deactivate () { _c_input->disable (); _action->deactivate ();}
    virtual ~UnaryOperator ();
  protected:
    void init_couplings (Process* action);
    AbstractProperty* _input, *_output;
    Coupling *_c_input;
    Process *_action;
  };
}
