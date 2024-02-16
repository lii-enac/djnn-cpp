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
#include "gui/style/abstract_gradient.h"

namespace djnn {
class AbstractPropLinearGradient : public AbstractGradient {
  public:
    AbstractPropLinearGradient (CoreProcess* parent, const string& name, double x1, double y1, double x2, double y2, int spread, int coords);
    virtual ~AbstractPropLinearGradient ();

    void                                    get_properties_values (double& x1, double& y1, double& x2, double& y2, int& spread, int& coords);
    const djnnstl::vector<djnnstl::string>& get_properties_name () const override;
    virtual CoreProcess*                    find_child_impl (const string&) override;
    AbstractDoubleProperty*                 x1 () { return (AbstractDoubleProperty*)find_child_impl ("x1"); }
    AbstractDoubleProperty*                 y1 () { return (AbstractDoubleProperty*)find_child_impl ("y1"); }
    AbstractDoubleProperty*                 x2 () { return (AbstractDoubleProperty*)find_child_impl ("x2"); }
    AbstractDoubleProperty*                 y2 () { return (AbstractDoubleProperty*)find_child_impl ("y2"); }

  protected:
    struct raw_props_t {
        double x1;
        double y1;
        double x2;
        double y2;
    };
    raw_props_t raw_props;
    Coupling *  _cx1, *_cy1, *_cx2, *_cy2;
    void        impl_activate () override;
    void        impl_deactivate () override;
};
} // namespace djnn
