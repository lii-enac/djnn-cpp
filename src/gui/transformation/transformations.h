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

#pragma once

#include "../abstract_gobj.h"
#include "../../core/tree/process.h"
#include "../../core/control/coupling.h"
#include "../../core/tree/double_property.h"
#include "../shapes/shapes.h"

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
    virtual ~AbstractTranslation ();
    void get_properties_values (double &tx, double &ty);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* tx () { return (AbstractDoubleProperty*) find_component("tx"); }
    AbstractDoubleProperty* ty () { return (AbstractDoubleProperty*) find_component("ty"); }
    void activate () override;
    void deactivate () override;
    virtual void draw () override = 0;
  protected:
    struct raw_props_t { double tx,ty; };
    raw_props_t raw_props;
    //DoubleProperty *_tx, *_ty;
    Coupling *_ctx, *_cty;
  };

  class Translation : public AbstractTranslation
  {
  public:
    Translation (Process *p, const string &n, double tx, double ty);
    Translation (double tx, double ty);
    virtual ~Translation ();
    void draw () override;
    Process* clone () override;
  };

  class GradientTranslation : public AbstractTranslation
  {
  public:
    GradientTranslation (Process *p, const string &n, double tx, double ty);
    GradientTranslation (double tx, double ty);
    Process* clone () override;
    virtual ~GradientTranslation ();
    void draw () override;
  };

  class AbstractRotation : public AbstractTransformation
  {
  public:
    AbstractRotation (Process *p, const string &n, double a, double cx, double cy);
    AbstractRotation (double a, double cx, double cy);
    virtual ~AbstractRotation ();
    void get_properties_values (double &a, double &cx, double &cy);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* a () { return (AbstractDoubleProperty*) find_component("a"); }
    // TODO Rot by (u,v)
    // AbstractDoubleProperty* ux () { return (AbstractDoubleProperty*) find_component("ux"); }
    // AbstractDoubleProperty* uy () { return (AbstractDoubleProperty*) find_component("uy"); }
    // AbstractDoubleProperty* vx () { return (AbstractDoubleProperty*) find_component("vx"); }
    // AbstractDoubleProperty* vy () { return (AbstractDoubleProperty*) find_component("vy"); }
    AbstractDoubleProperty* cx () { return (AbstractDoubleProperty*) find_component("cx"); }
    AbstractDoubleProperty* cy () { return (AbstractDoubleProperty*) find_component("cy"); }
    void activate () override;
    void deactivate () override;
    virtual void draw () override = 0;
  protected:
    struct raw_props_t { double a,cx,cy; };
    raw_props_t raw_props;
    //DoubleProperty *_a, *_cx, *_cy;
    Coupling *_ca, *_ccx, *_ccy;
  };

  class Rotation : public AbstractRotation
  {
  public:
    Rotation (Process *p, const string &n, double a, double cx, double cy);
    Rotation (double a, double cx, double cy);
    virtual ~Rotation ();
    void draw () override;
    Process* clone () override;
  };

  class GradientRotation : public AbstractRotation
  {
  public:
    GradientRotation (Process *p, const string &n, double a, double cx, double cy);
    GradientRotation (double a, double cx, double cy);
    virtual ~GradientRotation ();
    void draw () override;
    Process* clone () override;
  };

  class AbstractScaling : public AbstractTransformation
  {
  public:
    AbstractScaling (Process *p, const string &n, double sx, double sy, double cx, double cy);
    AbstractScaling (double sx, double sy, double cx, double cy);
    virtual ~AbstractScaling ();
    void get_properties_values (double &sx, double &sy, double &cx, double &cy);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* sx () { return (AbstractDoubleProperty*) find_component("sx"); }
    AbstractDoubleProperty* sy () { return (AbstractDoubleProperty*) find_component("sy"); }
    AbstractDoubleProperty* cx () { return (AbstractDoubleProperty*) find_component("cx"); }
    AbstractDoubleProperty* cy () { return (AbstractDoubleProperty*) find_component("cy"); }
    void activate () override;
    void deactivate () override;
    virtual void draw () override = 0;
  protected:
    struct raw_props_t { double sx,sy,cx,cy; };
    raw_props_t raw_props;
    //DoubleProperty *_sx, *_sy, *_cx, *_cy;
    Coupling *_csx, *_csy, *_ccx, *_ccy;
  };

  class Scaling : public AbstractScaling
  {
  public:
    Scaling (Process *p, const string &n, double sx, double sy, double cx, double cy);
    Scaling (double sx, double sy, double cx, double cy);
    virtual ~Scaling ();
    void draw () override;
    Process* clone () override;
  };

  class GradientScaling : public AbstractScaling
  {
  public:
    GradientScaling (Process *p, const string &n, double sx, double sy, double cx, double cy);
    GradientScaling (double sx, double sy, double cx, double cy);
    virtual ~GradientScaling ();
    void draw () override;
    Process* clone () override;
  };

  class AbstractSkew : public AbstractTransformation
  {
  public:
    AbstractSkew (Process *p, const string &n, double a);
    AbstractSkew (double a);
    virtual ~AbstractSkew ();
    void get_properties_values (double &a);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* a () { return (AbstractDoubleProperty*) find_component("a"); }
    void activate () override;
    void deactivate () override;
    virtual void draw () override = 0;
  protected:
    struct raw_props_t { double a; };
    raw_props_t raw_props;
    Coupling *_ca;
  };

  class SkewX : public AbstractSkew
  {
  public:
    SkewX (Process *p, const string &n, double a);
    SkewX (double a);
    virtual ~SkewX ();
    void draw () override;
    Process* clone () override;
  };

  class GradientSkewX : public AbstractSkew
  {
  public:
    GradientSkewX (Process *p, const string &n, double a);
    GradientSkewX (double a);
    virtual ~GradientSkewX ();
    void draw () override;
    Process* clone () override;
  };

  class SkewY : public AbstractSkew
  {
  public:
    SkewY (Process *p, const string &n, double a);
    SkewY (double a);
    virtual ~SkewY ();
    void draw () override;
    Process* clone () override;
  };

  class GradientSkewY : public AbstractSkew
  {
  public:
    GradientSkewY (Process *p, const string &n, double a);
    GradientSkewY (double a);
    virtual ~GradientSkewY ();
    void draw () override;
    Process* clone () override;
  };

  class AbstractHomography : public AbstractTransformation
  {
  /*** private Class translateBy Actions ***/ 
  private:
     class TranslateByAction : public Process
    {
    public:
      TranslateByAction (Process* parent, const string &name, AbstractHomography *h) :
      Process (parent, name), _h(h) { Process::finalize ();}
      virtual ~TranslateByAction () {}
      void activate () override;
      void deactivate () override {}
    private:
      AbstractHomography *_h;
    };

  /*** private Class scaleBy Actions ***/ 
  private:
     class ScaleByAction : public Process
    {
    public:
      ScaleByAction (Process* parent, const string &name, AbstractHomography *h) :
      Process (parent, name), _h(h) { Process::finalize ();}
      virtual ~ScaleByAction () {}
      void activate () override;
      void deactivate () override {}
    private:
      AbstractHomography *_h;
    };

  /*** private Class rotateBy Actions ***/ 
  private:
     class RotateByAction : public Process
    {
    public:
      RotateByAction (Process* parent, const string &name, AbstractHomography *h) :
      Process (parent, name), _h(h) { Process::finalize ();}
      virtual ~RotateByAction () {}
      void activate () override;
      void deactivate () override {}
    private:
      AbstractHomography *_h;
    };

  /*** private Class skew_X_By Actions ***/ 
  private:
     class Skew_X_ByAction : public Process
    {
    public:
      Skew_X_ByAction (Process* parent, const string &name, AbstractHomography *h) :
      Process (parent, name), _h(h) { Process::finalize ();}
      virtual ~Skew_X_ByAction () {}
      void activate () override;
      void deactivate () override {}
    private:
      AbstractHomography *_h;
    };

  /*** private Class skew_Y_By Actions ***/ 
  private:
     class Skew_Y_ByAction : public Process
    {
    public:
      Skew_Y_ByAction (Process* parent, const string &name, AbstractHomography *h) :
      Process (parent, name), _h(h) { Process::finalize ();}
      virtual ~Skew_Y_ByAction () {}
      void activate () override;
      void deactivate () override {}
    private:
      AbstractHomography *_h;
    };

  public:
    AbstractHomography (Process *p, const string &n, double m11=1, double m12=0, double m13=0, double m14=0,
     double m21=0, double m22=1, double m23=0, double m24=0,
     double m31=0, double m32=0, double m33=1, double m34=0,
     double m41=0, double m42=0, double m43=0, double m44=1);
    AbstractHomography (double m11=1, double m12=0, double m13=0, double m14=0,
      double m21=0, double m22=1, double m23=0, double m24=0,
      double m31=0, double m32=0, double m33=1, double m34=0,
      double m41=0, double m42=0, double m43=0, double m44=1);
    virtual ~AbstractHomography ();
    void activate () override;
    void deactivate () override;
    virtual void draw () override = 0;
    DoubleProperty *_m11, *_m12, *_m13, *_m14, *_m21, *_m22, *_m23, *_m24, *_m31, *_m32, *_m33, *_m34, *_m41, *_m42,
    *_m43, *_m44;
  protected:
    /* mxx coupling */
    Coupling *_cm11, *_cm12, *_cm13, *_cm14, *_cm21, *_cm22, *_cm23, *_cm24, *_cm31, *_cm32, *_cm33, *_cm34, *_cm41,
    *_cm42, *_cm43, *_cm44;
    /* translateBy ptr */
    Spike  *_translateBy_spike;
    DoubleProperty *_translateBy_dx, *_translateBy_dy;
    Process* _translateBy_action;
    Coupling *_tranlateBy_dx_coupling, *_tranlateBy_dy_coupling;
    /* scaleBy ptr */
    Spike  *_scaleBy_spike;
    DoubleProperty *_scaleBy_cx, *_scaleBy_cy ,*_scaleBy_sx, *_scaleBy_sy;
    Process *_scaleBy_action;
    Coupling *_scaleBy_cx_coupling, *_scaleBy_cy_coupling, *_scaleBy_sx_coupling, *_scaleBy_sy_coupling;
    /* rotateBy ptr */
    Spike *_rotateBy_spike;
    DoubleProperty *_rotateBy_cx, *_rotateBy_cy, *_rotateBy_da;
    Process *_rotateBy_action;
    Coupling *_rotateBy_cx_coupling, *_rotateBy_cy_coupling, *_rotateBy_da_coupling;
    /* skewXBy ptr */
    Spike *_skew_X_By_spike;
    DoubleProperty *_skew_X_By_cx, *_skew_X_By_cy, *_skew_X_By_da;
    Process *_skew_X_By_action;
    Coupling *_skew_X_By_cx_coupling, *_skew_X_By_cy_coupling, *_skew_X_By_da_coupling;
    /* skewXBy ptr */
    Spike *_skew_Y_By_spike;
    DoubleProperty *_skew_Y_By_cx, *_skew_Y_By_cy, *_skew_Y_By_da;
    Process *_skew_Y_By_action;
    Coupling *_skew_Y_By_cx_coupling, *_skew_Y_By_cy_coupling, *_skew_Y_By_da_coupling;

  private:
    void init_abstractHomography (double m11, double m12, double m13, double m14,
                                  double m21, double m22, double m23, double m24,
                                  double m31, double m32, double m33, double m34,
                                  double m41, double m42, double m43, double m44);
  };

  class Homography : public AbstractHomography
  {
  public:
    Homography (Process *p, const string &n, double m11=1, double m12=0, double m13=0, double m14=0,
      double m21=0, double m22=1, double m23=0, double m24=0,
      double m31=0, double m32=0, double m33=1, double m34=0,
      double m41=0, double m42=0, double m43=0, double m44=1);
    Homography (double m11=1, double m12=0, double m13=0, double m14=0,
      double m21=0, double m22=1, double m23=0, double m24=0,
      double m31=0, double m32=0, double m33=1, double m34=0,
      double m41=0, double m42=0, double m43=0, double m44=1);
    virtual ~Homography ();
    void draw () override;
    Process* clone () override;
  };

  class GradientHomography : public AbstractHomography
  {
  public:
    GradientHomography (Process *p, const string &n, double m11=1, double m12=0, double m13=0, 
      double m21=0,	double m22=1, double m23=0,
      double m31=0, double m32=0, double m33=1);
    GradientHomography (double m11=1, double m12=0, double m13=0, 
      double m21=0, double m22=1, double m23=0,
      double m31=0, double m32=0, double m33=1);
    virtual ~GradientHomography ();
    void draw () override;
    Process* clone () override;
  };

  class SimpleGradientTransform : public AbstractHomography
  {
  public:
    SimpleGradientTransform (Process *p, const string &n, double a, double b, double c, double d, double e,
      double f);
    SimpleGradientTransform (double a, double b, double c, double d, double e,
     double f);
    virtual ~SimpleGradientTransform ();
    void draw () override;
    Process* clone () override;
  };

  class ScreenToLocal : public Process 
  {
    /*** private Class stl_action Actions ***/ 
    private:
     class stl_action : public Process
    {
    public:
      stl_action (Process* parent, const string &name, ScreenToLocal *stl) :
      Process (parent, name), _stl(stl) { Process::finalize ();}
      virtual ~stl_action () {}
      void activate () override;
      void deactivate () override {}
    private:
      ScreenToLocal *_stl;
    };

    public:
      ScreenToLocal (Process* parent, const string &name, Process* shape);
      virtual ~ScreenToLocal () override; 
      void activate () override;
      void deactivate () override;
    private:
      AbstractGShape* _shape;
      DoubleProperty *_inX, *_inY;
      DoubleProperty *_outX, *_outY;
      stl_action *_action;
      Coupling *_cinX, *_cinY;
  };

  class LocalToScreen : public Process 
  {
    /*** private Class lts_action Actions ***/ 
    private:
     class lts_action : public Process
    {
    public:
      lts_action (Process* parent, const string &name, LocalToScreen *lts) :
      Process (parent, name), _lts(lts) { Process::finalize ();}
      virtual ~lts_action () {}
      void activate () override;
      void deactivate () override {}
    private:
      LocalToScreen *_lts;
    };

    public:
      LocalToScreen (Process* parent, const string &name, Process* shape);
      virtual ~LocalToScreen () override; 
      void activate () override;
      void deactivate () override;
    private:
      AbstractGShape* _shape;
      DoubleProperty *_inX, *_inY;
      DoubleProperty *_outX, *_outY;
      lts_action *_action;
      Coupling *_cinX, *_cinY;
  };
}
