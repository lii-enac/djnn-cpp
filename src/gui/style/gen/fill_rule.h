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
  class FillRule : public AbstractStyle
  {
  public:
    FillRule (Process *parent, const std::string& name, int rule);
    FillRule (int rule);
    virtual ~FillRule ();
    void draw () override;
    Process* clone () override;
    void get_properties_values (int& rule);
    virtual Process* find_component (const string&) override;
		AbstractIntProperty* rule () { return (AbstractIntProperty*) find_component ("rule"); }

  protected:
    struct raw_props_t { int rule; };
    raw_props_t raw_props;
    Coupling *_crule;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
