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

#include "gui/style/abstract_style.h"


namespace djnn
{
  class OutlineWidth : public AbstractStyle
  {
  public:
    OutlineWidth (ParentProcess* parent, const djnn::string& name, double width);
    virtual ~OutlineWidth ();
    void draw () override;
    OutlineWidth* clone () override;
    void get_properties_values (double& width);
    const vector<djnn::string>& get_properties_name () const override;
    virtual FatChildProcess* find_child_impl (const djnn::string&) override;
		AbstractDoubleProperty* width () { return (AbstractDoubleProperty*) find_child_impl ("width"); }

  protected:
    struct raw_props_t { double width; };
    raw_props_t raw_props;
    Coupling *_cwidth;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
