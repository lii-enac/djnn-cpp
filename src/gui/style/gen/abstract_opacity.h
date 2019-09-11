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
 */


#pragma once

namespace djnn
{
  class AbstractOpacity : public AbstractStyle
  {
  public:
    AbstractOpacity (Process *p, const std::string& n, double alpha);
    AbstractOpacity (double alpha);
    virtual ~AbstractOpacity ();
    
    
    void get_properties_values (double& alpha);
    virtual Process* find_component (const string&) override;
		AbstractDoubleProperty* alpha () { return (AbstractDoubleProperty*) find_component ("alpha"); }

  protected:
    struct raw_props_t { double alpha; };
    raw_props_t raw_props;
    Coupling *_calpha;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
