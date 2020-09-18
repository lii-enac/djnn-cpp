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
  class AbstractSkew : public AbstractTransformation
  {
  public:
    AbstractSkew (ParentProcess* parent, const std::string& name, double a);
    virtual ~AbstractSkew ();
    
    
    void get_properties_values (double& a);
    std::vector<std::string> get_properties_name () override {
      std::vector<std::string> res;
      res.push_back ("a");
      return res;
    }
    virtual FatChildProcess* find_child (const std::string&) override;
		AbstractDoubleProperty* a () { return (AbstractDoubleProperty*) find_child ("a"); }

  protected:
    struct raw_props_t { double a; };
    raw_props_t raw_props;
    CouplingWithData *_ca;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
