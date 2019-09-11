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
  class AbstractPropFontFamily : public AbstractStyle
  {
  public:
    AbstractPropFontFamily (Process *p, const std::string& n, std::string family);
    AbstractPropFontFamily (std::string family);
    virtual ~AbstractPropFontFamily ();
    
    
    void get_properties_values (std::string& family);
    virtual Process* find_component (const string&) override;
		AbstractTextProperty* family () { return (AbstractTextProperty*) find_component ("family"); }

  protected:
    struct raw_props_t { std::string family; };
    raw_props_t raw_props;
    Coupling *_cfamily;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
