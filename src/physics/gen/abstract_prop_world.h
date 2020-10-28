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
  class AbstractPropWorld : public AbstractPObj
  {
  public:
    AbstractPropWorld (ParentProcess* parent, const std::string& name, double x, double y, double z, double dt);
    virtual ~AbstractPropWorld ();
    
    
    void get_properties_values (double& x, double& y, double& z, double& dt);
    std::vector<std::string> get_properties_name () override {
      std::vector<std::string> res;
      res.push_back ("x");
			res.push_back ("y");
			res.push_back ("z");
			res.push_back ("dt");
      return res;
    }
    virtual FatChildProcess* find_child_impl (const std::string&) override;
		AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_child_impl ("x"); }
		AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_child_impl ("y"); }
		AbstractDoubleProperty* z () { return (AbstractDoubleProperty*) find_child_impl ("z"); }
		AbstractDoubleProperty* dt () { return (AbstractDoubleProperty*) find_child_impl ("dt"); }

  protected:
    struct raw_props_t { double x; double y; double z; double dt; };
    raw_props_t raw_props;
    CouplingWithData *_cx, *_cy, *_cz, *_cdt;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
