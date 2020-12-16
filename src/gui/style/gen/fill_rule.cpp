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


#include "gui/shapes/abstract_gshape.h"
#include "display/window.h"
#include "gui/style/abstract_style.h"

#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display-dev.h" // DisplayBackend::instance
#include "display/abstract_display.h"

#include "audio/style/audio_style.h"
#include "physics/abstract_pobj.h"
#include "physics/physics.h"


#include "fill_rule.h"

namespace djnn
{
  FillRule::FillRule (ParentProcess* parent, const std::string& name, int rule) :
    AbstractStyle (parent, name),
    raw_props{.rule=rule},
    _crule (nullptr)
  {
    
    finalize_construction (parent, name);
  }

  FillRule::~FillRule ()
  {
    remove_edge (_crule);
    delete _crule;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > 0) {
      symtable_t::iterator it;

      it = find_child_iterator ("rule");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  FatChildProcess*
  FillRule::find_child_impl (const std::string& name)
  {
    auto * res = AbstractStyle::find_child_impl(name);
    if (res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    CouplingWithData ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef std::string text;
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
    auto _frame = get_frame ();
    enable(_crule, _frame->damaged ());
  }

  void
  FillRule::impl_deactivate ()
  {
    disable(_crule);
    AbstractStyle::impl_deactivate ();
  }

  
  void
  FillRule::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_fill_rule (this);
    }
  }


  
  FillRule*
  FillRule::clone ()
  {
    return new FillRule (nullptr, get_name (), raw_props.rule);
  }


} /* namespace djnn */
