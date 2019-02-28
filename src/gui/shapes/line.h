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
  class Line : public AbstractGShape
  {
  public:
    Line (Process *p, const std::string& n, double x1, double y1, double x2, double y2);
    Line (double x1, double y1, double x2, double y2);
    virtual ~Line ();
    void draw () override;
    Process* clone () override;
    void get_properties_values (double& x1, double& y1, double& x2, double& y2);
    virtual Process* find_component (const string&) override;
		AbstractDoubleProperty* x1 () { return (AbstractDoubleProperty*) find_component ("x1"); }
		AbstractDoubleProperty* y1 () { return (AbstractDoubleProperty*) find_component ("y1"); }
		AbstractDoubleProperty* x2 () { return (AbstractDoubleProperty*) find_component ("x2"); }
		AbstractDoubleProperty* y2 () { return (AbstractDoubleProperty*) find_component ("y2"); }

  private:
    struct raw_props_t { double x1; double y1; double x2; double y2; };
    raw_props_t raw_props;
    Coupling *_cx1, *_cy1, *_cx2, *_cy2;
    void activate () override;
    void deactivate () override;
  };
}
