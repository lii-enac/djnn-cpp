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

#include "trigonometry.h"

namespace djnn
{
  Cosine::Cosine (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", in_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", cos (in_val)));
    init_couplings (shared_ptr<Process> (new CosineAction (this, "action", _input, _output)));
    Process::finalize ();
  }

  Sine::Sine (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", in_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", sin (in_val)));
    init_couplings (shared_ptr<Process> (new SineAction (this, "action", _input, _output)));
    Process::finalize ();
  }

  Tangent::Tangent (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", in_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", tan (in_val)));
    init_couplings (shared_ptr<Process> (new TangentAction (this, "action", _input, _output)));
    Process::finalize ();
  }

  ArcCosine::ArcCosine (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", in_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", acos (in_val)));
    init_couplings (shared_ptr<Process> (new ArcCosineAction (this, "action", _input, _output)));
    Process::finalize ();
  }

  ArcSine::ArcSine (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", in_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", asin (in_val)));
    init_couplings (shared_ptr<Process> (new ArcSineAction (this, "action", _input, _output)));
    Process::finalize ();
  }

  ArcTangent::ArcTangent (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", in_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", atan (in_val)));
    init_couplings (shared_ptr<Process> (new ArcTangentAction (this, "action", _input, _output)));
    Process::finalize ();
  }

  ArcTangent2::ArcTangent2 (Process *p, const string &n, double y_val, double x_val) :
      BinaryOperator (p, n)
  {
    _left = shared_ptr<AbstractProperty> (new DoubleProperty (this, "y", y_val));
    _right = shared_ptr<AbstractProperty> (new DoubleProperty (this, "x", x_val));
    _result = shared_ptr<AbstractProperty> (new DoubleProperty (this, "result", atan2 (y_val, x_val)));
    init_couplings (shared_ptr<Process> (new ArcTangent2Action (this, "action", _left, _right, _result)));
    Process::finalize ();
  }

  HyperbolicCosine::HyperbolicCosine (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", in_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", cosh (in_val)));
    init_couplings (shared_ptr<Process> (new HyperbolicCosineAction (this, "action", _input, _output)));
    Process::finalize ();
  }

  HyperbolicSine::HyperbolicSine (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", in_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", sinh (in_val)));
    init_couplings (shared_ptr<Process> (new HyperbolicSineAction (this, "action", _input, _output)));
    Process::finalize ();
  }

  HyperbolicTangent::HyperbolicTangent (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", in_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", tanh (in_val)));
    init_couplings (shared_ptr<Process> (new HyperbolicTangentAction (this, "action", _input, _output)));
    Process::finalize ();
  }

  HyperbolicArcCosine::HyperbolicArcCosine (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", in_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", acosh (in_val)));
    init_couplings (shared_ptr<Process> (new HyperbolicArcCosineAction (this, "action", _input, _output)));
    Process::finalize ();
  }

  HyperbolicArcSine::HyperbolicArcSine (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", in_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", asinh (in_val)));
    init_couplings (shared_ptr<Process> (new HyperbolicArcSineAction (this, "action", _input, _output)));
    Process::finalize ();
  }

  HyperbolicArcTangent::HyperbolicArcTangent (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", in_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", atanh (in_val)));
    init_couplings (shared_ptr<Process> (new HyperbolicArcTangentAction (this, "action", _input, _output)));
    Process::finalize ();
  }
}

