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
#include "gui/transformation/abstract_transformation.h"

namespace djnn {
class AbstractScaling : public AbstractTransformation
{
  public:
    AbstractScaling (CoreProcess* parent, const string& name, double sx, double sy, double cx, double cy);
    virtual ~AbstractScaling ();

    void                                    get_properties_values (double& sx, double& sy, double& cx, double& cy);
    const djnnstl::vector<djnnstl::string>& get_properties_name () const override;
    virtual CoreProcess*                    find_child_impl (const string&) override;
    AbstractDoubleProperty*                 sx () { return (AbstractDoubleProperty*)find_child_impl ("sx"); }
    AbstractDoubleProperty*                 sy () { return (AbstractDoubleProperty*)find_child_impl ("sy"); }
    AbstractDoubleProperty*                 cx () { return (AbstractDoubleProperty*)find_child_impl ("cx"); }
    AbstractDoubleProperty*                 cy () { return (AbstractDoubleProperty*)find_child_impl ("cy"); }

  protected:
    struct raw_props_t
    {
        double sx;
        double sy;
        double cx;
        double cy;
    };
    raw_props_t raw_props;
    Coupling *  _csx, *_csy, *_ccx, *_ccy;
    void        impl_activate () override;
    void        impl_deactivate () override;
};
} // namespace djnn
