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

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"



namespace djnn
{
  class AbstractVolume : public AbstractAudioStyle
  {
  public:
    AbstractVolume (ParentProcess* parent, const std::string& name, double volume);
    virtual ~AbstractVolume ();
    
    
    void get_properties_values (double& volume);
    const vector<std::string>& get_properties_name () const override;
    virtual FatChildProcess* find_child_impl (const std::string&) override;
		AbstractDoubleProperty* volume () { return (AbstractDoubleProperty*) find_child_impl ("volume"); }

  protected:
    struct raw_props_t { double volume; };
    raw_props_t raw_props;
    Coupling *_cvolume;
    void impl_activate () override;
    void impl_deactivate () override;
  };
}
