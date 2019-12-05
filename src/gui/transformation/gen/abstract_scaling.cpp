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

#include "abstract_scaling.h"

namespace djnn
{
  AbstractScaling::AbstractScaling (Process *parent, const std::string& name, double sx, double sy, double cx, double cy) :
    AbstractTransformation (parent, name),
    raw_props{.sx=sx, .sy=sy, .cx=cx, .cy=cy},
    _csx (nullptr), _csy (nullptr), _ccx (nullptr), _ccy (nullptr)
  {
    
    
  }

  AbstractScaling::AbstractScaling (double sx, double sy, double cx, double cy) :
    AbstractTransformation (), 
    raw_props{.sx=sx, .sy=sy, .cx=cx, .cy=cy},
    _csx (nullptr), _csy (nullptr), _ccx (nullptr), _ccy (nullptr)
  {
    
  }

  AbstractScaling::~AbstractScaling ()
  {
    delete _csx;
		delete _csy;
		delete _ccx;
		delete _ccy;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (symtable ().size () > 0) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("sx");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("sy");
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
  AbstractScaling::find_component (const string& name)
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
    
    if(name=="sx") {
      coupling=&_csx;
      rawp_Double=&raw_props.sx;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="sy") {
      coupling=&_csy;
      rawp_Double=&raw_props.sy;
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
  AbstractScaling::get_properties_values (double& sx, double& sy, double& cx, double& cy)
  {
    sx = raw_props.sx;
		sy = raw_props.sy;
		cx = raw_props.cx;
		cy = raw_props.cy;
    
  }

  void
  AbstractScaling::impl_activate ()
  {
    AbstractTransformation::impl_activate ();
    auto _frame = frame ();
    if(_csx) _csx->enable (_frame);
		if(_csy) _csy->enable (_frame);
		if(_ccx) _ccx->enable (_frame);
		if(_ccy) _ccy->enable (_frame);
  }

  void
  AbstractScaling::impl_deactivate ()
  {
    if(_csx) _csx->disable ();
		if(_csy) _csy->disable ();
		if(_ccx) _ccx->disable ();
		if(_ccy) _ccy->disable ();
    AbstractTransformation::impl_deactivate ();
  }

  

  
  
} /* namespace djnn */
