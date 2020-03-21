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


#include "abstract_prop_gradient.h"

namespace djnn
{
  AbstractPropGradient::AbstractPropGradient (Process *parent, const std::string& name, int spread, int coords) :
    AbstractStyle (parent, name),
    raw_props{.spread=spread, .coords=coords},
    _cspread (nullptr), _ccoords (nullptr)
  {
    
    
  }

  AbstractPropGradient::~AbstractPropGradient ()
  {
    delete _cspread;
		delete _ccoords;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (symtable ().size () > 0) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("spread");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("coords");
			if (it != symtable ().end ())
				delete it->second;
    }
  }
 
  Process*
  AbstractPropGradient::find_child (const std::string& name)
  {
    Process* res = AbstractStyle::find_child(name);
    if(res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    Coupling ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef std::string text;
    text* rawp_Text = nullptr;
    int notify_mask = notify_none;
    
    if(name=="spread") {
      coupling=&_cspread;
      rawp_Int=&raw_props.spread;
      notify_mask = notify_damaged_style;
      prop_Int=true;
    } else
    if(name=="coords") {
      coupling=&_ccoords;
      rawp_Int=&raw_props.coords;
      notify_mask = notify_damaged_style;
      prop_Int=true;
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
  AbstractPropGradient::get_properties_values (int& spread, int& coords)
  {
    spread = raw_props.spread;
		coords = raw_props.coords;
    
  }

  void
  AbstractPropGradient::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if(_cspread) _cspread->enable (_frame);
		if(_ccoords) _ccoords->enable (_frame);
  }

  void
  AbstractPropGradient::impl_deactivate ()
  {
    if(_cspread) _cspread->disable ();
		if(_ccoords) _ccoords->disable ();
    AbstractStyle::impl_deactivate ();
  }

  

  

} /* namespace djnn */
