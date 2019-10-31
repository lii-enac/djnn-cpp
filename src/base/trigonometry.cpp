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

#include "trigonometry.h"
#include "../core/serializer/serializer.h"

namespace djnn
{

#if NEW_OP

  template <> const char name_info<my_cos<double>>::left[] = "left";
  template <> const char name_info<my_cos<double>>::right[] = "right";
  template <> const char name_info<my_cos<double>>::serialize[] = "cosine";

  template <> const char name_info<my_sin<double>>::left[] = "left";
  template <> const char name_info<my_sin<double>>::right[] = "right";
  template <> const char name_info<my_sin<double>>::serialize[] = "sine";

  template <> const char name_info<my_tan<double>>::left[] = "left";
  template <> const char name_info<my_tan<double>>::right[] = "right";
  template <> const char name_info<my_tan<double>>::serialize[] = "tangent";

  template <> const char name_info<my_acos<double>>::left[] = "left";
  template <> const char name_info<my_acos<double>>::right[] = "right";
  template <> const char name_info<my_acos<double>>::serialize[] = "arccosine";

  template <> const char name_info<my_asin<double>>::left[] = "left";
  template <> const char name_info<my_asin<double>>::right[] = "right";
  template <> const char name_info<my_asin<double>>::serialize[] = "arcsine";

  template <> const char name_info<my_atan<double>>::left[] = "left";
  template <> const char name_info<my_atan<double>>::right[] = "right";
  template <> const char name_info<my_atan<double>>::serialize[] = "arctangent";

  template <> const char name_info<my_atan2<double>>::left[] = "left";
  template <> const char name_info<my_atan2<double>>::right[] = "right";
  template <> const char name_info<my_atan2<double>>::serialize[] = "arctangent2";

  template <> const char name_info<my_cosh<double>>::left[] = "left";
  template <> const char name_info<my_cosh<double>>::right[] = "right";
  template <> const char name_info<my_cosh<double>>::serialize[] = "hyperboliccosine";

  template <> const char name_info<my_sinh<double>>::left[] = "left";
  template <> const char name_info<my_sinh<double>>::right[] = "right";
  template <> const char name_info<my_sinh<double>>::serialize[] = "hyperbolicsine";

  template <> const char name_info<my_tanh<double>>::left[] = "left";
  template <> const char name_info<my_tanh<double>>::right[] = "right";
  template <> const char name_info<my_tanh<double>>::serialize[] = "hyperbolictangent";

  template <> const char name_info<my_acosh<double>>::left[] = "left";
  template <> const char name_info<my_acosh<double>>::right[] = "right";
  template <> const char name_info<my_acosh<double>>::serialize[] = "hyperbolicarccosine";

  template <> const char name_info<my_asinh<double>>::left[] = "left";
  template <> const char name_info<my_asinh<double>>::right[] = "right";
  template <> const char name_info<my_asinh<double>>::serialize[] = "hyperbolicarcsine";

  template <> const char name_info<my_atanh<double>>::left[] = "left";
  template <> const char name_info<my_atanh<double>>::right[] = "right";
  template <> const char name_info<my_atanh<double>>::serialize[] = "hyperbolicarctangent";
  
#else

  Cosine::Cosine (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", in_val);
    _output = new DoubleProperty (this, "output", cos (in_val));
    init_couplings (new CosineAction (this, "action", _input, _output));
    Process::finalize_construction (p);
  }

  void
  Cosine::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:cosine");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  Sine::Sine (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", in_val);
    _output = new DoubleProperty (this, "output", sin (in_val));
    init_couplings (new SineAction (this, "action", _input, _output));
    Process::finalize_construction (p);
  }

  void
  Sine::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:sine");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  Tangent::Tangent (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", in_val);
    _output = new DoubleProperty (this, "output", tan (in_val));
    init_couplings (new TangentAction (this, "action", _input, _output));
    Process::finalize_construction (p);
  }

  void
  Tangent::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:tangent");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  ArcCosine::ArcCosine (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", in_val);
    _output = new DoubleProperty (this, "output", acos (in_val));
    init_couplings (new ArcCosineAction (this, "action", _input, _output));
    Process::finalize_construction (p);
  }

  void
  ArcCosine::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:arccosine");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  ArcSine::ArcSine (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", in_val);
    _output = new DoubleProperty (this, "output", asin (in_val));
    init_couplings (new ArcSineAction (this, "action", _input, _output));
    Process::finalize_construction (p);
  }

  void
  ArcSine::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:arcsine");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  ArcTangent::ArcTangent (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", in_val);
    _output = new DoubleProperty (this, "output", atan (in_val));
    init_couplings (new ArcTangentAction (this, "action", _input, _output));
    Process::finalize_construction (p);
  }

  void
  ArcTangent::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:arctangent");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  ArcTangent2::ArcTangent2 (Process *p, const string &n, double y_val, double x_val) :
      BinaryOperator (p, n)
  {
    _left = new DoubleProperty (this, "y", y_val);
    _right = new DoubleProperty (this, "x", x_val);
    _result = new DoubleProperty (this, "result", atan2 (y_val, x_val));
    init_couplings (new ArcTangent2Action (this, "action", _left, _right, _result));
    Process::finalize_construction (p);
  }

  void
  ArcTangent2::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:arctangent2");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("y", dynamic_cast<DoubleProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->float_attribute ("x", dynamic_cast<DoubleProperty*> (_right)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  HyperbolicCosine::HyperbolicCosine (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", in_val);
    _output = new DoubleProperty (this, "output", cosh (in_val));
    init_couplings (new HyperbolicCosineAction (this, "action", _input, _output));
    Process::finalize_construction (p);
  }

  void
  HyperbolicCosine::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:hyperboliccosine");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  HyperbolicSine::HyperbolicSine (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", in_val);
    _output = new DoubleProperty (this, "output", sinh (in_val));
    init_couplings (new HyperbolicSineAction (this, "action", _input, _output));
    Process::finalize_construction (p);
  }

  void
  HyperbolicSine::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:hyperbolicsine");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  HyperbolicTangent::HyperbolicTangent (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", in_val);
    _output = new DoubleProperty (this, "output", tanh (in_val));
    init_couplings (new HyperbolicTangentAction (this, "action", _input, _output));
    Process::finalize_construction (p);
  }

  void
  HyperbolicTangent::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:hyperbolictangent");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  HyperbolicArcCosine::HyperbolicArcCosine (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", in_val);
    _output = new DoubleProperty (this, "output", acosh (in_val));
    init_couplings (new HyperbolicArcCosineAction (this, "action", _input, _output));
    Process::finalize_construction (p);
  }

  void
  HyperbolicArcCosine::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:hyperbolicarccosine");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  HyperbolicArcSine::HyperbolicArcSine (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", in_val);
    _output = new DoubleProperty (this, "output", asinh (in_val));
    init_couplings (new HyperbolicArcSineAction (this, "action", _input, _output));
    Process::finalize_construction (p);
  }

  void
  HyperbolicArcSine::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:hyperbolicarcsine");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  HyperbolicArcTangent::HyperbolicArcTangent (Process *p, const string &n, double in_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", in_val);
    _output = new DoubleProperty (this, "output", atanh (in_val));
    init_couplings (new HyperbolicArcTangentAction (this, "action", _input, _output));
    Process::finalize_construction (p);
  }

  void
  HyperbolicArcTangent::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:hyperbolicarctangent");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif
}

