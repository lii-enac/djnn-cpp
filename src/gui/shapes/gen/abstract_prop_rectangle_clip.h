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
    AbstractPropRectangleClip (ParentProcess* parent, const std::string& name, double x, double y, double width, double height);
    virtual ~AbstractPropRectangleClip ();
    
    
    void get_properties_values (double& x, double& y, double& width, double& height);
    const std::vector<std::string>& get_properties_name () const override {
      static const std::vector<std::string> res = {
      "x",
			"y",
			"width",
			"height",
      };
      return res;
    }
    virtual FatChildProcess* find_child_impl (const std::string&) override;
		AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_child_impl ("x"); }
		AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_child_impl ("y"); }
		AbstractDoubleProperty* width () { return (AbstractDoubleProperty*) find_child_impl ("width"); }
		AbstractDoubleProperty* height () { return (AbstractDoubleProperty*) find_child_impl ("height"); }

  protected:
    struct raw_props_t { double x; double y; double width; double height; };
    raw_props_t raw_props;
    CouplingWithData *_cx, *_cy, *_cwidth, *_cheight;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
