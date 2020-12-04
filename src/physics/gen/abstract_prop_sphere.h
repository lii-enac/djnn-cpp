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
  class AbstractPropSphere : public PhyObj
  {
  public:
    AbstractPropSphere (ParentProcess* parent, const std::string& name, double radius);
    virtual ~AbstractPropSphere ();
    
    
    void get_properties_values (double& radius);
    const std::vector<std::string>& get_properties_name () const override {
      static const std::vector<std::string> res = {
      "radius",
      };
      return res;
    }
    virtual FatChildProcess* find_child_impl (const std::string&) override;
		AbstractDoubleProperty* radius () { return (AbstractDoubleProperty*) find_child_impl ("radius"); }

  protected:
    struct raw_props_t { double radius; };
    raw_props_t raw_props;
    CouplingWithData *_cradius;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
