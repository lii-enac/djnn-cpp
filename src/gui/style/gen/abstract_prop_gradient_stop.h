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
    std::vector<std::string> get_properties_name () override {
      std::vector<std::string> res;
      res.push_back ("r");
			res.push_back ("g");
			res.push_back ("b");
			res.push_back ("a");
			res.push_back ("offset");
      return res;
    }
    virtual FatChildProcess* find_child_impl (const std::string&) override;
		AbstractDoubleProperty* r () { return (AbstractDoubleProperty*) find_child_impl ("r"); }
		AbstractDoubleProperty* g () { return (AbstractDoubleProperty*) find_child_impl ("g"); }
		AbstractDoubleProperty* b () { return (AbstractDoubleProperty*) find_child_impl ("b"); }
		AbstractDoubleProperty* a () { return (AbstractDoubleProperty*) find_child_impl ("a"); }
		AbstractDoubleProperty* offset () { return (AbstractDoubleProperty*) find_child_impl ("offset"); }

  protected:
    struct raw_props_t { double r; double g; double b; double a; double offset; };
    raw_props_t raw_props;
    CouplingWithData *_cr, *_cg, *_cb, *_ca, *_coffset;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
