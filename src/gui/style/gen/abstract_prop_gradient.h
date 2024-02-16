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
#include "gui/style/abstract_style.h"

namespace djnn {
class AbstractPropGradient : public AbstractStyle {
  public:
    AbstractPropGradient (CoreProcess* parent, const string& name, int spread, int coords);
    virtual ~AbstractPropGradient ();

    void                                    get_properties_values (int& spread, int& coords);
    const djnnstl::vector<djnnstl::string>& get_properties_name () const override;
    virtual CoreProcess*                    find_child_impl (const string&) override;
    AbstractIntProperty*                    spread () { return (AbstractIntProperty*)find_child_impl ("spread"); }
    AbstractIntProperty*                    coords () { return (AbstractIntProperty*)find_child_impl ("coords"); }

  protected:
    struct raw_props_t {
        int spread;
        int coords;
    };
    raw_props_t raw_props;
    Coupling *  _cspread, *_ccoords;
    void        impl_activate () override;
    void        impl_deactivate () override;
};
} // namespace djnn
