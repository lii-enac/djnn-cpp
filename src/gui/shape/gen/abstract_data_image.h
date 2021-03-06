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

#include "gui/shape/gen/abstract_image.h"

namespace djnn
{
  class AbstractDataImage : public AbstractImage
  {
  public:
    AbstractDataImage (ParentProcess* parent, const std::string& name, const std::string& data, double x, double y, double width, double height);
    virtual ~AbstractDataImage ();
    
    
    void get_properties_values (std::string& data, double& x, double& y, double& width, double& height);
    const std::vector<std::string>& get_properties_name () const override;
    virtual FatChildProcess* find_child_impl (const std::string&) override;
		AbstractTextProperty* data () { return (AbstractTextProperty*) find_child_impl ("data"); }

  protected:
    struct raw_props_t { std::string data; };
    raw_props_t raw_props;
    Coupling *_cdata;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
