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
  Rectangle::Rectangle (Process *p, const std::string& n, double x, double y, double width, double height, double rx, double ry):
    AbstractGShape (p, n),
    raw_props{.x=x, .y=y, .width=width, .height=height, .rx=rx, .ry=ry},
    _cx (nullptr), _cy (nullptr), _cwidth (nullptr), _cheight (nullptr), _crx (nullptr), _cry (nullptr)
  {
    set_origin (x, y);
    Process::finalize ();
  }

  Rectangle::Rectangle (double x, double y, double width, double height, double rx, double ry):
    AbstractGShape (), 
    raw_props{.x=x, .y=y, .width=width, .height=height, .rx=rx, .ry=ry},
    _cx (nullptr), _cy (nullptr), _cwidth (nullptr), _cheight (nullptr), _crx (nullptr), _cry (nullptr)
  {
    set_origin (x, y);
  }

  Rectangle::~Rectangle ()
  {
    delete _cx;
		delete _cy;
		delete _cwidth;
		delete _cheight;
		delete _crx;
		delete _cry;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (_symtable.size () > 2) {
      std::map<std::string, Process*>::iterator it;

      it = _symtable.find ("x");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("y");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("width");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("height");
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
  Rectangle::find_component (const string& name)
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
    
    if(name=="x") {
      coupling=&_cx;
      rawp_Double=&raw_props.x;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="y") {
      coupling=&_cy;
      rawp_Double=&raw_props.y;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="width") {
      coupling=&_cwidth;
      rawp_Double=&raw_props.width;
      notify_mask = notify_damaged_geometry;
      prop_Double=true;
    } else
    if(name=="height") {
      coupling=&_cheight;
      rawp_Double=&raw_props.height;
      notify_mask = notify_damaged_geometry;
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
  Rectangle::get_properties_values (double& x, double& y, double& width, double& height, double& rx, double& ry)
  {
    x = raw_props.x;
		y = raw_props.y;
		width = raw_props.width;
		height = raw_props.height;
		rx = raw_props.rx;
		ry = raw_props.ry;
  }

  void
  Rectangle::activate ()
  {
    AbstractGObj::activate ();
    if(_cx) _cx->enable (_frame);
		if(_cy) _cy->enable (_frame);
		if(_cwidth) _cwidth->enable (_frame);
		if(_cheight) _cheight->enable (_frame);
		if(_crx) _crx->enable (_frame);
		if(_cry) _cry->enable (_frame);
  }

  void
  Rectangle::deactivate ()
  {
    AbstractGObj::deactivate ();
    if(_cx) _cx->disable ();
		if(_cy) _cy->disable ();
		if(_cwidth) _cwidth->disable ();
		if(_cheight) _cheight->disable ();
		if(_crx) _crx->disable ();
		if(_cry) _cry->disable ();
  }

  void
  Rectangle::draw ()
  {
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_rectangle (this);
    }
  }

  Process*
  Rectangle::clone ()
  {
    return new Rectangle (raw_props.x, raw_props.y, raw_props.width, raw_props.height, raw_props.rx, raw_props.ry);
  }
} /* namespace djnn */
