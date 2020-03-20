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
  class AbstractPropGradient : public AbstractStyle
  {
  public:
    AbstractPropGradient (Process *parent, const std::string& name, int spread, int coords);
    virtual ~AbstractPropGradient ();
    
    
    void get_properties_values (int& spread, int& coords);
    virtual Process* find_component (const std::string&) override;
		AbstractIntProperty* spread () { return (AbstractIntProperty*) find_component ("spread"); }
		AbstractIntProperty* coords () { return (AbstractIntProperty*) find_component ("coords"); }

  protected:
    struct raw_props_t { int spread; int coords; };
    raw_props_t raw_props;
    Coupling *_cspread, *_ccoords;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
