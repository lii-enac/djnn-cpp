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
  class AbstractScaling : public AbstractTransformation
  {
  public:
    AbstractScaling (ParentProcess* parent, const std::string& name, double sx, double sy, double cx, double cy);
    virtual ~AbstractScaling ();
    
    
    void get_properties_values (double& sx, double& sy, double& cx, double& cy);
    std::vector<std::string> get_properties_name () override {
      std::vector<std::string> res;
      res.push_back ("sx");
			res.push_back ("sy");
			res.push_back ("cx");
			res.push_back ("cy");
      return res;
    }
    virtual FatChildProcess* find_child (const std::string&) override;
		AbstractDoubleProperty* sx () { return (AbstractDoubleProperty*) find_child ("sx"); }
		AbstractDoubleProperty* sy () { return (AbstractDoubleProperty*) find_child ("sy"); }
		AbstractDoubleProperty* cx () { return (AbstractDoubleProperty*) find_child ("cx"); }
		AbstractDoubleProperty* cy () { return (AbstractDoubleProperty*) find_child ("cy"); }

  protected:
    struct raw_props_t { double sx; double sy; double cx; double cy; };
    raw_props_t raw_props;
    CouplingWithData *_csx, *_csy, *_ccx, *_ccy;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
