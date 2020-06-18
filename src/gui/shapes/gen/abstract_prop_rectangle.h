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
  class AbstractPropRectangle : public AbstractGShape
  {
  public:
    AbstractPropRectangle (FatProcess *parent, const std::string& name, double x, double y, double width, double height, double rx=0, double ry=0);
    virtual ~AbstractPropRectangle ();
    
    
    void get_properties_values (double& x, double& y, double& width, double& height, double& rx, double& ry);
    virtual FatChildProcess* find_child (const std::string&) override;
		AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_child ("x"); }
		AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_child ("y"); }
		AbstractDoubleProperty* width () { return (AbstractDoubleProperty*) find_child ("width"); }
		AbstractDoubleProperty* height () { return (AbstractDoubleProperty*) find_child ("height"); }
		AbstractDoubleProperty* rx () { return (AbstractDoubleProperty*) find_child ("rx"); }
		AbstractDoubleProperty* ry () { return (AbstractDoubleProperty*) find_child ("ry"); }

  protected:
    struct raw_props_t { double x; double y; double width; double height; double rx; double ry; };
    raw_props_t raw_props;
    CouplingWithData *_cx, *_cy, *_cwidth, *_cheight, *_crx, *_cry;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
