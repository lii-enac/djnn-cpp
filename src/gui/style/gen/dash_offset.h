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
  class DashOffset : public AbstractStyle
  {
  public:
    DashOffset (Process *parent, const std::string& name, double offset);
    virtual ~DashOffset ();
    void draw () override;
    Process* clone () override;
    void get_properties_values (double& offset);
    virtual Process* find_child (const std::string&) override;
		AbstractDoubleProperty* offset () { return (AbstractDoubleProperty*) find_child ("offset"); }

  protected:
    struct raw_props_t { double offset; };
    raw_props_t raw_props;
    Coupling *_coffset;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
