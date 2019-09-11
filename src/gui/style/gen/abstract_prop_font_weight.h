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
  class AbstractPropFontWeight : public AbstractStyle
  {
  public:
    AbstractPropFontWeight (Process *p, const std::string& n, int weight);
    AbstractPropFontWeight (int weight);
    virtual ~AbstractPropFontWeight ();
    
    
    void get_properties_values (int& weight);
    virtual Process* find_component (const string&) override;
		AbstractIntProperty* weight () { return (AbstractIntProperty*) find_component ("weight"); }

  protected:
    struct raw_props_t { int weight; };
    raw_props_t raw_props;
    Coupling *_cweight;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
