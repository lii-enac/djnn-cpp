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
  class AbstractRotation : public AbstractTransformation
  {
  public:
    AbstractRotation (ParentProcess* parent, const std::string& name, double a, double cx, double cy);
    virtual ~AbstractRotation ();
    
    
    void get_properties_values (double& a, double& cx, double& cy);
    std::vector<std::string> get_properties_name () override {
      std::vector<std::string> res;
      res.push_back ("a");
			res.push_back ("cx");
			res.push_back ("cy");
      return res;
    }
    virtual FatChildProcess* find_child_impl (const std::string&) override;
		AbstractDoubleProperty* a () { return (AbstractDoubleProperty*) find_child_impl ("a"); }
		AbstractDoubleProperty* cx () { return (AbstractDoubleProperty*) find_child_impl ("cx"); }
		AbstractDoubleProperty* cy () { return (AbstractDoubleProperty*) find_child_impl ("cy"); }

  protected:
    struct raw_props_t { double a; double cx; double cy; };
    raw_props_t raw_props;
    CouplingWithData *_ca, *_ccx, *_ccy;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
