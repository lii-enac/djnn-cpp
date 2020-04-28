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
  class AbstractPropFontSize : public AbstractStyle
  {
  public:
    AbstractPropFontSize (Process *parent, const std::string& name, int unit, double size);
    virtual ~AbstractPropFontSize ();
    
    
    void get_properties_values (int& unit, double& size);
    virtual Process* find_child (const std::string&) override;
		AbstractIntProperty* unit () { return (AbstractIntProperty*) find_child ("unit"); }
		AbstractDoubleProperty* size () { return (AbstractDoubleProperty*) find_child ("size"); }

  protected:
    struct raw_props_t { int unit; double size; };
    raw_props_t raw_props;
    CouplingWithData *_cunit, *_csize;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
