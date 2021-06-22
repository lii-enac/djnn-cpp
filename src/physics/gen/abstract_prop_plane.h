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
  class AbstractPropPlane : public PhyObj
  {
  public:
    AbstractPropPlane (ParentProcess* parent, const string& name, double a, double b, double c, double d);
    virtual ~AbstractPropPlane ();
    
    
    void get_properties_values (double& a, double& b, double& c, double& d);
    const vector<string>& get_properties_name () const override;
    virtual FatChildProcess* find_child_impl (const string&) override;
		AbstractDoubleProperty* a () { return (AbstractDoubleProperty*) find_child_impl ("a"); }
		AbstractDoubleProperty* b () { return (AbstractDoubleProperty*) find_child_impl ("b"); }
		AbstractDoubleProperty* c () { return (AbstractDoubleProperty*) find_child_impl ("c"); }
		AbstractDoubleProperty* d () { return (AbstractDoubleProperty*) find_child_impl ("d"); }

  protected:
    struct raw_props_t { double a; double b; double c; double d; };
    raw_props_t raw_props;
    Coupling *_ca, *_cb, *_cc, *_cd;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
