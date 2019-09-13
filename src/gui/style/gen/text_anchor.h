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
  class TextAnchor : public AbstractStyle
  {
  public:
    TextAnchor (Process *p, const std::string& n, int anchor);
    TextAnchor (int anchor);
    virtual ~TextAnchor ();
    void draw () override;
    Process* clone () override;
    void get_properties_values (int& anchor);
    virtual Process* find_component (const string&) override;
		AbstractIntProperty* anchor () { return (AbstractIntProperty*) find_component ("anchor"); }

  protected:
    struct raw_props_t { int anchor; };
    raw_props_t raw_props;
    Coupling *_canchor;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
