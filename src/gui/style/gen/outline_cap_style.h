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
  class OutlineCapStyle : public AbstractStyle
  {
  public:
    OutlineCapStyle (Process *parent, const std::string& name, int cap);
    OutlineCapStyle (int cap);
    virtual ~OutlineCapStyle ();
    void draw () override;
    Process* clone () override;
    void get_properties_values (int& cap);
    virtual Process* find_component (const string&) override;
		AbstractIntProperty* cap () { return (AbstractIntProperty*) find_component ("cap"); }

  protected:
    struct raw_props_t { int cap; };
    raw_props_t raw_props;
    Coupling *_ccap;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
