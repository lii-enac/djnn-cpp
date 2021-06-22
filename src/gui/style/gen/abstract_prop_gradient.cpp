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


#include "gui/shape/abstract_gshape.h"
#include "display/window.h"
#include "gui/style/abstract_style.h"

#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display-dev.h" // DisplayBackend::instance
#include "display/abstract_display.h"

#include "audio/style/audio_style.h"
#include "physics/abstract_pobj.h"
#include "physics/physics.h"


#include "abstract_prop_gradient.h"

namespace djnn
{
  AbstractPropGradient::AbstractPropGradient (ParentProcess* parent, const djnn::string& name, int spread, int coords) :
    AbstractStyle (parent, name),
    raw_props{.spread=spread, .coords=coords},
    _cspread (nullptr), _ccoords (nullptr)
  {
    
    
  }

  AbstractPropGradient::~AbstractPropGradient ()
  {
    remove_edge (_cspread);
		remove_edge (_ccoords);
    delete _cspread;
		delete _ccoords;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > 0) {
      symtable_t::iterator it;

      it = find_child_iterator ("spread");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("coords");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  FatChildProcess*
  AbstractPropGradient::find_child_impl (const djnn::string& name)
  {
    auto * res = AbstractStyle::find_child_impl(name);
    if (res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    Coupling ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef djnn::string text;
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

  const vector<djnn::string>&
  AbstractPropGradient::get_properties_name () const
  {
    static const vector<djnn::string> res = {
    "spread",
			"coords",
    };
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
    auto _frame = get_frame ();
    enable(_cspread, _frame->damaged ());
		enable(_ccoords, _frame->damaged ());
  }

  void
  AbstractPropGradient::impl_deactivate ()
  {
    disable(_cspread);
		disable(_ccoords);
    AbstractStyle::impl_deactivate ();
  }

  

  

} /* namespace djnn */
