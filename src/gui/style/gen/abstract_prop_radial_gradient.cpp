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

#include "abstract_prop_radial_gradient.h"

namespace djnn
{
  AbstractPropRadialGradient::AbstractPropRadialGradient (Process *p, const std::string& n, double cx, double cy, double r, double fx, double fy, int spread, int coords) :
    AbstractGradient (p, n, spread, coords),
    raw_props{.cx=cx, .cy=cy, .r=r, .fx=fx, .fy=fy},
    _ccx (nullptr), _ccy (nullptr), _cr (nullptr), _cfx (nullptr), _cfy (nullptr)
  {
    
    
  }

  AbstractPropRadialGradient::AbstractPropRadialGradient (double cx, double cy, double r, double fx, double fy, int spread, int coords) :
    AbstractGradient (spread, coords), 
    raw_props{.cx=cx, .cy=cy, .r=r, .fx=fx, .fy=fy},
    _ccx (nullptr), _ccy (nullptr), _cr (nullptr), _cfx (nullptr), _cfy (nullptr)
  {
    
  }

  AbstractPropRadialGradient::~AbstractPropRadialGradient ()
  {
    delete _ccx;
		delete _ccy;
		delete _cr;
		delete _cfx;
		delete _cfy;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (_symtable.size () > 2) {
      std::map<std::string, Process*>::iterator it;

      it = _symtable.find ("cx");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("cy");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("r");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("fx");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("fy");
			if (it != _symtable.end ())
				delete it->second;
    }
  }
 
  Process*
  AbstractPropRadialGradient::find_component (const string& name)
  {
    Process* res = AbstractGradient::find_component(name);
    if(res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    Coupling ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef string text;
    text* rawp_Text = nullptr;
    int notify_mask = notify_none;
    
    if(name=="cx") {
      coupling=&_ccx;
      rawp_Double=&raw_props.cx;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
    if(name=="cy") {
      coupling=&_ccy;
      rawp_Double=&raw_props.cy;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
    if(name=="r") {
      coupling=&_cr;
      rawp_Double=&raw_props.r;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
    if(name=="fx") {
      coupling=&_cfx;
      rawp_Double=&raw_props.fx;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
    if(name=="fy") {
      coupling=&_cfy;
      rawp_Double=&raw_props.fy;
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
  AbstractPropRadialGradient::get_properties_values (double& cx, double& cy, double& r, double& fx, double& fy, int& spread, int& coords)
  {
    cx = raw_props.cx;
		cy = raw_props.cy;
		r = raw_props.r;
		fx = raw_props.fx;
		fy = raw_props.fy;
  }

  void
  AbstractPropRadialGradient::impl_activate ()
  {
    AbstractGradient::impl_activate ();
    auto _frame = frame ();
    if(_ccx) _ccx->enable (_frame);
		if(_ccy) _ccy->enable (_frame);
		if(_cr) _cr->enable (_frame);
		if(_cfx) _cfx->enable (_frame);
		if(_cfy) _cfy->enable (_frame);
  }

  void
  AbstractPropRadialGradient::impl_deactivate ()
  {
    if(_ccx) _ccx->disable ();
		if(_ccy) _ccy->disable ();
		if(_cr) _cr->disable ();
		if(_cfx) _cfx->disable ();
		if(_cfy) _cfy->disable ();
    AbstractGradient::impl_deactivate ();
  }

  

  
  
} /* namespace djnn */
