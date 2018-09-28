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

#include "../core/tree/process.h"
#include "../core/tree/bool_property.h"
#include "../core/tree/int_property.h"
#include "../core/tree/double_property.h"
#include "../core/control/coupling.h"

#include <string>

namespace djnn {
  using namespace std;
  typedef union {
    bool b_val;
    int i_val;
    double d_val;
    Process* r_val;
  } init_val;

  class BinaryOperatorAction : public Process
  {
  public:
    BinaryOperatorAction (Process* parent, const string &name, shared_ptr<AbstractProperty> left, shared_ptr<AbstractProperty> right,
                          shared_ptr<AbstractProperty> result) :
    Process (parent, name), _left (left), _right (right), _result (result) {}
    virtual ~BinaryOperatorAction () {};
  protected:
    shared_ptr<AbstractProperty> _left;
    shared_ptr<AbstractProperty> _right;
    shared_ptr<AbstractProperty> _result;
  };

  class BinaryOperator : public Process
  {
  public:
    BinaryOperator (Process *p, const string &n);
    void activate () { _c_left->enable(); _c_right->enable (); _action->activation (); };
    void deactivate () { _c_left->disable (); _c_right->disable (); _action->deactivation ();};
    virtual ~BinaryOperator ();
  protected:
    void init_couplings (shared_ptr <Process> action);
    shared_ptr<AbstractProperty> _left, _right, _result;
    unique_ptr<Coupling> _c_left, _c_right;
    shared_ptr<Process> _action;
  };

  class UnaryOperatorAction : public Process
  {
  public:
    UnaryOperatorAction (Process* parent, const string &name, shared_ptr<AbstractProperty> input, shared_ptr<AbstractProperty> output) :
    Process (parent, name), _input (input), _output (output){}
    virtual ~UnaryOperatorAction () {};
  protected:
    shared_ptr<AbstractProperty> _input;
    shared_ptr<AbstractProperty> _output;
  };
  class UnaryOperator : public Process
  {
  public:
    UnaryOperator (Process *p, const string &n);
    void activate () { _c_input->enable (); _action->activation ();}
    void deactivate () { _c_input->disable (); _action->deactivation ();}
    virtual ~UnaryOperator ();
  protected:
    void init_couplings (shared_ptr <Process> action);
    shared_ptr<AbstractProperty> _input, _output;
    unique_ptr<Coupling> _c_input;
    shared_ptr<Process> _action;
  };
}
