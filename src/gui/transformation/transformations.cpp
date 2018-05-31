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

#include "../transformation/transformations.h"

#include "../backend.h"
#include "../abstract_backend.h"

namespace djnn
{
  AbstractTranslation::AbstractTranslation (Process *p, const string &n, double tx, double ty) :
      AbstractGObj (p, n)
  {
    _tx = new DoubleProperty (this, "tx", tx);
    _ty = new DoubleProperty (this, "ty", ty);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _ctx = new Coupling (_tx, ACTIVATION, update, ACTIVATION);
    _cty = new Coupling (_ty, ACTIVATION, update, ACTIVATION);
  }

  AbstractTranslation::~AbstractTranslation ()
  {
    delete _ctx;
    delete _cty;
    delete _tx;
    delete _ty;
  }

  void
  AbstractTranslation::activate ()
  {
    AbstractGObj::activate ();
    _ctx->enable (_frame);
    _cty->enable (_frame);
  }

  void
  AbstractTranslation::deactivate ()
  {
    AbstractGObj::deactivate ();
    _ctx->disable ();
    _cty->disable ();
  }

  Translation::Translation (Process *p, const string &n, double tx, double ty) :
      AbstractTranslation (p, n, tx, ty)
  {
    Process::finalize ();
  }

  Translation::~Translation ()
  {
  }

  void
  Translation::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      double tx = _tx->get_value ();
      double ty = _ty->get_value ();
      Backend::instance ()->load_translation (tx, ty);
    }
  }
  GradientTranslation::GradientTranslation (Process *p, const string &n, double tx, double ty) :
      AbstractTranslation (p, n, tx, ty)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (p);
    if (grad == nullptr) {
      cerr << "Parent of gradient translation must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
  }

  GradientTranslation::~GradientTranslation ()
  {
  }

  void
  GradientTranslation::draw ()
  {
    cout << "gradient translation\n";
    double tx = _tx->get_value ();
    double ty = _ty->get_value ();
    Backend::instance ()->load_gradient_translation (tx, ty);
  }

  AbstractRotation::AbstractRotation (Process *p, const string &n, double a, double cx, double cy) :
      AbstractGObj (p, n)
  {
    _a = new DoubleProperty (this, "a", a);
    _cx = new DoubleProperty (this, "cx", cx);
    _cy = new DoubleProperty (this, "cy", cy);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _ca = new Coupling (_a, ACTIVATION, update, ACTIVATION);
    _ccx = new Coupling (_cx, ACTIVATION, update, ACTIVATION);
    _ccy = new Coupling (_cy, ACTIVATION, update, ACTIVATION);
  }

  AbstractRotation::~AbstractRotation ()
  {
    delete _ca;
    delete _ccx;
    delete _ccy;
    delete _a;
    delete _cx;
    delete _cy;
  }

  void
  AbstractRotation::activate ()
  {
    AbstractGObj::activate ();
    _ca->enable (_frame);
    _ccx->enable (_frame);
    _ccy->enable (_frame);
  }

  void
  AbstractRotation::deactivate ()
  {
    AbstractGObj::deactivate ();
    _ca->disable ();
    _ccx->disable ();
    _ccy->disable ();
  }

  Rotation::Rotation (Process *p, const string &n, double a, double cx, double cy) :
      AbstractRotation (p, n, a, cx, cy)
  {
    Process::finalize ();
  }

  Rotation::~Rotation ()
  {
  }

  void
  Rotation::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      double a = _a->get_value ();
      double cx = _cx->get_value ();
      double cy = _cy->get_value ();
      Backend::instance ()->load_rotation (a, cx, cy);
    }
  }
  GradientRotation::GradientRotation (Process *p, const string &n, double a, double cx, double cy) :
      AbstractRotation (p, n, a, cx, cy)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (p);
    if (grad == nullptr) {
      cerr << "Parent of gradient rotation must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
  }

  GradientRotation::~GradientRotation ()
  {
  }

  void
  GradientRotation::draw ()
  {
    double a = _a->get_value ();
    double cx = _cx->get_value ();
    double cy = _cy->get_value ();
    Backend::instance ()->load_gradient_rotation (a, cx, cy);
  }

  AbstractScaling::AbstractScaling (Process *p, const string &n, double sx, double sy, double cx, double cy) :
      AbstractGObj (p, n)
  {
    _sx = new DoubleProperty (this, "sx", sx);
    _sy = new DoubleProperty (this, "sy", sy);
    _cx = new DoubleProperty (this, "cx", cx);
    _cy = new DoubleProperty (this, "cy", cy);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _csx = new Coupling (_sx, ACTIVATION, update, ACTIVATION);
    _csy = new Coupling (_sy, ACTIVATION, update, ACTIVATION);
    _ccx = new Coupling (_cx, ACTIVATION, update, ACTIVATION);
    _ccy = new Coupling (_cy, ACTIVATION, update, ACTIVATION);
  }

  AbstractScaling::~AbstractScaling ()
  {
    delete _csx;
    delete _csy;
    delete _ccx;
    delete _ccy;
    delete _sx;
    delete _sy;
    delete _cx;
    delete _cy;
  }

  void
  AbstractScaling::activate ()
  {
    AbstractGObj::activate ();
    _csx->enable (_frame);
    _csy->enable (_frame);
    _ccx->enable (_frame);
    _ccy->enable (_frame);
  }

  void
  AbstractScaling::deactivate ()
  {
    AbstractGObj::deactivate ();
    _csx->disable ();
    _csy->disable ();
    _ccx->disable ();
    _ccy->disable ();
  }

  Scaling::Scaling (Process *p, const string &n, double sx, double sy, double cx, double cy) :
      AbstractScaling (p, n, sx, sy, cx, cy)
  {
    Process::finalize ();
  }

  Scaling::~Scaling ()
  {
  }

  void
  Scaling::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      double sx = _sx->get_value ();
      double sy = _sy->get_value ();
      double cx = _cx->get_value ();
      double cy = _cy->get_value ();
      Backend::instance ()->load_scaling (sx, sy, cx, cy);
    }
  }
  GradientScaling::GradientScaling (Process *p, const string &n, double sx, double sy, double cx, double cy) :
      AbstractScaling (p, n, sx, sy, cx, cy)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (p);
    if (grad == nullptr) {
      cerr << "Parent of gradient scaling must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
  }

  GradientScaling::~GradientScaling ()
  {
  }

  void
  GradientScaling::draw ()
  {
    double sx = _sx->get_value ();
    double sy = _sy->get_value ();
    double cx = _cx->get_value ();
    double cy = _cy->get_value ();
    Backend::instance ()->load_gradient_scaling (sx, sy, cx, cy);
  }

  AbstractSkew::AbstractSkew (Process *p, const string &n, double a) :
      AbstractGObj (p, n)
  {
    _a = new DoubleProperty (this, "a", a);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _ca = new Coupling (_a, ACTIVATION, update, ACTIVATION);
  }

  AbstractSkew::~AbstractSkew ()
  {
    delete _ca;
    delete _a;
  }

  void
  AbstractSkew::activate ()
  {
    AbstractGObj::activate ();
    _ca->enable (_frame);
  }

  void
  AbstractSkew::deactivate ()
  {
    AbstractGObj::deactivate ();
    _ca->disable ();
  }

  SkewX::SkewX (Process *p, const string &n, double a) :
      AbstractSkew (p, n, a)
  {
    Process::finalize ();
  }

  SkewX::~SkewX ()
  {
  }

  void
  SkewX::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      double a = _a->get_value ();
      Backend::instance ()->load_skew_x (a);
    }
  }

  GradientSkewX::GradientSkewX (Process *p, const string &n, double a) :
      AbstractSkew (p, n, a)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (p);
    if (grad == nullptr) {
      cerr << "Parent of gradient skewX must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
  }

  GradientSkewX::~GradientSkewX ()
  {
  }

  void
  GradientSkewX::draw ()
  {
    double a = _a->get_value ();
    Backend::instance ()->load_gradient_skew_x (a);
  }

  SkewY::SkewY (Process *p, const string &n, double a) :
      AbstractSkew (p, n, a)
  {
    Process::finalize ();
  }

  SkewY::~SkewY ()
  {
    //AbstractSkew::~AbstractSkew ();
  }

  void
  SkewY::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      double a = _a->get_value ();
      Backend::instance ()->load_skew_y (a);
    }
  }

  GradientSkewY::GradientSkewY (Process *p, const string &n, double a) :
      AbstractSkew (p, n, a)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (p);
    if (grad == nullptr) {
      cerr << "Parent of gradient skewY must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
  }

  GradientSkewY::~GradientSkewY ()
  {
  }

  void
  GradientSkewY::draw ()
  {
    double a = _a->get_value ();
    Backend::instance ()->load_gradient_skew_y (a);
  }

  AbstractHomography::AbstractHomography (Process *p, const string &n, double m11, double m12, double m13, double m14,
                                          double m21, double m22, double m23, double m24, double m31, double m32,
                                          double m33, double m34, double m41, double m42, double m43, double m44) :
      AbstractGObj (p, n)
  {
    _m11 = new DoubleProperty (this, "m11", m11);
    _m12 = new DoubleProperty (this, "m12", m12);
    _m13 = new DoubleProperty (this, "m13", m13);
    _m14 = new DoubleProperty (this, "m14", m14);

    _m21 = new DoubleProperty (this, "m21", m21);
    _m22 = new DoubleProperty (this, "m22", m22);
    _m23 = new DoubleProperty (this, "m23", m23);
    _m24 = new DoubleProperty (this, "m24", m24);

    _m31 = new DoubleProperty (this, "m31", m31);
    _m32 = new DoubleProperty (this, "m32", m32);
    _m33 = new DoubleProperty (this, "m33", m33);
    _m34 = new DoubleProperty (this, "m34", m34);

    _m41 = new DoubleProperty (this, "m41", m41);
    _m42 = new DoubleProperty (this, "m42", m42);
    _m43 = new DoubleProperty (this, "m43", m43);
    _m44 = new DoubleProperty (this, "m44", m44);

    UpdateDrawing *update = UpdateDrawing::instance ();
    _cm11 = new Coupling (_m11, ACTIVATION, update, ACTIVATION);
    _cm12 = new Coupling (_m12, ACTIVATION, update, ACTIVATION);
    _cm13 = new Coupling (_m13, ACTIVATION, update, ACTIVATION);
    _cm14 = new Coupling (_m14, ACTIVATION, update, ACTIVATION);

    _cm21 = new Coupling (_m21, ACTIVATION, update, ACTIVATION);
    _cm22 = new Coupling (_m22, ACTIVATION, update, ACTIVATION);
    _cm23 = new Coupling (_m23, ACTIVATION, update, ACTIVATION);
    _cm24 = new Coupling (_m24, ACTIVATION, update, ACTIVATION);

    _cm31 = new Coupling (_m31, ACTIVATION, update, ACTIVATION);
    _cm32 = new Coupling (_m32, ACTIVATION, update, ACTIVATION);
    _cm33 = new Coupling (_m33, ACTIVATION, update, ACTIVATION);
    _cm34 = new Coupling (_m34, ACTIVATION, update, ACTIVATION);

    _cm41 = new Coupling (_m41, ACTIVATION, update, ACTIVATION);
    _cm42 = new Coupling (_m42, ACTIVATION, update, ACTIVATION);
    _cm43 = new Coupling (_m43, ACTIVATION, update, ACTIVATION);
    _cm44 = new Coupling (_m44, ACTIVATION, update, ACTIVATION);
  }

  AbstractHomography::~AbstractHomography ()
  {
    delete _cm11;
    delete _cm12;
    delete _cm13;
    delete _cm14;

    delete _cm21;
    delete _cm22;
    delete _cm23;
    delete _cm24;

    delete _cm31;
    delete _cm32;
    delete _cm33;
    delete _cm34;

    delete _cm41;
    delete _cm42;
    delete _cm43;
    delete _cm44;

    delete _m11;
    delete _m12;
    delete _m13;
    delete _m14;

    delete _m21;
    delete _m22;
    delete _m23;
    delete _m24;

    delete _m31;
    delete _m32;
    delete _m33;
    delete _m34;

    delete _m41;
    delete _m42;
    delete _m43;
    delete _m44;
  }

  void
  AbstractHomography::activate ()
  {
    AbstractGObj::activate ();
    _cm11->enable (_frame);
    _cm12->enable (_frame);
    _cm13->enable (_frame);
    _cm14->enable (_frame);

    _cm21->enable (_frame);
    _cm22->enable (_frame);
    _cm23->enable (_frame);
    _cm24->enable (_frame);

    _cm31->enable (_frame);
    _cm32->enable (_frame);
    _cm33->enable (_frame);
    _cm34->enable (_frame);

    _cm41->enable (_frame);
    _cm42->enable (_frame);
    _cm43->enable (_frame);
    _cm44->enable (_frame);
  }

  void
  AbstractHomography::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cm11->disable ();
    _cm12->disable ();
    _cm13->disable ();
    _cm14->disable ();

    _cm21->disable ();
    _cm22->disable ();
    _cm23->disable ();
    _cm24->disable ();

    _cm31->disable ();
    _cm32->disable ();
    _cm33->disable ();
    _cm34->disable ();

    _cm41->disable ();
    _cm42->disable ();
    _cm43->disable ();
    _cm44->disable ();
  }

  Homography::Homography (Process *p, const string &n, double m11, double m12, double m13, double m14, double m21,
                          double m22, double m23, double m24, double m31, double m32, double m33, double m34,
                          double m41, double m42, double m43, double m44) :
      AbstractHomography (p, n, m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44)
  {
    Process::finalize ();
  }

  Homography::~Homography ()
  {
  }

  void
  Homography::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      double m11 = _m11->get_value ();
      double m12 = _m12->get_value ();
      double m13 = _m13->get_value ();
      double m14 = _m14->get_value ();

      double m21 = _m21->get_value ();
      double m22 = _m22->get_value ();
      double m23 = _m23->get_value ();
      double m24 = _m24->get_value ();

      double m31 = _m31->get_value ();
      double m32 = _m32->get_value ();
      double m33 = _m33->get_value ();
      double m34 = _m34->get_value ();

      double m41 = _m41->get_value ();
      double m42 = _m42->get_value ();
      double m43 = _m43->get_value ();
      double m44 = _m44->get_value ();
      Backend::instance ()->load_homography (m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43,
                                             m44);
    }
  }

  GradientHomography::GradientHomography (Process *p, const string &n, double m11, double m12, double m13, double m21,
                                          double m22, double m23, double m31, double m32, double m33) :
      AbstractHomography (p, n, m11, m12, m13, 0, m21, m22, m23, 0, m31, m32, m33, 0, 0, 0, 0, 1)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (p);
    if (grad == nullptr) {
      cerr << "Parent of gradient homography must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
  }

  GradientHomography::~GradientHomography ()
  {
  }

  void
  GradientHomography::draw ()
  {
    double m11 = _m11->get_value ();
    double m12 = _m12->get_value ();
    double m13 = _m13->get_value ();

    double m21 = _m21->get_value ();
    double m22 = _m22->get_value ();
    double m23 = _m23->get_value ();

    double m31 = _m31->get_value ();
    double m32 = _m32->get_value ();
    double m33 = _m33->get_value ();

    Backend::instance ()->load_gradient_homography (m11, m12, m13, m21, m22, m23, m31, m32, m33);
  }

  SimpleGradientTransform::SimpleGradientTransform (Process *p, const string &n, double a, double b, double c, double d,
                                                    double e, double f) :
      AbstractHomography (p, n, a, b, 0, 0, c, d, 0, 0, e, f, 1, 0, 0, 0, 0, 1)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (p);
    if (grad == nullptr) {
      cerr << "Parent of simple gradient transform must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
  }

  SimpleGradientTransform::~SimpleGradientTransform ()
  {
  }

  void
  SimpleGradientTransform::draw ()
  {
    double m11 = _m11->get_value ();
    double m12 = _m12->get_value ();
    double m13 = _m13->get_value ();

    double m21 = _m21->get_value ();
    double m22 = _m22->get_value ();
    double m23 = _m23->get_value ();

    double m31 = _m31->get_value ();
    double m32 = _m32->get_value ();
    double m33 = _m33->get_value ();

    Backend::instance ()->load_gradient_homography (m11, m12, m13, m21, m22, m23, m31, m32, m33);
  }
}

