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
  class AbstractScaling : public AbstractTransformation
  {
  public:
    AbstractScaling (Process *p, const std::string& n, double sx, double sy, double cx, double cy);
    AbstractScaling (double sx, double sy, double cx, double cy);
    virtual ~AbstractScaling ();
    
    
    void get_properties_values (double& sx, double& sy, double& cx, double& cy);
    virtual Process* find_component (const string&) override;
		AbstractDoubleProperty* sx () { return (AbstractDoubleProperty*) find_component ("sx"); }
		AbstractDoubleProperty* sy () { return (AbstractDoubleProperty*) find_component ("sy"); }
		AbstractDoubleProperty* cx () { return (AbstractDoubleProperty*) find_component ("cx"); }
		AbstractDoubleProperty* cy () { return (AbstractDoubleProperty*) find_component ("cy"); }

  protected:
    struct raw_props_t { double sx; double sy; double cx; double cy; };
    raw_props_t raw_props;
    Coupling *_csx, *_csy, *_ccx, *_ccy;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
