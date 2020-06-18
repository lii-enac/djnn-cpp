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
 *      Nicolas Saporito <nicolas.saporito@enac.fr>
 *
 */

#pragma once

#include "gui/abstract_gobj.h"
#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/tree/double_property.h"
#include "gui/shapes/shapes.h"

namespace djnn
{
  //using namespace std;

  class AbstractTransformation : public AbstractGObj
  {
  public:
    AbstractTransformation (FatProcess *parent, const std::string& name);
    AbstractTransformation ();
    virtual ~AbstractTransformation ();
  };
}

#include "gen/abstract_translation.h"

namespace djnn
{
/*
  class AbstractTranslation : public AbstractTransformation
  {
  public:
    AbstractTranslation (FatProcess *parent, const std::string& name, double tx, double ty);
    AbstractTranslation (double tx, double ty);
    virtual ~AbstractTranslation ();
    void get_properties_values (double &tx, double &ty);
    virtual FatProcess* find_child (const std::string&) override;
    AbstractDoubleProperty* tx () { return (AbstractDoubleProperty*) find_child("tx"); }
    AbstractDoubleProperty* ty () { return (AbstractDoubleProperty*) find_child("ty"); }
    void impl_activate () override;
    void impl_deactivate () override;
    virtual void draw () override = 0;
  protected:
    struct raw_props_t { double tx,ty; };
    raw_props_t raw_props;
    Coupling *_ctx, *_cty;
  };
*/
  class Translation : public AbstractTranslation
  {
  public:
    Translation (FatProcess *parent, const std::string& name, double tx, double ty);
    Translation (double tx, double ty);
    virtual ~Translation ();
    void draw () override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    FatProcess* clone () override;
  };

  class GradientTranslation : public AbstractTranslation
  {
  public:
    GradientTranslation (FatProcess *parent, const std::string& name, double tx, double ty);
    GradientTranslation (double tx, double ty);
    FatProcess* clone () override;
    virtual ~GradientTranslation ();
    void draw () override;
  };
}

#include "gen/abstract_rotation.h"

namespace djnn {
  /*
  class AbstractRotation : public AbstractTransformation
  {
  public:
    AbstractRotation (FatProcess *parent, const std::string& name, double a, double cx, double cy);
    AbstractRotation (double a, double cx, double cy);
    virtual ~AbstractRotation ();
    void get_properties_values (double &a, double &cx, double &cy);
    virtual FatProcess* find_child (const std::string&) override;
    AbstractDoubleProperty* a () { return (AbstractDoubleProperty*) find_child("a"); }
    // TODO Rot by (u,v)
    // AbstractDoubleProperty* ux () { return (AbstractDoubleProperty*) find_child("ux"); }
    // AbstractDoubleProperty* uy () { return (AbstractDoubleProperty*) find_child("uy"); }
    // AbstractDoubleProperty* vx () { return (AbstractDoubleProperty*) find_child("vx"); }
    // AbstractDoubleProperty* vy () { return (AbstractDoubleProperty*) find_child("vy"); }
    AbstractDoubleProperty* cx () { return (AbstractDoubleProperty*) find_child("cx"); }
    AbstractDoubleProperty* cy () { return (AbstractDoubleProperty*) find_child("cy"); }
    void impl_activate () override;
    void impl_deactivate () override;
    virtual void draw () override = 0;
  protected:
    struct raw_props_t { double a,cx,cy; };
    raw_props_t raw_props;
    Coupling *_ca, *_ccx, *_ccy;
  };*/

  class Rotation : public AbstractRotation
  {
  public:
    Rotation (FatProcess *parent, const std::string& name, double a, double cx, double cy);
    Rotation (double a, double cx, double cy);
    virtual ~Rotation ();
    void draw () override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    FatProcess* clone () override;
  };

  class GradientRotation : public AbstractRotation
  {
  public:
    GradientRotation (FatProcess *parent, const std::string& name, double a, double cx, double cy);
    GradientRotation (double a, double cx, double cy);
    virtual ~GradientRotation ();
    void draw () override;
    FatProcess* clone () override;
  };
}

#include "gen/abstract_scaling.h"

namespace djnn {
  /*
  class AbstractScaling : public AbstractTransformation
  {
  public:
    AbstractScaling (FatProcess *parent, const std::string& name, double sx, double sy, double cx, double cy);
    AbstractScaling (double sx, double sy, double cx, double cy);
    virtual ~AbstractScaling ();
    void get_properties_values (double &sx, double &sy, double &cx, double &cy);
    virtual FatProcess* find_child (const std::string&) override;
    AbstractDoubleProperty* sx () { return (AbstractDoubleProperty*) find_child("sx"); }
    AbstractDoubleProperty* sy () { return (AbstractDoubleProperty*) find_child("sy"); }
    AbstractDoubleProperty* cx () { return (AbstractDoubleProperty*) find_child("cx"); }
    AbstractDoubleProperty* cy () { return (AbstractDoubleProperty*) find_child("cy"); }
    void impl_activate () override;
    void impl_deactivate () override;
    virtual void draw () override = 0;
  protected:
    struct raw_props_t { double sx,sy,cx,cy; };
    raw_props_t raw_props;
    Coupling *_csx, *_csy, *_ccx, *_ccy;
  };*/

  class Scaling : public AbstractScaling
  {
  public:
    Scaling (FatProcess *parent, const std::string& name, double sx, double sy, double cx, double cy);
    Scaling (double sx, double sy, double cx, double cy);
    virtual ~Scaling ();
    void draw () override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    FatProcess* clone () override;
  };

  class GradientScaling : public AbstractScaling
  {
  public:
    GradientScaling (FatProcess *parent, const std::string& name, double sx, double sy, double cx, double cy);
    GradientScaling (double sx, double sy, double cx, double cy);
    virtual ~GradientScaling ();
    void draw () override;
    FatProcess* clone () override;
  };

}

#include "gen/abstract_skew.h"

namespace djnn {
  /*class AbstractSkew : public AbstractTransformation
  {
  public:
    AbstractSkew (FatProcess *parent, const std::string& name, double a);
    AbstractSkew (double a);
    virtual ~AbstractSkew ();
    void get_properties_values (double &a);
    virtual FatProcess* find_child (const std::string&) override;
    AbstractDoubleProperty* a () { return (AbstractDoubleProperty*) find_child("a"); }
    void impl_activate () override;
    void impl_deactivate () override;
    virtual void draw () override = 0;
  protected:
    struct raw_props_t { double a; };
    raw_props_t raw_props;
    Coupling *_ca;
  };*/

  class SkewX : public AbstractSkew
  {
  public:
    SkewX (FatProcess *parent, const std::string& name, double a);
    SkewX (double a);
    virtual ~SkewX ();
    void draw () override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    FatProcess* clone () override;
  };

  class GradientSkewX : public AbstractSkew
  {
  public:
    GradientSkewX (FatProcess *parent, const std::string& name, double a);
    GradientSkewX (double a);
    virtual ~GradientSkewX ();
    void draw () override;
    FatProcess* clone () override;
  };

  class SkewY : public AbstractSkew
  {
  public:
    SkewY (FatProcess *parent, const std::string& name, double a);
    SkewY (double a);
    virtual ~SkewY ();
    void draw () override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    FatProcess* clone () override;
  };

  class GradientSkewY : public AbstractSkew
  {
  public:
    GradientSkewY (FatProcess *parent, const std::string& name, double a);
    GradientSkewY (double a);
    virtual ~GradientSkewY ();
    void draw () override;
    FatProcess* clone () override;
  };
}

#include "gen/abstract_prop_homography.h"

namespace djnn {
  class AbstractHomography : public AbstractPropHomography
  {
  /*** private Class rightTranslateBy Actions ***/ 
  private:
     class RightTranslateByAction : public Action
    {
    public:
      RightTranslateByAction (FatProcess* parent, const std::string& name, AbstractHomography *h) :
      Action (parent, name), _h(h) {}
      virtual ~RightTranslateByAction () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      AbstractHomography *_h;
    };

  /*** private Class leftTranslateBy Actions ***/ 
  private:
     class LeftTranslateByAction : public Action
    {
    public:
      LeftTranslateByAction (FatProcess* parent, const std::string& name, AbstractHomography *h) :
      Action (parent, name), _h(h) {}
      virtual ~LeftTranslateByAction () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      AbstractHomography *_h;
    };

  /*** private Class rightScaleBy Actions ***/ 
  private:
     class RightScaleByAction : public Action
    {
    public:
      RightScaleByAction (FatProcess* parent, const std::string& name, AbstractHomography *h) :
      Action (parent, name), _h(h) {}
      virtual ~RightScaleByAction () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      AbstractHomography *_h;
    };

  /*** private Class leftScaleBy Actions ***/ 
  private:
     class LeftScaleByAction : public Action
    {
    public:
      LeftScaleByAction (FatProcess* parent, const std::string& name, AbstractHomography *h) :
      Action (parent, name), _h(h) {}
      virtual ~LeftScaleByAction () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      AbstractHomography *_h;
    };

  /*** private Class rightRotateBy Actions ***/ 
  private:
     class RightRotateByAction : public Action
    {
    public:
      RightRotateByAction (FatProcess* parent, const std::string& name, AbstractHomography *h) :
      Action (parent, name), _h(h) {}
      virtual ~RightRotateByAction () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      AbstractHomography *_h;
    };

  /*** private Class leftRotateBy Actions ***/ 
  private:
     class LeftRotateByAction : public Action
    {
    public:
      LeftRotateByAction (FatProcess* parent, const std::string& name, AbstractHomography *h) :
      Action (parent, name), _h(h) {}
      virtual ~LeftRotateByAction () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      AbstractHomography *_h;
    };

  /*** private Class right_skew_X_By Actions ***/ 
  private:
     class Right_Skew_X_ByAction : public Action
    {
    public:
      Right_Skew_X_ByAction (FatProcess* parent, const std::string& name, AbstractHomography *h) :
      Action (parent, name), _h(h) {}
      virtual ~Right_Skew_X_ByAction () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      AbstractHomography *_h;
    };

  /*** private Class left_skew_X_By Actions ***/ 
  private:
     class Left_Skew_X_ByAction : public Action
    {
    public:
      Left_Skew_X_ByAction (FatProcess* parent, const std::string& name, AbstractHomography *h) :
      Action (parent, name), _h(h) {}
      virtual ~Left_Skew_X_ByAction () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      AbstractHomography *_h;
    };

  /*** private Class right_skew_Y_By Actions ***/ 
  private:
     class Right_Skew_Y_ByAction : public Action
    {
    public:
      Right_Skew_Y_ByAction (FatProcess* parent, const std::string& name, AbstractHomography *h) :
      Action (parent, name), _h(h) {}
      virtual ~Right_Skew_Y_ByAction () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      AbstractHomography *_h;
    };

  /*** private Class left_skew_Y_By Actions ***/ 
  private:
     class Left_Skew_Y_ByAction : public Action
    {
    public:
      Left_Skew_Y_ByAction (FatProcess* parent, const std::string& name, AbstractHomography *h) :
      Action (parent, name), _h(h) {}
      virtual ~Left_Skew_Y_ByAction () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      AbstractHomography *_h;
    };

  public:
    AbstractHomography (FatProcess *parent, const std::string& name, double m11=1, double m12=0, double m13=0, double m14=0,
     double m21=0, double m22=1, double m23=0, double m24=0,
     double m31=0, double m32=0, double m33=1, double m34=0,
     double m41=0, double m42=0, double m43=0, double m44=1);
    AbstractHomography (double m11=1, double m12=0, double m13=0, double m14=0,
      double m21=0, double m22=1, double m23=0, double m24=0,
      double m31=0, double m32=0, double m33=1, double m34=0,
      double m41=0, double m42=0, double m43=0, double m44=1);
    virtual ~AbstractHomography ();
    virtual FatChildProcess* find_child (const std::string&) override;
    void impl_activate () override;
    void impl_deactivate () override;
    
    raw_props_t raw_props;
    
    /* accumulators (rotation angle + scaling coefficients) */
    DoubleProperty *_acca, *_accsx, *_accsy;

  protected:
    /* mxx coupling */
    //Coupling *_cm11, *_cm12, *_cm13, *_cm14, *_cm21, *_cm22, *_cm23, *_cm24, *_cm31, *_cm32, *_cm33, *_cm34, *_cm41,
    //*_cm42, *_cm43, *_cm44;
    /* rightTranslateBy ptr */
    Spike  *_rightTranslateBy_spike;
    Action *_rightTranslateBy_action;
    DoubleProperty *_rightTranslateBy_dx, *_rightTranslateBy_dy;
    CouplingWithData *_rightTranslateBy_dx_coupling, *_rightTranslateBy_dy_coupling;
    /* leftTranslateBy ptr */
    Spike  *_leftTranslateBy_spike;
    Action *_leftTranslateBy_action;
    DoubleProperty *_leftTranslateBy_dx, *_leftTranslateBy_dy;
    CouplingWithData *_leftTranslateBy_dx_coupling, *_leftTranslateBy_dy_coupling;
    /* rightScaleBy ptr */
    Spike  *_rightScaleBy_spike;
    Action *_rightScaleBy_action;
    DoubleProperty *_rightScaleBy_cx, *_rightScaleBy_cy ,*_rightScaleBy_sx, *_rightScaleBy_sy;
    CouplingWithData *_rightScaleBy_cx_coupling, *_rightScaleBy_cy_coupling, *_rightScaleBy_sx_coupling, *_rightScaleBy_sy_coupling;
    /* leftScaleBy ptr */
    Spike  *_leftScaleBy_spike;
    Action *_leftScaleBy_action;
    DoubleProperty *_leftScaleBy_cx, *_leftScaleBy_cy ,*_leftScaleBy_sx, *_leftScaleBy_sy;
    CouplingWithData *_leftScaleBy_cx_coupling, *_leftScaleBy_cy_coupling, *_leftScaleBy_sx_coupling, *_leftScaleBy_sy_coupling;
    /* rightRotateBy ptr */
    Spike *_rightRotateBy_spike;
    Action *_rightRotateBy_action;
    DoubleProperty *_rightRotateBy_cx, *_rightRotateBy_cy, *_rightRotateBy_da;
    CouplingWithData *_rightRotateBy_cx_coupling, *_rightRotateBy_cy_coupling, *_rightRotateBy_da_coupling;
    /* leftRotateBy ptr */
    Spike *_leftRotateBy_spike;
    Action *_leftRotateBy_action;
    DoubleProperty *_leftRotateBy_cx, *_leftRotateBy_cy, *_leftRotateBy_da;
    CouplingWithData *_leftRotateBy_cx_coupling, *_leftRotateBy_cy_coupling, *_leftRotateBy_da_coupling;
    /* rightSkewXBy ptr */
    Spike *_rightSkew_X_By_spike;
    Action *_rightSkew_X_By_action;
    DoubleProperty *_rightSkew_X_By_cx, *_rightSkew_X_By_cy, *_rightSkew_X_By_da;
    CouplingWithData *_rightSkew_X_By_cx_coupling, *_rightSkew_X_By_cy_coupling, *_rightSkew_X_By_da_coupling;
    /* leftSkewXBy ptr */
    Spike *_leftSkew_X_By_spike;
    Action *_leftSkew_X_By_action;
    DoubleProperty *_leftSkew_X_By_cx, *_leftSkew_X_By_cy, *_leftSkew_X_By_da;
    CouplingWithData *_leftSkew_X_By_cx_coupling, *_leftSkew_X_By_cy_coupling, *_leftSkew_X_By_da_coupling;
    /* rightSkewXBy ptr */
    Spike *_rightSkew_Y_By_spike;
    Action *_rightSkew_Y_By_action;
    DoubleProperty *_rightSkew_Y_By_cx, *_rightSkew_Y_By_cy, *_rightSkew_Y_By_da;
    CouplingWithData *_rightSkew_Y_By_cx_coupling, *_rightSkew_Y_By_cy_coupling, *_rightSkew_Y_By_da_coupling;
    /* leftSkewXBy ptr */
    Spike *_leftSkew_Y_By_spike;
    Action *_leftSkew_Y_By_action;
    DoubleProperty *_leftSkew_Y_By_cx, *_leftSkew_Y_By_cy, *_leftSkew_Y_By_da;
    CouplingWithData *_leftSkew_Y_By_cx_coupling, *_leftSkew_Y_By_cy_coupling, *_leftSkew_Y_By_da_coupling;

    /* Coordinate calculations */

    /*
     * No calculations need to be performed, the source
     * coordinates only need to be copied to their destinations to
     * complete the transformation equation of this transform.
     */
    const static int APPLY_IDENTITY = 0;
    /*
     * The translation components of the matrix need to be added
     * to complete the transformation equation of this transform.
     */
    const static int APPLY_TRANSLATE = 1;
    /*
     * The scaling components of the matrix need to be factored in 
     * to complete the transformation equation of this transform.
     * If the APPLY_SHEAR bit is also set then it indicates
     * that the scaling components are 0.0.
     * If the APPLY_SHEAR bit is not also set then it indicates that
     * the scaling components are not 1.0. 
     * If neither the APPLY_SHEAR nor the APPLY_SCALE bits are set
     * then the scaling components are 1.0, which means that the x and y
     * components contribute to the transformed coordinate,
     * but they are not multiplied by any scaling factor.
     */
    const static int APPLY_SCALE = 2;
    /*
     * The shearing components of the matrix (m12 and m21) need
     * to be factored in to complete the transformation equation of this
     * transform.  The presence of this bit in the state variable changes
     * the interpretation of the APPLY_SCALE bit as indicated in its
     * documentation.
     */
    const static int APPLY_SHEAR = 4;
    /*
     * Table of 2D state changes during predictable quadrant rotations
     * where the shear and scale values are swapped and negated.
     */
    int rot90conversion[8] = {
        /* IDENTITY => */        APPLY_SHEAR,
        /* TRANSLATE (TR) => */  APPLY_SHEAR | APPLY_TRANSLATE,
        /* SCALE (SC) => */      APPLY_SHEAR,
        /* SC | TR => */         APPLY_SHEAR | APPLY_TRANSLATE,
        /* SHEAR (SH) => */      APPLY_SCALE,
        /* SH | TR => */         APPLY_SCALE | APPLY_TRANSLATE,
        /* SH | SC => */         APPLY_SHEAR | APPLY_SCALE,
        /* SH | SC | TR => */    APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE,
    };
    /*
     * This field keeps track of which components of the matrix
     * need to be applied when performing a transformation.
     */
    int state2d = APPLY_IDENTITY;
  public:
    void updateState2d ();
    void rightTranslate (double dx, double dy);
    void rightTranslate2d (double dx, double dy);
    void leftTranslate (double dx, double dy);
    void leftTranslate2d (double dx, double dy);
    void rightScale (double sx, double sy);
    void rightScale (double sx, double sy, double cx, double cy);
    void rightScale2d (double sx, double sy);
    void leftScale (double sx, double sy);
    void leftScale (double sx, double sy, double cx, double cy);
    void leftScale2d (double sx, double sy);
    void rightRotate (double da);
    void rightRotate (double da, double cx, double cy);
    void rightRotate2d (double da_in_degrees);
    void rightRotate2d_90 ();
    void rightRotate2d_180 ();
    void rightRotate2d_270 ();
    void leftRotate (double da);
    void leftRotate (double da, double cx, double cy);
    void leftRotate2d (double da_in_degrees);
    void leftRotate2d_90 ();
    void leftRotate2d_180 ();
    void leftRotate2d_270 ();
    void rightSkew (double dax, double day);
    void rightSkew (double dax, double day, double cx, double cy);
    void rightSkew2d (double dax_in_degrees, double day_in_degrees);
    void leftSkew (double dax, double day);
    void leftSkew (double dax, double day, double cx, double cy);
    void leftSkew2d (double dax_in_degrees, double day_in_degrees);

  private:
    void init_rightTranslateBy ();
    void init_leftTranslateBy ();
    void init_rightScaleBy ();
    void init_leftScaleBy ();
    void init_rightRotateBy ();
    void init_leftRotateBy ();
    void init_rightSkewXBy ();
    void init_leftSkewXBy ();
    void init_rightSkewYBy ();
    void init_leftSkewYBy ();
  };

  class Homography : public AbstractHomography
  {
  public:
    Homography (FatProcess *parent, const std::string& name, double m11=1, double m12=0, double m13=0, double m14=0,
      double m21=0, double m22=1, double m23=0, double m24=0,
      double m31=0, double m32=0, double m33=1, double m34=0,
      double m41=0, double m42=0, double m43=0, double m44=1);
    Homography (double m11=1, double m12=0, double m13=0, double m14=0,
      double m21=0, double m22=1, double m23=0, double m24=0,
      double m31=0, double m32=0, double m33=1, double m34=0,
      double m41=0, double m42=0, double m43=0, double m44=1);
    virtual ~Homography ();
    void draw () override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    FatProcess* clone () override;
  };

  class GradientHomography : public AbstractHomography
  {
  public:
    GradientHomography (FatProcess *parent, const std::string& name, double m11=1, double m12=0, double m13=0, 
      double m21=0,	double m22=1, double m23=0,
      double m31=0, double m32=0, double m33=1);
    GradientHomography (double m11=1, double m12=0, double m13=0, 
      double m21=0, double m22=1, double m23=0,
      double m31=0, double m32=0, double m33=1);
    virtual ~GradientHomography ();
    void draw () override;
    FatProcess* clone () override;
  };

  class SimpleGradientTransform : public AbstractHomography
  {
  public:
    SimpleGradientTransform (FatProcess *parent, const std::string& name, double a, double b, double c, double d, double e,
      double f);
    SimpleGradientTransform (double a, double b, double c, double d, double e,
     double f);
    virtual ~SimpleGradientTransform ();
    void draw () override;
    FatProcess* clone () override;
  };

  class ScreenToLocal : public FatProcess 
  {
    /*** private Class stl_action Actions ***/ 
    private:
     class stl_action : public Action
    {
    public:
      stl_action (FatProcess* parent, const std::string& name, ScreenToLocal *stl) :
      Action (parent, name), _stl(stl) {}
      virtual ~stl_action () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      ScreenToLocal *_stl;
    };

    public:
      ScreenToLocal (FatProcess* parent, const std::string& name, CoreProcess* shape);
      virtual ~ScreenToLocal () override; 
      void impl_activate () override;
      void impl_deactivate () override;
    private:
      void set_parent (FatProcess* p) override;
      AbstractGShape* _shape;
      DoubleProperty *_inX, *_inY;
      DoubleProperty *_outX, *_outY;
      stl_action *_action;
      Coupling *_cinX, *_cinY;
  };

  class LocalToScreen : public FatProcess 
  {
    /*** private Class lts_action Actions ***/ 
    private:
     class lts_action : public Action
    {
    public:
      lts_action (FatProcess* parent, const std::string& name, LocalToScreen *lts) :
      Action (parent, name), _lts(lts) {}
      virtual ~lts_action () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      LocalToScreen *_lts;
    };

    public:
      LocalToScreen (FatProcess* parent, const std::string& name, CoreProcess* shape);
      virtual ~LocalToScreen () override; 
      void impl_activate () override;
      void impl_deactivate () override;
    private:
      void set_parent (FatProcess* p) override;
      AbstractGShape* _shape;
      DoubleProperty *_inX, *_inY;
      DoubleProperty *_outX, *_outY;
      lts_action *_action;
      Coupling *_cinX, *_cinY;
  };
}
