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


#include "../backend.h"
#include "../abstract_backend.h"
#include "shapes.h"
#include "../../core/control/coupling.h"

namespace djnn
{
  Circle::Circle (Process *p, const std::string& n, double cx, double cy, double r):
    AbstractGShape (p, n),
    raw_props{.cx=cx, .cy=cy, .r=r},
    _ccx (nullptr), _ccy (nullptr), _cr (nullptr)
  {
    set_origin (cx, cy);
    Process::finalize ();
  }

  Circle::Circle (double cx, double cy, double r):
    AbstractGShape (), 
    raw_props{.cx=cx, .cy=cy, .r=r},
    _ccx (nullptr), _ccy (nullptr), _cr (nullptr)
  {
    set_origin (cx, cy);
  }

  Circle::~Circle ()
  {
    delete _ccx;
		delete _ccy;
		delete _cr;

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
    }
  }
 
  Process*
  Circle::find_component (const string& name)
  {
    Process* res = AbstractGShape::find_component(name);
    if(res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    Coupling ** coupling;
    double* rawp_Double;
    int* rawp_Int;
    typedef string text;
    text* rawp_Text;
    int notify_mask = notify_none;
    
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
    if(name=="r") {
      coupling=&_cr;
      rawp_Double=&raw_props.r;
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
  Circle::get_properties_values (double& cx, double& cy, double& r)
  {
    cx = raw_props.cx;
		cy = raw_props.cy;
		r = raw_props.r;
  }

  void
  Circle::activate ()
  {
    AbstractGObj::activate ();
    if(_ccx) _ccx->enable (_frame);
		if(_ccy) _ccy->enable (_frame);
		if(_cr) _cr->enable (_frame);
  }

  void
  Circle::deactivate ()
  {
    AbstractGObj::deactivate ();
    if(_ccx) _ccx->disable ();
		if(_ccy) _ccy->disable ();
		if(_cr) _cr->disable ();
  }

  void
  Circle::draw ()
  {
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_circle (this);
    }
  }

  Process*
  Circle::clone ()
  {
    return new Circle (raw_props.cx, raw_props.cy, raw_props.r);
  }
} /* namespace djnn */
