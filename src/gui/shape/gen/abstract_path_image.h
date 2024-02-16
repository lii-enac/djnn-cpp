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
#include "gui/shape/gen/abstract_image.h"

namespace djnn {
class AbstractPathImage : public AbstractImage {
  public:
    AbstractPathImage (CoreProcess* parent, const string& name, const string& path, double x, double y, double width, double height);
    virtual ~AbstractPathImage ();

    void                                    get_properties_values (string& path, double& x, double& y, double& width, double& height);
    const djnnstl::vector<djnnstl::string>& get_properties_name () const override;
    virtual CoreProcess*                    find_child_impl (const string&) override;
    AbstractTextProperty*                   path () { return (AbstractTextProperty*)find_child_impl ("path"); }

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
