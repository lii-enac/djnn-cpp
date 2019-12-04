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
  class AbstractPathImage : public AbstractImage
  {
  public:
    AbstractPathImage (Process *parent, const std::string& name, std::string path, double x, double y, double width, double height);
    AbstractPathImage (std::string path, double x, double y, double width, double height);
    virtual ~AbstractPathImage ();
    
    
    void get_properties_values (std::string& path, double& x, double& y, double& width, double& height);
    virtual Process* find_component (const string&) override;
		AbstractTextProperty* path () { return (AbstractTextProperty*) find_component ("path"); }

  protected:
    struct raw_props_t { std::string path; };
    raw_props_t raw_props;
    Coupling *_cpath;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
