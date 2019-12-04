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
 */


#pragma once

namespace djnn
{
  class AbstractPropRectangle : public AbstractGShape
  {
  public:
    AbstractPropRectangle (Process *parent, const std::string& name, double x, double y, double width, double height, double rx=0, double ry=0);
    AbstractPropRectangle (double x, double y, double width, double height, double rx=0, double ry=0);
    virtual ~AbstractPropRectangle ();
    
    Process* clone () override;
    void get_properties_values (double& x, double& y, double& width, double& height, double& rx, double& ry);
    virtual Process* find_component (const string&) override;
		AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_component ("x"); }
		AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_component ("y"); }
		AbstractDoubleProperty* width () { return (AbstractDoubleProperty*) find_component ("width"); }
		AbstractDoubleProperty* height () { return (AbstractDoubleProperty*) find_component ("height"); }
		AbstractDoubleProperty* rx () { return (AbstractDoubleProperty*) find_component ("rx"); }
		AbstractDoubleProperty* ry () { return (AbstractDoubleProperty*) find_component ("ry"); }

  protected:
    struct raw_props_t { double x; double y; double width; double height; double rx; double ry; };
    raw_props_t raw_props;
    Coupling *_cx, *_cy, *_cwidth, *_cheight, *_crx, *_cry;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
