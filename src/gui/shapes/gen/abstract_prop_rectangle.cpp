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


#include "abstract_prop_rectangle.h"

namespace djnn
{
  AbstractPropRectangle::AbstractPropRectangle (FatProcess *parent, const std::string& name, double x, double y, double width, double height, double rx, double ry) :
    AbstractGShape (parent, name),
    raw_props{.x=x, .y=y, .width=width, .height=height, .rx=rx, .ry=ry},
    _cx (nullptr), _cy (nullptr), _cwidth (nullptr), _cheight (nullptr), _crx (nullptr), _cry (nullptr)
  {
    set_origin (x, y);
    
  }

  AbstractPropRectangle::~AbstractPropRectangle ()
  {
    remove_edge (_cx);
		remove_edge (_cy);
		remove_edge (_cwidth);
		remove_edge (_cheight);
		remove_edge (_crx);
		remove_edge (_cry);
    delete _cx;
		delete _cy;
		delete _cwidth;
		delete _cheight;
		delete _crx;
		delete _cry;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > 2) {
      symtable_t::iterator it;

      it = find_child_iterator ("x");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("y");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("width");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("height");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("rx");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("ry");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  FatChildProcess*
  AbstractPropRectangle::find_child (const std::string& name)
  {
    auto * res = AbstractGShape::find_child(name);
    if(res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    CouplingWithData ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef std::string text;
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
    auto _frame = get_frame ();
    enable(_cx, _frame->damaged ());
		enable(_cy, _frame->damaged ());
		enable(_cwidth, _frame->damaged ());
		enable(_cheight, _frame->damaged ());
		enable(_crx, _frame->damaged ());
		enable(_cry, _frame->damaged ());
  }

  void
  AbstractPropRectangle::impl_deactivate ()
  {
    disable(_cx);
		disable(_cy);
		disable(_cwidth);
		disable(_cheight);
		disable(_crx);
		disable(_cry);
    AbstractGShape::impl_deactivate ();
  }

  

  

} /* namespace djnn */
