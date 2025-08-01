#pragma once

#include "abstract_style.h"
#include "gen/abstract_prop_gradient.h"
#include "gen/abstract_prop_gradient_stop.h"
#include "style_types.h"
namespace djnn {

class List;

class AbstractGradient : public AbstractPropGradient {
  public:
    AbstractGradient (CoreProcess* parent, const string& name, int spread, int coords);
    AbstractGradient (CoreProcess* parent, const string& name);
    virtual ~AbstractGradient ();
    virtual void update ();
    List*        transforms () { return _transforms; }
    List*        stops () { return _stops; }
    bool         is_linear () { return _linear; }
    // does this gradient owned thoses children or it merged with another gradient so it should not delete them
    bool bstops, btransforms, bspread, bcoords;

  protected:
    List *            _stops, *_transforms;
    AbstractGradient* _g;
    int               _linear;
};
} // namespace djnn

#include "gen/abstract_prop_linear_gradient.h"
#include "gen/abstract_prop_radial_gradient.h"

namespace djnn {

class GradientStop : public AbstractPropGradientStop {
  public:
    GradientStop (CoreProcess* parent, const string& name, double r, double g, double b, double a, double offset);
    virtual ~GradientStop ();
    void          draw () override;
    GradientStop* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
};

class LinearGradient : public AbstractPropLinearGradient {
  public:
    LinearGradient (CoreProcess* parent, const string& name, double x1, double y1, double x2, double y2,
                    djnFillSpread s, djnFillCoords fc);
    LinearGradient (CoreProcess* parent, const string& name, double x1, double y1, double x2, double y2,
                    int s, int fc);
    LinearGradient (CoreProcess* parent, const string& name, LinearGradient* lg);
    virtual ~LinearGradient ();
    void            draw () override;
    LinearGradient* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
};

class RefLinearGradient : public AbstractStyle {
  public:
    RefLinearGradient (CoreProcess* parent, const string& name, LinearGradient* lg);
    virtual ~RefLinearGradient () {};
    void               impl_activate () override;
    void               impl_deactivate () override;
    void               draw () override;
    RefLinearGradient* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;

  private:
    void            activate_children ();
    LinearGradient* _lg;
};

class RadialGradient : public AbstractPropRadialGradient {
  public:
    RadialGradient (CoreProcess* parent, const string& name, double cx, double cy, double r, double fx, double fy,
                    djnFillSpread s, djnFillCoords fc);
    RadialGradient (CoreProcess* parent, const string& name, double cx, double cy, double r, double fx, double fy,
                    int s, int fc);
    RadialGradient (CoreProcess* parent, const string& name, RadialGradient* rg);
    virtual ~RadialGradient ();
    void            draw () override;
    RadialGradient* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
};

class RefRadialGradient : public AbstractStyle {
  public:
    RefRadialGradient (CoreProcess* parent, const string& name, RadialGradient* rg);
    virtual ~RefRadialGradient () {};
    void               impl_activate () override;
    void               impl_deactivate () override;
    void               draw () override;
    RefRadialGradient* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;

  private:
    void            activate_children ();
    RadialGradient* _rg;
};

} // namespace djnn
