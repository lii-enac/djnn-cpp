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

#include "gui/style/abstract_style.h"

namespace djnn {
class AbstractVisibility : public AbstractStyle {
  public:
    AbstractVisibility (CoreProcess* parent, const string& name, int v);
    virtual ~AbstractVisibility ();

    void                  get_properties_values (int& v);
    const vector<string>& get_properties_name () const override;
    virtual CoreProcess*  find_child_impl (const string&) override;
    AbstractIntProperty*  v () { return (AbstractIntProperty*)find_child_impl ("v"); }

  protected:
    struct raw_props_t {
        int v;
    };
    raw_props_t raw_props;
    Coupling*   _cv;
    void        impl_activate () override;
    void        impl_deactivate () override;
};
} // namespace djnn
