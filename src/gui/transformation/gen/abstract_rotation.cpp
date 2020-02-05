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
#include "core/ontology/process.h"
#include "core/ontology/coupling.h"

#include "audio/style/audio_style.h"


#include "abstract_rotation.h"

namespace djnn
{
  AbstractRotation::AbstractRotation (Process *parent, const std::string& name, double a, double cx, double cy) :
    AbstractTransformation (parent, name),
    raw_props{.a=a, .cx=cx, .cy=cy},
    _ca (nullptr), _ccx (nullptr), _ccy (nullptr)
  {
    
    
  }

  AbstractRotation::~AbstractRotation ()
  {
    delete _ca;
		delete _ccx;
		delete _ccy;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (symtable ().size () > 0) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("a");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("cx");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("cy");
			if (it != symtable ().end ())
				delete it->second;
    }
  }
 
  Process*
  AbstractRotation::find_component (const string& name)
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
    
    if(name=="a") {
      coupling=&_ca;
      rawp_Double=&raw_props.a;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="cx") {
      coupling=&_ccx;
      rawp_Double=&raw_props.cx;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="cy") {
      coupling=&_ccy;
      rawp_Double=&raw_props.cy;
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
  AbstractRotation::get_properties_values (double& a, double& cx, double& cy)
  {
    a = raw_props.a;
		cx = raw_props.cx;
		cy = raw_props.cy;
    
  }

  void
  AbstractRotation::impl_activate ()
  {
    AbstractTransformation::impl_activate ();
    auto _frame = frame ();
    if(_ca) _ca->enable (_frame);
		if(_ccx) _ccx->enable (_frame);
		if(_ccy) _ccy->enable (_frame);
  }

  void
  AbstractRotation::impl_deactivate ()
  {
    if(_ca) _ca->disable ();
		if(_ccx) _ccx->disable ();
		if(_ccy) _ccy->disable ();
    AbstractTransformation::impl_deactivate ();
  }

  

  

} /* namespace djnn */
