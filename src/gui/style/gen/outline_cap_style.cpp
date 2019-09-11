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

#include "outline_cap_style.h"

namespace djnn
{
  OutlineCapStyle::OutlineCapStyle (Process *p, const std::string& n, int cap) :
    AbstractStyle (p, n),
    raw_props{.cap=cap},
    _ccap (nullptr)
  {
    
    Process::finalize_construction (p);
  }

  OutlineCapStyle::OutlineCapStyle (int cap) :
    AbstractStyle (), 
    raw_props{.cap=cap},
    _ccap (nullptr)
  {
    
  }

  OutlineCapStyle::~OutlineCapStyle ()
  {
    delete _ccap;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (_symtable.size () > 2) {
      std::map<std::string, Process*>::iterator it;

      it = _symtable.find ("cap");
			if (it != _symtable.end ())
				delete it->second;
    }
  }
 
  Process*
  OutlineCapStyle::find_component (const string& name)
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
    
    if(name=="cap") {
      coupling=&_ccap;
      rawp_Int=&raw_props.cap;
      notify_mask = notify_damaged_geometry;
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
  OutlineCapStyle::get_properties_values (int& cap)
  {
    cap = raw_props.cap;
  }

  void
  OutlineCapStyle::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if(_ccap) _ccap->enable (_frame);
  }

  void
  OutlineCapStyle::impl_deactivate ()
  {
    AbstractStyle::impl_deactivate ();
    if(_ccap) _ccap->disable ();
  }

  
  void
  OutlineCapStyle::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_cap_style (this);
    }
  }


  
  Process*
  OutlineCapStyle::clone ()
  {
    return new OutlineCapStyle (raw_props.cap);
  }

  
} /* namespace djnn */
