/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 *  !! this file has been automatically generated - do NOT modify !!
 *
 */

#pragma once

#include "core/ontology/coupling.h"
#include "core/ontology/process.h"
#include "gui/shape/abstract_gshape.h"

namespace djnn {
class AbstractPropRectangle : public AbstractGShape {
  public:
    AbstractPropRectangle (CoreProcess* parent, const string& name, double x, double y, double width, double height, double rx = 0, double ry = 0);
    virtual ~AbstractPropRectangle ();

    void                                    get_properties_values (double& x, double& y, double& width, double& height, double& rx, double& ry);
    const djnnstl::vector<djnnstl::string>& get_properties_name () const override;
    virtual CoreProcess*                    find_child_impl (const string&) override;
    AbstractDoubleProperty*                 x () { return (AbstractDoubleProperty*)find_child_impl ("x"); }
    AbstractDoubleProperty*                 y () { return (AbstractDoubleProperty*)find_child_impl ("y"); }
    AbstractDoubleProperty*                 width () { return (AbstractDoubleProperty*)find_child_impl ("width"); }
    AbstractDoubleProperty*                 height () { return (AbstractDoubleProperty*)find_child_impl ("height"); }
    AbstractDoubleProperty*                 rx () { return (AbstractDoubleProperty*)find_child_impl ("rx"); }
    AbstractDoubleProperty*                 ry () { return (AbstractDoubleProperty*)find_child_impl ("ry"); }

  protected:
    struct raw_props_t {
        double x;
        double y;
        double width;
        double height;
        double rx;
        double ry;
    };
    raw_props_t raw_props;
    Coupling *  _cx, *_cy, *_cwidth, *_cheight, *_crx, *_cry;
    void        impl_activate () override;
    void        impl_deactivate () override;
};
} // namespace djnn
