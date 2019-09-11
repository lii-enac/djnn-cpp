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
  class AbstractRotation : public AbstractTransformation
  {
  public:
    AbstractRotation (Process *p, const std::string& n, double a, double cx, double cy);
    AbstractRotation (double a, double cx, double cy);
    virtual ~AbstractRotation ();
    
    
    void get_properties_values (double& a, double& cx, double& cy);
    virtual Process* find_component (const string&) override;
		AbstractDoubleProperty* a () { return (AbstractDoubleProperty*) find_component ("a"); }
		AbstractDoubleProperty* cx () { return (AbstractDoubleProperty*) find_component ("cx"); }
		AbstractDoubleProperty* cy () { return (AbstractDoubleProperty*) find_component ("cy"); }

  protected:
    struct raw_props_t { double a; double cx; double cy; };
    raw_props_t raw_props;
    Coupling *_ca, *_ccx, *_ccy;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
