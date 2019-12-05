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
  class AbstractPropEllipse : public AbstractGShape
  {
  public:
    AbstractPropEllipse (Process *parent, const std::string& name, double cx, double cy, double rx, double ry);
    AbstractPropEllipse (double cx, double cy, double rx, double ry);
    virtual ~AbstractPropEllipse ();
    
    
    void get_properties_values (double& cx, double& cy, double& rx, double& ry);
    virtual Process* find_component (const string&) override;
		AbstractDoubleProperty* cx () { return (AbstractDoubleProperty*) find_component ("cx"); }
		AbstractDoubleProperty* cy () { return (AbstractDoubleProperty*) find_component ("cy"); }
		AbstractDoubleProperty* rx () { return (AbstractDoubleProperty*) find_component ("rx"); }
		AbstractDoubleProperty* ry () { return (AbstractDoubleProperty*) find_component ("ry"); }

  protected:
    struct raw_props_t { double cx; double cy; double rx; double ry; };
    raw_props_t raw_props;
    Coupling *_ccx, *_ccy, *_crx, *_cry;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
