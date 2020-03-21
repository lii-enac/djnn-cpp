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

namespace djnn
{
  class AbstractPropRectangleClip : public AbstractGShape
  {
  public:
    AbstractPropRectangleClip (Process *parent, const std::string& name, double x, double y, double width, double height);
    virtual ~AbstractPropRectangleClip ();
    
    
    void get_properties_values (double& x, double& y, double& width, double& height);
    virtual Process* find_child (const std::string&) override;
		AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_child ("x"); }
		AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_child ("y"); }
		AbstractDoubleProperty* width () { return (AbstractDoubleProperty*) find_child ("width"); }
		AbstractDoubleProperty* height () { return (AbstractDoubleProperty*) find_child ("height"); }

  protected:
    struct raw_props_t { double x; double y; double width; double height; };
    raw_props_t raw_props;
    Coupling *_cx, *_cy, *_cwidth, *_cheight;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
