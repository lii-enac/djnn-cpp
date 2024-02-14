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


#include "display/window.h"
#include "gui/style/abstract_style.h"
#include "core/control/action.h"
#include "core/core-dev.h" // graph add/remove edge

#include "color.h"

#include <cmath>

namespace djnn
{
  // D65 Reference White
#define X_R 0.95047
#define Y_R 1.00000
#define Z_R 1.08883

#define KAPPA 903.3
#define EPSILON 0.008856

  static double
  f (double v)
  {
    if (v > EPSILON)
      return pow (v, 1.0 / 3.0);
    else
      return (KAPPA * v + 16) / 116;
  }

  RGBToLCHConverter::ConverterAction::ConverterAction (RGBToLCHConverter * parent, const string& name) :
      Action (parent, name), _p (parent)
  {
  }

  void
  RGBToLCHConverter::ConverterAction::impl_activate ()
  {
    double R = _p->_r->get_value () / 255.0;
    double G = _p->_g->get_value () / 255.0;
    double B = _p->_b->get_value () / 255.0;

    double rgb[3];
    rgb[0] = R <= 0.04045 ? (R / 12.92) : pow ((R + 0.055) / 1.055, 2.4);
    rgb[1] = G <= 0.04045 ? (G / 12.92) : pow ((G + 0.055) / 1.055, 2.4);
    rgb[2] = B <= 0.04045 ? (B / 12.92) : pow ((B + 0.055) / 1.055, 2.4);

    double matrix[3][3] =
      {
  { 0.4124564, 0.3575761, 0.1804375 },
  { 0.2126729, 0.7151522, 0.0721750 },
  { 0.0193339, 0.1191920, 0.9503041 } };
    double X = rgb[0] * matrix[0][0] + rgb[1] * matrix[0][1] + rgb[2] * matrix[0][2];
    double Y = rgb[0] * matrix[1][0] + rgb[1] * matrix[1][1] + rgb[2] * matrix[1][2];
    double Z = rgb[0] * matrix[2][0] + rgb[1] * matrix[2][1] + rgb[2] * matrix[2][2];

    double f_x = f (X / X_R);
    double f_y = f (Y / Y_R);
    double f_z = f (Z / Z_R);
    double L = 116 * f_y - 16;
    double a = 500 * (f_x - f_y);
    double b = 200 * (f_y - f_z);

    double C = sqrt (a * a + b * b);
    double H = atan2 (b, a) * (180.0 / 3.141592653589793238463);
    if (H < 0)
      H += 360;

    _p->_l->set_value (L, true);
    _p->_c->set_value (C, true);
    _p->_h->set_value (H, true);
  }

  RGBToLCHConverter::RGBToLCHConverter (CoreProcess* parent, const string& name) :
      FatProcess (name)
  {
    _r = new IntProperty (this, "r", 0);
    _g = new IntProperty (this, "g", 0);
    _b = new IntProperty (this, "b", 0);
    _l = new DoubleProperty (this, "l", 0);
    _c = new DoubleProperty (this, "c", 0);
    _h = new DoubleProperty (this, "h", 0);
    _action = new ConverterAction (this, "action");
    _cr = new Coupling (_r, ACTIVATION, _action, ACTIVATION);
    _cr->disable ();
    _cg = new Coupling (_g, ACTIVATION, _action, ACTIVATION);
    _cg->disable ();
    _cb = new Coupling (_b, ACTIVATION, _action, ACTIVATION);
    _cb->disable ();

    graph_add_edge (_action, _l);
    graph_add_edge (_action, _c);
    graph_add_edge (_action, _h);
    finalize_construction (parent, name);
  }

  RGBToLCHConverter::~RGBToLCHConverter ()
  {

    if (get_parent ()) {
      remove_state_dependency (get_parent (), _action);
    }

    graph_remove_edge (_b, _action);
    graph_remove_edge (_g, _action);
    graph_remove_edge (_r, _action);

    delete _cb;
    delete _cg;
    delete _cr;
    delete _action;
    delete _h;
    delete _c;
    delete _l;
    delete _b;
    delete _g;
    delete _r;
  }

  void
  RGBToLCHConverter::set_parent (CoreProcess* parent)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
      remove_state_dependency (get_parent (), _action);
    }

    add_state_dependency (parent, _action);
    
    FatProcess::set_parent (parent); 
  }

  void
  RGBToLCHConverter::impl_activate ()
  {
    _cr->enable ();
    _cg->enable ();
    _cb->enable ();
  }

  void
  RGBToLCHConverter::impl_deactivate ()
  {
    _cr->disable ();
    _cg->disable ();
    _cb->disable ();
  }

  LCHToRGBConverter::ConverterAction::ConverterAction (LCHToRGBConverter * parent, const string& name) :
      Action (parent, name), _p (parent)
  {
  }

  void
  LCHToRGBConverter::ConverterAction::impl_activate ()
  {
    double L = _p->_l->get_value ();
    double C = _p->_c->get_value ();
    double H = _p->_h->get_value ();

    double a = C * cos (H * 0.0174533);
    double b = C * sin (H * 0.0174533);

    double f_y = (L + 16) / 116.0;
    double f_z = f_y - (b / 200.0);
    double f_x = (a / 500.0) + f_y;

    double f_x_3 = pow (f_x, 3);
    double f_y_3 = pow (f_y, 3);
    double f_z_3 = pow (f_z, 3);
    double x_r = f_x_3 > EPSILON ? f_x_3 : (116 * f_x - 16) / KAPPA;
    double y_r = L > (KAPPA * EPSILON) ? f_y_3 : (L / KAPPA);
    double z_r = f_z_3 > EPSILON ? f_z_3 : (116 * f_z - 16) / KAPPA;

    double X = X_R * x_r;
    double Y = Y_R * y_r;
    double Z = Z_R * z_r;

    double invmatrix[3][3] =
      {
  { 3.2404542, -1.5371385, -0.4985314 },
  { -0.969266, 1.8760108, 0.04155600 },
  { 0.0556434, -0.2040259, 1.05722520 } };

    double rgb[3];
    rgb[0] = X * invmatrix[0][0] + Y * invmatrix[0][1] + Z * invmatrix[0][2];
    rgb[1] = X * invmatrix[1][0] + Y * invmatrix[1][1] + Z * invmatrix[1][2];
    rgb[2] = X * invmatrix[2][0] + Y * invmatrix[2][1] + Z * invmatrix[2][2];

    int R = (int) 255 * (rgb[0] <= 0.0031308 ? rgb[0] * 12.92 : 1.055 * pow (rgb[0], 1 / 2.4) - 0.055);
    int G = (int) 255 * (rgb[1] <= 0.0031308 ? rgb[1] * 12.92 : 1.055 * pow (rgb[1], 1 / 2.4) - 0.055);
    int B = (int) 255 * (rgb[2] <= 0.0031308 ? rgb[2] * 12.92 : 1.055 * pow (rgb[2], 1 / 2.4) - 0.055);

    _p->_r->set_value (R < 0 ? 0 : (R > 255) ? 255 : R, true);
    _p->_g->set_value (G < 0 ? 0 : (G > 255) ? 255 : G, true);
    _p->_b->set_value (B < 0 ? 0 : (B > 255) ? 255 : B, true);
  }

  LCHToRGBConverter::LCHToRGBConverter (CoreProcess* parent, const string& name) :
      FatProcess (name)
  {
    _r = new IntProperty (this, "r", 0);
    _g = new IntProperty (this, "g", 0);
    _b = new IntProperty (this, "b", 0);
    _l = new DoubleProperty (this, "l", 0);
    _c = new DoubleProperty (this, "c", 0);
    _h = new DoubleProperty (this, "h", 0);
    _action = new ConverterAction (this, "action");
    _cl = new Coupling (_l, ACTIVATION, _action, ACTIVATION);
    _cl->disable ();
    _cc = new Coupling (_c, ACTIVATION, _action, ACTIVATION);
    _cc->disable ();
    _ch = new Coupling (_h, ACTIVATION, _action, ACTIVATION);
    _ch->disable ();

    graph_add_edge (_action, _r);
    graph_add_edge (_action, _g);
    graph_add_edge (_action, _b);
    finalize_construction (parent, name);
  }

  void
  LCHToRGBConverter::set_parent (CoreProcess* parent)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
      remove_state_dependency (get_parent (), _action);
    }

    add_state_dependency (parent, _action);
    
    FatProcess::set_parent (parent); 
  }

  LCHToRGBConverter::~LCHToRGBConverter ()
  {
    if (get_parent ()) {
      remove_state_dependency (get_parent (), _action);
    }
    graph_remove_edge (_h, _action);
    graph_remove_edge (_c, _action);
    graph_remove_edge (_l, _action);

    delete _ch;
    delete _cc;
    delete _cl;
    delete _action;
    delete _h;
    delete _c;
    delete _l;
    delete _b;
    delete _g;
    delete _r;
  }

  void
  LCHToRGBConverter::impl_activate ()
  {
    _cl->enable ();
    _cc->enable ();
    _ch->enable ();
  }

  void
  LCHToRGBConverter::impl_deactivate ()
  {
    _cl->disable ();
    _cc->disable ();
    _ch->disable ();
  }

  AbstractColor::AbstractColor (CoreProcess* parent, const string& name, int r, int g, int b) :
    AbstractStyle (parent, name),
    raw_props{.r=r, .g=g, .b=b},
    _cr (nullptr), _cg (nullptr), _cb (nullptr), _cv (nullptr), _c_rv (nullptr), _c_gv (nullptr), _c_bv (nullptr), _c_vrgb (nullptr),
    _toValue (this, "toValue"),
    _toRGB (this, "toRGB"),
    _is_updating (false)
  {
    raw_props.value = ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
  }

  AbstractColor::AbstractColor (CoreProcess* parent, const string& name, int v) :
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

  AbstractColor::~AbstractColor ()
  {
    delete _cr;
		delete _cg;
		delete _cb;
		delete _cv;
		if (_c_rv) {
		  delete _c_rv;
		}
		if (_c_gv) {
		  delete _c_gv;
		}
		if (_c_bv) {
		  delete _c_bv;
		}
		delete _c_vrgb;
  
    if (children_size () > 0) {
      symtable_t::iterator it;

      it = find_child_iterator ("r");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("g");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("b");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("value");
			if (it != children_end ())
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

  CoreProcess*
  AbstractColor::find_child_impl (const string& name)
  {
    auto * res = AbstractStyle::find_child_impl(name);
    if(res) return res;

    CouplingWithData ** coupling = nullptr;
    int* rawp_Int = nullptr;
    int notify_mask = notify_none;
    IntPropertyProxy* prop = nullptr; // do not cache

    if(name=="r") {
      coupling=&_cr;
      rawp_Int=&raw_props.r;
      notify_mask = notify_damaged_style;
      res = create_GObj_prop(&prop, coupling, rawp_Int, name, notify_mask);
      _c_rv = new CouplingWithData (res, ACTIVATION, &_toValue, ACTIVATION);
      if (!somehow_activating())
        _c_rv->disable ();
    } else
    if(name=="g") {
      coupling=&_cg;
      rawp_Int=&raw_props.g;
      notify_mask = notify_damaged_style;
      res = create_GObj_prop(&prop, coupling, rawp_Int, name, notify_mask);
      _c_gv = new CouplingWithData (res, ACTIVATION, &_toValue, ACTIVATION);
      if (!somehow_activating())
        _c_gv->disable ();
    } else
    if(name=="b") {
      coupling=&_cb;
      rawp_Int=&raw_props.b;
      notify_mask = notify_damaged_style;
      res = create_GObj_prop(&prop, coupling, rawp_Int, name, notify_mask);
      _c_bv = new CouplingWithData (res, ACTIVATION, &_toValue, ACTIVATION);
      if (!somehow_activating())
        _c_bv->disable ();
    } else
    if(name=="value") {
      coupling=&_cv;
      rawp_Int=&raw_props.value;
      notify_mask = notify_damaged_style;
      res = create_GObj_prop(&prop, coupling, rawp_Int, name, notify_mask);
      //_c_vrgb = new CouplingWithData (res, ACTIVATION, &_toRGB, ACTIVATION, nullptr, true);
      _c_vrgb = new CouplingWithData (res, ACTIVATION, &_toRGB, ACTIVATION, true);
      if (!somehow_activating())
        _c_vrgb->disable ();
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
    //auto _frame = get_frame ();
    auto * damaged = get_frame ()->damaged ();
    enable (_cr, damaged);
    enable (_cg, damaged);
    enable (_cb, damaged);
    enable (_cv, damaged);

		if (_c_rv) _c_rv->enable ();
		if (_c_gv) _c_gv->enable ();
		if (_c_bv) _c_bv->enable ();
		if (_c_vrgb) _c_vrgb->enable ();
  }

  void
  AbstractColor::impl_deactivate ()
  {
    disable (_cr);
		disable (_cg);
		disable (_cb);
		disable (_cv);
    if (_c_rv) _c_rv->disable ();
    if (_c_gv) _c_gv->disable ();
    if (_c_bv) _c_bv->disable ();
    if (_c_vrgb) _c_vrgb->disable ();
    AbstractStyle::impl_deactivate ();
  }

  

  
  
} /* namespace djnn */
