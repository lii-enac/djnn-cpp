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
class Texture : public AbstractStyle {
  public:
    Texture (CoreProcess* parent, const string& name, const string& path);
    virtual ~Texture ();
    void                  draw () override;
    Texture*              impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
    void                  get_properties_values (string& path);
    const vector<string>& get_properties_name () const override;
    virtual CoreProcess*  find_child_impl (const string&) override;
    AbstractTextProperty* path () { return (AbstractTextProperty*)find_child_impl ("path"); }

  protected:
    struct raw_props_t {
        string path;
    };
    raw_props_t raw_props;
    Coupling*   _cpath;
    void        impl_activate () override;
    void        impl_deactivate () override;
};
} // namespace djnn
