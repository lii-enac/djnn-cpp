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
  class AbstractPropCircle : public AbstractGShape
  {
  public:
    AbstractPropCircle (Process *parent, const std::string& name, double cx, double cy, double r);
    virtual ~AbstractPropCircle ();
    
    
    void get_properties_values (double& cx, double& cy, double& r);
    virtual Process* find_child (const std::string&) override;
		AbstractDoubleProperty* cx () { return (AbstractDoubleProperty*) find_child ("cx"); }
		AbstractDoubleProperty* cy () { return (AbstractDoubleProperty*) find_child ("cy"); }
		AbstractDoubleProperty* r () { return (AbstractDoubleProperty*) find_child ("r"); }

  protected:
    struct raw_props_t { double cx; double cy; double r; };
    raw_props_t raw_props;
    CouplingWithData *_ccx, *_ccy, *_cr;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
