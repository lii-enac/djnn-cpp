/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020-2025)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */
#pragma once

#include "core/property/double_property.h"
#include "gen/abstract_prop_rectangle_clip.h"
#include "gui/shape/abstract_gshape.h"

namespace djnn {
class RectangleClip : public AbstractPropRectangleClip {
  public:
    RectangleClip (CoreProcess* parent, const string& name, double x, double y, double width, double height);
    RectangleClip*  impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
    void            draw () override;
    void            get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double          sdf (double x, double y) const override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
};
} // namespace djnn