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
  class AbstractDataImage : public AbstractImage
  {
  public:
    AbstractDataImage (Process *parent, const std::string& name, std::string data, double x, double y, double width, double height);
    virtual ~AbstractDataImage ();
    
    
    void get_properties_values (std::string& data, double& x, double& y, double& width, double& height);
    virtual Process* find_component (const string&) override;
		AbstractTextProperty* data () { return (AbstractTextProperty*) find_component ("data"); }

  protected:
    struct raw_props_t { std::string data; };
    raw_props_t raw_props;
    Coupling *_cdata;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}