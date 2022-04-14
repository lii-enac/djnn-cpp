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


#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/property/double_property.h"

#include "gui/abstract_gobj.h"

#include "abstract_transformation.h"
#include "homography.h"
#include "translation.h"
#include "scaling.h"

namespace djnn
{
  class GradientTranslation : public AbstractTranslation
  {
  public:
    GradientTranslation (ParentProcess* parent, const string& name, double tx, double ty);
    GradientTranslation (double tx, double ty);
    FatProcess* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
    virtual ~GradientTranslation ();
    void draw () override;
  };
}

#include "gen/abstract_rotation.h"

namespace djnn {

  class Rotation : public AbstractRotation
  {
  public:
    Rotation (ParentProcess* parent, const string& name, double a, double cx, double cy);
    Rotation (double a, double cx, double cy);
    virtual ~Rotation ();
    void draw () override;
    virtual void transform( double& x, double& y) override;
    virtual void inverse_transform( double& x, double& y) override;
    FatProcess* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  };

  class GradientRotation : public AbstractRotation
  {
  public:
    GradientRotation (ParentProcess* parent, const string& name, double a, double cx, double cy);
    GradientRotation (double a, double cx, double cy);
    virtual ~GradientRotation ();
    void draw () override;
    FatProcess* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  };
}

#include "gen/abstract_scaling.h"

namespace djnn {

  class GradientScaling : public AbstractScaling
  {
  public:
    GradientScaling (ParentProcess* parent, const string& name, double sx, double sy, double cx, double cy);
    GradientScaling (double sx, double sy, double cx, double cy);
    virtual ~GradientScaling ();
    void draw () override;
    FatProcess* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  };

}

#include "gen/abstract_skew.h"

namespace djnn {

  class SkewX : public AbstractSkew
  {
  public:
    SkewX (ParentProcess* parent, const string& name, double a);
    SkewX (double a);
    virtual ~SkewX ();
    void draw () override;
    virtual void transform( double& x, double& y) override;
    virtual void inverse_transform( double& x, double& y) override;
    FatProcess* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  };

  class GradientSkewX : public AbstractSkew
  {
  public:
    GradientSkewX (ParentProcess* parent, const string& name, double a);
    GradientSkewX (double a);
    virtual ~GradientSkewX ();
    void draw () override;
    FatProcess* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  };

  class SkewY : public AbstractSkew
  {
  public:
    SkewY (ParentProcess* parent, const string& name, double a);
    SkewY (double a);
    virtual ~SkewY ();
    void draw () override;
    virtual void transform( double& x, double& y) override;
    virtual void inverse_transform( double& x, double& y) override;
    FatProcess* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  };

  class GradientSkewY : public AbstractSkew
  {
  public:
    GradientSkewY (ParentProcess* parent, const string& name, double a);
    GradientSkewY (double a);
    virtual ~GradientSkewY ();
    void draw () override;
    FatProcess* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  };

  class ScreenToLocal : public FatProcess 
  {
    /*** private Class stl_action Actions ***/ 
    private:
     class stl_action : public Action
    {
    public:
      stl_action (ParentProcess* parent, const string& name, ScreenToLocal *stl) :
      Action (parent, name), _stl(stl) {}
      virtual ~stl_action () {}
      void impl_activate () override;
    private:
      ScreenToLocal *_stl;
    };

    public:
      ScreenToLocal (ParentProcess* parent, const string& name, CoreProcess* shape);
      virtual ~ScreenToLocal () override; 
      void impl_activate () override;
      void impl_deactivate () override;
    private:
      void set_parent (ParentProcess* parent) override;
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
      lts_action (ParentProcess* parent, const string& name, LocalToScreen *lts) :
      Action (parent, name), _lts(lts) {}
      virtual ~lts_action () {}
      void impl_activate () override;
    private:
      LocalToScreen *_lts;
    };

    public:
      LocalToScreen (ParentProcess* parent, const string& name, CoreProcess* shape);
      virtual ~LocalToScreen () override; 
      void impl_activate () override;
      void impl_deactivate () override;
    private:
      void set_parent (ParentProcess* parent) override;
      AbstractGShape* _shape;
      DoubleProperty *_inX, *_inY;
      DoubleProperty *_outX, *_outY;
      lts_action *_action;
      Coupling *_cinX, *_cinY;
  };
}
