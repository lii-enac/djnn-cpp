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
#include "../../display/display.h"
#include "../../display/abstract_display.h"
#include "../../display/window.h"
#include "shapes.h"
#include "../../core/control/coupling.h"

namespace djnn
{
  Ellipse::Ellipse (Process *p, const std::string& n, double cx, double cy, double rx, double ry):
    AbstractGShape (p, n),
    raw_props{.cx=cx, .cy=cy, .rx=rx, .ry=ry},
    _ccx (nullptr), _ccy (nullptr), _crx (nullptr), _cry (nullptr)
  {
    set_origin (cx, cy);
    Process::finalize ();
  }

  Ellipse::Ellipse (double cx, double cy, double rx, double ry):
    AbstractGShape (), 
    raw_props{.cx=cx, .cy=cy, .rx=rx, .ry=ry},
    _ccx (nullptr), _ccy (nullptr), _crx (nullptr), _cry (nullptr)
  {
    set_origin (cx, cy);
  }

  Ellipse::~Ellipse ()
  {
    delete _ccx;
		delete _ccy;
		delete _crx;
		delete _cry;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (_symtable.size () > 2) {
      std::map<std::string, Process*>::iterator it;

      it = _symtable.find ("cx");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("cy");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("rx");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("ry");
			if (it != _symtable.end ())
				delete it->second;
    }
  }
 
  Process*
  Ellipse::find_component (const string& name)
  {
    Process* res = AbstractGShape::find_component(name);
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
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="cy") {
      coupling=&_ccy;
      rawp_Double=&raw_props.cy;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="rx") {
      coupling=&_crx;
      rawp_Double=&raw_props.rx;
      notify_mask = notify_damaged_geometry;
      prop_Double=true;
    } else
    if(name=="ry") {
      coupling=&_cry;
      rawp_Double=&raw_props.ry;
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
  Ellipse::get_properties_values (double& cx, double& cy, double& rx, double& ry)
  {
    cx = raw_props.cx;
		cy = raw_props.cy;
		rx = raw_props.rx;
		ry = raw_props.ry;
  }

  void
  Ellipse::activate ()
  {
    AbstractGObj::activate ();
    if(_ccx) _ccx->enable (_frame);
		if(_ccy) _ccy->enable (_frame);
		if(_crx) _crx->enable (_frame);
		if(_cry) _cry->enable (_frame);
  }

  void
  Ellipse::deactivate ()
  {
    AbstractGObj::deactivate ();
    if(_ccx) _ccx->disable ();
		if(_ccy) _ccy->disable ();
		if(_crx) _crx->disable ();
		if(_cry) _cry->disable ();
  }

  void
  Ellipse::draw ()
  {
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_ellipse (this);
    }
  }

  Process*
  Ellipse::clone ()
  {
    return new Ellipse (raw_props.cx, raw_props.cy, raw_props.rx, raw_props.ry);
  }
} /* namespace djnn */
