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

#include "math_functions.h"

namespace djnn
{
  Exp::Exp (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", i_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", exp (i_val)));
    init_couplings (shared_ptr<Process> (new ExpAction (this, "action", _input, _output)));
    Process::finalize ();
  }

  Log::Log (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", i_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", log (i_val)));
    init_couplings (shared_ptr<Process> (new LogAction (this, "action", _input, _output)));
    Process::finalize ();
  }

  Log10::Log10 (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", i_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", log10 (i_val)));
    init_couplings (shared_ptr<Process> (new Log10Action (this, "action", _input, _output)));
    Process::finalize ();
  }

  Pow::Pow (Process *p, const string &n, double base, double exponent) :
      BinaryOperator (p, n)
  {
    _left = shared_ptr<AbstractProperty> (new DoubleProperty (this, "base", base));
    _right = shared_ptr<AbstractProperty> (new DoubleProperty (this, "exponent", exponent));
    _result = shared_ptr<AbstractProperty> (new DoubleProperty (this, "result", pow (base, exponent)));
    init_couplings (shared_ptr<Process> (new PowAction (this, "action", _left, _right, _result)));
    Process::finalize ();
  }

  Sqrt::Sqrt (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", i_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", sqrt (i_val)));
    init_couplings (shared_ptr<Process> (new SqrtAction (this, "action", _input, _output)));
    Process::finalize ();
  }
}

