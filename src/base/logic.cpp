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

#include "logic.h"

namespace djnn
{
  And::And (Process *p, const string &n, int l_val, int r_val) :
      BinaryOperator (p, n)
  {
    _left = shared_ptr<AbstractProperty> (new BoolProperty (this, "left", l_val));
    _right = shared_ptr<AbstractProperty> (new BoolProperty (this, "right", r_val));
    _result = shared_ptr<AbstractProperty> (new BoolProperty (this, "result", l_val && r_val));
    init_couplings (shared_ptr<Process> (new AndAction (this, "action", _left, _right, _result)));
    Process::finalize ();
  }

  Or::Or (Process *p, const string &n, int l_val, int r_val) :
      BinaryOperator (p, n)
  {
    _left = shared_ptr<AbstractProperty> (new BoolProperty (this, "left", l_val));
    _right = shared_ptr<AbstractProperty> (new BoolProperty (this, "right", r_val));
    _result = shared_ptr<AbstractProperty> (new BoolProperty (this, "result", l_val || r_val));
    init_couplings (shared_ptr<Process> (new OrAction (this, "action", _left, _right, _result)));
    Process::finalize ();
  }

  XOr::XOr (Process *p, const string &n, int l_val, int r_val) :
      BinaryOperator (p, n)
  {
    _left = shared_ptr<AbstractProperty> (new BoolProperty (this, "left", l_val));
    _right = shared_ptr<AbstractProperty> (new BoolProperty (this, "right", r_val));
    _result = shared_ptr<AbstractProperty> (new BoolProperty (this, "result", l_val != r_val));
    init_couplings (shared_ptr<Process> (new XOrAction (this, "action", _left, _right, _result)));
    Process::finalize ();
  }

  Not::Not (Process *p, const string &n, int in_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new BoolProperty (this, "input", in_val));
    _output = shared_ptr<AbstractProperty> (new BoolProperty (this, "output", !in_val));
    init_couplings (shared_ptr<Process> (new NotAction (this, "action", _input, _output)));
    Process::finalize ();
  }
}

