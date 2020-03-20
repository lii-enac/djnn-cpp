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
  class OutlineJoinStyle : public AbstractStyle
  {
  public:
    OutlineJoinStyle (Process *parent, const std::string& name, int join);
    virtual ~OutlineJoinStyle ();
    void draw () override;
    Process* clone () override;
    void get_properties_values (int& join);
    virtual Process* find_component (const std::string&) override;
		AbstractIntProperty* join () { return (AbstractIntProperty*) find_component ("join"); }

  protected:
    struct raw_props_t { int join; };
    raw_props_t raw_props;
    Coupling *_cjoin;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
