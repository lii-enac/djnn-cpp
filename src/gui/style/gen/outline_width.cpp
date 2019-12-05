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

#include "outline_width.h"

#include "gui/picking/analytical_picking_context.h"

#include <iostream>

namespace djnn
{
  OutlineWidth::OutlineWidth (Process *parent, const std::string& name, double width) :
    AbstractStyle (parent, name),
    raw_props{.width=width},
    _cwidth (nullptr)
  {
    
    Process::finalize_construction (parent, name);
  }

  OutlineWidth::OutlineWidth (double width) :
    AbstractStyle (), 
    raw_props{.width=width},
    _cwidth (nullptr)
  {
    
  }

  OutlineWidth::~OutlineWidth ()
  {
    delete _cwidth;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (symtable ().size () > 0) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("width");
			if (it != symtable ().end ())
				delete it->second;
    }
  }
 
  Process*
  OutlineWidth::find_component (const string& name)
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
    
    if(name=="width") {
      coupling=&_cwidth;
      rawp_Double=&raw_props.width;
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
  OutlineWidth::get_properties_values (double& width)
  {
    width = raw_props.width;
  }

  void
  OutlineWidth::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if(_cwidth) _cwidth->enable (_frame);
  }

  void
  OutlineWidth::impl_deactivate ()
  {
    if(_cwidth) _cwidth->disable ();
    AbstractStyle::impl_deactivate ();
  }

  
  void
  OutlineWidth::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_width (this);
    }
  }

  AbstractGShape*
  OutlineWidth::pick_analytical (PickAnalyticalContext& pac)
  {
    //std::cerr << "OutlineWidth::pick_analytical " << pac.half_outline_width << std::endl;
    //std::cerr << raw_props.width << std::endl;
    pac.half_outline_width = raw_props.width / 2;
    return nullptr;
  }
  
  Process*
  OutlineWidth::clone ()
  {
    return new OutlineWidth (raw_props.width);
  }

  
} /* namespace djnn */
