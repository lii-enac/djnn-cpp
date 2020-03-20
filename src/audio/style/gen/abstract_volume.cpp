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


#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display.h"
#include "display/abstract_display.h"
#include "display/window.h"
#include "gui/shapes/shapes.h"
#include "gui/style/style.h"
#include "core/ontology/process.h"
#include "core/ontology/coupling.h"

#include "audio/style/audio_style.h"


#include "abstract_volume.h"

namespace djnn
{
  AbstractVolume::AbstractVolume (Process *parent, const std::string& name, double volume) :
    AbstractAudioStyle (parent, name),
    raw_props{.volume=volume},
    _cvolume (nullptr)
  {
    
    
  }

  AbstractVolume::~AbstractVolume ()
  {
    delete _cvolume;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (symtable ().size () > 0) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("volume");
			if (it != symtable ().end ())
				delete it->second;
    }
  }
 
  Process*
  AbstractVolume::find_component (const std::string& name)
  {
    Process* res = AbstractAudioStyle::find_component(name);
    if(res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    Coupling ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef std::string text;
    text* rawp_Text = nullptr;
    int notify_mask = notify_none;
    
    if(name=="volume") {
      coupling=&_cvolume;
      rawp_Double=&raw_props.volume;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
    return nullptr;

    if(prop_Double) {
      DoublePropertyProxy* prop = nullptr; // do not cache
      res = create_GObj_prop(&prop, coupling, rawp_Double, name, notify_mask);
    }
    else if(prop_Int) {
      IntPropertyProxy* prop = nullptr; // do not cache
      res = create_GObj_prop(&prop, coupling, rawp_Int, name, notify_mask);
    }
    else if(prop_Text) {
      TextPropertyProxy* prop = nullptr; // do not cache
      res = create_GObj_prop(&prop, coupling, rawp_Text, name, notify_mask);
    }

    return res;
  }

  void
  AbstractVolume::get_properties_values (double& volume)
  {
    volume = raw_props.volume;
    
  }

  void
  AbstractVolume::impl_activate ()
  {
    AbstractAudioStyle::impl_activate ();
    //auto _frame = frame ();
    //if(_cvolume) _cvolume->enable (_frame);
  }

  void
  AbstractVolume::impl_deactivate ()
  {
    if(_cvolume) _cvolume->disable ();
    AbstractAudioStyle::impl_deactivate ();
  }

  

  

} /* namespace djnn */
