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
#include "core/ontology/coupling.h"

#include "abstract_prop_font_weight.h"

namespace djnn
{
  AbstractPropFontWeight::AbstractPropFontWeight (Process *parent, const std::string& name, int weight) :
    AbstractStyle (parent, name),
    raw_props{.weight=weight},
    _cweight (nullptr)
  {
    
    
  }

  AbstractPropFontWeight::~AbstractPropFontWeight ()
  {
    delete _cweight;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (symtable ().size () > 0) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("weight");
			if (it != symtable ().end ())
				delete it->second;
    }
  }
 
  Process*
  AbstractPropFontWeight::find_component (const string& name)
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
    
    if(name=="weight") {
      coupling=&_cweight;
      rawp_Int=&raw_props.weight;
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
  AbstractPropFontWeight::get_properties_values (int& weight)
  {
    weight = raw_props.weight;
    
  }

  void
  AbstractPropFontWeight::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if(_cweight) _cweight->enable (_frame);
  }

  void
  AbstractPropFontWeight::impl_deactivate ()
  {
    if(_cweight) _cweight->disable ();
    AbstractStyle::impl_deactivate ();
  }

  

  

} /* namespace djnn */
