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

#include "../abstract_gobj.h"
#include "../../core/tree/process.h"
#include "../../core/control/coupling.h"
#include "../../core/tree/double_property.h"

namespace djnn
{
  using namespace std;

  class AbstractTransformation : public AbstractGObj
  {
  public:
    AbstractTransformation (Process *p, const string &n);
    AbstractTransformation ();
    virtual ~AbstractTransformation ();
  };

  class AbstractTranslation : public AbstractTransformation
  {
  public:
    AbstractTranslation (Process *p, const string &n, double tx, double ty);
    AbstractTranslation (double tx, double ty);
    virtual
    ~AbstractTranslation ();
    void
    activate () override;
    void
    deactivate () override;
    virtual void
    draw () override = 0;
  protected:
    DoubleProperty *_tx, *_ty;
    Coupling *_ctx, *_cty;
  };

  class Translation : public AbstractTranslation
  {
  public:
    Translation (Process *p, const string &n, double tx, double ty);
    virtual
    ~Translation ();
    void
    draw () override;
  };
  class GradientTranslation : public AbstractTranslation
  {
  public:
    GradientTranslation (Process *p, const string &n, double tx, double ty);
    GradientTranslation (double tx, double ty);
    Process* clone () override;
    virtual
    ~GradientTranslation ();
    void
    draw () override;
  };

  class AbstractRotation : public AbstractTransformation
  {
  public:
    AbstractRotation (Process *p, const string &n, double a, double cx, double cy);
    AbstractRotation (double a, double cx, double cy);
    virtual
    ~AbstractRotation ();
    void
    activate () override;
    void
    deactivate () override;
    virtual void
    draw () override = 0;
  protected:
    DoubleProperty *_a, *_cx, *_cy;
    Coupling *_ca, *_ccx, *_ccy;
  };

  class Rotation : public AbstractRotation
  {
  public:
    Rotation (Process *p, const string &n, double a, double cx, double cy);
    virtual
    ~Rotation ();
    void
    draw () override;
  };
  class GradientRotation : public AbstractRotation
  {
  public:
    GradientRotation (Process *p, const string &n, double a, double cx, double cy);
    GradientRotation (double a, double cx, double cy);
    Process* clone () override;
    virtual
    ~GradientRotation ();
    void
    draw () override;
  };

  class AbstractScaling : public AbstractTransformation
  {
  public:
    AbstractScaling (Process *p, const string &n, double sx, double sy, double cx, double cy);
    AbstractScaling (double sx, double sy, double cx, double cy);
    virtual
    ~AbstractScaling ();
    void
    activate () override;
    void
    deactivate () override;
    virtual void
    draw () override = 0;
  protected:
    DoubleProperty *_sx, *_sy, *_cx, *_cy;
    Coupling *_csx, *_csy, *_ccx, *_ccy;
  };

  class Scaling : public AbstractScaling
  {
  public:
    Scaling (Process *p, const string &n, double sx, double sy, double cx, double cy);
    virtual
    ~Scaling ();
    void
    draw () override;
  };

  class GradientScaling : public AbstractScaling
  {
  public:
    GradientScaling (Process *p, const string &n, double sx, double sy, double cx, double cy);
    GradientScaling (double sx, double sy, double cx, double cy);
    Process* clone () override;
    virtual
    ~GradientScaling ();
    void
    draw () override;
  };

  class AbstractSkew : public AbstractTransformation
  {
  public:
    AbstractSkew (Process *p, const string &n, double a);
    AbstractSkew (double a);
    virtual
    ~AbstractSkew ();
    void
    activate () override;
    void
    deactivate () override;
    virtual void
    draw () override = 0;
  protected:
    DoubleProperty *_a;
    Coupling *_ca;
  };

  class SkewX : public AbstractSkew
  {
  public:
    SkewX (Process *p, const string &n, double a);
    SkewX (double a);
    virtual
    ~SkewX ();
    void
    draw () override;
  };

  class GradientSkewX : public AbstractSkew
  {
  public:
    GradientSkewX (Process *p, const string &n, double a);
    GradientSkewX (double a);
    Process* clone () override;
    virtual
    ~GradientSkewX ();
    void
    draw () override;
  };

  class SkewY : public AbstractSkew
  {
  public:
    SkewY (Process *p, const string &n, double a);
    virtual
    ~SkewY ();
    void
    draw () override;
  };

  class GradientSkewY : public AbstractSkew
  {
  public:
    GradientSkewY (Process *p, const string &n, double a);
    GradientSkewY (double a);
    Process* clone () override;
    virtual
    ~GradientSkewY ();
    void
    draw () override;
  };

  class AbstractHomography : public AbstractTransformation
  {
  public:
    AbstractHomography (Process *p, const string &n, double m11, double m12, double m13, double m14,
			double m21, double m22, double m23, double m24, double m31, double m32, double m33, double m34,
			double m41, double m42, double m43, double m44);
    AbstractHomography (double m11, double m12, double m13, double m14,
          double m21, double m22, double m23, double m24, double m31, double m32, double m33, double m34,
          double m41, double m42, double m43, double m44);
    virtual
    ~AbstractHomography ();
    void
    activate () override;
    void
    deactivate () override;
    virtual void
    draw () override = 0;
    DoubleProperty *_m11, *_m12, *_m13, *_m14, *_m21, *_m22, *_m23, *_m24, *_m31, *_m32, *_m33, *_m34, *_m41, *_m42,
	*_m43, *_m44;
  protected:
    Coupling *_cm11, *_cm12, *_cm13, *_cm14, *_cm21, *_cm22, *_cm23, *_cm24, *_cm31, *_cm32, *_cm33, *_cm34, *_cm41,
	*_cm42, *_cm43, *_cm44;
  };

  class Homography : public AbstractHomography
  {
  public:
    Homography (Process *p, const string &n, double m11, double m12, double m13, double m14, double m21,
		double m22, double m23, double m24, double m31, double m32, double m33, double m34, double m41,
		double m42, double m43, double m44);
    virtual
    ~Homography ();
    void
    draw () override;
  };

  class GradientHomography : public AbstractHomography
  {
  public:
    GradientHomography (Process *p, const string &n, double m11, double m12, double m13, double m21,
			double m22, double m23, double m31, double m32, double m33);
    GradientHomography (double m11, double m12, double m13, double m21,
          double m22, double m23, double m31, double m32, double m33);
    Process* clone () override;
    virtual
    ~GradientHomography ();
    void
    draw () override;
  };

  class SimpleGradientTransform : public AbstractHomography
  {
  public:
    SimpleGradientTransform (Process *p, const string &n, double a, double b, double c, double d, double e,
			     double f);
    SimpleGradientTransform (double a, double b, double c, double d, double e,
               double f);
    Process* clone () override;
    virtual
    ~SimpleGradientTransform ();
    void
    draw () override;
  };
}
