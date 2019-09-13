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

#include "fill_rule.h"

namespace djnn
{
  FillRule::FillRule (Process *p, const std::string& n, int rule) :
    AbstractStyle (p, n),
    raw_props{.rule=rule},
    _crule (nullptr)
  {
    
    Process::finalize_construction (p);
  }

  FillRule::FillRule (int rule) :
    AbstractStyle (), 
    raw_props{.rule=rule},
    _crule (nullptr)
  {
    
  }

  FillRule::~FillRule ()
  {
    delete _crule;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (_symtable.size () > 2) {
      std::map<std::string, Process*>::iterator it;

      it = _symtable.find ("rule");
			if (it != _symtable.end ())
				delete it->second;
    }
  }
 
  Process*
  FillRule::find_component (const string& name)
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
    
    if(name=="rule") {
      coupling=&_crule;
      rawp_Int=&raw_props.rule;
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
  FillRule::get_properties_values (int& rule)
  {
    rule = raw_props.rule;
  }

  void
  FillRule::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if(_crule) _crule->enable (_frame);
  }

  void
  FillRule::impl_deactivate ()
  {
    if(_crule) _crule->disable ();
    AbstractStyle::impl_deactivate ();
  }

  
  void
  FillRule::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_fill_rule (this);
    }
  }


  
  Process*
  FillRule::clone ()
  {
    return new FillRule (raw_props.rule);
  }

  
} /* namespace djnn */
