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
#include "core/execution/graph.h"
#include "abstract_color.h"

namespace djnn
{
  AbstractColor::AbstractColor (Process *parent, const std::string& name, int r, int g, int b) :
    AbstractStyle (parent, name),
    raw_props{.r=r, .g=g, .b=b},
    _cr (nullptr), _cg (nullptr), _cb (nullptr), _cv (nullptr), _c_rv (nullptr), _c_gv (nullptr), _c_bv (nullptr), _c_vrgb (nullptr),
    _toValue (this, "toValue"),
    _toRGB (this, "toRGB"),
    _is_updating (false)
  {
    raw_props.value = ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
  }

  AbstractColor::AbstractColor (Process *parent, const std::string& name, int v) :
    AbstractStyle (parent, name),
    _cr (nullptr), _cg (nullptr), _cb (nullptr), _cv (nullptr), _c_rv (nullptr), _c_gv (nullptr), _c_bv (nullptr), _c_vrgb (nullptr),
    _toValue (this, "toValue"),
    _toRGB (this, "toRGB"),
    _is_updating (false)
  {
    raw_props.r = (v >> 16) & 0xFF;
    raw_props.g = (v >> 8) & 0xFF;
    raw_props.b = v & 0xFF;
    raw_props.value = v;
  }

  AbstractColor::AbstractColor (int r, int g, int b) :
  AbstractStyle (),
  raw_props{.r=r, .g=g, .b=b},
  _cr (nullptr), _cg (nullptr), _cb (nullptr), _cv (nullptr), _c_rv (nullptr), _c_gv (nullptr), _c_bv (nullptr), _c_vrgb (nullptr),
  _toValue (this, "toValue"),
  _toRGB (this, "toRGB"),
  _is_updating (false)
  {
    raw_props.value = ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
  }

  AbstractColor::~AbstractColor ()
  {
    delete _cr;
		delete _cg;
		delete _cb;
		delete _cv;
		if (_c_rv) {
		  Graph::instance ().remove_edge (r(), &_toValue);
		  delete _c_rv;
		}
		if (_c_gv) {
		  Graph::instance ().remove_edge (g(), &_toValue);
		  delete _c_gv;
		}
		if (_c_bv) {
		  Graph::instance ().remove_edge (b(), &_toValue);
		  delete _c_bv;
		}
		delete _c_vrgb;
    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (symtable ().size () > 0) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("r");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("g");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("b");
			if (it != symtable ().end ())
				delete it->second;

			it = symtable ().find ("value");
			if (it != symtable ().end ())
			  delete it->second;
    }
  }

  void
  AbstractColor::update_rvb_from_hex () {
    if (_is_updating)
      return;
    _is_updating = true;
    int r_res = (raw_props.value >> 16) & 0xFF;
    if (_cr)
      r ()->set_value (r_res, true);
    else
      raw_props.r = r_res;
    int g_res = (raw_props.value >> 8) & 0xFF;
    if (_cg)
      g()->set_value (g_res, true);
    else
      raw_props.g = g_res;
    int b_res = raw_props.value & 0xFF;
    if (_cb)
      b()->set_value(b_res, true);
    else
      raw_props.b = b_res;
    _is_updating = false;
  }

  void
  AbstractColor::update_hex_from_rvb () {
    if (_is_updating)
      return;
    _is_updating = true;
    int v = ((raw_props.r & 0xff) << 16) + ((raw_props.g & 0xff) << 8) + (raw_props.b & 0xff);
    if (_cv)
      value()->set_value (v, true);
    else
      raw_props.value = v;
    _is_updating = false;
  }

  Process*
  AbstractColor::find_component (const string& name)
  {
    Process* res = AbstractStyle::find_component(name);
    if(res) return res;

    Coupling ** coupling = nullptr;
    int* rawp_Int = nullptr;
    int notify_mask = notify_none;
    IntPropertyProxy* prop = nullptr; // do not cache

    if(name=="r") {
      coupling=&_cr;
      rawp_Int=&raw_props.r;
      notify_mask = notify_damaged_style;
      res = create_GObj_prop(&prop, coupling, rawp_Int, name, notify_mask);
      Graph::instance ().add_edge (res, &_toValue);
      _c_rv = new Coupling (res, ACTIVATION, &_toValue, ACTIVATION);
    } else
    if(name=="g") {
      coupling=&_cg;
      rawp_Int=&raw_props.g;
      notify_mask = notify_damaged_style;
      res = create_GObj_prop(&prop, coupling, rawp_Int, name, notify_mask);
      Graph::instance ().add_edge (res, &_toValue);
      _c_gv = new Coupling (res, ACTIVATION, &_toValue, ACTIVATION);
    } else
    if(name=="b") {
      coupling=&_cb;
      rawp_Int=&raw_props.b;
      notify_mask = notify_damaged_style;
      res = create_GObj_prop(&prop, coupling, rawp_Int, name, notify_mask);
      Graph::instance ().add_edge (res, &_toValue);
      _c_bv = new Coupling (res, ACTIVATION, &_toValue, ACTIVATION);
    } else
    if(name=="value") {
      coupling=&_cv;
      rawp_Int=&raw_props.value;
      notify_mask = notify_damaged_style;
      res = create_GObj_prop(&prop, coupling, rawp_Int, name, notify_mask);
      _c_vrgb = new Coupling (res, ACTIVATION, &_toRGB, ACTIVATION, true);
    } else
    return nullptr;
    
    return res;
  }

  void
  AbstractColor::get_properties_values (double& r, double& g, double& b)
  {
    r = raw_props.r;
		g = raw_props.g;
		b = raw_props.b;
  }

  void
  AbstractColor::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if(_cr) _cr->enable (_frame);
		if(_cg) _cg->enable (_frame);
		if(_cb) _cb->enable (_frame);
		if(_cv) _cv->enable (_frame);
		if (_c_rv) _c_rv->enable ();
		if (_c_gv) _c_gv->enable ();
		if (_c_bv) _c_bv->enable ();
		if (_c_vrgb) _c_vrgb->enable ();
  }

  void
  AbstractColor::impl_deactivate ()
  {
    if(_cr) _cr->disable ();
    if(_cg) _cg->disable ();
    if(_cb) _cb->disable ();
    if(_cv) _cv->disable ();
    if (_c_rv) _c_rv->disable ();
    if (_c_gv) _c_gv->disable ();
    if (_c_bv) _c_bv->disable ();
    if (_c_vrgb) _c_vrgb->disable ();
    AbstractStyle::impl_deactivate ();
  }

  

  
  
} /* namespace djnn */
