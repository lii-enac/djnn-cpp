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
  class AbstractPropEllipse : public AbstractGShape
  {
  public:
    AbstractPropEllipse (ParentProcess* parent, const std::string& name, double cx, double cy, double rx, double ry);
    virtual ~AbstractPropEllipse ();
    
    
    void get_properties_values (double& cx, double& cy, double& rx, double& ry);
    std::vector<std::string> get_properties_name () override {
      std::vector<std::string> res;
      res.push_back ("cx");
			res.push_back ("cy");
			res.push_back ("rx");
			res.push_back ("ry");
      return res;
    }
    virtual FatChildProcess* find_child_impl (const std::string&) override;
		AbstractDoubleProperty* cx () { return (AbstractDoubleProperty*) find_child_impl ("cx"); }
		AbstractDoubleProperty* cy () { return (AbstractDoubleProperty*) find_child_impl ("cy"); }
		AbstractDoubleProperty* rx () { return (AbstractDoubleProperty*) find_child_impl ("rx"); }
		AbstractDoubleProperty* ry () { return (AbstractDoubleProperty*) find_child_impl ("ry"); }

  protected:
    struct raw_props_t { double cx; double cy; double rx; double ry; };
    raw_props_t raw_props;
    CouplingWithData *_ccx, *_ccy, *_crx, *_cry;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
