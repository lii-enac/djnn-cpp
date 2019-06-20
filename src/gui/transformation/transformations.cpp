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
 *      Nicolas Saporito <nicolas.saporito@enac.fr>
 *
 */

#include "../transformation/transformations.h"
#include "../../core/tree/spike.h"

#include "../backend.h"
#include "../abstract_backend.h"
#include "../../display/display.h"
#include "../../display/abstract_display.h"
#include "../../display/window.h"

#include <array>
#include <cmath>

#include <iostream>

namespace djnn
{
  AbstractTransformation::AbstractTransformation (Process *p, const string &n) :
      AbstractGObj (p, n)
  {
  }

  AbstractTransformation::AbstractTransformation ()
  {
  }

  AbstractTransformation::~AbstractTransformation ()
  {
  }

  AbstractTranslation::AbstractTranslation (Process *p, const string &n, double tx, double ty) :
      AbstractTransformation (p, n),
      raw_props{.tx=tx, .ty=ty},
      _ctx (nullptr), _cty (nullptr)
  {
  }

  AbstractTranslation::AbstractTranslation (double tx, double ty) :
      AbstractTransformation (),
      raw_props{.tx=tx, .ty=ty},
      _ctx (nullptr), _cty (nullptr)
  {
  }

  AbstractTranslation::~AbstractTranslation ()
  {
    delete _ctx;
    delete _cty;

    if (_symtable.empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = _symtable.find ("tx");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("ty");
      if (it != _symtable.end ())
        delete it->second;
    }
  }

  Process*
  AbstractTranslation::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="tx") {
      coupling=&_ctx;
      rawp=&raw_props.tx;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="ty") {
      coupling=&_cty;
      rawp=&raw_props.ty;
      notify_mask = notify_damaged_transform;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr; // do not cache
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  AbstractTranslation::get_properties_values (double &tx, double &ty)
  {
    tx = raw_props.tx;
    ty = raw_props.ty;
  }

  void
  AbstractTranslation::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    if(_ctx) _ctx->enable (_frame);
    if(_cty) _cty->enable (_frame);
  }

  void
  AbstractTranslation::impl_deactivate ()
  {
    AbstractGObj::impl_deactivate ();
    if(_ctx)_ctx->disable ();
    if(_cty) _cty->disable ();
  }

  Translation::Translation (Process *p, const string &n, double tx, double ty) :
      AbstractTranslation (p, n, tx, ty)
  {
    Process::finalize_construction ();
  }

  Translation::Translation (double tx, double ty) :
      AbstractTranslation (tx, ty)
  {
  }

  Translation::~Translation ()
  {
  }

  void
  Translation::draw ()
  {
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_translation (this);
    }
  }

  Process*
  Translation::clone ()
  {
    return new Translation (raw_props.tx, raw_props.ty);
  }

  GradientTranslation::GradientTranslation (Process *p, const string &n, double tx, double ty) :
      AbstractTranslation (p, n, tx, ty)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (p);
    if (grad == nullptr) {
      cerr << "Parent of gradient translation must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
  }

  GradientTranslation::GradientTranslation (double tx, double ty) :
      AbstractTranslation (tx, ty)
  {
  }

  GradientTranslation::~GradientTranslation ()
  {
  }

  void
  GradientTranslation::draw ()
  {
    Backend::instance ()->load_gradient_translation (this);
  }

  Process*
  GradientTranslation::clone ()
  {
    return new GradientTranslation (raw_props.tx, raw_props.ty);
  }

  AbstractRotation::AbstractRotation (Process *p, const string &n, double a, double cx, double cy) :
      AbstractTransformation (p, n),
      raw_props{.a=a, .cx=cx, .cy=cy},
      _ca(nullptr), _ccx (nullptr), _ccy (nullptr)
  {
  }

  AbstractRotation::AbstractRotation (double a, double cx, double cy) :
      AbstractTransformation (),
      raw_props{.a=a, .cx=cx, .cy=cy},
      _ca(nullptr), _ccx (nullptr), _ccy (nullptr)
  {
  }

  AbstractRotation::~AbstractRotation ()
  {
    delete _ca;
    delete _ccx;
    delete _ccy;

    if (_symtable.empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = _symtable.find ("a");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("cx");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("cy");
      if (it != _symtable.end ())
        delete it->second;
    }
  }

  Process*
  AbstractRotation::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="a") {
      coupling=&_ca;
      rawp=&raw_props.a;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="cx") {
      coupling=&_ccx;
      rawp=&raw_props.cx;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="cy") {
      coupling=&_ccy;
      rawp=&raw_props.cy;
      notify_mask = notify_damaged_transform;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr; // do not cache
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  AbstractRotation::get_properties_values (double &a, double &cx, double &cy)
  {
    a = raw_props.a;
    cx = raw_props.cx;
    cy = raw_props.cy;
  }


  void
  AbstractRotation::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    if(_ca) _ca->enable (_frame);
    if(_ccx) _ccx->enable (_frame);
    if(_ccy) _ccy->enable (_frame);
  }

  void
  AbstractRotation::impl_deactivate ()
  {
    AbstractGObj::impl_deactivate ();
    if(_ca) _ca->disable ();
    if(_ccx) _ccx->disable ();
    if(_ccy) _ccy->disable ();
  }

  Rotation::Rotation (Process *p, const string &n, double a, double cx, double cy) :
      AbstractRotation (p, n, a, cx, cy)
  {
    Process::finalize_construction ();
  }

  Rotation::Rotation (double a, double cx, double cy) :
      AbstractRotation (a, cx, cy)
  {
  }

  Rotation::~Rotation ()
  {
  }

  void
  Rotation::draw ()
  {
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_rotation (this);
    }
  }

  Process*
  Rotation::clone ()
  {
    return new Rotation (raw_props.a, raw_props.cx, raw_props.cy);
  }

  GradientRotation::GradientRotation (Process *p, const string &n, double a, double cx, double cy) :
      AbstractRotation (p, n, a, cx, cy)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (p);
    if (grad == nullptr) {
      cerr << "Parent of gradient rotation must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
  }

  GradientRotation::GradientRotation (double a, double cx, double cy) :
      AbstractRotation (a, cx, cy)
  {
  }

  GradientRotation::~GradientRotation ()
  {
  }

  void
  GradientRotation::draw ()
  {
    Backend::instance ()->load_gradient_rotation (this);
  }

  Process*
  GradientRotation::clone ()
  {
    return new GradientRotation (raw_props.a, raw_props.cx, raw_props.cy);
  }

  AbstractScaling::AbstractScaling (Process *p, const string &n, double sx, double sy, double cx, double cy) :
      AbstractTransformation (p, n),
      raw_props{.sx=sx, .sy=sy, .cx=cx, .cy=cy},
      _csx (nullptr), _csy (nullptr), _ccx (nullptr), _ccy (nullptr)
  {

  }

  AbstractScaling::AbstractScaling (double sx, double sy, double cx, double cy) :
      AbstractTransformation (),
      raw_props{.sx=sx, .sy=sy, .cx=cx, .cy=cy},
      _csx (nullptr), _csy (nullptr), _ccx (nullptr), _ccy (nullptr)
  {
  }

  AbstractScaling::~AbstractScaling ()
  {
    delete _csx;
    delete _csy;
    delete _ccx;
    delete _ccy;

    if (_symtable.empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = _symtable.find ("sx");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("sy");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("cx");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("cy");
      if (it != _symtable.end ())
        delete it->second;
    }
  }

  Process*
  AbstractScaling::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="sx") {
      coupling=&_csx;
      rawp=&raw_props.sx;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="sy") {
      coupling=&_csy;
      rawp=&raw_props.sy;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="cx") {
      coupling=&_ccx;
      rawp=&raw_props.cx;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="cy") {
      coupling=&_ccy;
      rawp=&raw_props.cy;
      notify_mask = notify_damaged_transform;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr; // do not cache
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  AbstractScaling::get_properties_values (double &sx, double &sy, double &cx, double &cy)
  {
    sx = raw_props.sx;
    sy = raw_props.sy;
    cx = raw_props.cx;
    cy = raw_props.cy;
  }

  void
  AbstractScaling::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    if(_csx) _csx->enable (_frame);
    if(_csy) _csy->enable (_frame);
    if(_ccx) _ccx->enable (_frame);
    if(_ccy) _ccy->enable (_frame);
  }

  void
  AbstractScaling::impl_deactivate ()
  {
    AbstractGObj::impl_deactivate ();
    if(_csx) _csx->disable ();
    if(_csy) _csy->disable ();
    if(_ccx) _ccx->disable ();
    if(_ccy) _ccy->disable ();
  }

  Scaling::Scaling (Process *p, const string &n, double sx, double sy, double cx, double cy) :
      AbstractScaling (p, n, sx, sy, cx, cy)
  {
    Process::finalize_construction ();
  }

  Scaling::Scaling (double sx, double sy, double cx, double cy) :
      AbstractScaling (sx, sy, cx, cy)
  {
    
  }

  Scaling::~Scaling ()
  {
  }

  void
  Scaling::draw ()
  {
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_scaling (this);
    }
  }

  Process*
  Scaling::clone ()
  {
    return new Scaling (raw_props.sx, raw_props.sy, raw_props.cx, raw_props.cy);
  }

  GradientScaling::GradientScaling (Process *p, const string &n, double sx, double sy, double cx, double cy) :
      AbstractScaling (p, n, sx, sy, cx, cy)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (p);
    if (grad == nullptr) {
      cerr << "Parent of gradient scaling must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
  }

  GradientScaling::GradientScaling (double sx, double sy, double cx, double cy) :
      AbstractScaling (sx, sy, cx, cy)
  {
  }

  GradientScaling::~GradientScaling ()
  {
  }

  void
  GradientScaling::draw ()
  {
    Backend::instance ()->load_gradient_scaling (this);
  }

  Process*
  GradientScaling::clone ()
  {
    return new GradientScaling (raw_props.sx, raw_props.sy, raw_props.cx, raw_props.cy);
  }

  AbstractSkew::AbstractSkew (Process *p, const string &n, double a) :
      AbstractTransformation (p, n), 
      raw_props{ .a=a },
      _ca (nullptr)
  {
  }

  AbstractSkew::AbstractSkew (double a) :
      AbstractTransformation (), 
      raw_props{ .a=a },
      _ca (nullptr)
  {
  }

  AbstractSkew::~AbstractSkew ()
  {
    delete _ca;

    if (_symtable.empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = _symtable.find ("a");
      if (it != _symtable.end ())
        delete it->second;
    }
  }

  Process*
  AbstractSkew::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="a") {
      coupling=&_ca;
      rawp=&raw_props.a;
      notify_mask = notify_damaged_transform;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr; // do not cache
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  AbstractSkew::get_properties_values (double &a)
  {
    a = raw_props.a;
  }

  void
  AbstractSkew::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    if (_ca) _ca->enable (_frame);
  }

  void
  AbstractSkew::impl_deactivate ()
  {
    AbstractGObj::impl_deactivate ();
    if (_ca) _ca->disable ();
  }

  SkewX::SkewX (Process *p, const string &n, double a) :
      AbstractSkew (p, n, a)
  {
    Process::finalize_construction ();
  }

  SkewX::SkewX (double a) :
      AbstractSkew (a)
  {
  }

  SkewX::~SkewX ()
  {
  }

  void
  SkewX::draw ()
  {
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_skew_x (this);
    }
  }

  Process*
  SkewX::clone ()
  {
    return new SkewX (raw_props.a);
  }

  GradientSkewX::GradientSkewX (Process *p, const string &n, double a) :
      AbstractSkew (p, n, a)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (p);
    if (grad == nullptr) {
      cerr << "Parent of gradient skewX must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
  }

  GradientSkewX::GradientSkewX (double a) :
      AbstractSkew (a)
  {
  }

  GradientSkewX::~GradientSkewX ()
  {
  }

  void
  GradientSkewX::draw ()
  {
    Backend::instance ()->load_gradient_skew_x (this);
  }

  Process*
  GradientSkewX::clone ()
  {
    return new GradientSkewX (raw_props.a);
  }

  SkewY::SkewY (Process *p, const string &n, double a) :
      AbstractSkew (p, n, a)
  {
    Process::finalize_construction ();
  }

  SkewY::SkewY (double a) :
      AbstractSkew (a)
  {
  }

  SkewY::~SkewY ()
  {
  }

  void
  SkewY::draw ()
  {
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_skew_y (this);
    }
  }

  Process*
  SkewY::clone ()
  {
    return new SkewY (raw_props.a);
  }

  GradientSkewY::GradientSkewY (Process *p, const string &n, double a) :
      AbstractSkew (p, n, a)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (p);
    if (grad == nullptr) {
      cerr << "Parent of gradient skewY must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
  }

  GradientSkewY::GradientSkewY (double a) :
      AbstractSkew (a)
  {
  }

  GradientSkewY::~GradientSkewY ()
  {
  }

  void
  GradientSkewY::draw ()
  {
    Backend::instance ()->load_gradient_skew_y (this);
  }

  Process*
  GradientSkewY::clone ()
  {
    return new GradientSkewY (raw_props.a);
  }


  /* ----  HOMOGRAPHIES ---- */ 


  static double
  degrees_to_radians (double angle) {
    return angle * 3.1415927 / 180;
  }

  void
  AbstractHomography::updateState2d () 
  {
    double m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44;
    get_properties_values (m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
    if (m12 == 0.0 && m21 == 0.0) {
      if (m11 == 1.0 && m22 == 1.0) {
        if (m14 == 0.0 && m24 == 0.0) {
          state2d = APPLY_IDENTITY;
        } else {
          state2d = APPLY_TRANSLATE;
        }
      } else {
        if (m14 == 0.0 && m24 == 0.0) {
          state2d = APPLY_SCALE;
        } else {
          state2d = (APPLY_SCALE | APPLY_TRANSLATE);
        }
      }
    } else {
      if (m11 == 0.0 && m22 == 0.0) {
        if (m14 == 0.0 && m24 == 0.0) {
          state2d = APPLY_SHEAR;
        } else {
          state2d = (APPLY_SHEAR | APPLY_TRANSLATE);
        }
      } else {
        if (m14 == 0.0 && m24 == 0.0) {
          state2d = (APPLY_SHEAR | APPLY_SCALE);
        } else {
          state2d = (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE);
        }
      }
    }
  }

  void 
  AbstractHomography::rightTranslate (double dx, double dy) 
  {
    rightTranslate2d (dx, dy);
    // Hack to propagate the matrix modification after complete calculation FIXME
    m11 ()->set_value (m11 ()->get_value (), true);
  }

  void 
  AbstractHomography::rightTranslate2d (double dx, double dy) 
  {
    double old_m11, old_m12, old_m13, old_m14, old_m21, old_m22, old_m23, old_m24, old_m31, old_m32, old_m33, old_m34, old_m41, old_m42, old_m43, old_m44;
    get_properties_values (old_m11, old_m12, old_m13, old_m14, old_m21, old_m22, old_m23, old_m24, old_m31, old_m32, old_m33, old_m34, old_m41, old_m42, old_m43, old_m44);
    double new_m14, new_m24;
    switch (state2d) {
      case APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE:
        new_m14 = dx * old_m11 + dy * old_m12 + old_m14;
        new_m24 = dx * old_m21 + dy * old_m22 + old_m24;
        m14 ()->set_value (new_m14, false);
        m24 ()->set_value (new_m24, false);
        if (new_m14 == 0.0 && new_m24 == 0.0) {
            state2d = APPLY_SHEAR | APPLY_SCALE;
        }
        break;
      case APPLY_SHEAR | APPLY_SCALE:
        new_m14 = dx * old_m11 + dy * old_m12;
        new_m24 = dx * old_m21 + dy * old_m22;
        m14 ()->set_value (new_m14, false);
        m24 ()->set_value (new_m24, false);
        if (new_m14 != 0.0 || new_m24 != 0.0) {
            state2d = APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE;
        }
        break;
      case APPLY_SHEAR | APPLY_TRANSLATE:
        new_m14 = dy * old_m12 + old_m14;
        new_m24 = dx * old_m21 + old_m24;
        m14 ()->set_value (new_m14, false);
        m24 ()->set_value (new_m24, false);
        if (new_m14 == 0.0 && new_m24 == 0.0) {
            state2d = APPLY_SHEAR;
        }
        break;
      case APPLY_SHEAR:
        new_m14 = dy * old_m12;
        new_m24 = dx * old_m21;
        m14 ()->set_value (new_m14, false);
        m24 ()->set_value (new_m24, false);
        if (new_m14 != 0.0 || new_m24 != 0.0) {
            state2d = APPLY_SHEAR | APPLY_TRANSLATE;
        }
        break;
      case APPLY_SCALE | APPLY_TRANSLATE:
        new_m14 = dx * old_m11 + old_m14;
        new_m24 = dy * old_m22 + old_m24;
        m14 ()->set_value (new_m14, false);
        m24 ()->set_value (new_m24, false);
        if (new_m14 == 0.0 && new_m24 == 0.0) {
            state2d = APPLY_SCALE;
        }
        break;
      case APPLY_SCALE:
        new_m14 = dx * old_m11;
        new_m24 = dy * old_m22;
        m14 ()->set_value (new_m14, false);
        m24 ()->set_value (new_m24, false);
        if (new_m14 != 0.0 || new_m24 != 0.0) {
            state2d = APPLY_SCALE | APPLY_TRANSLATE;
        }
        break;
      case APPLY_TRANSLATE:
        new_m14 = dx + old_m14;
        new_m24 = dy + old_m24;
        m14 ()->set_value (new_m14, false);
        m24 ()->set_value (new_m24, false);
        if (new_m14 == 0.0 && new_m24 == 0.0) {
            state2d = APPLY_IDENTITY;
        }
        break;
      case APPLY_IDENTITY:
        m14 ()->set_value (dx, false);
        m24 ()->set_value (dy, false);
        if (dx != 0.0 || dy != 0.0) {
            state2d = APPLY_TRANSLATE;
        }
        break;
      default:
        cerr << "AbstractHomography::rightTranslate2d -> missing case in a switch " << state2d << endl;
        // cannot reach
        break;
    }
  }

  void 
  AbstractHomography::leftTranslate (double dx, double dy) 
  {
    leftTranslate2d (dx, dy);
    // Hack to propagate the matrix modification after complete calculation FIXME
    m11 ()->set_value (m11 ()->get_value (), true);
  }

  void
  AbstractHomography::leftTranslate2d (double dx, double dy) 
  {
    double new_m14 = m14 ()->get_value () + dx;
    double new_m24 = m24 ()->get_value () + dy;
    m14 ()->set_value (new_m14, false);
    m24 ()->set_value (new_m24, false);
    if (new_m14 == 0.0 && new_m24 == 0.0) {
        state2d &= ~APPLY_TRANSLATE;
    } else {
        state2d |= APPLY_TRANSLATE;
    }
  }

  void
  AbstractHomography::rightScale (double sx, double sy) 
  {
    rightScale2d (sx, sy);
    // Hack to propagate the matrix modification after complete calculation FIXME
    m11 ()->set_value (m11 ()->get_value (), true);
  }

  void
  AbstractHomography::rightScale (double sx, double sy, double cx, double cy) 
  {
    if (cx != 0.0 || cy != 0.0) {
      rightTranslate2d (cx, cy);
      rightScale2d (sx, sy);
      rightTranslate2d (-cx, -cy);
    } else {
      rightScale2d (sx, sy);
    }
    // Hack to propagate the matrix modification after complete calculation FIXME
    m11 ()->set_value (m11 ()->get_value (), true);
  }

  void
  AbstractHomography::rightScale2d (double sx, double sy) {
    double old_m11, old_m12, old_m13, old_m14, old_m21, old_m22, old_m23, old_m24, old_m31, old_m32, old_m33, old_m34, old_m41, old_m42, old_m43, old_m44;
    get_properties_values (old_m11, old_m12, old_m13, old_m14, old_m21, old_m22, old_m23, old_m24, old_m31, old_m32, old_m33, old_m34, old_m41, old_m42, old_m43, old_m44);
    double new_m11, new_m12, new_m21, new_m22;
    int mystate = state2d;
    switch (mystate) {
      case APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE:
      case APPLY_SHEAR | APPLY_SCALE:
        m11 ()->set_value (old_m11 * sx, false);
        m22 ()->set_value (old_m22 * sy, false);
        // fall-through
      case APPLY_SHEAR | APPLY_TRANSLATE:
      case APPLY_SHEAR:
        new_m12 = old_m12 * sy;
        new_m21 = old_m21 * sx;
        m12 ()->set_value (new_m12, false);
        m21 ()->set_value (new_m21, false);
        if (new_m12 == 0.0 && new_m21 == 0.0) {
            mystate &= APPLY_TRANSLATE;
            if (old_m11 != 1.0 || old_m22 != 1.0) {
                mystate |= APPLY_SCALE;
            }
            state2d = mystate;
        } else if (old_m11 == 0.0 && old_m22 == 0.0) {
            state2d &= ~APPLY_SCALE;
        }
        break;
      case APPLY_SCALE | APPLY_TRANSLATE:
      case APPLY_SCALE:
        new_m11 = old_m11 * sx;
        new_m22 = old_m22 * sy;
        m11 ()->set_value (new_m11, false);
        m22 ()->set_value (new_m22, false);
        if (new_m11 == 1.0 && new_m22 == 1.0) {
            state2d = (mystate &= APPLY_TRANSLATE);
        }
        break;
      case APPLY_TRANSLATE:
      case APPLY_IDENTITY:
        m11 ()->set_value (sx, false);
        m22 ()->set_value (sy, false);
        if (sx != 1.0 || sy != 1.0) {
            state2d = (mystate | APPLY_SCALE);
        }
        break;
      default:
        cerr << "AbstractHomography::rightScale2d -> missing case in a switch " << state2d << endl;
        // cannot reach
        break;
    }
  }

  void
  AbstractHomography::leftScale (double sx, double sy) 
  {
    leftScale2d (sx, sy);
    // Hack to propagate the matrix modification after complete calculation FIXME
    m11 ()->set_value (m11 ()->get_value (), true);
  }

  void
  AbstractHomography::leftScale (double sx, double sy, double cx, double cy) 
  {
    if (cx != 0.0 || cy != 0.0) {
      leftTranslate2d (-cx, -cy);
      leftScale2d (sx, sy);
      leftTranslate2d (cx, cy);
    } else {
      leftScale2d (sx, sy);
    }
    // Hack to propagate the matrix modification after complete calculation FIXME
    m11 ()->set_value (m11 ()->get_value (), true);
  }

  void
  AbstractHomography::leftScale2d (double sx, double sy) 
  {
    double old_m11, old_m12, old_m13, old_m14, old_m21, old_m22, old_m23, old_m24, old_m31, old_m32, old_m33, old_m34, old_m41, old_m42, old_m43, old_m44;
    get_properties_values (old_m11, old_m12, old_m13, old_m14, old_m21, old_m22, old_m23, old_m24, old_m31, old_m32, old_m33, old_m34, old_m41, old_m42, old_m43, old_m44);
    double new_m11, new_m12, new_m21, new_m22, new_m14, new_m24;
    int mystate = state2d;
    switch (mystate) {
      case APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE:
        new_m14 = old_m14 * sx;
        new_m24 = old_m24 * sy;
        m14 ()->set_value (new_m14, false);
        m24 ()->set_value (new_m24, false);
        if (new_m14 == 0.0 && new_m24 == 0.0) {
          mystate = mystate & ~APPLY_TRANSLATE;
          state2d = mystate;
        }
        // fall-through
      case APPLY_SHEAR | APPLY_SCALE:
        m11 ()->set_value (old_m11 * sx, false);
        m22 ()->set_value (old_m22 * sy, false);
        // fall-through
      case APPLY_SHEAR:
        new_m12 = old_m12 * sx;
        new_m21 = old_m21 * sy;
        m12 ()->set_value (new_m12, false);
        m21 ()->set_value (new_m21, false);
        if (new_m12 == 0.0 && new_m21 == 0.0) {
          mystate &= APPLY_TRANSLATE;
          if (old_m11 != 1.0 || old_m22 != 1.0) {
            mystate |= APPLY_SCALE;
          }
          state2d = mystate;
        }
        break;
      case APPLY_SHEAR | APPLY_TRANSLATE:
        new_m14 = old_m14 * sx;
        new_m24 = old_m24 * sy;
        new_m12 = old_m12 * sx;
        new_m21 = old_m21 * sy;
        m14 ()->set_value (new_m14, false);
        m24 ()->set_value (new_m24, false);
        m12 ()->set_value (new_m12, false);
        m21 ()->set_value (new_m21, false);
        if (new_m12 == 0.0 && new_m21 == 0.0) {
          if (new_m14 == 0.0 && new_m24 == 0.0) {
            state2d = APPLY_SCALE;
          } else {
            state2d = APPLY_SCALE | APPLY_TRANSLATE;
          }
        } else if (new_m14 ==0.0 && new_m24 == 0.0) {
          state2d = APPLY_SHEAR;
        }
        break;
      case APPLY_SCALE | APPLY_TRANSLATE:
        new_m14 = old_m14 * sx;
        new_m24 = old_m24 * sy;
        m14 ()->set_value (new_m14, false);
        m24 ()->set_value (new_m24, false);
        if (new_m14 == 0.0 && new_m24 == 0.0) {
          mystate = mystate & ~APPLY_TRANSLATE;
          state2d = mystate;
        }
        // fall-through
      case APPLY_SCALE:
        new_m11 = old_m11 * sx;
        new_m22 = old_m22 * sy;
        m11 ()->set_value (new_m11, false);
        m22 ()->set_value (new_m22, false);
        if (new_m11 == 1.0 && new_m22 == 1.0) {
          state2d = (mystate &= APPLY_TRANSLATE);
        }
        break;
      case APPLY_TRANSLATE:
        new_m14 = old_m14 * sx;
        new_m24 = old_m24 * sy;
        m14 ()->set_value (new_m14, false);
        m24 ()->set_value (new_m24, false);
        if (new_m14 == 0.0 && new_m24 == 0.0) {
          mystate = mystate & ~APPLY_TRANSLATE;
          state2d = mystate;
        }
        // fall-through
      case APPLY_IDENTITY:
        m11 ()->set_value (sx, false);
        m22 ()->set_value (sy, false);
        if (sx != 1.0 || sy != 1.0) {
          state2d = mystate | APPLY_SCALE;
        }
        break;
      default:
        cerr << "AbstractHomography::leftScale2d -> missing case in a switch " << state2d << endl;
        // cannot reach
        break;
    }
  }

  void
  AbstractHomography::rightRotate (double da) 
  {
    rightRotate2d (da);
    // Hack to propagate the matrix modification after complete calculation FIXME
    m11 ()->set_value (m11 ()->get_value (), true);
  }

  void
  AbstractHomography::rightRotate (double da, double cx, double cy) 
  {
    if (cx != 0.0 || cy != 0.0) {
      rightTranslate2d (cx, cy);
      rightRotate2d (da);
      rightTranslate2d (-cx, -cy);
    } else {
      rightRotate2d (da);
    }
    // Hack to propagate the matrix modification after complete calculation FIXME
    m11 ()->set_value (m11 ()->get_value (), true);
  }

  void
  AbstractHomography::rightRotate2d (double da_in_degrees) 
  {
    double old_m11 = m11 ()->get_value ();
    double old_m12 = m12 ()->get_value ();
    double old_m21 = m21 ()->get_value ();
    double old_m22 = m22 ()->get_value ();
    double da_in_radians = degrees_to_radians (da_in_degrees);
    double cos = std::cos (da_in_radians);
    double sin = std::sin (da_in_radians);
    if (sin == 1.0) {
      rightRotate2d_90 ();
    } else if (sin == -1.0) {
      rightRotate2d_270 ();
    } else {
      if (cos == -1.0) {
        rightRotate2d_180 ();
      } else if (cos != 1.0) {
        m11 ()->set_value (cos * old_m11 + sin * old_m12, false);
        m12 ()->set_value (-sin * old_m11 + cos * old_m12, false);
        m21 ()->set_value (cos * old_m21 + sin * old_m22, false);
        m22 ()->set_value (-sin * old_m21 + cos * old_m22, false);
        updateState2d ();
      }
    }
  }

  void
  AbstractHomography::rightRotate2d_90 () 
  {
    double new_m11 = m12 ()->get_value ();
    double new_m12 = - m11 ()->get_value ();
    double new_m21 = m22 ()->get_value ();
    double new_m22 = - m21 ()->get_value ();
    m11 ()->set_value (new_m11, false);
    m12 ()->set_value (new_m12, false);
    m21 ()->set_value (new_m21, false);
    m22 ()->set_value (new_m22, false);
    int newstate = rot90conversion[state2d];
    if ((newstate & (APPLY_SHEAR | APPLY_SCALE)) == APPLY_SCALE && new_m11 == 1.0 && new_m22 == 1.0) {
      newstate -= APPLY_SCALE;
    } else if ((newstate & (APPLY_SHEAR | APPLY_SCALE)) == APPLY_SHEAR && new_m12 == 0.0 && new_m21 == 0.0) {
      newstate = ((newstate & ~APPLY_SHEAR) | APPLY_SCALE);
    }
    state2d = newstate;
  }

  void
  AbstractHomography::rightRotate2d_180 () 
  {
    double new_m11 = - m11 ()->get_value ();
    double new_m22 = - m22 ()->get_value ();
    m11 ()->set_value (new_m11, false);
    m22 ()->set_value (new_m22, false);
    int oldstate = state2d;
    if ((oldstate & (APPLY_SHEAR)) != 0) {
      // If there was a shear, then this rotation has no
      // effect on the state.
      m12 ()->set_value (- m12 ()->get_value (), false);
      m21 ()->set_value (- m21 ()->get_value (), false);
    } else {
      // No shear means the SCALE state may toggle when
      // m11 and m22 are negated.
      if (new_m11 == 1.0 && new_m22 == 1.0) {
        state2d = oldstate & ~APPLY_SCALE;
      } else {
        state2d = oldstate | APPLY_SCALE;
      }
    }
  }

  void
  AbstractHomography::rightRotate2d_270 () 
  {
    double new_m11 = - m12 ()->get_value ();
    double new_m12 = m11 ()->get_value ();
    double new_m21 = - m22 ()->get_value ();
    double new_m22 = m21 ()->get_value ();
    m11 ()->set_value (new_m11, false);
    m12 ()->set_value (new_m12, false);
    m21 ()->set_value (new_m21, false);
    m22 ()->set_value (new_m22, false);
    int newstate = rot90conversion[state2d];
    if ((newstate & (APPLY_SHEAR | APPLY_SCALE)) == APPLY_SCALE && new_m11 == 1.0 && new_m22 == 1.0) {
      newstate -= APPLY_SCALE;
    } else if ((newstate & (APPLY_SHEAR | APPLY_SCALE)) == APPLY_SHEAR && new_m12 == 0.0 && new_m21 == 0.0) {
      newstate = ((newstate & ~APPLY_SHEAR) | APPLY_SCALE);
    }
    state2d = newstate;
  }

  void
  AbstractHomography::leftRotate (double angle) 
  {
    leftRotate2d(angle);
    // Hack to propagate the matrix modification after complete calculation FIXME
    m11 ()->set_value (m11 ()->get_value (), true);
  }

  void
  AbstractHomography::leftRotate (double angle, double cx, double cy) 
  {
    if (cx != 0.0 || cy != 0.0) {
      leftTranslate2d (-cx, -cy);
      leftRotate2d (angle);
      leftTranslate2d (cx, cy);
    } else {
      leftRotate2d (angle);
    }
    // Hack to propagate the matrix modification after complete calculation FIXME
    m11 ()->set_value (m11 ()->get_value (), true);
  }

  void
  AbstractHomography::leftRotate2d (double da_in_degrees) 
  {
    double old_m11, old_m12, old_m13, old_m14, old_m21, old_m22, old_m23, old_m24, old_m31, old_m32, old_m33, old_m34, old_m41, old_m42, old_m43, old_m44;
    get_properties_values (old_m11, old_m12, old_m13, old_m14, old_m21, old_m22, old_m23, old_m24, old_m31, old_m32, old_m33, old_m34, old_m41, old_m42, old_m43, old_m44);
    double da_in_radians = degrees_to_radians (da_in_degrees);
    double cos = std::cos (da_in_radians);
    double sin = std::sin (da_in_radians);
    if (sin == 1.0) {
      leftRotate2d_90 ();
    } else if (sin == -1.0) {
      leftRotate2d_270 ();
    } else {
      if (cos == -1.0) {
        leftRotate2d_180 ();
      } else if (cos != 1.0) {
        m11 ()->set_value (cos * old_m11 - sin * old_m21, false);
        m21 ()->set_value (sin * old_m11 + cos * old_m21, false);
        m12 ()->set_value (cos * old_m12 - sin * old_m22, false);
        m22 ()->set_value (sin * old_m12 + cos * old_m22, false);
        m14 ()->set_value (cos * old_m14 - sin * old_m24, false);
        m24 ()->set_value (sin * old_m14 + cos * old_m24, false);
        updateState2d ();
      }
    }
  }

  void
  AbstractHomography::leftRotate2d_90() {
    double new_m11 = - m21 ()->get_value ();
    double new_m12 = - m22 ()->get_value ();
    double new_m14 = - m24 ()->get_value ();
    double new_m21 = m11 ()->get_value ();
    double new_m22 = m12 ()->get_value ();
    double new_m24 = m14 ()->get_value ();
    m11 ()->set_value (new_m11, false);
    m12 ()->set_value (new_m12, false);
    m14 ()->set_value (new_m14, false);
    m21 ()->set_value (new_m21, false);
    m22 ()->set_value (new_m22, false);
    m24 ()->set_value (new_m24, false);
    int newstate = rot90conversion[state2d];
    if ((newstate & (APPLY_SHEAR | APPLY_SCALE)) == APPLY_SCALE && new_m11 == 1.0 && new_m22 == 1.0) {
      newstate -= APPLY_SCALE;
    } else if ((newstate & (APPLY_SHEAR | APPLY_SCALE)) == APPLY_SHEAR && new_m12 == 0.0 && new_m21 == 0.0) {
      newstate = ((newstate & ~APPLY_SHEAR) | APPLY_SCALE);
    }
    state2d = newstate;
  }

  void
  AbstractHomography::leftRotate2d_180 () 
  {
    double new_m11 = - m11 ()->get_value ();
    double new_m22 = - m22 ()->get_value ();
    m11 ()->set_value (new_m11, false);
    m12 ()->set_value (- m12 ()->get_value (), false);
    m14 ()->set_value (- m14 ()->get_value (), false);
    m21 ()->set_value (- m21 ()->get_value (), false);
    m22 ()->set_value (new_m22, false);
    m24 ()->set_value (- m24 ()->get_value (), false);
    if ((state2d & APPLY_SHEAR) != 0) {
      if (new_m11 == 0.0 && new_m22 == 0.0) {
        state2d &= ~APPLY_SCALE;
      } else {
        state2d |= APPLY_SCALE;
      }
    } else {
      if (new_m11 == 1.0 && new_m22 == 1.0) {
        state2d &= ~APPLY_SCALE;
      } else {
        state2d |= APPLY_SCALE;
      }
    }
  }

  void
  AbstractHomography::leftRotate2d_270 ()
  {
    double new_m11 = m21 ()->get_value ();
    double new_m12 = m22 ()->get_value ();
    double new_m14 = m24 ()->get_value ();
    double new_m21 = - m11 ()->get_value ();
    double new_m22 = - m12 ()->get_value ();
    double new_m24 = - m14 ()->get_value ();
    m11 ()->set_value (new_m11, false);
    m12 ()->set_value (new_m12, false);
    m21 ()->set_value (new_m21, false);
    m22 ()->set_value (new_m22, false);
    m14 ()->set_value (new_m14, false);
    m24 ()->set_value (new_m24, false);
    int newstate = rot90conversion[state2d];
    if ((newstate & (APPLY_SHEAR | APPLY_SCALE)) == APPLY_SCALE && new_m11 == 1.0 && new_m22 == 1.0) {
      newstate -= APPLY_SCALE;
    } else if ((newstate & (APPLY_SHEAR | APPLY_SCALE)) == APPLY_SHEAR && new_m12 == 0.0 && new_m21 == 0.0) {
      newstate = ((newstate & ~APPLY_SHEAR) | APPLY_SCALE);
    }
    state2d = newstate;
  }

  void
  AbstractHomography::rightSkew (double dax, double day)
  {
    rightSkew2d (dax, day);
    // Hack to propagate the matrix modification after complete calculation FIXME
    m11 ()->set_value (m11 ()->get_value (), true);
  }

  void
  AbstractHomography::rightSkew (double dax, double day, double cx, double cy)
  {
    if (cx != 0.0 || cy != 0.0) {
      rightTranslate2d (cx, cy);
      rightSkew2d (dax, day);
      rightTranslate2d (-cx, -cy);
    } else {
      rightSkew2d (dax, day);
    }
    // Hack to propagate the matrix modification after complete calculation FIXME
    m11 ()->set_value (m11 ()->get_value (), true);
  }

  void
  AbstractHomography::rightSkew2d (double dax_in_degrees, double day_in_degrees)
  {
    double dax_in_radians = degrees_to_radians (dax_in_degrees);
    double shx = - std::tan (dax_in_radians);
    double day_in_radians = degrees_to_radians (day_in_degrees);
    double shy = std::tan (day_in_radians);
    int mystate = state2d;
    double old_m11, old_m12, old_m21, old_m22, new_m11, new_m12, new_m21, new_m22;
    switch (mystate) {
      case APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE:
      case APPLY_SHEAR | APPLY_SCALE:
        old_m11 = m11 ()->get_value ();
        old_m12 = m12 ()->get_value ();
        m11 ()-> set_value (old_m11 + old_m12 * shy, false);
        m12 ()-> set_value (old_m11 * shx + old_m12, false);
        old_m21 = m21 ()->get_value ();
        old_m22 = m22 ()->get_value ();
        m21 ()->set_value (old_m21 + old_m22 * shy, false);
        m22 ()->set_value (old_m21 * shx + old_m22, false);
        updateState2d();
        return;
      case APPLY_SHEAR | APPLY_TRANSLATE:
      case APPLY_SHEAR:
        new_m11 = m12 ()->get_value () * shy;
        new_m22 = m21 ()->get_value () * shx;
        m11 ()->set_value (new_m11, false);
        m22 ()->set_value (new_m22, false);
        if (new_m11 != 0.0 || new_m22 != 0.0) {
          state2d = mystate | APPLY_SCALE;
        }
        return;
      case APPLY_SCALE | APPLY_TRANSLATE:
      case APPLY_SCALE:
        new_m12 = m11 ()->get_value () * shx;
        new_m21 = m22 ()->get_value () * shy;
        m12 ()->set_value (new_m12, false);
        m21 ()->set_value (new_m21, false);
        if (new_m12 != 0.0 || new_m21 != 0.0) {
          state2d = mystate | APPLY_SHEAR;
        }
        return;
      case APPLY_TRANSLATE:
      case APPLY_IDENTITY:
        new_m12 = shx;
        new_m21 = shy;
        m12 ()->set_value (new_m12, false);
        m21 ()->set_value (new_m21, false);
        if (new_m12 != 0.0 || new_m21 != 0.0) {
          state2d = mystate | APPLY_SCALE | APPLY_SHEAR;
        }
        return;
      default:
        cerr << "AbstractHomography::rightSkew2d -> missing case in a switch " << state2d << endl;
        // cannot reach
        break;
    }
  }

  void
  AbstractHomography::leftSkew (double dax, double day)
  {
    leftSkew2d(dax, day);
    // Hack to propagate the matrix modification after complete calculation FIXME
    m11 ()->set_value (m11 ()->get_value (), true);
  }
  
  void
  AbstractHomography::leftSkew (double dax, double day, double cx, double cy)
  {
    if (cx != 0.0 || cy != 0.0) {
      leftTranslate2d (-cx, -cy);
      leftSkew2d (dax, day);
      leftTranslate2d (cx, cy);
    } else {
      leftSkew2d (dax, day);
    }
    // Hack to propagate the matrix modification after complete calculation FIXME
    m11 ()->set_value (m11 ()->get_value (), true);
  }
  
  void
  AbstractHomography::leftSkew2d (double dax_in_degrees, double day_in_degrees)
  {
    double dax_in_radians = degrees_to_radians (dax_in_degrees);
    double shx = - std::tan (dax_in_radians);
    double day_in_radians = degrees_to_radians (day_in_degrees);
    double shy = std::tan (day_in_radians);
    int mystate = state2d;
    double old_m11, old_m12, old_m14, old_m21, old_m22, old_m24, new_m12, new_m14, new_m21, new_m24;
    switch (mystate) {
      case APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE:
      case APPLY_SHEAR | APPLY_TRANSLATE:
        old_m14 = m14 ()->get_value ();
        old_m24 = m24 ()->get_value ();
        m14 ()->set_value (old_m14 + shx * old_m24, false);
        m24 ()->set_value (old_m24 + shy * old_m14, false);
        // fall-through
      case APPLY_SHEAR | APPLY_SCALE:
      case APPLY_SHEAR:
        old_m11 = m11 ()->get_value ();
        old_m12 = m12 ()->get_value ();
        old_m21 = m21 ()->get_value ();
        old_m22 = m22 ()->get_value ();
        m11 ()->set_value (old_m11 + shx * old_m21, false);
        m12 ()->set_value (old_m12 + shx * old_m22, false);
        m21 ()->set_value (shy * old_m11 + old_m21, false);
        m22 ()->set_value (shy * old_m12 + old_m22, false);
        updateState2d();
        return;
      case APPLY_SCALE | APPLY_TRANSLATE:
        old_m14 = m14 ()->get_value ();
        old_m24 = m24 ()->get_value ();
        new_m14 = old_m14 + shx * old_m24;
        new_m24 = old_m24 + shy * old_m14;
        m14 ()->set_value (new_m14, false);
        m24 ()->set_value (new_m24, false);
        if (new_m14 == 0.0 && new_m24 == 0.0) {
          mystate = mystate & ~APPLY_TRANSLATE;
          state2d = mystate;
        }
        // fall-through
      case APPLY_SCALE:
        new_m12 = shx * m22 ()->get_value ();
        new_m21 = shy * m11 ()->get_value ();
        m12 ()->set_value (new_m12, false);
        m21 ()->set_value (new_m21, false);
        if (new_m12 != 0.0 || new_m21 != 0.0) {
          state2d = mystate | APPLY_SHEAR;
        }
        return;
      case APPLY_TRANSLATE:
        old_m14 = m14 ()->get_value ();
        old_m24 = m24 ()->get_value ();
        new_m14 = old_m14 + shx * old_m24;
        new_m24 = old_m24 + shy * old_m14;
        m14 ()->set_value (new_m14, false);
        m24 ()->set_value (new_m24, false);
        if (new_m14 == 0.0 && new_m24 == 0.0) {
          mystate = mystate & ~APPLY_TRANSLATE;
          state2d = mystate;
        }
        // fall-through
      case APPLY_IDENTITY:
        new_m12 = shx;
        new_m21 = shy;
        m12 ()->set_value (new_m12, false);
        m21 ()->set_value (new_m21, false);
        if (new_m12 != 0.0 || new_m21 != 0.0) {
          state2d = mystate | APPLY_SCALE | APPLY_SHEAR;
        }
        return;
      default:
        cerr << "AbstractHomography::leftSkew2d -> missing case in a switch " << state2d << endl;
        // cannot reach
        break;
    }
  }

  void
  AbstractHomography::RightTranslateByAction::impl_activate () 
  { 
    double dx = _h->_rightTranslateBy_dx->get_value ();
    double dy = _h->_rightTranslateBy_dy->get_value ();

    _h->rightTranslate (dx, dy);
    
    _h->_rightTranslateBy_dx->set_value (0, false);
    _h->_rightTranslateBy_dy->set_value (0, false);
  }

  void
  AbstractHomography::LeftTranslateByAction::impl_activate () 
  { 
    double dx = _h->_leftTranslateBy_dx->get_value ();
    double dy = _h->_leftTranslateBy_dy->get_value ();

    _h->leftTranslate (dx, dy);
    
    _h->_leftTranslateBy_dx->set_value (0, false);
    _h->_leftTranslateBy_dy->set_value (0, false);
  }

  void
  AbstractHomography::RightScaleByAction::impl_activate () 
  { 
    double cx = _h->_rightScaleBy_cx->get_value ();
    double cy = _h->_rightScaleBy_cy->get_value ();
    double sx = _h->_rightScaleBy_sx->get_value ();
    double sy = _h->_rightScaleBy_sy->get_value ();

    _h->rightScale (sx, sy, cx, cy);

    _h->_accsx->set_value (_h->_accsx->get_value () * sx, true);
    _h->_accsy->set_value (_h->_accsy->get_value () * sy, true);
    _h->_rightScaleBy_sx->set_value (1, false);
    _h->_rightScaleBy_sy->set_value (1, false);
  }

  void
  AbstractHomography::LeftScaleByAction::impl_activate () 
  { 
    double cx = _h->_leftScaleBy_cx->get_value ();
    double cy = _h->_leftScaleBy_cy->get_value ();
    double sx = _h->_leftScaleBy_sx->get_value ();
    double sy = _h->_leftScaleBy_sy->get_value ();

    _h->leftScale (sx, sy, cx, cy);

    _h->_accsx->set_value (_h->_accsx->get_value () * sx, true);
    _h->_accsy->set_value (_h->_accsy->get_value () * sy, true);
    _h->_leftScaleBy_sx->set_value (1, false);
    _h->_leftScaleBy_sy->set_value (1, false);
  }

  void
  AbstractHomography::RightRotateByAction::impl_activate () 
  { 
    double cx = _h->_rightRotateBy_cx->get_value ();
    double cy = _h->_rightRotateBy_cy->get_value ();
    double da = _h->_rightRotateBy_da->get_value ();

    _h->rightRotate (da, cx, cy);

    _h->_acca->set_value (_h->_acca->get_value () + da, true);
    _h->_rightRotateBy_da->set_value (0, false);
  }

  void
  AbstractHomography::LeftRotateByAction::impl_activate () 
  { 
    double cx = _h->_leftRotateBy_cx->get_value ();
    double cy = _h->_leftRotateBy_cy->get_value ();
    double da = _h->_leftRotateBy_da->get_value ();

    _h->leftRotate (da, cx, cy);

    _h->_acca->set_value (_h->_acca->get_value () + da, true);
    _h->_leftRotateBy_da->set_value (0, false);
  }

  void
  AbstractHomography::Right_Skew_X_ByAction::impl_activate ()
  { 
    double cx = _h->_rightSkew_X_By_cx->get_value ();
    double cy = _h->_rightSkew_X_By_cy->get_value ();
    double dax = _h->_rightSkew_X_By_da->get_value ();

    _h->rightSkew (dax, 0, cx, cy);

    _h->_rightSkew_X_By_da->set_value (0, false);
  }

  void
  AbstractHomography::Left_Skew_X_ByAction::impl_activate ()
  { 
    double cx = _h->_leftSkew_X_By_cx->get_value ();
    double cy = _h->_leftSkew_X_By_cy->get_value ();
    double dax = _h->_leftSkew_X_By_da->get_value ();

    _h->leftSkew (dax, 0, cx, cy);

    _h->_leftSkew_X_By_da->set_value (0, false);
  }

  void
  AbstractHomography::Right_Skew_Y_ByAction::impl_activate ()
  { 
    double cx = _h->_rightSkew_Y_By_cx->get_value ();
    double cy = _h->_rightSkew_Y_By_cy->get_value ();
    double day = _h->_rightSkew_Y_By_da->get_value ();

    _h->rightSkew (0, day, cx, cy);

    _h->_rightSkew_Y_By_da->set_value (0, false);
  }
 
  void
  AbstractHomography::Left_Skew_Y_ByAction::impl_activate ()
  { 
    double cx = _h->_leftSkew_Y_By_cx->get_value ();
    double cy = _h->_leftSkew_Y_By_cy->get_value ();
    double day = _h->_leftSkew_Y_By_da->get_value ();

    _h->leftSkew (0, day, cx, cy);

    _h->_leftSkew_Y_By_da->set_value (0, false);
  }

  void
  AbstractHomography::init_rightTranslateBy ()
  {
    _rightTranslateBy_spike = new Spike (this, "rightTranslateBy");
    _rightTranslateBy_dx = new DoubleProperty (0);
    _rightTranslateBy_dy = new DoubleProperty (0);
    _rightTranslateBy_spike->add_symbol( "dx", _rightTranslateBy_dx);
    _rightTranslateBy_spike->add_symbol( "dy", _rightTranslateBy_dy);
    _rightTranslateBy_action = new RightTranslateByAction (this, "action_right_translate_by", this);
    _rightTranslateBy_dx_coupling = new Coupling (_rightTranslateBy_dx, ACTIVATION, _rightTranslateBy_action, ACTIVATION);
    this->somehow_activating () ? _rightTranslateBy_dx_coupling->enable (_frame) : _rightTranslateBy_dx_coupling->disable ();
    _rightTranslateBy_dy_coupling = new Coupling (_rightTranslateBy_dy, ACTIVATION, _rightTranslateBy_action, ACTIVATION);
    this->somehow_activating () ? _rightTranslateBy_dy_coupling->enable(_frame) : _rightTranslateBy_dy_coupling->disable ();
    Graph::instance().add_edge(_rightTranslateBy_dx, _rightTranslateBy_action);
    Graph::instance().add_edge(_rightTranslateBy_dy, _rightTranslateBy_action);
    Graph::instance().add_edge(_rightTranslateBy_action, m11 ());
    Graph::instance().add_edge(_rightTranslateBy_action, m12 ());
    Graph::instance().add_edge(_rightTranslateBy_action, m14 ());
    Graph::instance().add_edge(_rightTranslateBy_action, m21 ());
    Graph::instance().add_edge(_rightTranslateBy_action, m22 ());
    Graph::instance().add_edge(_rightTranslateBy_action, m24 ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _rightTranslateBy_action);
  }

  void
  AbstractHomography::init_leftTranslateBy ()
  {
    _leftTranslateBy_spike = new Spike (this, "leftTranslateBy");
    _leftTranslateBy_dx = new DoubleProperty (0);
    _leftTranslateBy_dy = new DoubleProperty (0);
    _leftTranslateBy_spike->add_symbol( "dx", _leftTranslateBy_dx);
    _leftTranslateBy_spike->add_symbol( "dy", _leftTranslateBy_dy);
    _leftTranslateBy_action = new LeftTranslateByAction (this, "action_left_translate_by", this);
    _leftTranslateBy_dx_coupling = new Coupling (_leftTranslateBy_dx, ACTIVATION, _leftTranslateBy_action, ACTIVATION);
    this->somehow_activating () ? _leftTranslateBy_dx_coupling->enable (_frame) : _leftTranslateBy_dx_coupling->disable ();
    _leftTranslateBy_dy_coupling = new Coupling (_leftTranslateBy_dy, ACTIVATION, _leftTranslateBy_action, ACTIVATION);
    this->somehow_activating () ? _leftTranslateBy_dy_coupling->enable(_frame) : _leftTranslateBy_dy_coupling->disable ();
    Graph::instance().add_edge(_leftTranslateBy_dx, _leftTranslateBy_action);
    Graph::instance().add_edge(_leftTranslateBy_dy, _leftTranslateBy_action);
    Graph::instance().add_edge(_leftTranslateBy_action, m11 ());
    Graph::instance().add_edge(_leftTranslateBy_action, m12 ());
    Graph::instance().add_edge(_leftTranslateBy_action, m14 ());
    Graph::instance().add_edge(_leftTranslateBy_action, m21 ());
    Graph::instance().add_edge(_leftTranslateBy_action, m22 ());
    Graph::instance().add_edge(_leftTranslateBy_action, m24 ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _leftTranslateBy_action);
  }

  void
  AbstractHomography::init_rightScaleBy () 
  {
    _rightScaleBy_spike = new Spike (this, "rightScaleBy");
    _rightScaleBy_cx = new DoubleProperty (0);
    _rightScaleBy_cy = new DoubleProperty (0);
    _rightScaleBy_sx = new DoubleProperty (1);
    _rightScaleBy_sy = new DoubleProperty (1);
    _rightScaleBy_spike->add_symbol ("cx", _rightScaleBy_cx);
    _rightScaleBy_spike->add_symbol ("cy", _rightScaleBy_cy);
    _rightScaleBy_spike->add_symbol ("sx", _rightScaleBy_sx);
    _rightScaleBy_spike->add_symbol ("sy", _rightScaleBy_sy);
    _rightScaleBy_action = new RightScaleByAction (this, "action_right_scale_by", this);
    _rightScaleBy_cx_coupling = new Coupling (_rightScaleBy_cx, ACTIVATION, _rightScaleBy_action, ACTIVATION);
    this->somehow_activating () ? _rightScaleBy_cx_coupling->enable(_frame) : _rightScaleBy_cx_coupling->disable ();
    _rightScaleBy_cy_coupling = new Coupling (_rightScaleBy_cy, ACTIVATION, _rightScaleBy_action, ACTIVATION);
    this->somehow_activating () ? _rightScaleBy_cy_coupling->enable(_frame) : _rightScaleBy_cy_coupling->disable ();
    _rightScaleBy_sx_coupling = new Coupling (_rightScaleBy_sx, ACTIVATION, _rightScaleBy_action, ACTIVATION);
    this->somehow_activating () ? _rightScaleBy_sx_coupling->enable(_frame) : _rightScaleBy_sx_coupling->disable ();
    _rightScaleBy_sy_coupling = new Coupling (_rightScaleBy_sy, ACTIVATION, _rightScaleBy_action, ACTIVATION);
    this->somehow_activating () ? _rightScaleBy_sy_coupling->enable(_frame) :_rightScaleBy_sy_coupling->disable ();
    Graph::instance().add_edge(_rightScaleBy_cx, _rightScaleBy_action);
    Graph::instance().add_edge(_rightScaleBy_cy, _rightScaleBy_action);
    Graph::instance().add_edge(_rightScaleBy_sx, _rightScaleBy_action);
    Graph::instance().add_edge(_rightScaleBy_sy, _rightScaleBy_action);
    Graph::instance().add_edge(_rightScaleBy_action, m11 ());
    Graph::instance().add_edge(_rightScaleBy_action, m12 ());
    Graph::instance().add_edge(_rightScaleBy_action, m14 ());
    Graph::instance().add_edge(_rightScaleBy_action, m21 ());
    Graph::instance().add_edge(_rightScaleBy_action, m22 ());
    Graph::instance().add_edge(_rightScaleBy_action, m24 ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _rightScaleBy_action);
  }

  void
  AbstractHomography::init_leftScaleBy () 
  {
    _leftScaleBy_spike = new Spike (this, "leftScaleBy");
    _leftScaleBy_cx = new DoubleProperty (0);
    _leftScaleBy_cy = new DoubleProperty (0);
    _leftScaleBy_sx = new DoubleProperty (1);
    _leftScaleBy_sy = new DoubleProperty (1);
    _leftScaleBy_spike->add_symbol ("cx", _leftScaleBy_cx);
    _leftScaleBy_spike->add_symbol ("cy", _leftScaleBy_cy);
    _leftScaleBy_spike->add_symbol ("sx", _leftScaleBy_sx);
    _leftScaleBy_spike->add_symbol ("sy", _leftScaleBy_sy);
    _leftScaleBy_action = new LeftScaleByAction (this, "action_left_scale_by", this);
    _leftScaleBy_cx_coupling = new Coupling (_leftScaleBy_cx, ACTIVATION, _leftScaleBy_action, ACTIVATION);
    this->somehow_activating () ? _leftScaleBy_cx_coupling->enable(_frame) : _leftScaleBy_cx_coupling->disable ();
    _leftScaleBy_cy_coupling = new Coupling (_leftScaleBy_cy, ACTIVATION, _leftScaleBy_action, ACTIVATION);
    this->somehow_activating () ? _leftScaleBy_cy_coupling->enable(_frame) : _leftScaleBy_cy_coupling->disable ();
    _leftScaleBy_sx_coupling = new Coupling (_leftScaleBy_sx, ACTIVATION, _leftScaleBy_action, ACTIVATION);
    this->somehow_activating () ? _leftScaleBy_sx_coupling->enable(_frame) : _leftScaleBy_sx_coupling->disable ();
    _leftScaleBy_sy_coupling = new Coupling (_leftScaleBy_sy, ACTIVATION, _leftScaleBy_action, ACTIVATION);
    this->somehow_activating () ? _leftScaleBy_sy_coupling->enable(_frame) :_leftScaleBy_sy_coupling->disable ();
    Graph::instance().add_edge(_leftScaleBy_cx, _leftScaleBy_action);
    Graph::instance().add_edge(_leftScaleBy_cy, _leftScaleBy_action);
    Graph::instance().add_edge(_leftScaleBy_sx, _leftScaleBy_action);
    Graph::instance().add_edge(_leftScaleBy_sy, _leftScaleBy_action);
    Graph::instance().add_edge(_leftScaleBy_action, m11 ());
    Graph::instance().add_edge(_leftScaleBy_action, m12 ());
    Graph::instance().add_edge(_leftScaleBy_action, m14 ());
    Graph::instance().add_edge(_leftScaleBy_action, m21 ());
    Graph::instance().add_edge(_leftScaleBy_action, m22 ());
    Graph::instance().add_edge(_leftScaleBy_action, m24 ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _leftScaleBy_action);
  }

  void
  AbstractHomography::init_rightRotateBy () 
  {
    _rightRotateBy_spike = new Spike (this, "rightRotateBy");
    _rightRotateBy_cx = new DoubleProperty (0);
    _rightRotateBy_cy = new DoubleProperty (0);
    _rightRotateBy_da = new DoubleProperty (0);
    _rightRotateBy_spike->add_symbol ("cx", _rightRotateBy_cx);
    _rightRotateBy_spike->add_symbol ("cy", _rightRotateBy_cy);
    _rightRotateBy_spike->add_symbol ("da", _rightRotateBy_da);
    _rightRotateBy_action = new RightRotateByAction (this, "action_right_rotate_by", this);
    _rightRotateBy_cx_coupling = new Coupling (_rightRotateBy_cx, ACTIVATION, _rightRotateBy_action, ACTIVATION);
    this->somehow_activating () ? _rightRotateBy_cx_coupling->enable(_frame) : _rightRotateBy_cx_coupling->disable ();
    _rightRotateBy_cy_coupling = new Coupling (_rightRotateBy_cy, ACTIVATION, _rightRotateBy_action, ACTIVATION);
    this->somehow_activating () ? _rightRotateBy_cy_coupling->enable(_frame) : _rightRotateBy_cy_coupling->disable ();
    _rightRotateBy_da_coupling = new Coupling (_rightRotateBy_da, ACTIVATION, _rightRotateBy_action, ACTIVATION);
    this->somehow_activating () ? _rightRotateBy_da_coupling->enable(_frame) : _rightRotateBy_da_coupling->disable ();
    Graph::instance().add_edge(_rightRotateBy_cx, _rightRotateBy_action);
    Graph::instance().add_edge(_rightRotateBy_cy, _rightRotateBy_action);
    Graph::instance().add_edge(_rightRotateBy_da, _rightRotateBy_action);
    Graph::instance().add_edge(_rightRotateBy_action, m11 ());
    Graph::instance().add_edge(_rightRotateBy_action, m12 ());
    Graph::instance().add_edge(_rightRotateBy_action, m14 ());
    Graph::instance().add_edge(_rightRotateBy_action, m21 ());
    Graph::instance().add_edge(_rightRotateBy_action, m22 ());
    Graph::instance().add_edge(_rightRotateBy_action, m24 ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _rightRotateBy_action);
  }

  void
  AbstractHomography::init_leftRotateBy () 
  {
    _leftRotateBy_spike = new Spike (this, "leftRotateBy");
    _leftRotateBy_cx = new DoubleProperty (0);
    _leftRotateBy_cy = new DoubleProperty (0);
    _leftRotateBy_da = new DoubleProperty (0);
    _leftRotateBy_spike->add_symbol ("cx", _leftRotateBy_cx);
    _leftRotateBy_spike->add_symbol ("cy", _leftRotateBy_cy);
    _leftRotateBy_spike->add_symbol ("da", _leftRotateBy_da);
    _leftRotateBy_action = new LeftRotateByAction (this, "action_left_rotate_by", this);
    _leftRotateBy_cx_coupling = new Coupling (_leftRotateBy_cx, ACTIVATION, _leftRotateBy_action, ACTIVATION);
    this->somehow_activating () ? _leftRotateBy_cx_coupling->enable(_frame) : _leftRotateBy_cx_coupling->disable ();
    _leftRotateBy_cy_coupling = new Coupling (_leftRotateBy_cy, ACTIVATION, _leftRotateBy_action, ACTIVATION);
    this->somehow_activating () ? _leftRotateBy_cy_coupling->enable(_frame) : _leftRotateBy_cy_coupling->disable ();
    _leftRotateBy_da_coupling = new Coupling (_leftRotateBy_da, ACTIVATION, _leftRotateBy_action, ACTIVATION);
    this->somehow_activating () ? _leftRotateBy_da_coupling->enable(_frame) : _leftRotateBy_da_coupling->disable ();
    Graph::instance().add_edge(_leftRotateBy_cx, _leftRotateBy_action);
    Graph::instance().add_edge(_leftRotateBy_cy, _leftRotateBy_action);
    Graph::instance().add_edge(_leftRotateBy_da, _leftRotateBy_action);
    Graph::instance().add_edge(_leftRotateBy_action, m11 ());
    Graph::instance().add_edge(_leftRotateBy_action, m12 ());
    Graph::instance().add_edge(_leftRotateBy_action, m14 ());
    Graph::instance().add_edge(_leftRotateBy_action, m21 ());
    Graph::instance().add_edge(_leftRotateBy_action, m22 ());
    Graph::instance().add_edge(_leftRotateBy_action, m24 ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _leftRotateBy_action);
  }

  void
  AbstractHomography::init_rightSkewXBy () 
  {
    _rightSkew_X_By_spike = new Spike (this, "rightSkewXBy");
    _rightSkew_X_By_cx = new DoubleProperty (0);
    _rightSkew_X_By_cy = new DoubleProperty (0);
    _rightSkew_X_By_da = new DoubleProperty (0);
    _rightSkew_X_By_spike->add_symbol ("cx", _rightSkew_X_By_cx);
    _rightSkew_X_By_spike->add_symbol ("cy", _rightSkew_X_By_cy);
    _rightSkew_X_By_spike->add_symbol ("da", _rightSkew_X_By_da);
    _rightSkew_X_By_action = new Right_Skew_X_ByAction (this, "action_right_skewX_by", this);
    _rightSkew_X_By_cx_coupling = new Coupling (_rightSkew_X_By_cx, ACTIVATION, _rightSkew_X_By_action, ACTIVATION);
    this->somehow_activating () ? _rightSkew_X_By_cx_coupling->enable(_frame) : _rightSkew_X_By_cx_coupling->disable ();
    _rightSkew_X_By_cy_coupling = new Coupling (_rightSkew_X_By_cy, ACTIVATION, _rightSkew_X_By_action, ACTIVATION);
    this->somehow_activating () ? _rightSkew_X_By_cy_coupling->enable(_frame) : _rightSkew_X_By_cy_coupling->disable ();
    _rightSkew_X_By_da_coupling = new Coupling (_rightSkew_X_By_da, ACTIVATION, _rightSkew_X_By_action, ACTIVATION);
    this->somehow_activating () ? _rightSkew_X_By_da_coupling->enable(_frame) : _rightSkew_X_By_da_coupling->disable ();
    Graph::instance().add_edge(_rightSkew_X_By_cx, _rightSkew_X_By_action);
    Graph::instance().add_edge(_rightSkew_X_By_cy, _rightSkew_X_By_action);
    Graph::instance().add_edge(_rightSkew_X_By_da, _rightSkew_X_By_action);
    Graph::instance().add_edge(_rightSkew_X_By_action, m11 ());
    Graph::instance().add_edge(_rightSkew_X_By_action, m12 ());
    Graph::instance().add_edge(_rightSkew_X_By_action, m14 ());
    Graph::instance().add_edge(_rightSkew_X_By_action, m21 ());
    Graph::instance().add_edge(_rightSkew_X_By_action, m22 ());
    Graph::instance().add_edge(_rightSkew_X_By_action, m24 ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _rightSkew_X_By_action);
  }

  void
  AbstractHomography::init_leftSkewXBy () 
  {
    _leftSkew_X_By_spike = new Spike (this, "leftSkewXBy");
    _leftSkew_X_By_cx = new DoubleProperty (0);
    _leftSkew_X_By_cy = new DoubleProperty (0);
    _leftSkew_X_By_da = new DoubleProperty (0);
    _leftSkew_X_By_spike->add_symbol ("cx", _leftSkew_X_By_cx);
    _leftSkew_X_By_spike->add_symbol ("cy", _leftSkew_X_By_cy);
    _leftSkew_X_By_spike->add_symbol ("da", _leftSkew_X_By_da);
    _leftSkew_X_By_action = new Left_Skew_X_ByAction (this, "action_left_skewX_by", this);
    _leftSkew_X_By_cx_coupling = new Coupling (_leftSkew_X_By_cx, ACTIVATION, _leftSkew_X_By_action, ACTIVATION);
    this->somehow_activating () ? _leftSkew_X_By_cx_coupling->enable(_frame) : _leftSkew_X_By_cx_coupling->disable ();
    _leftSkew_X_By_cy_coupling = new Coupling (_leftSkew_X_By_cy, ACTIVATION, _leftSkew_X_By_action, ACTIVATION);
    this->somehow_activating () ? _leftSkew_X_By_cy_coupling->enable(_frame) : _leftSkew_X_By_cy_coupling->disable ();
    _leftSkew_X_By_da_coupling = new Coupling (_leftSkew_X_By_da, ACTIVATION, _leftSkew_X_By_action, ACTIVATION);
    this->somehow_activating () ? _leftSkew_X_By_da_coupling->enable(_frame) : _leftSkew_X_By_da_coupling->disable ();
    Graph::instance().add_edge(_leftSkew_X_By_cx, _leftSkew_X_By_action);
    Graph::instance().add_edge(_leftSkew_X_By_cy, _leftSkew_X_By_action);
    Graph::instance().add_edge(_leftSkew_X_By_da, _leftSkew_X_By_action);
    Graph::instance().add_edge(_leftSkew_X_By_action, m11 ());
    Graph::instance().add_edge(_leftSkew_X_By_action, m12 ());
    Graph::instance().add_edge(_leftSkew_X_By_action, m14 ());
    Graph::instance().add_edge(_leftSkew_X_By_action, m21 ());
    Graph::instance().add_edge(_leftSkew_X_By_action, m22 ());
    Graph::instance().add_edge(_leftSkew_X_By_action, m24 ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _leftSkew_X_By_action);
  }

  void
  AbstractHomography::init_rightSkewYBy () 
  {
    _rightSkew_Y_By_spike = new Spike (this, "rightSkewYBy");
    _rightSkew_Y_By_cx = new DoubleProperty (0);
    _rightSkew_Y_By_cy = new DoubleProperty (0);
    _rightSkew_Y_By_da = new DoubleProperty (0);
    _rightSkew_Y_By_spike->add_symbol ("cx", _rightSkew_Y_By_cx);
    _rightSkew_Y_By_spike->add_symbol ("cy", _rightSkew_Y_By_cy);
    _rightSkew_Y_By_spike->add_symbol ("da", _rightSkew_Y_By_da);
    _rightSkew_Y_By_action = new Right_Skew_Y_ByAction (this, "action_right_skewY_by", this);
    _rightSkew_Y_By_cx_coupling = new Coupling (_rightSkew_Y_By_cx, ACTIVATION, _rightSkew_Y_By_action, ACTIVATION);
    this->somehow_activating () ? _rightSkew_Y_By_cx_coupling->enable(_frame) : _rightSkew_Y_By_cx_coupling->disable ();
    _rightSkew_Y_By_cy_coupling = new Coupling (_rightSkew_Y_By_cy, ACTIVATION, _rightSkew_Y_By_action, ACTIVATION);
    this->somehow_activating () ? _rightSkew_Y_By_cy_coupling->enable(_frame) : _rightSkew_Y_By_cy_coupling->disable ();
    _rightSkew_Y_By_da_coupling = new Coupling (_rightSkew_Y_By_da, ACTIVATION, _rightSkew_Y_By_action, ACTIVATION);
    this->somehow_activating () ? _rightSkew_Y_By_da_coupling->enable(_frame) : _rightSkew_Y_By_da_coupling->disable ();
    Graph::instance().add_edge(_rightSkew_Y_By_cx, _rightSkew_Y_By_action);
    Graph::instance().add_edge(_rightSkew_Y_By_cy, _rightSkew_Y_By_action);
    Graph::instance().add_edge(_rightSkew_Y_By_da, _rightSkew_Y_By_action);
    Graph::instance().add_edge(_rightSkew_Y_By_action, m11 ());
    Graph::instance().add_edge(_rightSkew_Y_By_action, m12 ());
    Graph::instance().add_edge(_rightSkew_Y_By_action, m14 ());
    Graph::instance().add_edge(_rightSkew_Y_By_action, m21 ());
    Graph::instance().add_edge(_rightSkew_Y_By_action, m22 ());
    Graph::instance().add_edge(_rightSkew_Y_By_action, m24 ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _rightSkew_Y_By_action);
  }

  void
  AbstractHomography::init_leftSkewYBy () 
  {
    _leftSkew_Y_By_spike = new Spike (this, "leftSkewYBy");
    _leftSkew_Y_By_cx = new DoubleProperty (0);
    _leftSkew_Y_By_cy = new DoubleProperty (0);
    _leftSkew_Y_By_da = new DoubleProperty (0);
    _leftSkew_Y_By_spike->add_symbol ("cx", _leftSkew_Y_By_cx);
    _leftSkew_Y_By_spike->add_symbol ("cy", _leftSkew_Y_By_cy);
    _leftSkew_Y_By_spike->add_symbol ("da", _leftSkew_Y_By_da);
    _leftSkew_Y_By_action = new Left_Skew_Y_ByAction (this, "action_left_skewY_by", this);
    _leftSkew_Y_By_cx_coupling = new Coupling (_leftSkew_Y_By_cx, ACTIVATION, _leftSkew_Y_By_action, ACTIVATION);
    this->somehow_activating () ? _leftSkew_Y_By_cx_coupling->enable(_frame) : _leftSkew_Y_By_cx_coupling->disable ();
    _leftSkew_Y_By_cy_coupling = new Coupling (_leftSkew_Y_By_cy, ACTIVATION, _leftSkew_Y_By_action, ACTIVATION);
    this->somehow_activating () ? _leftSkew_Y_By_cy_coupling->enable(_frame) : _leftSkew_Y_By_cy_coupling->disable ();
    _leftSkew_Y_By_da_coupling = new Coupling (_leftSkew_Y_By_da, ACTIVATION, _leftSkew_Y_By_action, ACTIVATION);
    this->somehow_activating () ? _leftSkew_Y_By_da_coupling->enable(_frame) : _leftSkew_Y_By_da_coupling->disable ();
    Graph::instance().add_edge(_leftSkew_Y_By_cx, _leftSkew_Y_By_action);
    Graph::instance().add_edge(_leftSkew_Y_By_cy, _leftSkew_Y_By_action);
    Graph::instance().add_edge(_leftSkew_Y_By_da, _leftSkew_Y_By_action);
    Graph::instance().add_edge(_leftSkew_Y_By_action, m11 ());
    Graph::instance().add_edge(_leftSkew_Y_By_action, m12 ());
    Graph::instance().add_edge(_leftSkew_Y_By_action, m14 ());
    Graph::instance().add_edge(_leftSkew_Y_By_action, m21 ());
    Graph::instance().add_edge(_leftSkew_Y_By_action, m22 ());
    Graph::instance().add_edge(_leftSkew_Y_By_action, m24 ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _leftSkew_Y_By_action);
  }

  AbstractHomography::AbstractHomography (Process *p, const string &n, 
                                          double m11, double m12, double m13, double m14,
                                          double m21, double m22, double m23, double m24, 
                                          double m31, double m32, double m33, double m34, 
                                          double m41, double m42, double m43, double m44) :
      AbstractTransformation (p, n),
      raw_props{.m11=m11, .m12=m12, .m13=m13, .m14=m14,
                .m21=m21, .m22=m22, .m23=m23, .m24=m24,
                .m31=m31, .m32=m32, .m33=m33, .m34=m34,
                .m41=m41, .m42=m42, .m43=m43, .m44=m44,},
      _cm11(nullptr), _cm12(nullptr), _cm13(nullptr), _cm14(nullptr),
      _cm21(nullptr), _cm22(nullptr), _cm23(nullptr), _cm24(nullptr),
      _cm31(nullptr), _cm32(nullptr), _cm33(nullptr), _cm34(nullptr),
      _cm41(nullptr), _cm42(nullptr), _cm43(nullptr), _cm44(nullptr),

      /* rightTranslateBy ptr */
      _rightTranslateBy_spike(nullptr), _rightTranslateBy_action(nullptr),
      _rightTranslateBy_dx(nullptr), _rightTranslateBy_dy(nullptr),
      _rightTranslateBy_dx_coupling(nullptr), _rightTranslateBy_dy_coupling(nullptr),
      /* leftTranslateBy ptr */
      _leftTranslateBy_spike(nullptr), _leftTranslateBy_action(nullptr),
      _leftTranslateBy_dx(nullptr), _leftTranslateBy_dy(nullptr),
      _leftTranslateBy_dx_coupling(nullptr), _leftTranslateBy_dy_coupling(nullptr),
      /* rightScaleBy ptr */
      _rightScaleBy_spike(nullptr), _rightScaleBy_action(nullptr),
      _rightScaleBy_cx(nullptr), _rightScaleBy_cy(nullptr), _rightScaleBy_sx(nullptr), _rightScaleBy_sy(nullptr),
      _rightScaleBy_cx_coupling(nullptr), _rightScaleBy_cy_coupling(nullptr), _rightScaleBy_sx_coupling(nullptr), _rightScaleBy_sy_coupling(nullptr),
      /* leftScaleBy ptr */
      _leftScaleBy_spike(nullptr), _leftScaleBy_action(nullptr),
      _leftScaleBy_cx(nullptr), _leftScaleBy_cy(nullptr), _leftScaleBy_sx(nullptr), _leftScaleBy_sy(nullptr),
      _leftScaleBy_cx_coupling(nullptr), _leftScaleBy_cy_coupling(nullptr), _leftScaleBy_sx_coupling(nullptr), _leftScaleBy_sy_coupling(nullptr),
      /* rightRotateBy ptr */
      _rightRotateBy_spike(nullptr), _rightRotateBy_action(nullptr), 
      _rightRotateBy_cx(nullptr), _rightRotateBy_cy(nullptr), _rightRotateBy_da(nullptr),
      _rightRotateBy_cx_coupling(nullptr), _rightRotateBy_cy_coupling(nullptr), _rightRotateBy_da_coupling(nullptr),
      /* leftRotateBy ptr */
      _leftRotateBy_spike(nullptr), _leftRotateBy_action(nullptr), 
      _leftRotateBy_cx(nullptr), _leftRotateBy_cy(nullptr), _leftRotateBy_da(nullptr),
      _leftRotateBy_cx_coupling(nullptr), _leftRotateBy_cy_coupling(nullptr), _leftRotateBy_da_coupling(nullptr),
      /* rightSkewXBy ptr */
      _rightSkew_X_By_spike(nullptr), _rightSkew_X_By_action(nullptr),
      _rightSkew_X_By_cx(nullptr), _rightSkew_X_By_cy(nullptr), _rightSkew_X_By_da(nullptr),
      _rightSkew_X_By_cx_coupling(nullptr), _rightSkew_X_By_cy_coupling(nullptr), _rightSkew_X_By_da_coupling(nullptr),
      /* leftSkewXBy ptr */
      _leftSkew_X_By_spike(nullptr), _leftSkew_X_By_action(nullptr),
      _leftSkew_X_By_cx(nullptr), _leftSkew_X_By_cy(nullptr), _leftSkew_X_By_da(nullptr),
      _leftSkew_X_By_cx_coupling(nullptr), _leftSkew_X_By_cy_coupling(nullptr), _leftSkew_X_By_da_coupling(nullptr),
      /* rightSkewYBy ptr */
      _rightSkew_Y_By_spike(nullptr), _rightSkew_Y_By_action(nullptr), 
      _rightSkew_Y_By_cx(nullptr), _rightSkew_Y_By_cy(nullptr), _rightSkew_Y_By_da(nullptr),
      _rightSkew_Y_By_cx_coupling(nullptr), _rightSkew_Y_By_cy_coupling(nullptr), _rightSkew_Y_By_da_coupling(nullptr),
      /* leftSkewYBy ptr */
      _leftSkew_Y_By_spike(nullptr), _leftSkew_Y_By_action(nullptr), 
      _leftSkew_Y_By_cx(nullptr), _leftSkew_Y_By_cy(nullptr), _leftSkew_Y_By_da(nullptr),
      _leftSkew_Y_By_cx_coupling(nullptr), _leftSkew_Y_By_cy_coupling(nullptr), _leftSkew_Y_By_da_coupling(nullptr)

  {
    _acca = new DoubleProperty (this, "acca", 0);
    _accsx = new DoubleProperty (this, "accsx", 1);
    _accsy = new DoubleProperty (this, "accsy", 1);
  }

  AbstractHomography::AbstractHomography (double m11, double m12, double m13, double m14,
                                          double m21, double m22, double m23, double m24,
                                          double m31, double m32, double m33, double m34,
                                          double m41, double m42, double m43, double m44) : 
      AbstractHomography(nullptr, "", m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44) {}

  AbstractHomography::~AbstractHomography ()
  {
    if (_rightTranslateBy_action){
      if (_parent && _parent->state_dependency () != nullptr)
        Graph::instance ().remove_edge (_parent->state_dependency (), _rightTranslateBy_action);
      Graph::instance().remove_edge(_rightTranslateBy_dx, _rightTranslateBy_action);
      Graph::instance().remove_edge(_rightTranslateBy_dy, _rightTranslateBy_action);
      Graph::instance().remove_edge(_rightTranslateBy_action, this->m11 ());
      Graph::instance().remove_edge(_rightTranslateBy_action, this->m12 ());
      Graph::instance().remove_edge(_rightTranslateBy_action, this->m14 ());
      Graph::instance().remove_edge(_rightTranslateBy_action, this->m21 ());
      Graph::instance().remove_edge(_rightTranslateBy_action, this->m22 ());
      Graph::instance().remove_edge(_rightTranslateBy_action, this->m24 ());
      delete _rightTranslateBy_dx_coupling;
      delete _rightTranslateBy_dy_coupling;
      delete _rightTranslateBy_action;
      delete _rightTranslateBy_dx;
      delete _rightTranslateBy_dy;
      delete _rightTranslateBy_spike;
    }

    if (_leftTranslateBy_action){
      if (_parent && _parent->state_dependency () != nullptr)
        Graph::instance ().remove_edge (_parent->state_dependency (), _leftTranslateBy_action);
      Graph::instance().remove_edge(_leftTranslateBy_dx, _leftTranslateBy_action);
      Graph::instance().remove_edge(_leftTranslateBy_dy, _leftTranslateBy_action);
      Graph::instance().remove_edge(_leftTranslateBy_action, this->m11 ());
      Graph::instance().remove_edge(_leftTranslateBy_action, this->m12 ());
      Graph::instance().remove_edge(_leftTranslateBy_action, this->m14 ());
      Graph::instance().remove_edge(_leftTranslateBy_action, this->m21 ());
      Graph::instance().remove_edge(_leftTranslateBy_action, this->m22 ());
      Graph::instance().remove_edge(_leftTranslateBy_action, this->m24 ());
      delete _leftTranslateBy_dx_coupling;
      delete _leftTranslateBy_dy_coupling;
      delete _leftTranslateBy_action;
      delete _leftTranslateBy_dx;
      delete _leftTranslateBy_dy;
      delete _leftTranslateBy_spike;
    }

    if (_rightScaleBy_action) {
      if (_parent && _parent->state_dependency () != nullptr)
        Graph::instance ().remove_edge (_parent->state_dependency (), _rightScaleBy_action);
      Graph::instance().remove_edge(_rightScaleBy_cx, _rightScaleBy_action);
      Graph::instance().remove_edge(_rightScaleBy_cy, _rightScaleBy_action);
      Graph::instance().remove_edge(_rightScaleBy_sx, _rightScaleBy_action);
      Graph::instance().remove_edge(_rightScaleBy_sy, _rightScaleBy_action);
      Graph::instance().remove_edge(_rightScaleBy_action, this->m11 ());
      Graph::instance().remove_edge(_rightScaleBy_action, this->m12 ());
      Graph::instance().remove_edge(_rightScaleBy_action, this->m14 ());
      Graph::instance().remove_edge(_rightScaleBy_action, this->m21 ());
      Graph::instance().remove_edge(_rightScaleBy_action, this->m22 ());
      Graph::instance().remove_edge(_rightScaleBy_action, this->m24 ());
      delete _rightScaleBy_cx_coupling;
      delete _rightScaleBy_cy_coupling;
      delete _rightScaleBy_sx_coupling;
      delete _rightScaleBy_sy_coupling;
      delete _rightScaleBy_action;
      delete _rightScaleBy_cx;
      delete _rightScaleBy_cy;
      delete _rightScaleBy_sx;
      delete _rightScaleBy_sy;
      delete _rightScaleBy_spike;
    }

    if (_leftScaleBy_action) {
      if (_parent && _parent->state_dependency () != nullptr)
        Graph::instance ().remove_edge (_parent->state_dependency (), _leftScaleBy_action);
      Graph::instance().remove_edge(_leftScaleBy_cx, _leftScaleBy_action);
      Graph::instance().remove_edge(_leftScaleBy_cy, _leftScaleBy_action);
      Graph::instance().remove_edge(_leftScaleBy_sx, _leftScaleBy_action);
      Graph::instance().remove_edge(_leftScaleBy_sy, _leftScaleBy_action);
      Graph::instance().remove_edge(_leftScaleBy_action, this->m11 ());
      Graph::instance().remove_edge(_leftScaleBy_action, this->m12 ());
      Graph::instance().remove_edge(_leftScaleBy_action, this->m14 ());
      Graph::instance().remove_edge(_leftScaleBy_action, this->m21 ());
      Graph::instance().remove_edge(_leftScaleBy_action, this->m22 ());
      Graph::instance().remove_edge(_leftScaleBy_action, this->m24 ());
      delete _leftScaleBy_cx_coupling;
      delete _leftScaleBy_cy_coupling;
      delete _leftScaleBy_sx_coupling;
      delete _leftScaleBy_sy_coupling;
      delete _leftScaleBy_action;
      delete _leftScaleBy_cx;
      delete _leftScaleBy_cy;
      delete _leftScaleBy_sx;
      delete _leftScaleBy_sy;
      delete _leftScaleBy_spike;
    }

    if (_rightRotateBy_action) {
      if (_parent && _parent->state_dependency () != nullptr)
        Graph::instance ().remove_edge (_parent->state_dependency (), _rightRotateBy_action);
      Graph::instance().remove_edge(_rightRotateBy_cx, _rightRotateBy_action);
      Graph::instance().remove_edge(_rightRotateBy_cy, _rightRotateBy_action);
      Graph::instance().remove_edge(_rightRotateBy_da, _rightRotateBy_action);
      Graph::instance().remove_edge(_rightRotateBy_action, this->m11 ());
      Graph::instance().remove_edge(_rightRotateBy_action, this->m12 ());
      Graph::instance().remove_edge(_rightRotateBy_action, this->m14 ());
      Graph::instance().remove_edge(_rightRotateBy_action, this->m21 ());
      Graph::instance().remove_edge(_rightRotateBy_action, this->m22 ());
      Graph::instance().remove_edge(_rightRotateBy_action, this->m24 ());
      delete _rightRotateBy_cx_coupling;
      delete _rightRotateBy_cy_coupling;
      delete _rightRotateBy_da_coupling;
      delete _rightRotateBy_action;
      delete _rightRotateBy_cx;
      delete _rightRotateBy_cy;
      delete _rightRotateBy_da;
      delete _rightRotateBy_spike;
    }

    if (_leftRotateBy_action) {
      if (_parent && _parent->state_dependency () != nullptr)
        Graph::instance ().remove_edge (_parent->state_dependency (), _leftRotateBy_action);
      Graph::instance().remove_edge(_leftRotateBy_cx, _leftRotateBy_action);
      Graph::instance().remove_edge(_leftRotateBy_cy, _leftRotateBy_action);
      Graph::instance().remove_edge(_leftRotateBy_da, _leftRotateBy_action);
      Graph::instance().remove_edge(_leftRotateBy_action, this->m11 ());
      Graph::instance().remove_edge(_leftRotateBy_action, this->m12 ());
      Graph::instance().remove_edge(_leftRotateBy_action, this->m14 ());
      Graph::instance().remove_edge(_leftRotateBy_action, this->m21 ());
      Graph::instance().remove_edge(_leftRotateBy_action, this->m22 ());
      Graph::instance().remove_edge(_leftRotateBy_action, this->m24 ());
      delete _leftRotateBy_cx_coupling;
      delete _leftRotateBy_cy_coupling;
      delete _leftRotateBy_da_coupling;
      delete _leftRotateBy_action;
      delete _leftRotateBy_cx;
      delete _leftRotateBy_cy;
      delete _leftRotateBy_da;
      delete _leftRotateBy_spike;
    }

    if (_rightSkew_X_By_action) {
      if (_parent && _parent->state_dependency () != nullptr)
        Graph::instance ().remove_edge (_parent->state_dependency (), _rightSkew_X_By_action);
      Graph::instance().remove_edge(_rightSkew_X_By_cx, _rightSkew_X_By_action);
      Graph::instance().remove_edge(_rightSkew_X_By_cy, _rightSkew_X_By_action);
      Graph::instance().remove_edge(_rightSkew_X_By_da, _rightSkew_X_By_action);
      Graph::instance().remove_edge(_rightSkew_X_By_action, this->m11 ());
      Graph::instance().remove_edge(_rightSkew_X_By_action, this->m12 ());
      Graph::instance().remove_edge(_rightSkew_X_By_action, this->m14 ());
      Graph::instance().remove_edge(_rightSkew_X_By_action, this->m21 ());
      Graph::instance().remove_edge(_rightSkew_X_By_action, this->m22 ());
      Graph::instance().remove_edge(_rightSkew_X_By_action, this->m24 ());
      delete _rightSkew_X_By_cx_coupling;
      delete _rightSkew_X_By_cy_coupling;
      delete _rightSkew_X_By_da_coupling;
      delete _rightSkew_X_By_action;
      delete _rightSkew_X_By_cx;
      delete _rightSkew_X_By_cy;
      delete _rightSkew_X_By_da;
      delete _rightSkew_X_By_spike;
    }

    if (_leftSkew_X_By_action) {
      if (_parent && _parent->state_dependency () != nullptr)
        Graph::instance ().remove_edge (_parent->state_dependency (), _leftSkew_X_By_action);
      Graph::instance().remove_edge(_leftSkew_X_By_cx, _leftSkew_X_By_action);
      Graph::instance().remove_edge(_leftSkew_X_By_cy, _leftSkew_X_By_action);
      Graph::instance().remove_edge(_leftSkew_X_By_da, _leftSkew_X_By_action);
      Graph::instance().remove_edge(_leftSkew_X_By_action, this->m11 ());
      Graph::instance().remove_edge(_leftSkew_X_By_action, this->m12 ());
      Graph::instance().remove_edge(_leftSkew_X_By_action, this->m14 ());
      Graph::instance().remove_edge(_leftSkew_X_By_action, this->m21 ());
      Graph::instance().remove_edge(_leftSkew_X_By_action, this->m22 ());
      Graph::instance().remove_edge(_leftSkew_X_By_action, this->m24 ());
      delete _leftSkew_X_By_cx_coupling;
      delete _leftSkew_X_By_cy_coupling;
      delete _leftSkew_X_By_da_coupling;
      delete _leftSkew_X_By_action;
      delete _leftSkew_X_By_cx;
      delete _leftSkew_X_By_cy;
      delete _leftSkew_X_By_da;
      delete _leftSkew_X_By_spike;
    }

    if (_rightSkew_Y_By_action) {
      if (_parent && _parent->state_dependency () != nullptr)
        Graph::instance ().remove_edge (_parent->state_dependency (), _rightSkew_Y_By_action);
      Graph::instance().remove_edge(_rightSkew_Y_By_cx, _rightSkew_Y_By_action);
      Graph::instance().remove_edge(_rightSkew_Y_By_cy, _rightSkew_Y_By_action);
      Graph::instance().remove_edge(_rightSkew_Y_By_da, _rightSkew_Y_By_action);
      Graph::instance().remove_edge(_rightSkew_Y_By_action, this->m11 ());
      Graph::instance().remove_edge(_rightSkew_Y_By_action, this->m12 ());
      Graph::instance().remove_edge(_rightSkew_Y_By_action, this->m14 ());
      Graph::instance().remove_edge(_rightSkew_Y_By_action, this->m21 ());
      Graph::instance().remove_edge(_rightSkew_Y_By_action, this->m22 ());
      Graph::instance().remove_edge(_rightSkew_Y_By_action, this->m24 ());
      delete _rightSkew_Y_By_cx_coupling;
      delete _rightSkew_Y_By_cy_coupling;
      delete _rightSkew_Y_By_da_coupling;
      delete _rightSkew_Y_By_action;
      delete _rightSkew_Y_By_cx;
      delete _rightSkew_Y_By_cy;
      delete _rightSkew_Y_By_da;
      delete _rightSkew_Y_By_spike;
    }

    if (_leftSkew_Y_By_action) {
      if (_parent && _parent->state_dependency () != nullptr)
        Graph::instance ().remove_edge (_parent->state_dependency (), _leftSkew_Y_By_action);
      Graph::instance().remove_edge(_leftSkew_Y_By_cx, _leftSkew_Y_By_action);
      Graph::instance().remove_edge(_leftSkew_Y_By_cy, _leftSkew_Y_By_action);
      Graph::instance().remove_edge(_leftSkew_Y_By_da, _leftSkew_Y_By_action);
      Graph::instance().remove_edge(_leftSkew_Y_By_action, this->m11 ());
      Graph::instance().remove_edge(_leftSkew_Y_By_action, this->m12 ());
      Graph::instance().remove_edge(_leftSkew_Y_By_action, this->m14 ());
      Graph::instance().remove_edge(_leftSkew_Y_By_action, this->m21 ());
      Graph::instance().remove_edge(_leftSkew_Y_By_action, this->m22 ());
      Graph::instance().remove_edge(_leftSkew_Y_By_action, this->m24 ());
      delete _leftSkew_Y_By_cx_coupling;
      delete _leftSkew_Y_By_cy_coupling;
      delete _leftSkew_Y_By_da_coupling;
      delete _leftSkew_Y_By_action;
      delete _leftSkew_Y_By_cx;
      delete _leftSkew_Y_By_cy;
      delete _leftSkew_Y_By_da;
      delete _leftSkew_Y_By_spike;
    }

    if (_symtable.empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = _symtable.find ("m11");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("m12");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("m13");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("m14");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("m21");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("m22");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("m23");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("m24");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("m31");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("m32");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("m33");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("m34");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("m41");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("m42");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("m43");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("m44");
      if (it != _symtable.end ())
        delete it->second;
    }

  }

  Process*
  AbstractHomography::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="m11") {
      coupling=&_cm11;
      rawp=&raw_props.m11;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="m12") {
      coupling=&_cm12;
      rawp=&raw_props.m12;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="m13") {
      coupling=&_cm13;
      rawp=&raw_props.m13;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="m14") {
      coupling=&_cm14;
      rawp=&raw_props.m14;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="m21") {
      coupling=&_cm21;
      rawp=&raw_props.m21;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="m22") {
      coupling=&_cm22;
      rawp=&raw_props.m22;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="m23") {
      coupling=&_cm23;
      rawp=&raw_props.m23;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="m24") {
      coupling=&_cm24;
      rawp=&raw_props.m24;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="m31") {
      coupling=&_cm31;
      rawp=&raw_props.m31;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="m32") {
      coupling=&_cm32;
      rawp=&raw_props.m32;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="m33") {
      coupling=&_cm33;
      rawp=&raw_props.m33;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="m33") {
      coupling=&_cm34;
      rawp=&raw_props.m34;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="m41") {
      coupling=&_cm41;
      rawp=&raw_props.m41;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="m42") {
      coupling=&_cm42;
      rawp=&raw_props.m42;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="m43") {
      coupling=&_cm43;
      rawp=&raw_props.m43;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="m44") {
      coupling=&_cm44;
      rawp=&raw_props.m44;
      notify_mask = notify_damaged_transform;
    } else
    if (name.find ("rightTranslateBy") != std::string::npos) {
      init_rightTranslateBy();
      return AbstractGObj::find_component(name);
    } else
    if (name.find ("leftTranslateBy") != std::string::npos) {
      init_leftTranslateBy();
      return AbstractGObj::find_component(name);
    } else
    if (name.find ("rightScaleBy") != std::string::npos) {
      init_rightScaleBy();
      return AbstractGObj::find_component(name);
    } else
    if (name.find ("leftScaleBy") != std::string::npos) {
      init_leftScaleBy();
      return AbstractGObj::find_component(name);
    } else
    if (name.find ("rightRotateBy") != std::string::npos) {
      init_rightRotateBy();
      return AbstractGObj::find_component(name);
    } else
    if (name.find ("leftRotateBy") != std::string::npos) {
      init_leftRotateBy();
      return AbstractGObj::find_component(name);
    } else
    if (name.find ("rightSkewXBy") != std::string::npos) {
      init_rightSkewXBy();
      return AbstractGObj::find_component(name);
    } else
    if (name.find ("leftSkewXBy") != std::string::npos) {
      init_leftSkewXBy();
      return AbstractGObj::find_component(name);
    } else
    if (name.find ("rightSkewYBy") != std::string::npos) {
      init_rightSkewYBy();
      return AbstractGObj::find_component(name);
    } else
    if (name.find ("leftSkewYBy") != std::string::npos) {
      init_leftSkewYBy();
      return AbstractGObj::find_component(name);
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr; // do not cache
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  AbstractHomography::get_properties_values (double &m11, double &m12, double &m13, double &m14, 
                                           double &m21, double &m22, double &m23, double &m24,
                                           double &m31, double &m32, double &m33, double &m34,
                                           double &m41, double &m42, double &m43, double &m44)
  {
    m11 = raw_props.m11;
    m12 = raw_props.m12;
    m13 = raw_props.m13;
    m14 = raw_props.m14;

    m21 = raw_props.m21;
    m22 = raw_props.m22;
    m23 = raw_props.m23;
    m24 = raw_props.m24;

    m31 = raw_props.m31;
    m32 = raw_props.m32;
    m33 = raw_props.m33;
    m34 = raw_props.m34;

    m41 = raw_props.m41;
    m42 = raw_props.m42;
    m43 = raw_props.m43;
    m44 = raw_props.m44;
  }

  void
  AbstractHomography::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    if (_cm11) _cm11->enable (_frame);
    if (_cm12) _cm12->enable (_frame);
    if (_cm13) _cm13->enable (_frame);
    if (_cm14) _cm14->enable (_frame);

    if (_cm21) _cm21->enable (_frame);
    if (_cm22) _cm22->enable (_frame);
    if (_cm23) _cm23->enable (_frame);
    if (_cm24) _cm24->enable (_frame);

    if (_cm31) _cm31->enable (_frame);
    if (_cm32) _cm32->enable (_frame);
    if (_cm33) _cm33->enable (_frame);
    if (_cm34) _cm34->enable (_frame);

    if (_cm41) _cm41->enable (_frame);
    if (_cm42) _cm42->enable (_frame);
    if (_cm43) _cm43->enable (_frame);
    if (_cm44) _cm44->enable (_frame);

    if (_rightTranslateBy_action) {
      _rightTranslateBy_dx_coupling->enable (_frame);
      _rightTranslateBy_dy_coupling->enable (_frame);
    }
    if (_leftTranslateBy_action) {
      _leftTranslateBy_dx_coupling->enable (_frame);
      _leftTranslateBy_dy_coupling->enable (_frame);
    }
    if (_rightScaleBy_action) {
      _rightScaleBy_cx_coupling->enable (_frame);
      _rightScaleBy_cy_coupling->enable (_frame);
      _rightScaleBy_sx_coupling->enable (_frame);
      _rightScaleBy_sy_coupling->enable (_frame);
    }
    if (_leftScaleBy_action) {
      _leftScaleBy_cx_coupling->enable (_frame);
      _leftScaleBy_cy_coupling->enable (_frame);
      _leftScaleBy_sx_coupling->enable (_frame);
      _leftScaleBy_sy_coupling->enable (_frame);
    }
    if (_rightRotateBy_action) {
      _rightRotateBy_cx_coupling->enable (_frame);
      _rightRotateBy_cy_coupling->enable (_frame);
      _rightRotateBy_da_coupling->enable (_frame);
    }
    if (_leftRotateBy_action) {
      _leftRotateBy_cx_coupling->enable (_frame);
      _leftRotateBy_cy_coupling->enable (_frame);
      _leftRotateBy_da_coupling->enable (_frame);
    }
    if (_rightSkew_X_By_action) {
      _rightSkew_X_By_cx_coupling->enable (_frame);
      _rightSkew_X_By_cy_coupling->enable (_frame);
      _rightSkew_X_By_da_coupling->enable (_frame);
    }
    if (_leftSkew_X_By_action) {
      _leftSkew_X_By_cx_coupling->enable (_frame);
      _leftSkew_X_By_cy_coupling->enable (_frame);
      _leftSkew_X_By_da_coupling->enable (_frame);
    }
    if (_rightSkew_Y_By_action) {
      _rightSkew_Y_By_cx_coupling->enable (_frame);
      _rightSkew_Y_By_cy_coupling->enable (_frame);
      _rightSkew_Y_By_da_coupling->enable (_frame);
    }    
    if (_leftSkew_Y_By_action) {
      _leftSkew_Y_By_cx_coupling->enable (_frame);
      _leftSkew_Y_By_cy_coupling->enable (_frame);
      _leftSkew_Y_By_da_coupling->enable (_frame);
    }
  }

  void
  AbstractHomography::impl_deactivate ()
  {
    AbstractGObj::impl_deactivate ();
    if (_cm11) _cm11->disable ();
    if (_cm12) _cm12->disable ();
    if (_cm13) _cm13->disable ();
    if (_cm14) _cm14->disable ();

    if (_cm21) _cm21->disable ();
    if (_cm22) _cm22->disable ();
    if (_cm23) _cm23->disable ();
    if (_cm24) _cm24->disable ();

    if (_cm31) _cm31->disable ();
    if (_cm32) _cm32->disable ();
    if (_cm33) _cm33->disable ();
    if (_cm34) _cm34->disable ();

    if (_cm41) _cm41->disable ();
    if (_cm42) _cm42->disable ();
    if (_cm43) _cm43->disable ();
    if (_cm44) _cm44->disable ();

    if (_rightTranslateBy_action) {
      _rightTranslateBy_dx_coupling->disable ();
      _rightTranslateBy_dy_coupling->disable ();
    }
    if (_leftTranslateBy_action) {
      _leftTranslateBy_dx_coupling->disable ();
      _leftTranslateBy_dy_coupling->disable ();
    }
    if (_rightScaleBy_action) {
      _rightScaleBy_cx_coupling->disable ();
      _rightScaleBy_cy_coupling->disable ();
      _rightScaleBy_sx_coupling->disable ();
      _rightScaleBy_sy_coupling->disable ();
    }
    if (_leftScaleBy_action) {
      _leftScaleBy_cx_coupling->disable ();
      _leftScaleBy_cy_coupling->disable ();
      _leftScaleBy_sx_coupling->disable ();
      _leftScaleBy_sy_coupling->disable ();
    }
    if (_rightRotateBy_action) {
      _rightRotateBy_cx_coupling->disable ();
      _rightRotateBy_cy_coupling->disable ();
      _rightRotateBy_da_coupling->disable ();
    }
    if (_leftRotateBy_action) {
      _leftRotateBy_cx_coupling->disable ();
      _leftRotateBy_cy_coupling->disable ();
      _leftRotateBy_da_coupling->disable ();
    }
    if (_rightSkew_X_By_action) {
      _rightSkew_X_By_cx_coupling->disable ();
      _rightSkew_X_By_cy_coupling->disable ();
      _rightSkew_X_By_da_coupling->disable ();
    }
    if (_leftSkew_X_By_action) {
      _leftSkew_X_By_cx_coupling->disable ();
      _leftSkew_X_By_cy_coupling->disable ();
      _leftSkew_X_By_da_coupling->disable ();
    }
    if (_rightSkew_Y_By_action) {
      _rightSkew_Y_By_cx_coupling->disable ();
      _rightSkew_Y_By_cy_coupling->disable ();
      _rightSkew_Y_By_da_coupling->disable ();
    }    
    if (_leftSkew_Y_By_action) {
      _leftSkew_Y_By_cx_coupling->disable ();
      _leftSkew_Y_By_cy_coupling->disable ();
      _leftSkew_Y_By_da_coupling->disable ();
    }
  }

  Homography::Homography (Process *p, const string &n, double m11, double m12, double m13, double m14, double m21,
                          double m22, double m23, double m24, double m31, double m32, double m33, double m34,
                          double m41, double m42, double m43, double m44) :
      AbstractHomography (p, n, m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44)
  {
    Process::finalize_construction ();
  }

  Homography::Homography (double m11, double m12, double m13, double m14, double m21,
                          double m22, double m23, double m24, double m31, double m32, double m33, double m34,
                          double m41, double m42, double m43, double m44) :
      AbstractHomography (m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44)
  {

  }

  Homography::~Homography ()
  {
  }

  void
  Homography::draw ()
  {
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_homography (this);
    }
  }

  Process*
  Homography::clone ()
  {
    return new Homography (raw_props.m11, raw_props.m12, raw_props.m13, raw_props.m14,
                           raw_props.m21, raw_props.m22, raw_props.m23, raw_props.m24,
                           raw_props.m31, raw_props.m32, raw_props.m33, raw_props.m34,
                           raw_props.m41, raw_props.m42, raw_props.m43, raw_props.m44);
  }

  GradientHomography::GradientHomography (Process *p, const string &n, double m11, double m12, double m13, double m21,
                                          double m22, double m23, double m31, double m32, double m33) :
      AbstractHomography (p, n, m11, m12, m13, 0, m21, m22, m23, 0, m31, m32, m33, 0, 0, 0, 0, 1)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (p);
    if (grad == nullptr) {
      cerr << "Parent of gradient homography must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
  }

  GradientHomography::GradientHomography (double m11, double m12, double m13, double m21, double m22, double m23,
                                          double m31, double m32, double m33) :
      AbstractHomography (m11, m12, m13, 0, m21, m22, m23, 0, m31, m32, m33, 0, 0, 0, 0, 1)
  {
  }

  GradientHomography::~GradientHomography ()
  {
  }

  void
  GradientHomography::draw ()
  {
    Backend::instance ()->load_gradient_homography (this);
  }

  Process*
  GradientHomography::clone ()
  {
    return new GradientHomography ( raw_props.m11, raw_props.m12, raw_props.m13,
                                    raw_props.m21, raw_props.m22, raw_props.m23,
                                    raw_props.m31, raw_props.m32, raw_props.m33);
  }

  SimpleGradientTransform::SimpleGradientTransform (Process *p, const string &n, double a, double b, double c, double d,
                                                    double e, double f) :
      AbstractHomography (p, n, a, b, 0, 0, c, d, 0, 0, e, f, 1, 0, 0, 0, 0, 1)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (p);
    if (grad == nullptr) {
      cerr << "Parent of simple gradient transform must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
  }

  SimpleGradientTransform::SimpleGradientTransform (double a, double b, double c, double d, double e, double f) :
      AbstractHomography (a, b, 0, 0, c, d, 0, 0, e, f, 1, 0, 0, 0, 0, 1)
  {
  }

  SimpleGradientTransform::~SimpleGradientTransform ()
  {
  }

  void
  SimpleGradientTransform::draw ()
  {
    // double m11 = new_m11->get_value ();
    // double m12 = new_m12->get_value ();
    // //double m13 = new_m13->get_value ();

    // double m21 = new_m21->get_value ();
    // double m22 = new_m22->get_value ();
    // //double m23 = new_m23->get_value ();

    // double m31 = _m31->get_value ();
    // double m32 = _m32->get_value ();
    // //double m33 = _m33->get_value ();

    //Backend::instance ()->load_gradient_homography (this, m11, m12, m13, m21, m22, m23, m31, m32, m33);
    //a, b, 0, c, d, 0, e, f, 1)
    Backend::instance ()->load_simple_gradient_transform (this);
  }

  Process*
  SimpleGradientTransform::clone ()
  {
    return new GradientHomography (raw_props.m11, raw_props.m12, raw_props.m13,
                                   raw_props.m21, raw_props.m22, raw_props.m23,
                                   raw_props.m31, raw_props.m32, raw_props.m33);
  }
}

