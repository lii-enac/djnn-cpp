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

#include "abstract_opacity.h"

namespace djnn
{
  AbstractOpacity::AbstractOpacity (Process *p, const std::string& n, double alpha) :
    AbstractStyle (p, n),
    raw_props{.alpha=alpha},
    _calpha (nullptr)
  {
    
    
  }

  AbstractOpacity::AbstractOpacity (double alpha) :
    AbstractStyle (), 
    raw_props{.alpha=alpha},
    _calpha (nullptr)
  {
    
  }

  AbstractOpacity::~AbstractOpacity ()
  {
    delete _calpha;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (_symtable.size () > 2) {
      std::map<std::string, Process*>::iterator it;

      it = _symtable.find ("alpha");
			if (it != _symtable.end ())
				delete it->second;
    }
  }
 
  Process*
  AbstractOpacity::find_component (const string& name)
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
    
    if(name=="alpha") {
      coupling=&_calpha;
      rawp_Double=&raw_props.alpha;
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
  AbstractOpacity::get_properties_values (double& alpha)
  {
    alpha = raw_props.alpha;
  }

  void
  AbstractOpacity::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if(_calpha) _calpha->enable (_frame);
  }

  void
  AbstractOpacity::impl_deactivate ()
  {
    if(_calpha) _calpha->disable ();
    AbstractStyle::impl_deactivate ();
  }

  

  
  
} /* namespace djnn */
