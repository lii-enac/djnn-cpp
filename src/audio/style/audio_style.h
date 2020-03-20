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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 */

#pragma once

#include "audio/abstract_sobj.h"
#include "core/ontology/process.h"

#include <vector>



namespace djnn
{
  class AbstractAudioStyle : public AbstractSObj
  {
  public:
    AbstractAudioStyle (Process *parent, const std::string &name) : AbstractSObj(parent, name) {}
    virtual ~AbstractAudioStyle () {};
    virtual void impl_activate () override {}
    virtual void impl_deactivate () override {}
  };
}

#include "gen/abstract_volume.h"

namespace djnn
{
  class Volume : public AbstractVolume
  {
  public:
  	Volume(Process* parent, const std::string& name, double volume)
  	: AbstractVolume(parent, name, volume)
  	{
  		Process::finalize_construction (parent, name);
  	}

  	/*
  	void sound () {
  		auto _frame = frame ();
    	if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
  			AudioBackend::instance ()->load_volume (this);
  		}
  	}*/

  };

}