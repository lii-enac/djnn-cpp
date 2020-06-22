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
  class AbstractPropGradientStop : public AbstractStyle
  {
  public:
    AbstractPropGradientStop (ParentProcess* parent, const std::string& name, double r, double g, double b, double a, double offset);
    virtual ~AbstractPropGradientStop ();
    
    
    void get_properties_values (double& r, double& g, double& b, double& a, double& offset);
    virtual FatChildProcess* find_child (const std::string&) override;
		AbstractDoubleProperty* r () { return (AbstractDoubleProperty*) find_child ("r"); }
		AbstractDoubleProperty* g () { return (AbstractDoubleProperty*) find_child ("g"); }
		AbstractDoubleProperty* b () { return (AbstractDoubleProperty*) find_child ("b"); }
		AbstractDoubleProperty* a () { return (AbstractDoubleProperty*) find_child ("a"); }
		AbstractDoubleProperty* offset () { return (AbstractDoubleProperty*) find_child ("offset"); }

  protected:
    struct raw_props_t { double r; double g; double b; double a; double offset; };
    raw_props_t raw_props;
    CouplingWithData *_cr, *_cg, *_cb, *_ca, *_coffset;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
