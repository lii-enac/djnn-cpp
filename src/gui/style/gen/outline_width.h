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
  class OutlineWidth : public AbstractStyle
  {
  public:
    OutlineWidth (ParentProcess* parent, const std::string& name, double width);
    virtual ~OutlineWidth ();
    void draw () override;
    OutlineWidth* clone () override;
    void get_properties_values (double& width);
    virtual FatChildProcess* find_child (const std::string&) override;
		AbstractDoubleProperty* width () { return (AbstractDoubleProperty*) find_child ("width"); }

  protected:
    struct raw_props_t { double width; };
    raw_props_t raw_props;
    CouplingWithData *_cwidth;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
