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
  class RectangleClip : public AbstractGShape
  {
  public:
    RectangleClip (Process *p, const std::string& n, double x, double y, double width, double height);
    RectangleClip (double x, double y, double width, double height);
    virtual ~RectangleClip ();
    void draw () override;
    Process* clone () override;
    void get_properties_values (double& x, double& y, double& width, double& height);
    virtual Process* find_component (const string&) override;
		AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_component ("x"); }
		AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_component ("y"); }
		AbstractDoubleProperty* width () { return (AbstractDoubleProperty*) find_component ("width"); }
		AbstractDoubleProperty* height () { return (AbstractDoubleProperty*) find_component ("height"); }

  private:
    struct raw_props_t { double x; double y; double width; double height; };
    raw_props_t raw_props;
    Coupling *_cx, *_cy, *_cwidth, *_cheight;
    void activate () override;
    void deactivate () override;
  };
}
