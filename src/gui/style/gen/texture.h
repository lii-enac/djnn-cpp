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
  class Texture : public AbstractStyle
  {
  public:
    Texture (Process *parent, const std::string& name, const std::string& path);
    virtual ~Texture ();
    void draw () override;
    Process* clone () override;
    void get_properties_values (std::string& path);
    virtual Process* find_child (const std::string&) override;
		AbstractTextProperty* path () { return (AbstractTextProperty*) find_child ("path"); }

  protected:
    struct raw_props_t { std::string path; };
    raw_props_t raw_props;
    Coupling *_cpath;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
