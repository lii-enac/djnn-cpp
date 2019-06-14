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

#include "../core/ontology/process.h"
#include "../core/tree/bool_property.h"
#include "../core/tree/int_property.h"
#include "../core/tree/double_property.h"
#include "../core/ontology/coupling.h"

#include <string>

namespace djnn {
  using namespace std;
  typedef union {
    bool b_val;
    int i_val;
    double d_val;
    Process* r_val;
  } init_val;

  class BinaryOperatorAction : public Action
  {
  public:
    BinaryOperatorAction (Process* parent, const string &name, AbstractProperty* left, AbstractProperty* right,
                          AbstractProperty* result) :
    Action (parent, name), _left (left), _right (right), _result (result) {}
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
    void activate () { _c_left->enable(); _c_right->enable (); _action->activation (); };
    void deactivate () { _c_left->disable (); _c_right->disable (); _action->deactivation ();};
    virtual ~BinaryOperator ();
  protected:
    void init_couplings (Process* action);
    AbstractProperty *_left, *_right, *_result;
    Coupling *_c_left, *_c_right;
    Process *_action;
  };

  class UnaryOperatorAction : public Action
  {
  public:
    UnaryOperatorAction (Process* parent, const string &name, AbstractProperty* input, AbstractProperty* output) :
    Action (parent, name), _input (input), _output (output){}
    virtual ~UnaryOperatorAction () {};
  protected:
    AbstractProperty* _input;
    AbstractProperty* _output;
  };

  class UnaryOperator : public Process
  {
  public:
    UnaryOperator (Process *p, const string &n);
    void activate () { _c_input->enable (); _action->activation ();}
    void deactivate () { _c_input->disable (); _action->deactivation ();}
    virtual ~UnaryOperator ();
  protected:
    void init_couplings (Process* action);
    AbstractProperty* _input, *_output;
    Coupling *_c_input;
    Process *_action;
  };
}
