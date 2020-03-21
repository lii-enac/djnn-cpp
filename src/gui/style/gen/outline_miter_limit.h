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
  class OutlineMiterLimit : public AbstractStyle
  {
  public:
    OutlineMiterLimit (Process *parent, const std::string& name, int limit);
    virtual ~OutlineMiterLimit ();
    void draw () override;
    Process* clone () override;
    void get_properties_values (int& limit);
    virtual Process* find_child (const std::string&) override;
		AbstractIntProperty* limit () { return (AbstractIntProperty*) find_child ("limit"); }

  protected:
    struct raw_props_t { int limit; };
    raw_props_t raw_props;
    Coupling *_climit;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
