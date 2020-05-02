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
  class FillRule : public AbstractStyle
  {
  public:
    FillRule (FatProcess *parent, const std::string& name, int rule);
    virtual ~FillRule ();
    void draw () override;
    FillRule* clone () override;
    void get_properties_values (int& rule);
    virtual ChildProcess* find_child (const std::string&) override;
		AbstractIntProperty* rule () { return (AbstractIntProperty*) find_child ("rule"); }

  protected:
    struct raw_props_t { int rule; };
    raw_props_t raw_props;
    CouplingWithData *_crule;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
