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
    AbstractSkew (Process *parent, const std::string& name, double a);
    virtual ~AbstractSkew ();
    
    
    void get_properties_values (double& a);
    virtual Process* find_component (const string&) override;
		AbstractDoubleProperty* a () { return (AbstractDoubleProperty*) find_component ("a"); }

  protected:
    struct raw_props_t { double a; };
    raw_props_t raw_props;
    Coupling *_ca;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
