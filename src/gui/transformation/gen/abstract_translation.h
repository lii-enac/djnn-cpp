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

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"

#include "core/utils/containers/vector.h"
using djnnstl::vector;

#include "gui/transformation/abstract_transformation.h"

namespace djnn {
class AbstractTranslation : public AbstractTransformation {
  public:
    AbstractTranslation (CoreProcess* parent, const string& name, double tx, double ty);
    virtual ~AbstractTranslation ();

    void                    get_properties_values (double& tx, double& ty);
    const vector<string>&   get_properties_name () const override;
    virtual CoreProcess*    find_child_impl (const string&) override;
    AbstractDoubleProperty* tx () { return (AbstractDoubleProperty*)find_child_impl ("tx"); }
    AbstractDoubleProperty* ty () { return (AbstractDoubleProperty*)find_child_impl ("ty"); }

  protected:
    struct raw_props_t {
        double tx;
        double ty;
    };
    raw_props_t raw_props;
    Coupling *  _ctx, *_cty;
    void        impl_activate () override;
    void        impl_deactivate () override;
};
} // namespace djnn
