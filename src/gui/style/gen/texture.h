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
    Texture (ParentProcess* parent, const std::string& name, const std::string& path);
    virtual ~Texture ();
    void draw () override;
    Texture* clone () override;
    void get_properties_values (std::string& path);
    std::vector<std::string> get_properties_name () override {
      std::vector<std::string> res;
      res.push_back ("path");
      return res;
    }
    virtual FatChildProcess* find_child (const std::string&) override;
		AbstractTextProperty* path () { return (AbstractTextProperty*) find_child ("path"); }

  protected:
    struct raw_props_t { std::string path; };
    raw_props_t raw_props;
    CouplingWithData *_cpath;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
