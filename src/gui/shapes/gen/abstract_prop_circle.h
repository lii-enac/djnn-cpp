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
  class AbstractPropCircle : public AbstractGShape
  {
  public:
    AbstractPropCircle (Process *parent, const std::string& name, double cx, double cy, double r);
    AbstractPropCircle (double cx, double cy, double r);
    virtual ~AbstractPropCircle ();
    
    
    void get_properties_values (double& cx, double& cy, double& r);
    virtual Process* find_component (const string&) override;
		AbstractDoubleProperty* cx () { return (AbstractDoubleProperty*) find_component ("cx"); }
		AbstractDoubleProperty* cy () { return (AbstractDoubleProperty*) find_component ("cy"); }
		AbstractDoubleProperty* r () { return (AbstractDoubleProperty*) find_component ("r"); }

  protected:
    struct raw_props_t { double cx; double cy; double r; };
    raw_props_t raw_props;
    Coupling *_ccx, *_ccy, *_cr;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
