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
#include "../../core/process.h"
#include "../../core/coupling.h"
#include "../../core/double_property.h"

namespace djnn
{
  using namespace std;

  class AbstractTranslation : public AbstractGObj
  {
  public:
    AbstractTranslation (Process *p, const string &n, double tx, double ty);
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
    virtual
    ~GradientTranslation ();
    void
    draw () override;
  };

  class AbstractRotation : public AbstractGObj
  {
  public:
    AbstractRotation (Process *p, const string &n, double a, double cx, double cy);
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
    virtual
    ~GradientRotation ();
    void
    draw () override;
  };

  class AbstractScaling : public AbstractGObj
  {
  public:
    AbstractScaling (Process *p, const string &n, double sx, double sy, double cx, double cy);
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
    virtual
    ~GradientScaling ();
    void
    draw () override;
  };

  class AbstractSkew : public AbstractGObj
  {
  public:
    AbstractSkew (Process *p, const string &n, double a);
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
    virtual
    ~SkewX ();
    void
    draw () override;
  };

  class GradientSkewX : public AbstractSkew
  {
  public:
    GradientSkewX (Process *p, const string &n, double a);
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
    virtual
    ~GradientSkewY ();
    void
    draw () override;
  };

  class AbstractHomography : public AbstractGObj
  {
  public:
    AbstractHomography (Process *p, const string &n, double m11, double m12, double m13, double m14,
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
  protected:
    DoubleProperty *_m11, *_m12, *_m13, *_m14, *_m21, *_m22, *_m23, *_m24, *_m31, *_m32, *_m33, *_m34, *_m41, *_m42,
	*_m43, *_m44;
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
    virtual
    ~SimpleGradientTransform ();
    void
    draw () override;
  };
}
