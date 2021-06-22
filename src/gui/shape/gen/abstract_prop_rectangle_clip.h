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



namespace djnn
{
  class AbstractPropRectangleClip : public AbstractGShape
  {
  public:
    AbstractPropRectangleClip (ParentProcess* parent, const djnn::string& name, double x, double y, double width, double height);
    virtual ~AbstractPropRectangleClip ();
    
    
    void get_properties_values (double& x, double& y, double& width, double& height);
    const vector<djnn::string>& get_properties_name () const override;
    virtual FatChildProcess* find_child_impl (const djnn::string&) override;
		AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_child_impl ("x"); }
		AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_child_impl ("y"); }
		AbstractDoubleProperty* width () { return (AbstractDoubleProperty*) find_child_impl ("width"); }
		AbstractDoubleProperty* height () { return (AbstractDoubleProperty*) find_child_impl ("height"); }

  protected:
    struct raw_props_t { double x; double y; double width; double height; };
    raw_props_t raw_props;
    Coupling *_cx, *_cy, *_cwidth, *_cheight;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
