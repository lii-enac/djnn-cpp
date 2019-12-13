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
  class TextAnchor : public AbstractStyle
  {
  public:
    TextAnchor (Process *parent, const std::string& name, int anchor);
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
