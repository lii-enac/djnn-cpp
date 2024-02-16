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
class AbstractPropRadialGradient : public AbstractGradient
{
  public:
    AbstractPropRadialGradient (CoreProcess* parent, const string& name, double cx, double cy, double r, double fx, double fy, int spread, int coords);
    virtual ~AbstractPropRadialGradient ();

    void                                    get_properties_values (double& cx, double& cy, double& r, double& fx, double& fy, int& spread, int& coords);
    const djnnstl::vector<djnnstl::string>& get_properties_name () const override;
    virtual CoreProcess*                    find_child_impl (const string&) override;
    AbstractDoubleProperty*                 cx () { return (AbstractDoubleProperty*)find_child_impl ("cx"); }
    AbstractDoubleProperty*                 cy () { return (AbstractDoubleProperty*)find_child_impl ("cy"); }
    AbstractDoubleProperty*                 r () { return (AbstractDoubleProperty*)find_child_impl ("r"); }
    AbstractDoubleProperty*                 fx () { return (AbstractDoubleProperty*)find_child_impl ("fx"); }
    AbstractDoubleProperty*                 fy () { return (AbstractDoubleProperty*)find_child_impl ("fy"); }

  protected:
    struct raw_props_t
    {
        double cx;
        double cy;
        double r;
        double fx;
        double fy;
    };
    raw_props_t raw_props;
    Coupling *  _ccx, *_ccy, *_cr, *_cfx, *_cfy;
    void        impl_activate () override;
    void        impl_deactivate () override;
};
} // namespace djnn
