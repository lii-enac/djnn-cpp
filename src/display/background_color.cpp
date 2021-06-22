#include "background_color.h"

#include "core/core-dev.h" // graph add/remove edge
#include "core/utils/utils-dev.h" // coupling add/remove edge

#include "core/tree/int_property.h"
#include "window.h"

namespace djnn {

  BackgroundColor::BackgroundColor (ParentProcess* parent, const djnn::string& name, int r, int g, int b) :
    FatProcess (name),
    raw_props{.r=r, .g=g, .b=b},
    _cr (nullptr), _cg (nullptr), _cb (nullptr), _cv (nullptr), _c_rv (nullptr), _c_gv (nullptr), _c_bv (nullptr), _c_vrgb (nullptr),
    _toValue (this, "toValue"),
    _toRGB (this, "toRGB"),
    _is_updating (false)
  {
    raw_props.value = ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
    finalize_construction (parent, name);
  }

  BackgroundColor::BackgroundColor (ParentProcess* parent, const djnn::string& name, int v) :
    FatProcess (name),
    _cr (nullptr), _cg (nullptr), _cb (nullptr), _cv (nullptr), _c_rv (nullptr), _c_gv (nullptr), _c_bv (nullptr), _c_vrgb (nullptr),
    _toValue (this, "toValue"),
    _toRGB (this, "toRGB"),
    _is_updating (false)
  {
    raw_props.r = (v >> 16) & 0xFF;
    raw_props.g = (v >> 8) & 0xFF;
    raw_props.b = v & 0xFF;
    raw_props.value = v;
    finalize_construction (parent, name);
  }

  BackgroundColor::~BackgroundColor ()
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
  BackgroundColor::update_rvb_from_hex () {
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
  BackgroundColor::update_hex_from_rvb () {
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

  void
  BackgroundColor::create_Gobj_update_coupling (CoreProcess **prop, CouplingWithData **cprop)
  {
    FatProcess *update = nullptr;
    //if (find_layer ()) update = find_layer()->damaged ();
    //else
    if (get_frame ()) update = get_frame ()->damaged ();
    *cprop = new CouplingWithData (*prop, ACTIVATION, update, ACTIVATION);
    if (somehow_activating ()) {
      (*cprop)->enable ();
    }
    else
      (*cprop)->disable ();
  }

  FatProcess*
  BackgroundColor::create_GObj_prop (IntPropertyProxy **prop, CouplingWithData **cprop, int *rawp, const djnn::string& name, int notify_mask)
  {
    *prop = new IntPropertyProxy (this, name, *rawp, notify_mask);
    create_Gobj_update_coupling(reinterpret_cast<CoreProcess**>(prop), cprop);
    return *prop;
  }

  FatChildProcess*
  BackgroundColor::find_child_impl (const djnn::string& name)
  {
    auto * res = FatProcess::find_child_impl(name);
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
  BackgroundColor::get_properties_values (double& r, double& g, double& b)
  {
    r = raw_props.r;
    g = raw_props.g;
    b = raw_props.b;
  }

  void
  BackgroundColor::impl_activate ()
  {
    //FatProcess::impl_activate ();
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
  BackgroundColor::impl_deactivate ()
  {
    disable (_cr);
    disable (_cg);
    disable (_cb);
    disable (_cv);
    if (_c_rv) _c_rv->disable ();
    if (_c_gv) _c_gv->disable ();
    if (_c_bv) _c_bv->disable ();
    if (_c_vrgb) _c_vrgb->disable ();
    //FatProcess::impl_deactivate ();
  }
}
