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
  class AbstractOpacity : public AbstractStyle
  {
  public:
    AbstractOpacity (ParentProcess* parent, const std::string& name, double a);
    virtual ~AbstractOpacity ();
    
    
    void get_properties_values (double& a);
    const std::vector<std::string>& get_properties_name () const override;
    virtual FatChildProcess* find_child_impl (const std::string&) override;
		AbstractDoubleProperty* a () { return (AbstractDoubleProperty*) find_child_impl ("a"); }

  protected:
    struct raw_props_t { double a; };
    raw_props_t raw_props;
    Coupling *_ca;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
