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
#include "../../core/ontology/coupling.h"

namespace djnn
{
  Line::Line (Process *p, const std::string& n, double x1, double y1, double x2, double y2):
    AbstractGShape (p, n),
    raw_props{.x1=x1, .y1=y1, .x2=x2, .y2=y2},
    _cx1 (nullptr), _cy1 (nullptr), _cx2 (nullptr), _cy2 (nullptr)
  {
    set_origin (x1, y1);
    Process::finalize_construction (p);
  }

  Line::Line (double x1, double y1, double x2, double y2):
    AbstractGShape (), 
    raw_props{.x1=x1, .y1=y1, .x2=x2, .y2=y2},
    _cx1 (nullptr), _cy1 (nullptr), _cx2 (nullptr), _cy2 (nullptr)
  {
    set_origin (x1, y1);
  }

  Line::~Line ()
  {
    delete _cx1;
		delete _cy1;
		delete _cx2;
		delete _cy2;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (_symtable.size () > 2) {
      std::map<std::string, Process*>::iterator it;

      it = _symtable.find ("x1");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("y1");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("x2");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("y2");
			if (it != _symtable.end ())
				delete it->second;
    }
  }
 
  Process*
  Line::find_component (const string& name)
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
  Line::get_properties_values (double& x1, double& y1, double& x2, double& y2)
  {
    x1 = raw_props.x1;
		y1 = raw_props.y1;
		x2 = raw_props.x2;
		y2 = raw_props.y2;
  }

  void
  Line::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    if(_cx1) _cx1->enable (_frame);
		if(_cy1) _cy1->enable (_frame);
		if(_cx2) _cx2->enable (_frame);
		if(_cy2) _cy2->enable (_frame);
  }

  void
  Line::impl_deactivate ()
  {
    AbstractGObj::impl_deactivate ();
    if(_cx1) _cx1->disable ();
		if(_cy1) _cy1->disable ();
		if(_cx2) _cx2->disable ();
		if(_cy2) _cy2->disable ();
  }

  void
  Line::draw ()
  {
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_line (this);
    }
  }

  Process*
  Line::clone ()
  {
    return new Line (raw_props.x1, raw_props.y1, raw_props.x2, raw_props.y2);
  }
} /* namespace djnn */
