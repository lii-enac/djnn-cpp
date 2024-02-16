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
class AbstractPropFontFamily : public AbstractStyle
{
  public:
    AbstractPropFontFamily (CoreProcess* parent, const string& name, const string& family);
    virtual ~AbstractPropFontFamily ();

    void                                    get_properties_values (string& family);
    const djnnstl::vector<djnnstl::string>& get_properties_name () const override;
    virtual CoreProcess*                    find_child_impl (const string&) override;
    AbstractTextProperty*                   family () { return (AbstractTextProperty*)find_child_impl ("family"); }

  protected:
    struct raw_props_t
    {
        string family;
    };
    raw_props_t raw_props;
    Coupling*   _cfamily;
    void        impl_activate () override;
    void        impl_deactivate () override;
};
} // namespace djnn
