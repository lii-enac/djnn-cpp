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
#include "core/ontology/coupling.h"

#include "abstract_prop_rectangle.h"

namespace djnn
{
  AbstractPropRectangle::AbstractPropRectangle (Process *parent, const std::string& name, double x, double y, double width, double height, double rx, double ry) :
    AbstractGShape (parent, name),
    raw_props{.x=x, .y=y, .width=width, .height=height, .rx=rx, .ry=ry},
    _cx (nullptr), _cy (nullptr), _cwidth (nullptr), _cheight (nullptr), _crx (nullptr), _cry (nullptr)
  {
    set_origin (x, y);
    
  }

  AbstractPropRectangle::~AbstractPropRectangle ()
  {
    delete _cx;
		delete _cy;
		delete _cwidth;
		delete _cheight;
		delete _crx;
		delete _cry;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (symtable ().size () > 2) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("x");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("y");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("width");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("height");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("rx");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("ry");
			if (it != symtable ().end ())
				delete it->second;
    }
  }
 
  Process*
  AbstractPropRectangle::find_component (const string& name)
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
  AbstractPropRectangle::get_properties_values (double& x, double& y, double& width, double& height, double& rx, double& ry)
  {
    x = raw_props.x;
		y = raw_props.y;
		width = raw_props.width;
		height = raw_props.height;
		rx = raw_props.rx;
		ry = raw_props.ry;
    
  }

  void
  AbstractPropRectangle::impl_activate ()
  {
    AbstractGShape::impl_activate ();
    auto _frame = frame ();
    if(_cx) _cx->enable (_frame);
		if(_cy) _cy->enable (_frame);
		if(_cwidth) _cwidth->enable (_frame);
		if(_cheight) _cheight->enable (_frame);
		if(_crx) _crx->enable (_frame);
		if(_cry) _cry->enable (_frame);
  }

  void
  AbstractPropRectangle::impl_deactivate ()
  {
    if(_cx) _cx->disable ();
		if(_cy) _cy->disable ();
		if(_cwidth) _cwidth->disable ();
		if(_cheight) _cheight->disable ();
		if(_crx) _crx->disable ();
		if(_cry) _cry->disable ();
    AbstractGShape::impl_deactivate ();
  }

  

  
  
} /* namespace djnn */