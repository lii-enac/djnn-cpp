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

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"



namespace djnn
{
  class AbstractPropPhyObj : public AbstractPObj
  {
  public:
    AbstractPropPhyObj (ParentProcess* parent, const string& name, double x, double y, double z, double dx, double dy, double dz, double roll, double pitch, double yall, double mass, double density, double friction);
    virtual ~AbstractPropPhyObj ();
    
    
    void get_properties_values (double& x, double& y, double& z, double& dx, double& dy, double& dz, double& roll, double& pitch, double& yall, double& mass, double& density, double& friction);
    const vector<string>& get_properties_name () const override;
    virtual FatChildProcess* find_child_impl (const string&) override;
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
    Coupling *_cx, *_cy, *_cz, *_cdx, *_cdy, *_cdz, *_croll, *_cpitch, *_cyall, *_cmass, *_cdensity, *_cfriction;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
