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
  class GradientStop : public AbstractStyle
  {
  public:
    GradientStop (Process *p, const std::string& n, double r, double g, double b, double a, double offset);
    GradientStop (double r, double g, double b, double a, double offset);
    virtual ~GradientStop ();
    void draw () override;
    Process* clone () override;
    void get_properties_values (double& r, double& g, double& b, double& a, double& offset);
    virtual Process* find_component (const string&) override;
		AbstractDoubleProperty* r () { return (AbstractDoubleProperty*) find_component ("r"); }
		AbstractDoubleProperty* g () { return (AbstractDoubleProperty*) find_component ("g"); }
		AbstractDoubleProperty* b () { return (AbstractDoubleProperty*) find_component ("b"); }
		AbstractDoubleProperty* a () { return (AbstractDoubleProperty*) find_component ("a"); }
		AbstractDoubleProperty* offset () { return (AbstractDoubleProperty*) find_component ("offset"); }

  protected:
    struct raw_props_t { double r; double g; double b; double a; double offset; };
    raw_props_t raw_props;
    Coupling *_cr, *_cg, *_cb, *_ca, *_coffset;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
