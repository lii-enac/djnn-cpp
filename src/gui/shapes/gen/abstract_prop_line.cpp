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

#include "abstract_prop_line.h"

namespace djnn
{
  AbstractPropLine::AbstractPropLine (Process *parent, const std::string& name, double x1, double y1, double x2, double y2) :
    AbstractGShape (parent, name),
    raw_props{.x1=x1, .y1=y1, .x2=x2, .y2=y2},
    _cx1 (nullptr), _cy1 (nullptr), _cx2 (nullptr), _cy2 (nullptr)
  {
    set_origin (x1,y1);
    
  }

  AbstractPropLine::~AbstractPropLine ()
  {
    delete _cx1;
		delete _cy1;
		delete _cx2;
		delete _cy2;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (symtable ().size () > 2) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("x1");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("y1");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("x2");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("y2");
			if (it != symtable ().end ())
				delete it->second;
    }
  }
 
  Process*
  AbstractPropLine::find_component (const string& name)
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
    
    if(name=="x1") {
      coupling=&_cx1;
      rawp_Double=&raw_props.x1;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="y1") {
      coupling=&_cy1;
      rawp_Double=&raw_props.y1;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="x2") {
      coupling=&_cx2;
      rawp_Double=&raw_props.x2;
      notify_mask = notify_damaged_geometry;
      prop_Double=true;
    } else
    if(name=="y2") {
      coupling=&_cy2;
      rawp_Double=&raw_props.y2;
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
  AbstractPropLine::get_properties_values (double& x1, double& y1, double& x2, double& y2)
  {
    x1 = raw_props.x1;
		y1 = raw_props.y1;
		x2 = raw_props.x2;
		y2 = raw_props.y2;
    
  }

  void
  AbstractPropLine::impl_activate ()
  {
    AbstractGShape::impl_activate ();
    auto _frame = frame ();
    if(_cx1) _cx1->enable (_frame);
		if(_cy1) _cy1->enable (_frame);
		if(_cx2) _cx2->enable (_frame);
		if(_cy2) _cy2->enable (_frame);
  }

  void
  AbstractPropLine::impl_deactivate ()
  {
    if(_cx1) _cx1->disable ();
		if(_cy1) _cy1->disable ();
		if(_cx2) _cx2->disable ();
		if(_cy2) _cy2->disable ();
    AbstractGShape::impl_deactivate ();
  }

  

  
  
} /* namespace djnn */