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

#include "gui/transformation/transformations.h"
#include "core/tree/spike.h"

#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display.h"
#include "display/abstract_display.h"
#include "display/window.h"

#include "core/execution/graph.h"
#include "gui/picking/analytical_picking_context.h"


#include <array>
#include <cmath>

#include <iostream>

namespace djnn
{
  AbstractTransformation::AbstractTransformation (Process *parent, const string &name) :
      AbstractGObj (parent, name)
  {
  }

  AbstractTransformation::~AbstractTransformation ()
  {
  }


  Translation::Translation (Process *parent, const string &name, double tx, double ty) :
      AbstractTranslation (parent, name, tx, ty)
  {
    Process::finalize_construction (parent, name);
  }

  Translation::~Translation ()
  {
  }

  void
  Translation::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_translation (this);
    }
  }

  AbstractGShape*
  Translation::pick_analytical (PickAnalyticalContext& pac)
  {
    pac.x -= raw_props.tx;
    pac.y -= raw_props.ty;
    return nullptr;
  }

  Process*
  Translation::clone ()
  {
    return new Translation (nullptr, get_name (), raw_props.tx, raw_props.ty);
  }


  GradientTranslation::GradientTranslation (Process *parent, const string &name, double tx, double ty) :
      AbstractTranslation (parent, name, tx, ty)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (parent);
    if (grad == nullptr) {
      cerr << "Parent of gradient translation must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
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
    return new GradientTranslation (nullptr, get_name (), raw_props.tx, raw_props.ty);
  }


  Rotation::Rotation (Process *parent, const string &name, double a, double cx, double cy) :
      AbstractRotation (parent, name, a, cx, cy)
  {
    Process::finalize_construction (parent, name);
  }

  Rotation::~Rotation ()
  {
  }

  void
  Rotation::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_rotation (this);
    }
  }

  AbstractGShape*
  Rotation::pick_analytical (PickAnalyticalContext& pac)
  {
    pac.x -= raw_props.cx;
    pac.y -= raw_props.cy;

    double cosa =  cos(- raw_props.a * M_PI / 180.),
           sina =  sin(- raw_props.a * M_PI / 180.);
    double
    pacx =   cosa * pac.x - sina * pac.y;
    pac.y =  sina * pac.x + cosa * pac.y;
    pac.x =  pacx;

    pac.x += raw_props.cx;
    pac.y += raw_props.cy;

    return nullptr;
  }

  Process*
  Rotation::clone ()
  {
    return new Rotation (nullptr, get_name (), raw_props.a, raw_props.cx, raw_props.cy);
  }


  GradientRotation::GradientRotation (Process *parent, const string &name, double a, double cx, double cy) :
      AbstractRotation (parent, name, a, cx, cy)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (parent);
    if (grad == nullptr) {
      cerr << "Parent of gradient rotation must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
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
    return new GradientRotation (nullptr, get_name (), raw_props.a, raw_props.cx, raw_props.cy);
  }


  Scaling::Scaling (Process *parent, const string &name, double sx, double sy, double cx, double cy) :
      AbstractScaling (parent, name, sx, sy, cx, cy)
  {
    Process::finalize_construction (parent, name);
  }

  Scaling::~Scaling ()
  {
  }

  void
  Scaling::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_scaling (this);
    }
  }

  AbstractGShape*
  Scaling::pick_analytical (PickAnalyticalContext& pac)
  {
    pac.x /= raw_props.sx;
    pac.y /= raw_props.sy;
    return nullptr;
  }

  Process*
  Scaling::clone ()
  {
    return new Scaling (nullptr, get_name (), raw_props.sx, raw_props.sy, raw_props.cx, raw_props.cy);
  }


  GradientScaling::GradientScaling (Process *parent, const string &name, double sx, double sy, double cx, double cy) :
      AbstractScaling (parent, name, sx, sy, cx, cy)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (parent);
    if (grad == nullptr) {
      cerr << "Parent of gradient scaling must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
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
    return new GradientScaling (nullptr, get_name (), raw_props.sx, raw_props.sy, raw_props.cx, raw_props.cy);
  }

  SkewX::SkewX (Process *parent, const string &name, double a) :
      AbstractSkew (parent, name, a)
  {
    Process::finalize_construction (parent, name);
  }

  SkewX::~SkewX ()
  {
  }

  void
  SkewX::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_skew_x (this);
    }
  }

  AbstractGShape*
  SkewX::pick_analytical (PickAnalyticalContext& pac)
  {
    double tana = tan(- raw_props.a * M_PI / 180.);
    pac.x += pac.x + tana * pac.y;

    return nullptr;
  }

  Process*
  SkewX::clone ()
  {
    return new SkewX (nullptr, get_name (), raw_props.a);
  }


  GradientSkewX::GradientSkewX (Process *parent, const string &name, double a) :
      AbstractSkew (parent, name, a)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (parent);
    if (grad == nullptr) {
      cerr << "Parent of gradient skewX must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
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
    return new GradientSkewX (nullptr, get_name (), raw_props.a);
  }


  SkewY::SkewY (Process *parent, const string &name, double a) :
      AbstractSkew (parent, name, a)
  {
    Process::finalize_construction (parent, name);
  }

  SkewY::~SkewY ()
  {
  }

  void
  SkewY::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_skew_y (this);
    }
  }

  AbstractGShape*
  SkewY::pick_analytical (PickAnalyticalContext& pac)
  {
    double tana = tan(- raw_props.a * M_PI / 180.);
    pac.y += tana * pac.x;

    return nullptr;
  }

  Process*
  SkewY::clone ()
  {
    return new SkewY (nullptr, get_name (), raw_props.a);
  }

  GradientSkewY::GradientSkewY (Process *parent, const string &name, double a) :
      AbstractSkew (parent, name, a)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (parent);
    if (grad == nullptr) {
      cerr << "Parent of gradient skewY must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
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
    return new GradientSkewY (nullptr, get_name (), raw_props.a);
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
    m14 ()->set_value (m14 ()->get_value (), true);
    m24 ()->set_value (m24 ()->get_value (), true);
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
    m14 ()->set_value (m14 ()->get_value (), true);
    m24 ()->set_value (m24 ()->get_value (), true);
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

    _accsx->set_value (_accsx->get_value () * sx, true);
    _accsy->set_value (_accsy->get_value () * sy, true);
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

    _accsx->set_value (_accsx->get_value () * sx, true);
    _accsy->set_value (_accsy->get_value () * sy, true);
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

    _accsx->set_value (_accsx->get_value () * sx, true);
    _accsy->set_value (_accsy->get_value () * sy, true);
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

    _accsx->set_value (_accsx->get_value () * sx, true);
    _accsy->set_value (_accsy->get_value () * sy, true);
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

    _acca->set_value (_acca->get_value () + da, true);
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

    _acca->set_value (_acca->get_value () + da, true);
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

    _acca->set_value (_acca->get_value () + angle, true);
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

    _acca->set_value (_acca->get_value () + angle, true);
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

     // moved in left/rightScale
    // _h->_accsx->set_value (_h->_accsx->get_value () * sx, true);
    // _h->_accsy->set_value (_h->_accsy->get_value () * sy, true);
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

    // moved in left/rightScale
    // _h->_accsx->set_value (_h->_accsx->get_value () * sx, true);
    // _h->_accsy->set_value (_h->_accsy->get_value () * sy, true);
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

    // moved in left/rightRotate
    //_h->_acca->set_value (_h->_acca->get_value () + da, true);
    _h->_rightRotateBy_da->set_value (0, false);
  }

  void
  AbstractHomography::LeftRotateByAction::impl_activate () 
  { 
    double cx = _h->_leftRotateBy_cx->get_value ();
    double cy = _h->_leftRotateBy_cy->get_value ();
    double da = _h->_leftRotateBy_da->get_value ();

    _h->leftRotate (da, cx, cy);

    // moved in left/rightRotate
    //_h->_acca->set_value (_h->_acca->get_value () + da, true);
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
    auto _frame = frame ();
    _rightTranslateBy_spike = new Spike (this, "rightTranslateBy");
    _rightTranslateBy_dx = new DoubleProperty (nullptr, "dx", 0);
    _rightTranslateBy_dy = new DoubleProperty (nullptr, "dy", 0);
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
    add_state_dependency (get_parent (), _rightTranslateBy_action);
  }

  void
  AbstractHomography::init_leftTranslateBy ()
  {
    auto _frame = frame ();
    _leftTranslateBy_spike = new Spike (this, "leftTranslateBy");
    _leftTranslateBy_dx = new DoubleProperty (nullptr, "dx", 0);
    _leftTranslateBy_dy = new DoubleProperty (nullptr, "dy", 0);
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
    add_state_dependency (get_parent (), _leftTranslateBy_action);
  }

  void
  AbstractHomography::init_rightScaleBy () 
  {
    auto _frame = frame ();
    _rightScaleBy_spike = new Spike (this, "rightScaleBy");
    _rightScaleBy_cx = new DoubleProperty (nullptr, "cx", 0);
    _rightScaleBy_cy = new DoubleProperty (nullptr, "cy", 0);
    _rightScaleBy_sx = new DoubleProperty (nullptr, "sx", 1);
    _rightScaleBy_sy = new DoubleProperty (nullptr, "sy", 1);
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
    Graph::instance().add_edge(_rightScaleBy_action, _accsx);
    Graph::instance().add_edge(_rightScaleBy_action, _accsy);
    Graph::instance().add_edge(_rightScaleBy_action, m11 ());
    Graph::instance().add_edge(_rightScaleBy_action, m12 ());
    Graph::instance().add_edge(_rightScaleBy_action, m14 ());
    Graph::instance().add_edge(_rightScaleBy_action, m21 ());
    Graph::instance().add_edge(_rightScaleBy_action, m22 ());
    Graph::instance().add_edge(_rightScaleBy_action, m24 ());
    add_state_dependency (get_parent (), _rightScaleBy_action);
  }

  void
  AbstractHomography::init_leftScaleBy () 
  {
    auto _frame = frame ();
    _leftScaleBy_spike = new Spike (this, "leftScaleBy");
    _leftScaleBy_cx = new DoubleProperty (nullptr, "cx", 0);
    _leftScaleBy_cy = new DoubleProperty (nullptr, "cy", 0);
    _leftScaleBy_sx = new DoubleProperty (nullptr, "sx", 1);
    _leftScaleBy_sy = new DoubleProperty (nullptr, "sy", 1);;
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
    Graph::instance().add_edge(_leftScaleBy_action, _accsx);
    Graph::instance().add_edge(_leftScaleBy_action, _accsy);
    Graph::instance().add_edge(_leftScaleBy_action, m11 ());
    Graph::instance().add_edge(_leftScaleBy_action, m12 ());
    Graph::instance().add_edge(_leftScaleBy_action, m14 ());
    Graph::instance().add_edge(_leftScaleBy_action, m21 ());
    Graph::instance().add_edge(_leftScaleBy_action, m22 ());
    Graph::instance().add_edge(_leftScaleBy_action, m24 ());
    add_state_dependency (get_parent (), _leftScaleBy_action);
  }

  void
  AbstractHomography::init_rightRotateBy () 
  {
    auto _frame = frame ();
    _rightRotateBy_spike = new Spike (this, "rightRotateBy");
    _rightRotateBy_cx = new DoubleProperty (nullptr, "cx", 0);
    _rightRotateBy_cy = new DoubleProperty (nullptr, "cy", 0);
    _rightRotateBy_da = new DoubleProperty (nullptr, "da", 0);
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
    Graph::instance().add_edge(_rightRotateBy_action, _acca);
    Graph::instance().add_edge(_rightRotateBy_action, m11 ());
    Graph::instance().add_edge(_rightRotateBy_action, m12 ());
    Graph::instance().add_edge(_rightRotateBy_action, m14 ());
    Graph::instance().add_edge(_rightRotateBy_action, m21 ());
    Graph::instance().add_edge(_rightRotateBy_action, m22 ());
    Graph::instance().add_edge(_rightRotateBy_action, m24 ());
    add_state_dependency (get_parent (), _rightRotateBy_action);
  }

  void
  AbstractHomography::init_leftRotateBy () 
  {
    auto _frame = frame ();
    _leftRotateBy_spike = new Spike (this, "leftRotateBy");
    _leftRotateBy_cx = new DoubleProperty (nullptr, "cx", 0);
    _leftRotateBy_cy = new DoubleProperty (nullptr, "cy", 0);
    _leftRotateBy_da = new DoubleProperty (nullptr, "da", 0);
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
    Graph::instance().add_edge(_leftRotateBy_action, _acca);
    Graph::instance().add_edge(_leftRotateBy_action, m11 ());
    Graph::instance().add_edge(_leftRotateBy_action, m12 ());
    Graph::instance().add_edge(_leftRotateBy_action, m14 ());
    Graph::instance().add_edge(_leftRotateBy_action, m21 ());
    Graph::instance().add_edge(_leftRotateBy_action, m22 ());
    Graph::instance().add_edge(_leftRotateBy_action, m24 ());
    add_state_dependency (get_parent (), _leftRotateBy_action);
  }

  void
  AbstractHomography::init_rightSkewXBy () 
  {
    auto _frame = frame ();
    _rightSkew_X_By_spike = new Spike (this, "rightSkewXBy");
    _rightSkew_X_By_cx = new DoubleProperty (nullptr, "cx", 0);
    _rightSkew_X_By_cy = new DoubleProperty (nullptr, "cy", 0);
    _rightSkew_X_By_da = new DoubleProperty (nullptr, "da", 0);
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
    add_state_dependency (get_parent (), _rightSkew_X_By_action);
  }

  void
  AbstractHomography::init_leftSkewXBy () 
  {
    auto _frame = frame ();
    _leftSkew_X_By_spike = new Spike (this, "leftSkewXBy");
    _leftSkew_X_By_cx = new DoubleProperty (nullptr, "cx", 0);
    _leftSkew_X_By_cy = new DoubleProperty (nullptr, "cy", 0);
    _leftSkew_X_By_da = new DoubleProperty (nullptr, "da", 0);
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
    add_state_dependency (get_parent (), _leftSkew_X_By_action);
  }

  void
  AbstractHomography::init_rightSkewYBy () 
  {
    auto _frame = frame ();
    _rightSkew_Y_By_spike = new Spike (this, "rightSkewYBy");
    _rightSkew_Y_By_cx = new DoubleProperty (nullptr, "cx", 0);
    _rightSkew_Y_By_cy = new DoubleProperty (nullptr, "cy", 0);
    _rightSkew_Y_By_da = new DoubleProperty (nullptr, "da", 0);
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
    add_state_dependency (get_parent (), _rightSkew_Y_By_action);
  }

  void
  AbstractHomography::init_leftSkewYBy () 
  {
    auto _frame = frame ();
    _leftSkew_Y_By_spike = new Spike (this, "leftSkewYBy");
    _leftSkew_Y_By_cx = new DoubleProperty (nullptr, "cx", 0);
    _leftSkew_Y_By_cy = new DoubleProperty (nullptr, "cy", 0);
    _leftSkew_Y_By_da = new DoubleProperty (nullptr, "da", 0);
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
    add_state_dependency (get_parent (), _leftSkew_Y_By_action);
  }

  AbstractHomography::AbstractHomography (Process *parent, const string &name, 
                                          double m11, double m12, double m13, double m14,
                                          double m21, double m22, double m23, double m24, 
                                          double m31, double m32, double m33, double m34, 
                                          double m41, double m42, double m43, double m44) :
      AbstractPropHomography (parent, name,
                                          m11, m12, m13, m14,
                                          m21, m22, m23, m24, 
                                          m31, m32, m33, m34, 
                                          m41, m42, m43, m44),
      raw_props(AbstractPropHomography::raw_props),

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

  AbstractHomography::~AbstractHomography ()
  {
    if (_rightTranslateBy_action){
      remove_state_dependency (get_parent (), _rightTranslateBy_action);
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
      remove_state_dependency (get_parent (), _leftTranslateBy_action);
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
      remove_state_dependency (get_parent (), _rightScaleBy_action);
      Graph::instance().remove_edge(_rightScaleBy_cx, _rightScaleBy_action);
      Graph::instance().remove_edge(_rightScaleBy_cy, _rightScaleBy_action);
      Graph::instance().remove_edge(_rightScaleBy_sx, _rightScaleBy_action);
      Graph::instance().remove_edge(_rightScaleBy_sy, _rightScaleBy_action);
      Graph::instance().remove_edge(_rightScaleBy_action, _accsx);
      Graph::instance().remove_edge(_rightScaleBy_action, _accsy);
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
      remove_state_dependency (get_parent (), _leftScaleBy_action);
      Graph::instance().remove_edge(_leftScaleBy_cx, _leftScaleBy_action);
      Graph::instance().remove_edge(_leftScaleBy_cy, _leftScaleBy_action);
      Graph::instance().remove_edge(_leftScaleBy_sx, _leftScaleBy_action);
      Graph::instance().remove_edge(_leftScaleBy_sy, _leftScaleBy_action);
      Graph::instance().remove_edge(_leftScaleBy_action, _accsx);
      Graph::instance().remove_edge(_leftScaleBy_action, _accsy);
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
      remove_state_dependency (get_parent (), _rightRotateBy_action);
      Graph::instance().remove_edge(_rightRotateBy_cx, _rightRotateBy_action);
      Graph::instance().remove_edge(_rightRotateBy_cy, _rightRotateBy_action);
      Graph::instance().remove_edge(_rightRotateBy_da, _rightRotateBy_action);
      Graph::instance().remove_edge(_rightRotateBy_action, _acca);
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
      remove_state_dependency (get_parent (), _leftRotateBy_action);
      Graph::instance().remove_edge(_leftRotateBy_cx, _leftRotateBy_action);
      Graph::instance().remove_edge(_leftRotateBy_cy, _leftRotateBy_action);
      Graph::instance().remove_edge(_leftRotateBy_da, _leftRotateBy_action);
      Graph::instance().remove_edge(_leftRotateBy_action, _acca);
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
      remove_state_dependency (get_parent (), _rightSkew_X_By_action);
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
      remove_state_dependency (get_parent (), _leftSkew_X_By_action);
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
      remove_state_dependency (get_parent (), _rightSkew_Y_By_action);
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
      remove_state_dependency (get_parent (), _leftSkew_Y_By_action);
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

    delete _acca;
    delete _accsx;
    delete _accsy;
  }

  Process*
  AbstractHomography::find_component (const string& name)
  {
    Process* res = AbstractPropHomography::find_component(name);
    if(res) return res;

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
    
    return res;
  }

  void
  AbstractHomography::impl_activate ()
  {
    AbstractPropHomography::impl_activate ();
    
    auto _frame = frame ();

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
    AbstractPropHomography::impl_deactivate ();
  }

  Homography::Homography (Process *parent, const string &name, double m11, double m12, double m13, double m14, double m21,
                          double m22, double m23, double m24, double m31, double m32, double m33, double m34,
                          double m41, double m42, double m43, double m44) :
      AbstractHomography (parent, name, m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44)
  {
    Process::finalize_construction (parent, name);
  }

  Homography::~Homography ()
  {
  }

  void
  Homography::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_homography (this);
    }
  }

  AbstractGShape*
  Homography::pick_analytical (PickAnalyticalContext& pac)
  {
    double
      a = raw_props.m11, b = raw_props.m12,
      c = raw_props.m21, d = raw_props.m22;
    double
    pacx = d * pac.x - b * pac.y;
    pac.y = -c * pac.x + a * pac.y;
    pac.x = pacx;

    double det = 1/(a*d-b*c);
    pac.x *= det;
    pac.y *= det;

    return nullptr;
  }

  Process*
  Homography::clone ()
  {
    return new Homography (nullptr, get_name (),
                           raw_props.m11, raw_props.m12, raw_props.m13, raw_props.m14,
                           raw_props.m21, raw_props.m22, raw_props.m23, raw_props.m24,
                           raw_props.m31, raw_props.m32, raw_props.m33, raw_props.m34,
                           raw_props.m41, raw_props.m42, raw_props.m43, raw_props.m44);
  }

  GradientHomography::GradientHomography (Process *parent, const string &name, double m11, double m12, double m13, double m21,
                                          double m22, double m23, double m31, double m32, double m33) :
      AbstractHomography (parent, name, m11, m12, m13, 0, m21, m22, m23, 0, m31, m32, m33, 0, 0, 0, 0, 1)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (parent);
    if (grad == nullptr) {
      cerr << "Parent of gradient homography must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
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
    return new GradientHomography ( nullptr, get_name (), 
                                    raw_props.m11, raw_props.m12, raw_props.m13,
                                    raw_props.m21, raw_props.m22, raw_props.m23,
                                    raw_props.m31, raw_props.m32, raw_props.m33);
  }

  SimpleGradientTransform::SimpleGradientTransform (Process *parent, const string &name, double a, double b, double c, double d,
                                                    double e, double f) :
      AbstractHomography (parent, name, a, b, 0, 0, c, d, 0, 0, e, f, 1, 0, 0, 0, 0, 1)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (parent);
    if (grad == nullptr) {
      cerr << "Parent of simple gradient transform must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->transforms ()->add_child (this, "");
  }

  SimpleGradientTransform::~SimpleGradientTransform ()
  {
  }

  void
  SimpleGradientTransform::draw ()
  {
    Backend::instance ()->load_simple_gradient_transform (this);
  }

  Process*
  SimpleGradientTransform::clone ()
  {
    return new GradientHomography (nullptr, get_name (), 
                                   raw_props.m11, raw_props.m12, raw_props.m13,
                                   raw_props.m21, raw_props.m22, raw_props.m23,
                                   raw_props.m31, raw_props.m32, raw_props.m33);
  }
}

