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
  class AbstractPropRadialGradient : public AbstractGradient
  {
  public:
    AbstractPropRadialGradient (Process *p, const std::string& n, double cx, double cy, double r, double fx, double fy, int spread, int coords);
    AbstractPropRadialGradient (double cx, double cy, double r, double fx, double fy, int spread, int coords);
    virtual ~AbstractPropRadialGradient ();
    
    
    void get_properties_values (double& cx, double& cy, double& r, double& fx, double& fy, int& spread, int& coords);
    virtual Process* find_component (const string&) override;
		AbstractDoubleProperty* cx () { return (AbstractDoubleProperty*) find_component ("cx"); }
		AbstractDoubleProperty* cy () { return (AbstractDoubleProperty*) find_component ("cy"); }
		AbstractDoubleProperty* r () { return (AbstractDoubleProperty*) find_component ("r"); }
		AbstractDoubleProperty* fx () { return (AbstractDoubleProperty*) find_component ("fx"); }
		AbstractDoubleProperty* fy () { return (AbstractDoubleProperty*) find_component ("fy"); }

  protected:
    struct raw_props_t { double cx; double cy; double r; double fx; double fy; };
    raw_props_t raw_props;
    Coupling *_ccx, *_ccy, *_cr, *_cfx, *_cfy;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
