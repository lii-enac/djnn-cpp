#pragma once

#include "abstract_homography.h"
#include "abstract_transformation.h"
#include "core/control/action.h"
#include "gen/abstract_prop_homography.h"

namespace djnn {

class Homography : public AbstractHomography
{
  public:
    Homography (CoreProcess* parent, const string& name, double m11 = 1, double m12 = 0, double m13 = 0, double m14 = 0,
                double m21 = 0, double m22 = 1, double m23 = 0, double m24 = 0,
                double m31 = 0, double m32 = 0, double m33 = 1, double m34 = 0,
                double m41 = 0, double m42 = 0, double m43 = 0, double m44 = 1);
    Homography (double m11 = 1, double m12 = 0, double m13 = 0, double m14 = 0,
                double m21 = 0, double m22 = 1, double m23 = 0, double m24 = 0,
                double m31 = 0, double m32 = 0, double m33 = 1, double m34 = 0,
                double m41 = 0, double m42 = 0, double m43 = 0, double m44 = 1);
    virtual ~Homography ();
    void         draw () override;
    virtual void transform (double& x, double& y) override;
    virtual void inverse_transform (double& x, double& y) override;
    FatProcess*  impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
};

class GradientHomography : public AbstractHomography
{
  public:
    GradientHomography (CoreProcess* parent, const string& name, double m11 = 1, double m12 = 0, double m13 = 0,
                        double m21 = 0, double m22 = 1, double m23 = 0,
                        double m31 = 0, double m32 = 0, double m33 = 1);
    GradientHomography (double m11 = 1, double m12 = 0, double m13 = 0,
                        double m21 = 0, double m22 = 1, double m23 = 0,
                        double m31 = 0, double m32 = 0, double m33 = 1);
    virtual ~GradientHomography ();
    void        draw () override;
    FatProcess* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
};

class SimpleGradientTransform : public AbstractHomography
{
  public:
    SimpleGradientTransform (CoreProcess* parent, const string& name, double a, double b, double c, double d, double e,
                             double f);
    SimpleGradientTransform (double a, double b, double c, double d, double e,
                             double f);
    virtual ~SimpleGradientTransform ();
    void        draw () override;
    FatProcess* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
};
} // namespace djnn
