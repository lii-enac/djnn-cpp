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

#include "dash_offset.h"

namespace djnn
{
  DashOffset::DashOffset (Process *p, const std::string& n, double offset) :
    AbstractStyle (p, n),
    raw_props{.offset=offset},
    _coffset (nullptr)
  {
    
    Process::finalize_construction (p);
  }

  DashOffset::DashOffset (double offset) :
    AbstractStyle (), 
    raw_props{.offset=offset},
    _coffset (nullptr)
  {
    
  }

  DashOffset::~DashOffset ()
  {
    delete _coffset;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (_symtable.size () > 2) {
      std::map<std::string, Process*>::iterator it;

      it = _symtable.find ("offset");
			if (it != _symtable.end ())
				delete it->second;
    }
  }
 
  Process*
  DashOffset::find_component (const string& name)
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
    
    if(name=="offset") {
      coupling=&_coffset;
      rawp_Double=&raw_props.offset;
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
  DashOffset::get_properties_values (double& offset)
  {
    offset = raw_props.offset;
  }

  void
  DashOffset::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if(_coffset) _coffset->enable (_frame);
  }

  void
  DashOffset::impl_deactivate ()
  {
    if(_coffset) _coffset->disable ();
    AbstractStyle::impl_deactivate ();
  }

  
  void
  DashOffset::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_dash_offset (this);
    }
  }


  
  Process*
  DashOffset::clone ()
  {
    return new DashOffset (raw_props.offset);
  }

  
} /* namespace djnn */