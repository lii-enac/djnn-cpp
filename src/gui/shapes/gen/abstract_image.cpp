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

#include "abstract_image.h"

namespace djnn
{
  AbstractImage::AbstractImage (Process *p, const std::string& n, std::string path, double x, double y, double width, double height) :
    AbstractGShape (p, n),
    raw_props{.path=path, .x=x, .y=y, .width=width, .height=height},
    _cpath (nullptr), _cx (nullptr), _cy (nullptr), _cwidth (nullptr), _cheight (nullptr)
  {
    set_origin (x, y);
    
  }

  AbstractImage::AbstractImage (std::string path, double x, double y, double width, double height) :
    AbstractGShape (), 
    raw_props{.path=path, .x=x, .y=y, .width=width, .height=height},
    _cpath (nullptr), _cx (nullptr), _cy (nullptr), _cwidth (nullptr), _cheight (nullptr)
  {
    set_origin (x, y);
  }

  AbstractImage::~AbstractImage ()
  {
    delete _cpath;
		delete _cx;
		delete _cy;
		delete _cwidth;
		delete _cheight;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (_symtable.size () > 2) {
      std::map<std::string, Process*>::iterator it;

      it = _symtable.find ("path");
			if (it != _symtable.end ())
				delete it->second;

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
    }
  }
 
  Process*
  AbstractImage::find_component (const string& name)
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
    
    if(name=="path") {
      coupling=&_cpath;
      rawp_Text=&raw_props.path;
      notify_mask = notify_damaged_geometry;
      prop_Text=true;
    } else
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
  AbstractImage::get_properties_values (std::string& path, double& x, double& y, double& width, double& height)
  {
    path = raw_props.path;
		x = raw_props.x;
		y = raw_props.y;
		width = raw_props.width;
		height = raw_props.height;
  }

  void
  AbstractImage::impl_activate ()
  {
    AbstractGShape::impl_activate ();
    auto _frame = frame ();
    if(_cpath) _cpath->enable (_frame);
		if(_cx) _cx->enable (_frame);
		if(_cy) _cy->enable (_frame);
		if(_cwidth) _cwidth->enable (_frame);
		if(_cheight) _cheight->enable (_frame);
  }

  void
  AbstractImage::impl_deactivate ()
  {
    if(_cpath) _cpath->disable ();
		if(_cx) _cx->disable ();
		if(_cy) _cy->disable ();
		if(_cwidth) _cwidth->disable ();
		if(_cheight) _cheight->disable ();
    AbstractGShape::impl_deactivate ();
  }

  

  
  
} /* namespace djnn */
