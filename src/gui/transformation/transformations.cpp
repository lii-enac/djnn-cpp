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

#include "../transformation/transformations.h"
#include "../../core/tree/spike.h"

#include "../backend.h"
#include "../abstract_backend.h"
#include "../../display/display.h"
#include "../../display/abstract_display.h"
#include "../../display/window.h"

#include <array>
#include <cmath>

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
  AbstractTranslation::activate ()
  {
    AbstractGObj::activate ();
    if(_ctx) _ctx->enable (_frame);
    if(_cty) _cty->enable (_frame);
  }

  void
  AbstractTranslation::deactivate ()
  {
    AbstractGObj::deactivate ();
    if(_ctx)_ctx->disable ();
    if(_cty) _cty->disable ();
  }

  Translation::Translation (Process *p, const string &n, double tx, double ty) :
      AbstractTranslation (p, n, tx, ty)
  {
    Process::finalize ();
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
  AbstractRotation::activate ()
  {
    AbstractGObj::activate ();
    if(_ca) _ca->enable (_frame);
    if(_ccx) _ccx->enable (_frame);
    if(_ccy) _ccy->enable (_frame);
  }

  void
  AbstractRotation::deactivate ()
  {
    AbstractGObj::deactivate ();
    if(_ca) _ca->disable ();
    if(_ccx) _ccx->disable ();
    if(_ccy) _ccy->disable ();
  }

  Rotation::Rotation (Process *p, const string &n, double a, double cx, double cy) :
      AbstractRotation (p, n, a, cx, cy)
  {
    Process::finalize ();
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
  AbstractScaling::activate ()
  {
    AbstractGObj::activate ();
    if(_csx) _csx->enable (_frame);
    if(_csy) _csy->enable (_frame);
    if(_ccx) _ccx->enable (_frame);
    if(_ccy) _ccy->enable (_frame);
  }

  void
  AbstractScaling::deactivate ()
  {
    AbstractGObj::deactivate ();
    if(_csx) _csx->disable ();
    if(_csy) _csy->disable ();
    if(_ccx) _ccx->disable ();
    if(_ccy) _ccy->disable ();
  }

  Scaling::Scaling (Process *p, const string &n, double sx, double sy, double cx, double cy) :
      AbstractScaling (p, n, sx, sy, cx, cy)
  {
    Process::finalize ();
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
  AbstractSkew::activate ()
  {
    AbstractGObj::activate ();
    if (_ca) _ca->enable (_frame);
  }

  void
  AbstractSkew::deactivate ()
  {
    AbstractGObj::deactivate ();
    if (_ca) _ca->disable ();
  }

  SkewX::SkewX (Process *p, const string &n, double a) :
      AbstractSkew (p, n, a)
  {
    Process::finalize ();
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
    Process::finalize ();
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

  static void
  homography_to_array (AbstractHomography *h, array<array<double, 4>, 4> &a) {

    a[0][0] = h->raw_props.m11;
    a[0][1] = h->raw_props.m12;
    a[0][2] = h->raw_props.m13;
    a[0][3] = h->raw_props.m14;

    a[1][0] = h->raw_props.m21;
    a[1][1] = h->raw_props.m22;
    a[1][2] = h->raw_props.m23;
    a[1][3] = h->raw_props.m24;

    a[2][0] = h->raw_props.m31;
    a[2][1] = h->raw_props.m32;
    a[2][2] = h->raw_props.m33;
    a[2][3] = h->raw_props.m34;

    a[3][0] = h->raw_props.m41;
    a[3][1] = h->raw_props.m42;
    a[3][2] = h->raw_props.m43;
    a[3][3] = h->raw_props.m44;

  }

  static void
  array_to_homography (array<array<double, 4>, 4> &a, AbstractHomography* h) {
  
    /* note:
     * works only for 2D 
     * propagation is not active for 3D
     *  |  x  x  .  x  |
     *  |  x  x  .  x  |
     *  |  .  .  1  .  |
     *  |  .  .  .  1  |
     * so those which are not properties write directly into raw_props
     */
    h->m11 ()->set_value (a[0][0], true);
    h->m12 ()->set_value (a[0][1], true);
    h->raw_props.m13 = a[0][2];
    h->m14 ()->set_value (a[0][3], true);

    h->m21 ()->set_value (a[1][0], true);
    h->m22 ()->set_value (a[1][1], true);
    h->raw_props.m23 = a[1][2];
    h->m24 ()->set_value (a[1][3], true);

    h->raw_props.m31 = a[2][0];
    h->raw_props.m32 = a[2][1];
    h->raw_props.m33 = a[2][2];
    h->raw_props.m34 = a[2][3];

    h->raw_props.m41 = a[3][0];
    h->raw_props.m42 = a[3][1];
    h->raw_props.m43 = a[3][2];
    h->raw_props.m44 = a[3][3];
  }


  static double
  degrees_to_radians (double angle) {
    return angle * 3.1415927 / 180;
  }

  static array<array<double, 4>, 4>
  multiply_arrays (array<array<double, 4>, 4> &a, array<array<double, 4>, 4> &b) {
    array<array<double, 4>, 4> c;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        c[i][j] = 0;
        for (int k = 0; k < 4; k++) {
          c[i][j] += a[i][k] * b[k][j];
        }
      }
    }
    return c;
  }

  //DEBUG
  #if 0
  static void
  print_array (array<array<double, 4>, 4> &a) {
    cout << endl;
    for (int l=0; l<4; l++) {
      for (int c=0; c<4; c++) {
        cout << a[l][c] <<  " ";
      }
      cout << endl;
    }
  }
  #endif

  static array<array<double, 4>, 4>
  create_translation_array (double dx, double dy) {
    array<array<double, 4>, 4> result {{
      { 1, 0, 0, dx},
      { 0, 1, 0, dy},
      { 0, 0, 1, 0 },
      { 0, 0, 0, 1 }}};
    return result;
  }

  static array<array<double, 4>, 4>
  create_scaling_array (double sx, double sy) {
    array<array<double, 4>, 4> result {{
    { sx,   0,  0,  0 },
    {  0,  sy,  0,  0 },
    {  0,   0,  1,  0 },
    {  0,   0,  0,  1 }}};
    return result;
  }

  static array<array<double, 4>, 4>
  create_skew_x_array (double angle_in_degrees) {
    // clockwise rotation
    double angle_in_radians = degrees_to_radians (angle_in_degrees);
    double tan = std::tan (angle_in_radians);
    array<array<double, 4>, 4> result {{
    { 1, -tan, 0,  0 },
    { 0,  1,  0,  0 },
    { 0,  0,  1,  0 },
    { 0,  0,  0,  1 }}};
    return result;
  }

  static array<array<double, 4>, 4>
  create_skew_y_array (double angle_in_degrees) {
    // clockwise rotation
    double angle_in_radians = degrees_to_radians (angle_in_degrees);
    double tan = std::tan (angle_in_radians);
    array<array<double, 4>, 4> result {{
    {  1,  0,  0,  0 },
    { tan, 1,  0,  0 },
    {  0,  0,  1,  0 },
    {  0,  0,  0,  1 }}};
    return result;
  }

  static array<array<double, 4>, 4>
  create_rotation_array (double angle_in_degrees) {
    // clockwise rotation
    double angle_in_radians = degrees_to_radians (angle_in_degrees);
    double cos = std::cos (angle_in_radians);
    double sin = std::sin (angle_in_radians);
    array<array<double, 4>, 4> result {{
    { cos, -sin, 0,  0 },
    { sin, cos,  0,  0 },
    {  0,   0,   1,  0 },
    {  0,   0,   0,  1 }}};
    return result;
  }

  static array<array<double, 4>, 4>
  create_pivot_transform_array (array<array<double, 4>, 4> &transform_array, double dx, double dy) {
    array<array<double, 4>, 4> transl = create_translation_array (dx, dy);
    array<array<double, 4>, 4> buff = multiply_arrays (transl, transform_array);
    array<array<double, 4>, 4> inv_transl = create_translation_array (-dx, -dy);
    array<array<double, 4>, 4> result = multiply_arrays (buff, inv_transl);
    return result;
  }

  void
  AbstractHomography::TranslateByAction::activate () 
  { 
    array<array<double, 4>, 4> current_h;
    homography_to_array (_h, current_h);

    double dx = _h->_translateBy_dx->get_value ();
    double dy = _h->_translateBy_dy->get_value ();
    array<array<double, 4>, 4> translation_array = create_translation_array (dx, dy);
  
    array<array<double, 4>, 4> new_homography_array = multiply_arrays (translation_array, current_h);

    array_to_homography (new_homography_array, _h);
    
    _h->_translateBy_dx->set_value (0, false);
    _h->_translateBy_dy->set_value (0, false);

  }

  void
  AbstractHomography::ScaleByAction::activate () 
  { 
    array<array<double, 4>, 4> current_h;
    homography_to_array (_h, current_h);

    double cx = _h->_scaleBy_cx->get_value ();
    double cy = _h->_scaleBy_cy->get_value ();
    double sx = _h->_scaleBy_sx->get_value ();
    double sy = _h->_scaleBy_sy->get_value ();

    array<array<double, 4>, 4> scaling_array = create_scaling_array (sx, sy);

    array<array<double, 4>, 4> pivot_scaling_array = create_pivot_transform_array (scaling_array, cx, cy);
    array<array<double, 4>, 4> new_homography_array = multiply_arrays (pivot_scaling_array, current_h);

    array_to_homography (new_homography_array, _h);

    _h->_scaleBy_sx->set_value (1, false);
    _h->_scaleBy_sy->set_value (1, false);
  }

  void
  AbstractHomography::RotateByAction::activate () 
  { 
    array<array<double, 4>, 4> current_h;
    homography_to_array (_h, current_h);

    double cx = _h->_rotateBy_cx->get_value ();
    double cy = _h->_rotateBy_cy->get_value ();
    double da = _h->_rotateBy_da->get_value ();

    array<array<double, 4>, 4> rotation_array = create_rotation_array (da);

    array<array<double, 4>, 4> pivot_rotation_array = create_pivot_transform_array (rotation_array, cx, cy);
    array<array<double, 4>, 4> new_homography_array = multiply_arrays (pivot_rotation_array, current_h);

    array_to_homography (new_homography_array, _h);

    _h->_rotateBy_da->set_value (0, false);
  }

  void
  AbstractHomography::Skew_X_ByAction::activate () 
  { 
    array<array<double, 4>, 4> current_h;
    homography_to_array (_h, current_h);

    double cx = _h->_skew_X_By_cx->get_value ();
    double cy = _h->_skew_X_By_cy->get_value ();
    double da = _h->_skew_X_By_da->get_value ();

    array<array<double, 4>, 4> skewx_array = create_skew_x_array (da);
    array<array<double, 4>, 4> pivot_skewx_array = create_pivot_transform_array (skewx_array, cx, cy);
    array<array<double, 4>, 4> new_homography_array = multiply_arrays (pivot_skewx_array, current_h);

    array_to_homography (new_homography_array, _h);

    _h->_skew_X_By_da->set_value (0, false);
  }

  void
  AbstractHomography::Skew_Y_ByAction::activate () 
  { 
    array<array<double, 4>, 4> current_h;
    homography_to_array (_h, current_h);

    double cx = _h->_skew_Y_By_cx->get_value ();
    double cy = _h->_skew_Y_By_cy->get_value ();
    double da = _h->_skew_Y_By_da->get_value ();

    array<array<double, 4>, 4> skewy_array = create_skew_y_array (da);
    array<array<double, 4>, 4> pivot_skewy_array = create_pivot_transform_array (skewy_array, cx, cy);
    array<array<double, 4>, 4> new_homography_array = multiply_arrays (pivot_skewy_array, current_h);

    array_to_homography (new_homography_array, _h);

     _h->_skew_Y_By_da->set_value (0, false);
  }
 
  void
  AbstractHomography::init_translationBy () 
  {
    _translateBy_spike = new Spike (this, "translateBy");
    _translateBy_dx = new DoubleProperty (0);
    _translateBy_dy = new DoubleProperty (0);
    _translateBy_spike->add_symbol( "dx", _translateBy_dx);
    _translateBy_spike->add_symbol( "dy", _translateBy_dy);
    _translateBy_action = new TranslateByAction (this, "action_translate_by", this);
    _tranlateBy_dx_coupling = new Coupling (_translateBy_dx, ACTIVATION, _translateBy_action, ACTIVATION);
    this->somehow_activating () ? _tranlateBy_dx_coupling->enable (_frame) : _tranlateBy_dx_coupling->disable ();
    _tranlateBy_dy_coupling = new Coupling (_translateBy_dy, ACTIVATION, _translateBy_action, ACTIVATION);
    this->somehow_activating () ? _tranlateBy_dy_coupling->enable(_frame) : _tranlateBy_dy_coupling->disable ();
    Graph::instance().add_edge(_translateBy_dx, _translateBy_action);
    Graph::instance().add_edge(_translateBy_dy, _translateBy_action);
    Graph::instance().add_edge(_translateBy_action, m11 ());
    Graph::instance().add_edge(_translateBy_action, m12 ());
    Graph::instance().add_edge(_translateBy_action, m14 ());
    Graph::instance().add_edge(_translateBy_action, m21 ());
    Graph::instance().add_edge(_translateBy_action, m22 ());
    Graph::instance().add_edge(_translateBy_action, m24 ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _translateBy_action);
  }

  void
  AbstractHomography::init_scaleBy () 
  {
    _scaleBy_spike = new Spike (this, "scaleBy");
    _scaleBy_cx = new DoubleProperty (0);
    _scaleBy_cy = new DoubleProperty (0);
    _scaleBy_sx = new DoubleProperty (1);
    _scaleBy_sy = new DoubleProperty (1);
    _scaleBy_spike->add_symbol ("cx", _scaleBy_cx);
    _scaleBy_spike->add_symbol ("cy", _scaleBy_cy);
    _scaleBy_spike->add_symbol ("sx", _scaleBy_sx);
    _scaleBy_spike->add_symbol ("sy", _scaleBy_sy);
    _scaleBy_action = new ScaleByAction (this, "action_scale_by", this);
    _scaleBy_cx_coupling = new Coupling (_scaleBy_cx, ACTIVATION, _scaleBy_action, ACTIVATION);
    this->somehow_activating () ? _scaleBy_cx_coupling->enable(_frame) : _scaleBy_cx_coupling->disable ();
    _scaleBy_cy_coupling = new Coupling (_scaleBy_cy, ACTIVATION, _scaleBy_action, ACTIVATION);
    this->somehow_activating () ? _scaleBy_cy_coupling->enable(_frame) : _scaleBy_cy_coupling->disable ();
    _scaleBy_sx_coupling = new Coupling (_scaleBy_sx, ACTIVATION, _scaleBy_action, ACTIVATION);
    this->somehow_activating () ? _scaleBy_sx_coupling->enable(_frame) : _scaleBy_sx_coupling->disable ();
    _scaleBy_sy_coupling = new Coupling (_scaleBy_sy, ACTIVATION, _scaleBy_action, ACTIVATION);
    this->somehow_activating () ? _scaleBy_sy_coupling->enable(_frame) :_scaleBy_sy_coupling->disable ();
    Graph::instance().add_edge(_scaleBy_cx, _scaleBy_action);
    Graph::instance().add_edge(_scaleBy_cy, _scaleBy_action);
    Graph::instance().add_edge(_scaleBy_sx, _scaleBy_action);
    Graph::instance().add_edge(_scaleBy_sy, _scaleBy_action);
    Graph::instance().add_edge(_scaleBy_action, m11 ());
    Graph::instance().add_edge(_scaleBy_action, m12 ());
    Graph::instance().add_edge(_scaleBy_action, m14 ());
    Graph::instance().add_edge(_scaleBy_action, m21 ());
    Graph::instance().add_edge(_scaleBy_action, m22 ());
    Graph::instance().add_edge(_scaleBy_action, m24 ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _scaleBy_action);
  }

  void
  AbstractHomography::init_rotateBy () 
  {
    _rotateBy_spike = new Spike (this, "rotateBy");
    _rotateBy_cx = new DoubleProperty (0);
    _rotateBy_cy = new DoubleProperty (0);
    _rotateBy_da = new DoubleProperty (0);
    _rotateBy_spike->add_symbol ("cx", _rotateBy_cx);
    _rotateBy_spike->add_symbol ("cy", _rotateBy_cy);
    _rotateBy_spike->add_symbol ("da", _rotateBy_da);
    _rotateBy_action = new RotateByAction (this, "action_rotateBy_by", this);
    _rotateBy_cx_coupling = new Coupling (_rotateBy_cx, ACTIVATION, _rotateBy_action, ACTIVATION);
    this->somehow_activating () ? _rotateBy_cx_coupling->enable(_frame) : _rotateBy_cx_coupling->disable ();
    _rotateBy_cy_coupling = new Coupling (_rotateBy_cy, ACTIVATION, _rotateBy_action, ACTIVATION);
    this->somehow_activating () ? _rotateBy_cy_coupling->enable(_frame) : _rotateBy_cy_coupling->disable ();
    _rotateBy_da_coupling = new Coupling (_rotateBy_da, ACTIVATION, _rotateBy_action, ACTIVATION);
    this->somehow_activating () ? _rotateBy_da_coupling->enable(_frame) : _rotateBy_da_coupling->disable ();
    Graph::instance().add_edge(_rotateBy_cx, _rotateBy_action);
    Graph::instance().add_edge(_rotateBy_cy, _rotateBy_action);
    Graph::instance().add_edge(_rotateBy_da, _rotateBy_action);
    Graph::instance().add_edge(_rotateBy_action, m11 ());
    Graph::instance().add_edge(_rotateBy_action, m12 ());
    Graph::instance().add_edge(_rotateBy_action, m14 ());
    Graph::instance().add_edge(_rotateBy_action, m21 ());
    Graph::instance().add_edge(_rotateBy_action, m22 ());
    Graph::instance().add_edge(_rotateBy_action, m24 ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _rotateBy_action);
  }

  void
  AbstractHomography::init_skewXBy () 
  {
    _skew_X_By_spike = new Spike (this, "skewXBy");
    _skew_X_By_cx = new DoubleProperty (0);
    _skew_X_By_cy = new DoubleProperty (0);
    _skew_X_By_da = new DoubleProperty (0);
    _skew_X_By_spike->add_symbol ("cx", _skew_X_By_cx);
    _skew_X_By_spike->add_symbol ("cy", _skew_X_By_cy);
    _skew_X_By_spike->add_symbol ("da", _skew_X_By_da);
    _skew_X_By_action = new Skew_X_ByAction (this, "action_skew_X_by", this);
    _skew_X_By_cx_coupling = new Coupling (_skew_X_By_cx, ACTIVATION, _skew_X_By_action, ACTIVATION);
    this->somehow_activating () ? _skew_X_By_cx_coupling->enable(_frame) : _skew_X_By_cx_coupling->disable ();
    _skew_X_By_cy_coupling = new Coupling (_skew_X_By_cy, ACTIVATION, _skew_X_By_action, ACTIVATION);
    this->somehow_activating () ? _skew_X_By_cy_coupling->enable(_frame) : _skew_X_By_cy_coupling->disable ();
    _skew_X_By_da_coupling = new Coupling (_skew_X_By_da, ACTIVATION, _skew_X_By_action, ACTIVATION);
    this->somehow_activating () ? _skew_X_By_da_coupling->enable(_frame) : _skew_X_By_da_coupling->disable ();
    Graph::instance().add_edge(_skew_X_By_cx, _skew_X_By_action);
    Graph::instance().add_edge(_skew_X_By_cy, _skew_X_By_action);
    Graph::instance().add_edge(_skew_X_By_da, _skew_X_By_action);
    Graph::instance().add_edge(_skew_X_By_action, m11 ());
    Graph::instance().add_edge(_skew_X_By_action, m12 ());
    Graph::instance().add_edge(_skew_X_By_action, m14 ());
    Graph::instance().add_edge(_skew_X_By_action, m21 ());
    Graph::instance().add_edge(_skew_X_By_action, m22 ());
    Graph::instance().add_edge(_skew_X_By_action, m24 ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _skew_X_By_action);
  }

  void
  AbstractHomography::init_skewYBy () 
  {
    _skew_Y_By_spike = new Spike (this, "skewYBy");
    _skew_Y_By_cx = new DoubleProperty (0);
    _skew_Y_By_cy = new DoubleProperty (0);
    _skew_Y_By_da = new DoubleProperty (0);
    _skew_Y_By_spike->add_symbol ("cx", _skew_Y_By_cx);
    _skew_Y_By_spike->add_symbol ("cy", _skew_Y_By_cy);
    _skew_Y_By_spike->add_symbol ("da", _skew_Y_By_da);
    _skew_Y_By_action = new Skew_Y_ByAction (this, "action_skew_Y_by", this);
    _skew_Y_By_cx_coupling = new Coupling (_skew_Y_By_cx, ACTIVATION, _skew_Y_By_action, ACTIVATION);
    this->somehow_activating () ? _skew_Y_By_cx_coupling->enable(_frame) : _skew_Y_By_cx_coupling->disable ();
    _skew_Y_By_cy_coupling = new Coupling (_skew_Y_By_cy, ACTIVATION, _skew_Y_By_action, ACTIVATION);
    this->somehow_activating () ? _skew_Y_By_cy_coupling->enable(_frame) : _skew_Y_By_cy_coupling->disable ();
    _skew_Y_By_da_coupling = new Coupling (_skew_Y_By_da, ACTIVATION, _skew_Y_By_action, ACTIVATION);
    this->somehow_activating () ? _skew_Y_By_da_coupling->enable(_frame) : _skew_Y_By_da_coupling->disable ();
    Graph::instance().add_edge(_skew_Y_By_cx, _skew_Y_By_action);
    Graph::instance().add_edge(_skew_Y_By_cy, _skew_Y_By_action);
    Graph::instance().add_edge(_skew_Y_By_da, _skew_Y_By_action);
    Graph::instance().add_edge(_skew_Y_By_action, m11 ());
    Graph::instance().add_edge(_skew_Y_By_action, m12 ());
    Graph::instance().add_edge(_skew_Y_By_action, m14 ());
    Graph::instance().add_edge(_skew_Y_By_action, m21 ());
    Graph::instance().add_edge(_skew_Y_By_action, m22 ());
    Graph::instance().add_edge(_skew_Y_By_action, m24 ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _skew_Y_By_action);
  }


  AbstractHomography::AbstractHomography (Process *p, const string &n, double m11, double m12, double m13, double m14,
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
      /* translateBy ptr */
      _translateBy_spike(nullptr), _translateBy_action(nullptr),
      _translateBy_dx(nullptr), _translateBy_dy(nullptr),
      _tranlateBy_dx_coupling(nullptr), _tranlateBy_dy_coupling(nullptr),
      /* scaleBy ptr */
      _scaleBy_spike(nullptr), _scaleBy_action(nullptr),
      _scaleBy_cx(nullptr), _scaleBy_cy(nullptr), _scaleBy_sx(nullptr), _scaleBy_sy(nullptr),
      _scaleBy_cx_coupling(nullptr), _scaleBy_cy_coupling(nullptr), _scaleBy_sx_coupling(nullptr), _scaleBy_sy_coupling(nullptr),
      /* rotateBy ptr */
      _rotateBy_spike(nullptr), _rotateBy_action(nullptr), 
      _rotateBy_cx(nullptr), _rotateBy_cy(nullptr), _rotateBy_da(nullptr),
      _rotateBy_cx_coupling(nullptr), _rotateBy_cy_coupling(nullptr), _rotateBy_da_coupling(nullptr),
      /* skewXBy ptr */
      _skew_X_By_spike(nullptr), _skew_X_By_action(nullptr),
      _skew_X_By_cx(nullptr), _skew_X_By_cy(nullptr), _skew_X_By_da(nullptr),
      _skew_X_By_cx_coupling(nullptr), _skew_X_By_cy_coupling(nullptr), _skew_X_By_da_coupling(nullptr),
      /* skewXBy ptr */
      _skew_Y_By_spike(nullptr), _skew_Y_By_action(nullptr), 
      _skew_Y_By_cx(nullptr), _skew_Y_By_cy(nullptr), _skew_Y_By_da(nullptr),
      _skew_Y_By_cx_coupling(nullptr), _skew_Y_By_cy_coupling(nullptr), _skew_Y_By_da_coupling(nullptr)
  {
  }

  AbstractHomography::AbstractHomography (double m11, double m12, double m13, double m14,
                                          double m21, double m22, double m23, double m24,
                                          double m31, double m32, double m33, double m34,
                                          double m41, double m42, double m43, double m44) :
      AbstractTransformation (),
      raw_props{.m11=m11, .m12=m12, .m13=m13, .m14=m14,
                .m21=m21, .m22=m22, .m23=m23, .m24=m24,
                .m31=m31, .m32=m32, .m33=m33, .m34=m34,
                .m41=m41, .m42=m42, .m43=m43, .m44=m44,},
      _cm11(nullptr), _cm12(nullptr), _cm13(nullptr), _cm14(nullptr),
      _cm21(nullptr), _cm22(nullptr), _cm23(nullptr), _cm24(nullptr),
      _cm31(nullptr), _cm32(nullptr), _cm33(nullptr), _cm34(nullptr),
      _cm41(nullptr), _cm42(nullptr), _cm43(nullptr), _cm44(nullptr),
      /* translateBy ptr */
      _translateBy_spike(nullptr), _translateBy_action(nullptr),
      _translateBy_dx(nullptr), _translateBy_dy(nullptr),
      _tranlateBy_dx_coupling(nullptr), _tranlateBy_dy_coupling(nullptr),
      /* scaleBy ptr */
      _scaleBy_spike(nullptr), _scaleBy_action(nullptr),
      _scaleBy_cx(nullptr), _scaleBy_cy(nullptr), _scaleBy_sx(nullptr), _scaleBy_sy(nullptr),
      _scaleBy_cx_coupling(nullptr), _scaleBy_cy_coupling(nullptr), _scaleBy_sx_coupling(nullptr), _scaleBy_sy_coupling(nullptr),
      /* rotateBy ptr */
      _rotateBy_spike(nullptr), _rotateBy_action(nullptr), 
      _rotateBy_cx(nullptr), _rotateBy_cy(nullptr), _rotateBy_da(nullptr),
      _rotateBy_cx_coupling(nullptr), _rotateBy_cy_coupling(nullptr), _rotateBy_da_coupling(nullptr),
      /* skewXBy ptr */
      _skew_X_By_spike(nullptr), _skew_X_By_action(nullptr),
      _skew_X_By_cx(nullptr), _skew_X_By_cy(nullptr), _skew_X_By_da(nullptr),
      _skew_X_By_cx_coupling(nullptr), _skew_X_By_cy_coupling(nullptr), _skew_X_By_da_coupling(nullptr),
      /* skewXBy ptr */
      _skew_Y_By_spike(nullptr), _skew_Y_By_action(nullptr), 
      _skew_Y_By_cx(nullptr), _skew_Y_By_cy(nullptr), _skew_Y_By_da(nullptr),
      _skew_Y_By_cx_coupling(nullptr), _skew_Y_By_cy_coupling(nullptr), _skew_Y_By_da_coupling(nullptr)
  {
  }

  AbstractHomography::~AbstractHomography ()
  {
    if (_translateBy_action){
      if (_parent && _parent->state_dependency () != nullptr)
        Graph::instance ().remove_edge (_parent->state_dependency (), _translateBy_action);
      Graph::instance().remove_edge(_translateBy_dx, _translateBy_action);
      Graph::instance().remove_edge(_translateBy_dy, _translateBy_action);
      Graph::instance().remove_edge(_translateBy_action, this->m11 ());
      Graph::instance().remove_edge(_translateBy_action, this->m12 ());
      Graph::instance().remove_edge(_translateBy_action, this->m14 ());
      Graph::instance().remove_edge(_translateBy_action, this->m21 ());
      Graph::instance().remove_edge(_translateBy_action, this->m22 ());
      Graph::instance().remove_edge(_translateBy_action, this->m24 ());
      delete _tranlateBy_dx_coupling;
      delete _tranlateBy_dy_coupling;
      delete _translateBy_action;
      delete _translateBy_dx;
      delete _translateBy_dy;
      delete _translateBy_spike;
    }

    if (_scaleBy_action) {
      if (_parent && _parent->state_dependency () != nullptr)
        Graph::instance ().remove_edge (_parent->state_dependency (), _scaleBy_action);
      Graph::instance().remove_edge(_scaleBy_cx, _scaleBy_action);
      Graph::instance().remove_edge(_scaleBy_cy, _scaleBy_action);
      Graph::instance().remove_edge(_scaleBy_sx, _scaleBy_action);
      Graph::instance().remove_edge(_scaleBy_sy, _scaleBy_action);
      Graph::instance().remove_edge(_scaleBy_action, this->m11 ());
      Graph::instance().remove_edge(_scaleBy_action, this->m12 ());
      Graph::instance().remove_edge(_scaleBy_action, this->m14 ());
      Graph::instance().remove_edge(_scaleBy_action, this->m21 ());
      Graph::instance().remove_edge(_scaleBy_action, this->m22 ());
      Graph::instance().remove_edge(_scaleBy_action, this->m24 ());
      delete _scaleBy_cx_coupling;
      delete _scaleBy_cy_coupling;
      delete _scaleBy_sx_coupling;
      delete _scaleBy_sy_coupling;
      delete _scaleBy_action;
      delete _scaleBy_cx;
      delete _scaleBy_cy;
      delete _scaleBy_sx;
      delete _scaleBy_sy;
      delete _scaleBy_spike;
    }

    if (_rotateBy_action) {
      if (_parent && _parent->state_dependency () != nullptr)
        Graph::instance ().remove_edge (_parent->state_dependency (), _rotateBy_action);
      Graph::instance().remove_edge(_rotateBy_cx, _rotateBy_action);
      Graph::instance().remove_edge(_rotateBy_cy, _rotateBy_action);
      Graph::instance().remove_edge(_rotateBy_da, _rotateBy_action);
      Graph::instance().remove_edge(_rotateBy_action, this->m11 ());
      Graph::instance().remove_edge(_rotateBy_action, this->m12 ());
      Graph::instance().remove_edge(_rotateBy_action, this->m14 ());
      Graph::instance().remove_edge(_rotateBy_action, this->m21 ());
      Graph::instance().remove_edge(_rotateBy_action, this->m22 ());
      Graph::instance().remove_edge(_rotateBy_action, this->m24 ());
      delete _rotateBy_cx_coupling;
      delete _rotateBy_cy_coupling;
      delete _rotateBy_da_coupling;
      delete _rotateBy_action;
      delete _rotateBy_cx;
      delete _rotateBy_cy;
      delete _rotateBy_da;
      delete _rotateBy_spike;
    }

    if (_skew_X_By_action) {
      if (_parent && _parent->state_dependency () != nullptr)
        Graph::instance ().remove_edge (_parent->state_dependency (), _skew_X_By_action);
      Graph::instance().remove_edge(_skew_X_By_cx, _skew_X_By_action);
      Graph::instance().remove_edge(_skew_X_By_cy, _skew_X_By_action);
      Graph::instance().remove_edge(_skew_X_By_da, _skew_X_By_action);
      Graph::instance().remove_edge(_skew_X_By_action, this->m11 ());
      Graph::instance().remove_edge(_skew_X_By_action, this->m12 ());
      Graph::instance().remove_edge(_skew_X_By_action, this->m14 ());
      Graph::instance().remove_edge(_skew_X_By_action, this->m21 ());
      Graph::instance().remove_edge(_skew_X_By_action, this->m22 ());
      Graph::instance().remove_edge(_skew_X_By_action, this->m24 ());
      delete _skew_X_By_cx_coupling;
      delete _skew_X_By_cy_coupling;
      delete _skew_X_By_da_coupling;
      delete _skew_X_By_action;
      delete _skew_X_By_cx;
      delete _skew_X_By_cy;
      delete _skew_X_By_da;
      delete _skew_X_By_spike;
    }

    if (_skew_Y_By_action) {
      if (_parent && _parent->state_dependency () != nullptr)
        Graph::instance ().remove_edge (_parent->state_dependency (), _skew_Y_By_action);
      Graph::instance().remove_edge(_skew_Y_By_cx, _skew_Y_By_action);
      Graph::instance().remove_edge(_skew_Y_By_cy, _skew_Y_By_action);
      Graph::instance().remove_edge(_skew_Y_By_da, _skew_Y_By_action);
      Graph::instance().remove_edge(_skew_Y_By_action, this->m11 ());
      Graph::instance().remove_edge(_skew_Y_By_action, this->m12 ());
      Graph::instance().remove_edge(_skew_Y_By_action, this->m14 ());
      Graph::instance().remove_edge(_skew_Y_By_action, this->m21 ());
      Graph::instance().remove_edge(_skew_Y_By_action, this->m22 ());
      Graph::instance().remove_edge(_skew_Y_By_action, this->m24 ());
      delete _skew_Y_By_cx_coupling;
      delete _skew_Y_By_cy_coupling;
      delete _skew_Y_By_da_coupling;
      delete _skew_Y_By_action;
      delete _skew_Y_By_cx;
      delete _skew_Y_By_cy;
      delete _skew_Y_By_da;
      delete _skew_Y_By_spike;
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
    if (name.find ("translateBy") != std::string::npos) {
      init_translationBy();
      return AbstractGObj::find_component(name);
    } else
    if (name.find ("scaleBy") != std::string::npos) {
      init_scaleBy();
      return AbstractGObj::find_component(name);
    } else
    if (name.find ("rotateBy") != std::string::npos) {
      init_rotateBy();
      return AbstractGObj::find_component(name);
    } else
    if (name.find ("skewXBy") != std::string::npos) {
      init_skewXBy();
      return AbstractGObj::find_component(name);
    } else
    if (name.find ("skewYBy") != std::string::npos) {
      init_skewYBy();
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
  AbstractHomography::activate ()
  {
    AbstractGObj::activate ();
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

    if (_translateBy_action) {
      _tranlateBy_dx_coupling->enable (_frame);
      _tranlateBy_dy_coupling->enable (_frame);
    }
    if (_scaleBy_action) {
      _scaleBy_cx_coupling->enable (_frame);
      _scaleBy_cy_coupling->enable (_frame);
      _scaleBy_sx_coupling->enable (_frame);
      _scaleBy_sy_coupling->enable (_frame);
    }
    if (_rotateBy_action) {
      _rotateBy_cx_coupling->enable (_frame);
      _rotateBy_cy_coupling->enable (_frame);
      _rotateBy_da_coupling->enable (_frame);
    }
    if (_skew_X_By_action) {
      _skew_X_By_cx_coupling->enable (_frame);
      _skew_X_By_cy_coupling->enable (_frame);
      _skew_X_By_da_coupling->enable (_frame);
    }
    if (_skew_Y_By_action) {
      _skew_Y_By_cx_coupling->enable (_frame);
      _skew_Y_By_cy_coupling->enable (_frame);
      _skew_Y_By_da_coupling->enable (_frame);
    }
  }

  void
  AbstractHomography::deactivate ()
  {
    AbstractGObj::deactivate ();
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

    if (_translateBy_action) {
      _tranlateBy_dx_coupling->disable ();
      _tranlateBy_dy_coupling->disable ();
    }
    if (_scaleBy_action) {
      _scaleBy_cx_coupling->disable ();
      _scaleBy_cy_coupling->disable ();
      _scaleBy_sx_coupling->disable ();
      _scaleBy_sy_coupling->disable ();
    }
    if (_rotateBy_action) {
      _rotateBy_cx_coupling->disable ();
      _rotateBy_cy_coupling->disable ();
      _rotateBy_da_coupling->disable ();
    }
    if (_skew_X_By_action) {
      _skew_X_By_cx_coupling->disable ();
      _skew_X_By_cy_coupling->disable ();
      _skew_X_By_da_coupling->disable ();
    }
    if (_skew_Y_By_action) {
      _skew_Y_By_cx_coupling->disable ();
      _skew_Y_By_cy_coupling->disable ();
      _skew_Y_By_da_coupling->disable ();
    }
  }

  Homography::Homography (Process *p, const string &n, double m11, double m12, double m13, double m14, double m21,
                          double m22, double m23, double m24, double m31, double m32, double m33, double m34,
                          double m41, double m42, double m43, double m44) :
      AbstractHomography (p, n, m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44)
  {
    Process::finalize ();
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
    // double m11 = _m11->get_value ();
    // double m12 = _m12->get_value ();
    // //double m13 = _m13->get_value ();

    // double m21 = _m21->get_value ();
    // double m22 = _m22->get_value ();
    // //double m23 = _m23->get_value ();

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

