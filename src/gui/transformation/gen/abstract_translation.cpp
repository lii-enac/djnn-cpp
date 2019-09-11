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

#include "abstract_translation.h"

namespace djnn
{
  AbstractTranslation::AbstractTranslation (Process *p, const std::string& n, double tx, double ty) :
    AbstractTransformation (p, n),
    raw_props{.tx=tx, .ty=ty},
    _ctx (nullptr), _cty (nullptr)
  {
    
    
  }

  AbstractTranslation::AbstractTranslation (double tx, double ty) :
    AbstractTransformation (), 
    raw_props{.tx=tx, .ty=ty},
    _ctx (nullptr), _cty (nullptr)
  {
    
  }

  AbstractTranslation::~AbstractTranslation ()
  {
    delete _ctx;
		delete _cty;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (_symtable.size () > 2) {
      std::map<std::string, Process*>::iterator it;

      it = _symtable.find ("tx");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("ty");
			if (it != _symtable.end ())
				delete it->second;
    }
  }
 
  Process*
  AbstractTranslation::find_component (const string& name)
  {
    Process* res = AbstractTransformation::find_component(name);
    if(res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    Coupling ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef string text;
    text* rawp_Text = nullptr;
    int notify_mask = notify_none;
    
    if(name=="tx") {
      coupling=&_ctx;
      rawp_Double=&raw_props.tx;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="ty") {
      coupling=&_cty;
      rawp_Double=&raw_props.ty;
      notify_mask = notify_damaged_transform;
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
  AbstractTranslation::get_properties_values (double& tx, double& ty)
  {
    tx = raw_props.tx;
		ty = raw_props.ty;
  }

  void
  AbstractTranslation::impl_activate ()
  {
    AbstractTransformation::impl_activate ();
    auto _frame = frame ();
    if(_ctx) _ctx->enable (_frame);
		if(_cty) _cty->enable (_frame);
  }

  void
  AbstractTranslation::impl_deactivate ()
  {
    AbstractTransformation::impl_deactivate ();
    if(_ctx) _ctx->disable ();
		if(_cty) _cty->disable ();
  }

  

  
  
} /* namespace djnn */
