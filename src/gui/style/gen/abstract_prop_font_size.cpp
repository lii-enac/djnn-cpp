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


#include "abstract_prop_font_size.h"

namespace djnn
{
  AbstractPropFontSize::AbstractPropFontSize (FatProcess *parent, const std::string& name, int unit, double size) :
    AbstractStyle (parent, name),
    raw_props{.unit=unit, .size=size},
    _cunit (nullptr), _csize (nullptr)
  {
    
    
  }

  AbstractPropFontSize::~AbstractPropFontSize ()
  {
    //remove_edge (_cunit) //don't know why it should not be done;
		//remove_edge (_csize) //don't know why it should not be done;
    delete _cunit;
		delete _csize;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > 0) {
      symtable_t::iterator it;

      it = find_child_iterator ("unit");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("size");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  ChildProcess*
  AbstractPropFontSize::find_child (const std::string& name)
  {
    auto * res = AbstractStyle::find_child(name);
    if(res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    CouplingWithData ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef std::string text;
    text* rawp_Text = nullptr;
    int notify_mask = notify_none;
    
    if(name=="unit") {
      coupling=&_cunit;
      rawp_Int=&raw_props.unit;
      notify_mask = notify_damaged_geometry;
      prop_Int=true;
    } else
    if(name=="size") {
      coupling=&_csize;
      rawp_Double=&raw_props.size;
      notify_mask = notify_damaged_geometry;
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
  AbstractPropFontSize::get_properties_values (int& unit, double& size)
  {
    unit = raw_props.unit;
		size = raw_props.size;
    
  }

  void
  AbstractPropFontSize::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    enable(_cunit, _frame->damaged ());
		enable(_csize, _frame->damaged ());
  }

  void
  AbstractPropFontSize::impl_deactivate ()
  {
    disable(_cunit);
		disable(_csize);
    AbstractStyle::impl_deactivate ();
  }

  

  

} /* namespace djnn */
