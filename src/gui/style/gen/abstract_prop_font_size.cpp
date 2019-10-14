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


#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display.h"
#include "display/abstract_display.h"
#include "display/window.h"
#include "gui/shapes/shapes.h"
#include "gui/style/style.h"
#include "core/ontology/coupling.h"

#include "abstract_prop_font_size.h"

namespace djnn
{
  AbstractPropFontSize::AbstractPropFontSize (Process *p, const std::string& n, int unit, double size) :
    AbstractStyle (p, n),
    raw_props{.unit=unit, .size=size},
    _cunit (nullptr), _csize (nullptr)
  {
    
    
  }

  AbstractPropFontSize::AbstractPropFontSize (int unit, double size) :
    AbstractStyle (), 
    raw_props{.unit=unit, .size=size},
    _cunit (nullptr), _csize (nullptr)
  {
    
  }

  AbstractPropFontSize::~AbstractPropFontSize ()
  {
    delete _cunit;
		delete _csize;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (_symtable.size () > 2) {
      std::map<std::string, Process*>::iterator it;

      it = _symtable.find ("unit");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("size");
			if (it != _symtable.end ())
				delete it->second;
    }
  }
 
  Process*
  AbstractPropFontSize::find_component (const string& name)
  {
    Process* res = AbstractStyle::find_component(name);
    if(res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    Coupling ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef string text;
    text* rawp_Text = nullptr;
    int notify_mask = notify_none;
    
    if(name=="unit") {
      coupling=&_cunit;
      rawp_Int=&raw_props.unit;
      notify_mask = notify_damaged_style;
      prop_Int=true;
    } else
    if(name=="size") {
      coupling=&_csize;
      rawp_Double=&raw_props.size;
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
    if(_cunit) _cunit->enable (_frame);
		if(_csize) _csize->enable (_frame);
  }

  void
  AbstractPropFontSize::impl_deactivate ()
  {
    if(_cunit) _cunit->disable ();
		if(_csize) _csize->disable ();
    AbstractStyle::impl_deactivate ();
  }

  

  
  
} /* namespace djnn */