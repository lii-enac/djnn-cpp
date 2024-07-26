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

#include "core/utils/containers/vector.h"
using djnnstl::vector;

#include "gui/shape/abstract_gshape.h"


namespace djnn
{
  class AbstractPropEllipse : public AbstractGShape
  {
  public:
    AbstractPropEllipse (CoreProcess* parent, const string& name, double cx, double cy, double rx, double ry);
    virtual ~AbstractPropEllipse ();
    
    
    void get_properties_values (double& cx, double& cy, double& rx, double& ry);
    const vector<string>& get_properties_name () const override;
    virtual CoreProcess* find_child_impl (const string&) override;
		AbstractDoubleProperty* cx () { return (AbstractDoubleProperty*) find_child_impl ("cx"); }
		AbstractDoubleProperty* cy () { return (AbstractDoubleProperty*) find_child_impl ("cy"); }
		AbstractDoubleProperty* rx () { return (AbstractDoubleProperty*) find_child_impl ("rx"); }
		AbstractDoubleProperty* ry () { return (AbstractDoubleProperty*) find_child_impl ("ry"); }

  protected:
    struct raw_props_t { double cx; double cy; double rx; double ry; };
    raw_props_t raw_props;
    Coupling *_ccx, *_ccy, *_crx, *_cry;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
