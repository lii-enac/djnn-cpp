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
#include "gui/shape/gen/abstract_image.h"
using AbstractTextpProperty = void; // hack to make 'AbstractTextpProperty* data ()' compile
#include "gui/shape/abstract_gshape.h"

namespace djnn {
class AbstractDataImage : public AbstractImage {
  public:
    AbstractDataImage (CoreProcess* parent, const string& name, string*& data, int format, double x, double y, double width, double height);
    virtual ~AbstractDataImage ();

    void                                    get_properties_values (string*& data, int& format, double& x, double& y, double& width, double& height);
    const djnnstl::vector<djnnstl::string>& get_properties_name () const override;
    virtual CoreProcess*                    find_child_impl (const string&) override;
    AbstractTextpProperty*                  data () { return (AbstractTextpProperty*)find_child_impl ("data"); }
    AbstractIntProperty*                    format () { return (AbstractIntProperty*)find_child_impl ("format"); }

  protected:
    struct raw_props_t {
        string* data;
        int     format;
    };
    raw_props_t raw_props;
    Coupling *  _cdata, *_cformat;
    void        impl_activate () override;
    void        impl_deactivate () override;
};
} // namespace djnn
