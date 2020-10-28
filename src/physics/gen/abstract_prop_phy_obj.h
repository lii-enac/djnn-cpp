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
  class AbstractPropPhyObj : public AbstractPObj
  {
  public:
    AbstractPropPhyObj (ParentProcess* parent, const std::string& name, double x, double y, double z, double dx, double dy, double dz, double roll, double pitch, double yall, double mass, double density, double friction);
    virtual ~AbstractPropPhyObj ();
    
    
    void get_properties_values (double& x, double& y, double& z, double& dx, double& dy, double& dz, double& roll, double& pitch, double& yall, double& mass, double& density, double& friction);
    std::vector<std::string> get_properties_name () override {
      std::vector<std::string> res;
      res.push_back ("x");
			res.push_back ("y");
			res.push_back ("z");
			res.push_back ("dx");
			res.push_back ("dy");
			res.push_back ("dz");
			res.push_back ("roll");
			res.push_back ("pitch");
			res.push_back ("yall");
			res.push_back ("mass");
			res.push_back ("density");
			res.push_back ("friction");
      return res;
    }
    virtual FatChildProcess* find_child_impl (const std::string&) override;
		AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_child_impl ("x"); }
		AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_child_impl ("y"); }
		AbstractDoubleProperty* z () { return (AbstractDoubleProperty*) find_child_impl ("z"); }
		AbstractDoubleProperty* dx () { return (AbstractDoubleProperty*) find_child_impl ("dx"); }
		AbstractDoubleProperty* dy () { return (AbstractDoubleProperty*) find_child_impl ("dy"); }
		AbstractDoubleProperty* dz () { return (AbstractDoubleProperty*) find_child_impl ("dz"); }
		AbstractDoubleProperty* roll () { return (AbstractDoubleProperty*) find_child_impl ("roll"); }
		AbstractDoubleProperty* pitch () { return (AbstractDoubleProperty*) find_child_impl ("pitch"); }
		AbstractDoubleProperty* yall () { return (AbstractDoubleProperty*) find_child_impl ("yall"); }
		AbstractDoubleProperty* mass () { return (AbstractDoubleProperty*) find_child_impl ("mass"); }
		AbstractDoubleProperty* density () { return (AbstractDoubleProperty*) find_child_impl ("density"); }
		AbstractDoubleProperty* friction () { return (AbstractDoubleProperty*) find_child_impl ("friction"); }

  protected:
    struct raw_props_t { double x; double y; double z; double dx; double dy; double dz; double roll; double pitch; double yall; double mass; double density; double friction; };
    raw_props_t raw_props;
    CouplingWithData *_cx, *_cy, *_cz, *_cdx, *_cdy, *_cdz, *_croll, *_cpitch, *_cyall, *_cmass, *_cdensity, *_cfriction;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
