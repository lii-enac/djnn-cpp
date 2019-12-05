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

#include "abstract_prop_font_family.h"

namespace djnn
{
  AbstractPropFontFamily::AbstractPropFontFamily (Process *parent, const std::string& name, std::string family) :
    AbstractStyle (parent, name),
    raw_props{.family=family},
    _cfamily (nullptr)
  {
    
    
  }

  AbstractPropFontFamily::AbstractPropFontFamily (std::string family) :
    AbstractStyle (), 
    raw_props{.family=family},
    _cfamily (nullptr)
  {
    
  }

  AbstractPropFontFamily::~AbstractPropFontFamily ()
  {
    delete _cfamily;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (symtable ().size () > 0) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("family");
			if (it != symtable ().end ())
				delete it->second;
    }
  }
 
  Process*
  AbstractPropFontFamily::find_component (const string& name)
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
    
    if(name=="family") {
      coupling=&_cfamily;
      rawp_Text=&raw_props.family;
      notify_mask = notify_damaged_style;
      prop_Text=true;
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
  AbstractPropFontFamily::get_properties_values (std::string& family)
  {
    family = raw_props.family;
    
  }

  void
  AbstractPropFontFamily::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if(_cfamily) _cfamily->enable (_frame);
  }

  void
  AbstractPropFontFamily::impl_deactivate ()
  {
    if(_cfamily) _cfamily->disable ();
    AbstractStyle::impl_deactivate ();
  }

  

  
  
} /* namespace djnn */
