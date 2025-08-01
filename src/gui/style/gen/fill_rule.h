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

namespace djnn {
class FillRule : public AbstractStyle {
  public:
    FillRule (CoreProcess* parent, const string& name, int rule);
    virtual ~FillRule ();
    void                  draw () override;
    FillRule*             impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
    void                  get_properties_values (int& rule);
    const vector<string>& get_properties_name () const override;
    virtual CoreProcess*  find_child_impl (const string&) override;
    AbstractIntProperty*  rule () { return (AbstractIntProperty*)find_child_impl ("rule"); }

  protected:
    struct raw_props_t {
        int rule;
    };
    raw_props_t raw_props;
    Coupling*   _crule;
    void        impl_activate () override;
    void        impl_deactivate () override;
};
} // namespace djnn
