/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "../transformation/transformations.h"
#include "../../core/tree/spike.h"

#include "../backend.h"
#include "../abstract_backend.h"

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
    /*_tx = new DoubleProperty (this, "tx", tx, notify_damaged_transform);
    _ty = new DoubleProperty (this, "ty", ty, notify_damaged_transform);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _ctx = new Coupling (_tx, ACTIVATION, update, ACTIVATION);
    _ctx->disable ();
    _cty = new Coupling (_ty, ACTIVATION, update, ACTIVATION);
    _cty->disable ();*/
  }

  AbstractTranslation::~AbstractTranslation ()
  {
    delete _ctx; //_cty = nullptr;
    delete _cty; //_ctx = nullptr; 
    //delete _ty; //_ty = nullptr;
    //delete _tx; //_tx = nullptr;
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
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_translation (this, raw_props.tx, raw_props.ty);
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
    Backend::instance ()->load_gradient_translation (this, raw_props.tx, raw_props.ty);
  }

  Process*
  GradientTranslation::clone ()
  {
    return new GradientTranslation (raw_props.tx, raw_props.ty);
  }

  AbstractRotation::AbstractRotation (Process *p, const string &n, double a, double cx, double cy) :
      AbstractTransformation (p, n),
      raw_props{.a=a, .cx=cx, .cy=cy},
      _ccx (nullptr), _ccy (nullptr)
  {
  }

  AbstractRotation::AbstractRotation (double a, double cx, double cy) :
      AbstractTransformation (),
      raw_props{.a=a, .cx=cx, .cy=cy},
      _ccx (nullptr), _ccy (nullptr)
  {
  }

  AbstractRotation::~AbstractRotation ()
  {
    delete _ca; //_ca = nullptr;
    delete _ccx; //_ccx = nullptr;
    delete _ccy; //_ccy = nullptr;
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
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_rotation (this, raw_props.a, raw_props.cx, raw_props.cy);
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
    Backend::instance ()->load_gradient_rotation (this, raw_props.a, raw_props.cx, raw_props.cy);
  }

  Process*
  GradientRotation::clone ()
  {
    return new GradientRotation (raw_props.a, raw_props.cx, raw_props.cy);
  }

  AbstractScaling::AbstractScaling (Process *p, const string &n, double sx, double sy, double cx, double cy) :
      AbstractTransformation (p, n)
  {
    _sx = new DoubleProperty (this, "sx", sx); //, notify_damaged_transform);
    _sy = new DoubleProperty (this, "sy", sy); //, notify_damaged_transform);
    _cx = new DoubleProperty (this, "cx", cx); //, notify_damaged_transform);
    _cy = new DoubleProperty (this, "cy", cy); //, notify_damaged_transform);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _csx = new Coupling (_sx, ACTIVATION, update, ACTIVATION);
    _csx->disable ();
    _csy = new Coupling (_sy, ACTIVATION, update, ACTIVATION);
    _csy->disable ();
    _ccx = new Coupling (_cx, ACTIVATION, update, ACTIVATION);
    _ccx->disable ();
    _ccy = new Coupling (_cy, ACTIVATION, update, ACTIVATION);
    _ccy->disable ();
  }

  AbstractScaling::AbstractScaling (double sx, double sy, double cx, double cy) :
      AbstractTransformation ()
  {
    _sx = new DoubleProperty (this, "sx", sx); //, notify_damaged_transform);
    _sy = new DoubleProperty (this, "sy", sy); //, notify_damaged_transform);
    _cx = new DoubleProperty (this, "cx", cx); //, notify_damaged_transform);
    _cy = new DoubleProperty (this, "cy", cy); //, notify_damaged_transform);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _csx = new Coupling (_sx, ACTIVATION, update, ACTIVATION);
    _csx->disable ();
    _csy = new Coupling (_sy, ACTIVATION, update, ACTIVATION);
    _csy->disable ();
    _ccx = new Coupling (_cx, ACTIVATION, update, ACTIVATION);
    _ccx->disable ();
    _ccy = new Coupling (_cy, ACTIVATION, update, ACTIVATION);
    _ccy->disable ();
  }

  AbstractScaling::~AbstractScaling ()
  {
    if (_csx) { delete _csx; _csx = nullptr; }
    if (_csy) { delete _csy; _csy = nullptr; }
    if (_ccx) { delete _ccx; _ccx = nullptr; }
    if (_ccy) { delete _ccy; _ccy = nullptr; }
    if (_sx) { delete _sx; _sx = nullptr; }
    if (_sy) { delete _sy; _sy = nullptr; }
    if (_cx) { delete _cx; _cx = nullptr; }
    if (_cy) { delete _cy; _cy = nullptr; }
  }

  void
  AbstractScaling::activate ()
  {
    AbstractGObj::activate ();
    _csx->enable (_frame);
    _csy->enable (_frame);
    _ccx->enable (_frame);
    _ccy->enable (_frame);
  }

  void
  AbstractScaling::deactivate ()
  {
    AbstractGObj::deactivate ();
    _csx->disable ();
    _csy->disable ();
    _ccx->disable ();
    _ccy->disable ();
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
    //if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
      double sx = _sx->get_value ();
      double sy = _sy->get_value ();
      double cx = _cx->get_value ();
      double cy = _cy->get_value ();
      Backend::instance ()->load_scaling (this, sx, sy, cx, cy);
    }
  }

  Process*
  Scaling::clone ()
  {
    return new Scaling (_sx->get_value (), _sy->get_value (), _cx->get_value (), _cy->get_value ());
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
    double sx = _sx->get_value ();
    double sy = _sy->get_value ();
    double cx = _cx->get_value ();
    double cy = _cy->get_value ();
    Backend::instance ()->load_gradient_scaling (this, sx, sy, cx, cy);
  }

  Process*
  GradientScaling::clone ()
  {
    return new GradientScaling (_sx->get_value (), _sy->get_value (), _cx->get_value (), _cy->get_value ());
  }

  AbstractSkew::AbstractSkew (Process *p, const string &n, double a) :
      AbstractTransformation (p, n)
  {
    _a = new DoubleProperty (this, "a", a);//, notify_damaged_transform);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _ca = new Coupling (_a, ACTIVATION, update, ACTIVATION);
    _ca->disable ();
  }

  AbstractSkew::AbstractSkew (double a) :
      AbstractTransformation ()
  {
    _a = new DoubleProperty (this, "a", a);//, notify_damaged_transform);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _ca = new Coupling (_a, ACTIVATION, update, ACTIVATION);
    _ca->disable ();
  }

  AbstractSkew::~AbstractSkew ()
  {
    if (_ca) { delete _ca; _ca = nullptr; }
    if (_a) { delete _a; _a = nullptr; }
  }

  void
  AbstractSkew::activate ()
  {
    AbstractGObj::activate ();
    _ca->enable (_frame);
  }

  void
  AbstractSkew::deactivate ()
  {
    AbstractGObj::deactivate ();
    _ca->disable ();
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
    //if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
      double a = _a->get_value ();
      Backend::instance ()->load_skew_x (this, a);
    }
  }

  Process*
  SkewX::clone ()
  {
    return new SkewX (_a->get_value ());
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
    double a = _a->get_value ();
    Backend::instance ()->load_gradient_skew_x (this, a);
  }

  Process*
  GradientSkewX::clone ()
  {
    return new GradientSkewX (_a->get_value ());
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
    //if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
      double a = _a->get_value ();
      Backend::instance ()->load_skew_y (this, a);
    }
  }

  Process*
  SkewY::clone ()
  {
    return new SkewY (_a->get_value ());
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
    double a = _a->get_value ();
    Backend::instance ()->load_gradient_skew_y (this, a);
  }

  Process*
  GradientSkewY::clone ()
  {
    return new GradientSkewY (_a->get_value ());
  }


  /* ----  HOMOGRAPHIES ---- */ 

  static void
  homography_to_array (AbstractHomography *h, array<array<double, 4>, 4> &a) {

    a[0][0] = h->_m11->get_value ();
    a[0][1] = h->_m12->get_value ();
    a[0][2] = h->_m13->get_value ();
    a[0][3] = h->_m14->get_value ();

    a[1][0] = h->_m21->get_value ();
    a[1][1] = h->_m22->get_value ();
    a[1][2] = h->_m23->get_value ();
    a[1][3] = h->_m24->get_value ();

    a[2][0] = h->_m31->get_value ();
    a[2][1] = h->_m32->get_value ();
    a[2][2] = h->_m33->get_value ();
    a[2][3] = h->_m34->get_value ();

    a[3][0] = h->_m41->get_value ();
    a[3][1] = h->_m42->get_value ();
    a[3][2] = h->_m43->get_value ();
    a[3][3] = h->_m44->get_value ();

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
     */
    h->_m11->set_value (a[0][0], true);
    h->_m12->set_value (a[0][1], true);
    h->_m13->set_value (a[0][2], false);
    h->_m14->set_value (a[0][3], true);

    h->_m21->set_value (a[1][0], true);
    h->_m22->set_value (a[1][1], true);
    h->_m23->set_value (a[1][2], false);
    h->_m24->set_value (a[1][3], true);

    h->_m31->set_value (a[2][0], false);
    h->_m32->set_value (a[2][1], false);
    h->_m33->set_value (a[2][2], false);
    h->_m34->set_value (a[2][3], false);

    h->_m41->set_value (a[3][0], false);
    h->_m42->set_value (a[3][1], false);
    h->_m43->set_value (a[3][2], false);
    h->_m44->set_value (a[3][3], false);
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
  AbstractHomography::init_abstractHomography (double m11, double m12, double m13, double m14,
                                               double m21, double m22, double m23, double m24,
                                               double m31, double m32, double m33, double m34,
                                               double m41, double m42, double m43, double m44)
  {
    _m11 = new DoubleProperty (this, "m11", m11);//, notify_damaged_transform);
    _m12 = new DoubleProperty (this, "m12", m12);//, notify_damaged_transform);
    _m13 = new DoubleProperty (this, "m13", m13);//, notify_damaged_transform);
    _m14 = new DoubleProperty (this, "m14", m14);//, notify_damaged_transform);

    _m21 = new DoubleProperty (this, "m21", m21); //, notify_damaged_transform);
    _m22 = new DoubleProperty (this, "m22", m22); //, notify_damaged_transform);
    _m23 = new DoubleProperty (this, "m23", m23); //, notify_damaged_transform);
    _m24 = new DoubleProperty (this, "m24", m24); //, notify_damaged_transform);

    _m31 = new DoubleProperty (this, "m31", m31); //, notify_damaged_transform);
    _m32 = new DoubleProperty (this, "m32", m32); //, notify_damaged_transform);
    _m33 = new DoubleProperty (this, "m33", m33); //, notify_damaged_transform);
    _m34 = new DoubleProperty (this, "m34", m34); //, notify_damaged_transform);

    _m41 = new DoubleProperty (this, "m41", m41); //, notify_damaged_transform);
    _m42 = new DoubleProperty (this, "m42", m42); //, notify_damaged_transform);
    _m43 = new DoubleProperty (this, "m43", m43); //, notify_damaged_transform);
    _m44 = new DoubleProperty (this, "m44", m44); //, notify_damaged_transform);

    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cm11 = new Coupling (_m11, ACTIVATION, update, ACTIVATION);
    _cm11->disable ();
    _cm12 = new Coupling (_m12, ACTIVATION, update, ACTIVATION);
    _cm12->disable ();
    _cm13 = new Coupling (_m13, ACTIVATION, update, ACTIVATION);
    _cm13->disable ();
    _cm14 = new Coupling (_m14, ACTIVATION, update, ACTIVATION);
    _cm14->disable ();

    _cm21 = new Coupling (_m21, ACTIVATION, update, ACTIVATION);
    _cm21->disable ();
    _cm22 = new Coupling (_m22, ACTIVATION, update, ACTIVATION);
    _cm22->disable ();
    _cm23 = new Coupling (_m23, ACTIVATION, update, ACTIVATION);
    _cm23->disable ();
    _cm24 = new Coupling (_m24, ACTIVATION, update, ACTIVATION);
    _cm24->disable ();

    _cm31 = new Coupling (_m31, ACTIVATION, update, ACTIVATION);
    _cm31->disable ();
    _cm32 = new Coupling (_m32, ACTIVATION, update, ACTIVATION);
    _cm32->disable ();
    _cm33 = new Coupling (_m33, ACTIVATION, update, ACTIVATION);
    _cm33->disable ();
    _cm34 = new Coupling (_m34, ACTIVATION, update, ACTIVATION);
    _cm34->disable ();

    _cm41 = new Coupling (_m41, ACTIVATION, update, ACTIVATION);
    _cm41->disable ();
    _cm42 = new Coupling (_m42, ACTIVATION, update, ACTIVATION);
    _cm42->disable ();
    _cm43 = new Coupling (_m43, ACTIVATION, update, ACTIVATION);
    _cm43->disable ();
    _cm44 = new Coupling (_m44, ACTIVATION, update, ACTIVATION);
    _cm44->disable ();

    /* note:
     * action work only for 2D 
     * propagation is not active for 3D
     *  |  x  x  .  x  |
     *  |  x  x  .  x  |
     *  |  .  .  1  .  |
     *  |  .  .  .  1  |
     */

    /* translateBy */
    _translateBy_spike = new Spike (this, "translateBy");
    _translateBy_dx = new DoubleProperty (0);
    _translateBy_dy = new DoubleProperty (0);
    _translateBy_spike->add_symbol( "dx", _translateBy_dx);
    _translateBy_spike->add_symbol( "dy", _translateBy_dy);
    _translateBy_action = new TranslateByAction (this, "action_translate_by", this);
    _tranlateBy_dx_coupling = new Coupling (_translateBy_dx, ACTIVATION, _translateBy_action, ACTIVATION);
    _tranlateBy_dx_coupling->disable ();
    _tranlateBy_dy_coupling = new Coupling (_translateBy_dy, ACTIVATION, _translateBy_action, ACTIVATION);
    _tranlateBy_dy_coupling->disable ();
    Graph::instance().add_edge(_translateBy_dx, _translateBy_action);
    Graph::instance().add_edge(_translateBy_dy, _translateBy_action);
    Graph::instance().add_edge(_translateBy_action, _m11);
    Graph::instance().add_edge(_translateBy_action, _m12);
    Graph::instance().add_edge(_translateBy_action, _m14);
    Graph::instance().add_edge(_translateBy_action, _m21);
    Graph::instance().add_edge(_translateBy_action, _m22);
    Graph::instance().add_edge(_translateBy_action, _m24);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _translateBy_action);

    /* scaleBy */
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
    _scaleBy_cx_coupling->disable ();
    _scaleBy_cy_coupling = new Coupling (_scaleBy_cy, ACTIVATION, _scaleBy_action, ACTIVATION);
    _scaleBy_cy_coupling->disable ();
    _scaleBy_sx_coupling = new Coupling (_scaleBy_sx, ACTIVATION, _scaleBy_action, ACTIVATION);
    _scaleBy_sx_coupling->disable ();
    _scaleBy_sy_coupling = new Coupling (_scaleBy_sy, ACTIVATION, _scaleBy_action, ACTIVATION);
    _scaleBy_sy_coupling->disable ();
    Graph::instance().add_edge(_scaleBy_cx, _scaleBy_action);
    Graph::instance().add_edge(_scaleBy_cy, _scaleBy_action);
    Graph::instance().add_edge(_scaleBy_sx, _scaleBy_action);
    Graph::instance().add_edge(_scaleBy_sy, _scaleBy_action);
    Graph::instance().add_edge(_scaleBy_action, _m11);
    Graph::instance().add_edge(_scaleBy_action, _m12);
    Graph::instance().add_edge(_scaleBy_action, _m14);
    Graph::instance().add_edge(_scaleBy_action, _m21);
    Graph::instance().add_edge(_scaleBy_action, _m22);
    Graph::instance().add_edge(_scaleBy_action, _m24);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _scaleBy_action);

    /* rotateBy */
    _rotateBy_spike = new Spike (this, "rotateBy");
    _rotateBy_cx = new DoubleProperty (0);
    _rotateBy_cy = new DoubleProperty (0);
    _rotateBy_da = new DoubleProperty (0);
    _rotateBy_spike->add_symbol ("cx", _rotateBy_cx);
    _rotateBy_spike->add_symbol ("cy", _rotateBy_cy);
    _rotateBy_spike->add_symbol ("da", _rotateBy_da);
    _rotateBy_action = new RotateByAction (this, "action_rotateBy_by", this);
    _rotateBy_cx_coupling = new Coupling (_rotateBy_cx, ACTIVATION, _rotateBy_action, ACTIVATION);
    _rotateBy_cx_coupling->disable ();
    _rotateBy_cy_coupling = new Coupling (_rotateBy_cy, ACTIVATION, _rotateBy_action, ACTIVATION);
    _rotateBy_cy_coupling->disable ();
    _rotateBy_da_coupling = new Coupling (_rotateBy_da, ACTIVATION, _rotateBy_action, ACTIVATION);
    _rotateBy_da_coupling->disable ();
    Graph::instance().add_edge(_rotateBy_cx, _rotateBy_action);
    Graph::instance().add_edge(_rotateBy_cy, _rotateBy_action);
    Graph::instance().add_edge(_rotateBy_da, _rotateBy_action);
    Graph::instance().add_edge(_rotateBy_action, _m11);
    Graph::instance().add_edge(_rotateBy_action, _m12);
    Graph::instance().add_edge(_rotateBy_action, _m14);
    Graph::instance().add_edge(_rotateBy_action, _m21);
    Graph::instance().add_edge(_rotateBy_action, _m22);
    Graph::instance().add_edge(_rotateBy_action, _m24);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _rotateBy_action);

    /* _skew_X_By */
    _skew_X_By_spike = new Spike (this, "skewXBy");
    _skew_X_By_cx = new DoubleProperty (0);
    _skew_X_By_cy = new DoubleProperty (0);
    _skew_X_By_da = new DoubleProperty (0);
    _skew_X_By_spike->add_symbol ("cx", _skew_X_By_cx);
    _skew_X_By_spike->add_symbol ("cy", _skew_X_By_cy);
    _skew_X_By_spike->add_symbol ("da", _skew_X_By_da);
    _skew_X_By_action = new Skew_X_ByAction (this, "action_skew_X_by", this);
    _skew_X_By_cx_coupling = new Coupling (_skew_X_By_cx, ACTIVATION, _skew_X_By_action, ACTIVATION);
    _skew_X_By_cx_coupling->disable ();
    _skew_X_By_cy_coupling = new Coupling (_skew_X_By_cy, ACTIVATION, _skew_X_By_action, ACTIVATION);
    _skew_X_By_cy_coupling->disable ();
    _skew_X_By_da_coupling = new Coupling (_skew_X_By_da, ACTIVATION, _skew_X_By_action, ACTIVATION);
    _skew_X_By_da_coupling->disable ();
    Graph::instance().add_edge(_skew_X_By_cx, _skew_X_By_action);
    Graph::instance().add_edge(_skew_X_By_cy, _skew_X_By_action);
    Graph::instance().add_edge(_skew_X_By_da, _skew_X_By_action);
    Graph::instance().add_edge(_skew_X_By_action, _m11);
    Graph::instance().add_edge(_skew_X_By_action, _m12);
    Graph::instance().add_edge(_skew_X_By_action, _m14);
    Graph::instance().add_edge(_skew_X_By_action, _m21);
    Graph::instance().add_edge(_skew_X_By_action, _m22);
    Graph::instance().add_edge(_skew_X_By_action, _m24);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _skew_X_By_action);


    /* _skew_Y_By */
    _skew_Y_By_spike = new Spike (this, "skewYBy");
    _skew_Y_By_cx = new DoubleProperty (0);
    _skew_Y_By_cy = new DoubleProperty (0);
    _skew_Y_By_da = new DoubleProperty (0);
    _skew_Y_By_spike->add_symbol ("cx", _skew_Y_By_cx);
    _skew_Y_By_spike->add_symbol ("cy", _skew_Y_By_cy);
    _skew_Y_By_spike->add_symbol ("da", _skew_Y_By_da);
    _skew_Y_By_action = new Skew_Y_ByAction (this, "action_skew_Y_by", this);
    _skew_Y_By_cx_coupling = new Coupling (_skew_Y_By_cx, ACTIVATION, _skew_Y_By_action, ACTIVATION);
    _skew_Y_By_cx_coupling->disable ();
    _skew_Y_By_cy_coupling = new Coupling (_skew_Y_By_cy, ACTIVATION, _skew_Y_By_action, ACTIVATION);
    _skew_Y_By_cy_coupling->disable ();
    _skew_Y_By_da_coupling = new Coupling (_skew_Y_By_da, ACTIVATION, _skew_Y_By_action, ACTIVATION);
    _skew_Y_By_da_coupling->disable ();
    Graph::instance().add_edge(_skew_Y_By_cx, _skew_Y_By_action);
    Graph::instance().add_edge(_skew_Y_By_cy, _skew_Y_By_action);
    Graph::instance().add_edge(_skew_Y_By_da, _skew_Y_By_action);
    Graph::instance().add_edge(_skew_Y_By_action, _m11);
    Graph::instance().add_edge(_skew_Y_By_action, _m12);
    Graph::instance().add_edge(_skew_Y_By_action, _m14);
    Graph::instance().add_edge(_skew_Y_By_action, _m21);
    Graph::instance().add_edge(_skew_Y_By_action, _m22);
    Graph::instance().add_edge(_skew_Y_By_action, _m24);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _skew_Y_By_action);



  }


  AbstractHomography::AbstractHomography (Process *p, const string &n, double m11, double m12, double m13, double m14,
                                          double m21, double m22, double m23, double m24, 
                                          double m31, double m32, double m33, double m34, 
                                          double m41, double m42, double m43, double m44) :
      AbstractTransformation (p, n)
  {
    init_abstractHomography (m11, m12, m13, m14,
                             m21, m22, m23, m24,
                             m31, m32, m33, m34,
                             m41, m42, m43, m44);
  }

  AbstractHomography::AbstractHomography (double m11, double m12, double m13, double m14,
                                          double m21, double m22, double m23, double m24,
                                          double m31, double m32, double m33, double m34,
                                          double m41, double m42, double m43, double m44) :
      AbstractTransformation ()
  {
    init_abstractHomography (m11, m12, m13, m14,
                             m21, m22, m23, m24,
                             m31, m32, m33, m34,
                             m41, m42, m43, m44);
  }

  AbstractHomography::~AbstractHomography ()
  {
    if (_cm11) {delete _cm11; _cm11 = nullptr;}
    if (_cm12) {delete _cm12; _cm12 = nullptr;}
    if (_cm13) {delete _cm13; _cm13 = nullptr;}
    if (_cm14) {delete _cm14; _cm14 = nullptr;}

    if (_cm21) {delete _cm21; _cm21 = nullptr;}
    if (_cm22) {delete _cm22; _cm22 = nullptr;}
    if (_cm23) {delete _cm23; _cm23 = nullptr;}
    if (_cm24) {delete _cm24; _cm24 = nullptr;}

    if (_cm31) {delete _cm31; _cm31 = nullptr;}
    if (_cm32) {delete _cm32; _cm32 = nullptr;}
    if (_cm33) {delete _cm33; _cm33 = nullptr;}
    if (_cm34) {delete _cm34; _cm34 = nullptr;}

    if (_cm41) {delete _cm41; _cm41 = nullptr;}
    if (_cm42) {delete _cm42; _cm42 = nullptr;}
    if (_cm43) {delete _cm43; _cm43 = nullptr;}
    if (_cm44) {delete _cm44; _cm44 = nullptr;}

    if (_m11) {delete _m11; _m11 = nullptr;}
    if (_m12) {delete _m12; _m12 = nullptr;}
    if (_m13) {delete _m13; _m13 = nullptr;}
    if (_m14) {delete _m14; _m14 = nullptr;}

    if (_m21) {delete _m21; _m21 = nullptr;}
    if (_m22) {delete _m22; _m22 = nullptr;}
    if (_m23) {delete _m23; _m23 = nullptr;}
    if (_m24) {delete _m24; _m24 = nullptr;}

    if (_m31) {delete _m31; _m31 = nullptr;}
    if (_m32) {delete _m32; _m32 = nullptr;}
    if (_m33) {delete _m33; _m33 = nullptr;}
    if (_m34) {delete _m34; _m34 = nullptr;}

    if (_m41) {delete _m41; _m41 = nullptr;}
    if (_m42) {delete _m42; _m42 = nullptr;}
    if (_m43) {delete _m43; _m43 = nullptr;}
    if (_m44) {delete _m44; _m44 = nullptr;}

    /* translate BY - Becarfull of the order */
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _translateBy_action);
    Graph::instance().remove_edge(_translateBy_action, _m11);
    Graph::instance().remove_edge(_translateBy_action, _m12);
    Graph::instance().remove_edge(_translateBy_action, _m14);
    Graph::instance().remove_edge(_translateBy_action, _m21);
    Graph::instance().remove_edge(_translateBy_action, _m22);
    Graph::instance().remove_edge(_translateBy_action, _m24);
    Graph::instance().remove_edge(_translateBy_dx, _translateBy_action);
    Graph::instance().remove_edge(_translateBy_dy, _translateBy_action);
    if (_tranlateBy_dx_coupling) {delete _tranlateBy_dx_coupling; _tranlateBy_dx_coupling=nullptr;}
    if (_tranlateBy_dy_coupling) {delete _tranlateBy_dy_coupling; _tranlateBy_dy_coupling=nullptr;}
    if (_translateBy_action) {delete _translateBy_action; _translateBy_action=nullptr;}
    if (_translateBy_dx) {delete _translateBy_dx; _translateBy_dx=nullptr;}
    if (_translateBy_dy) {delete _translateBy_dy; _translateBy_dy=nullptr;}
    if (_translateBy_spike) {delete _translateBy_spike; _translateBy_spike=nullptr;} 
 

    /* scale BY - Becarfull of the order */
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _scaleBy_action);
    Graph::instance().remove_edge(_scaleBy_cx, _scaleBy_action);
    Graph::instance().remove_edge(_scaleBy_cy, _scaleBy_action);
    Graph::instance().remove_edge(_scaleBy_sx, _scaleBy_action);
    Graph::instance().remove_edge(_scaleBy_sy, _scaleBy_action);
    Graph::instance().remove_edge(_scaleBy_action, _m11);
    Graph::instance().remove_edge(_scaleBy_action, _m12);
    Graph::instance().remove_edge(_scaleBy_action, _m14);
    Graph::instance().remove_edge(_scaleBy_action, _m21);
    Graph::instance().remove_edge(_scaleBy_action, _m22);
    Graph::instance().remove_edge(_scaleBy_action, _m24);
    if (_scaleBy_cx_coupling) {delete _scaleBy_cx_coupling; _scaleBy_cx_coupling=nullptr;}
    if (_scaleBy_cy_coupling) {delete _scaleBy_cy_coupling; _scaleBy_cy_coupling=nullptr;}
    if (_scaleBy_sx_coupling) {delete _scaleBy_sx_coupling; _scaleBy_sx_coupling=nullptr;}
    if (_scaleBy_sy_coupling) {delete _scaleBy_sy_coupling; _scaleBy_sy_coupling=nullptr;}
    if (_scaleBy_action) {delete _translateBy_action; _translateBy_action=nullptr;}
    if (_scaleBy_cx) {delete _scaleBy_cx; _scaleBy_cx=nullptr;}
    if (_scaleBy_cy) {delete _scaleBy_cy; _scaleBy_cy=nullptr;}
    if (_scaleBy_sx) {delete _scaleBy_sx; _scaleBy_sx=nullptr;}
    if (_scaleBy_sy) {delete _scaleBy_sy; _scaleBy_sy=nullptr;}
    if (_scaleBy_spike) {delete _scaleBy_spike; _scaleBy_spike=nullptr;} 

    /* rotate BY - Becarfull of the order */
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _rotateBy_action);
    Graph::instance().remove_edge(_rotateBy_cx, _rotateBy_action);
    Graph::instance().remove_edge(_rotateBy_cy, _rotateBy_action);
    Graph::instance().remove_edge(_rotateBy_da, _rotateBy_action);
    Graph::instance().remove_edge(_rotateBy_action, _m11);
    Graph::instance().remove_edge(_rotateBy_action, _m12);
    Graph::instance().remove_edge(_rotateBy_action, _m14);
    Graph::instance().remove_edge(_rotateBy_action, _m21);
    Graph::instance().remove_edge(_rotateBy_action, _m22);
    Graph::instance().remove_edge(_rotateBy_action, _m24);
    if (_rotateBy_cx_coupling) {delete _rotateBy_cx_coupling; _rotateBy_cx_coupling=nullptr;}
    if (_rotateBy_cy_coupling) {delete _rotateBy_cy_coupling; _rotateBy_cy_coupling=nullptr;}
    if (_rotateBy_da_coupling) {delete _rotateBy_da_coupling; _rotateBy_da_coupling=nullptr;}
    if (_rotateBy_action) {delete _rotateBy_action; _rotateBy_action=nullptr;}
    if (_rotateBy_cx) {delete _rotateBy_cx; _rotateBy_cx=nullptr;}
    if (_rotateBy_cy) {delete _rotateBy_cy; _rotateBy_cy=nullptr;}
    if (_rotateBy_da) {delete _rotateBy_da; _rotateBy_da=nullptr;}
    if (_rotateBy_spike) {delete _rotateBy_spike; _rotateBy_spike=nullptr;} 

    /* skew Y BY - Becarfull of the order */
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _skew_X_By_action);
    Graph::instance().remove_edge(_skew_X_By_cx, _skew_X_By_action);
    Graph::instance().remove_edge(_skew_X_By_cy, _skew_X_By_action);
    Graph::instance().remove_edge(_skew_X_By_da, _skew_X_By_action);
    Graph::instance().remove_edge(_skew_X_By_action, _m11);
    Graph::instance().remove_edge(_skew_X_By_action, _m12);
    Graph::instance().remove_edge(_skew_X_By_action, _m14);
    Graph::instance().remove_edge(_skew_X_By_action, _m21);
    Graph::instance().remove_edge(_skew_X_By_action, _m22);
    Graph::instance().remove_edge(_skew_X_By_action, _m24);
    if (_skew_X_By_cx_coupling) {delete _skew_X_By_cx_coupling; _skew_X_By_cx_coupling=nullptr;}
    if (_skew_X_By_cy_coupling) {delete _skew_X_By_cy_coupling; _skew_X_By_cy_coupling=nullptr;}
    if (_skew_X_By_da_coupling) {delete _skew_X_By_da_coupling; _skew_X_By_da_coupling=nullptr;}
    if (_skew_X_By_action) {delete _skew_X_By_action; _skew_X_By_action=nullptr;}
    if (_skew_X_By_cx) {delete _skew_X_By_cx; _skew_X_By_cx=nullptr;}
    if (_skew_X_By_cy) {delete _skew_X_By_cy; _skew_X_By_cy=nullptr;}
    if (_skew_X_By_da) {delete _skew_X_By_da; _skew_X_By_da=nullptr;}
    if (_skew_X_By_spike) {delete _skew_X_By_spike; _skew_X_By_spike=nullptr;} 

    /* skew Y BY - Becarfull of the order */
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _skew_Y_By_action);
    Graph::instance().remove_edge(_skew_Y_By_cx, _skew_Y_By_action);
    Graph::instance().remove_edge(_skew_Y_By_cy, _skew_Y_By_action);
    Graph::instance().remove_edge(_skew_Y_By_da, _skew_Y_By_action);
    Graph::instance().remove_edge(_skew_Y_By_action, _m11);
    Graph::instance().remove_edge(_skew_Y_By_action, _m12);
    Graph::instance().remove_edge(_skew_Y_By_action, _m14);
    Graph::instance().remove_edge(_skew_Y_By_action, _m21);
    Graph::instance().remove_edge(_skew_Y_By_action, _m22);
    Graph::instance().remove_edge(_skew_Y_By_action, _m24);
    if (_skew_Y_By_cx_coupling) {delete _skew_Y_By_cx_coupling; _skew_Y_By_cx_coupling=nullptr;}
    if (_skew_Y_By_cy_coupling) {delete _skew_Y_By_cy_coupling; _skew_Y_By_cy_coupling=nullptr;}
    if (_skew_Y_By_da_coupling) {delete _skew_Y_By_da_coupling; _skew_Y_By_da_coupling=nullptr;}
    if (_skew_Y_By_action) {delete _skew_Y_By_action; _skew_Y_By_action=nullptr;}
    if (_skew_Y_By_cx) {delete _skew_Y_By_cx; _skew_Y_By_cx=nullptr;}
    if (_skew_Y_By_cy) {delete _skew_Y_By_cy; _skew_Y_By_cy=nullptr;}
    if (_skew_Y_By_da) {delete _skew_Y_By_da; _skew_Y_By_da=nullptr;}
    if (_skew_Y_By_spike) {delete _skew_Y_By_spike; _skew_Y_By_spike=nullptr;}

  }

  void
  AbstractHomography::activate ()
  {
    AbstractGObj::activate ();
    _cm11->enable (_frame);
    _cm12->enable (_frame);
    _cm13->enable (_frame);
    _cm14->enable (_frame);

    _cm21->enable (_frame);
    _cm22->enable (_frame);
    _cm23->enable (_frame);
    _cm24->enable (_frame);

    _cm31->enable (_frame);
    _cm32->enable (_frame);
    _cm33->enable (_frame);
    _cm34->enable (_frame);

    _cm41->enable (_frame);
    _cm42->enable (_frame);
    _cm43->enable (_frame);
    _cm44->enable (_frame);

    _tranlateBy_dx_coupling->enable (_frame);
    _tranlateBy_dy_coupling->enable (_frame);
    _scaleBy_cx_coupling->enable (_frame);
    _scaleBy_cy_coupling->enable (_frame);
    _scaleBy_sx_coupling->enable (_frame);
    _scaleBy_sy_coupling->enable (_frame);
    _rotateBy_cx_coupling->enable (_frame);
    _rotateBy_cy_coupling->enable (_frame);
    _rotateBy_da_coupling->enable (_frame);
    _skew_X_By_cx_coupling->enable (_frame);
    _skew_X_By_cy_coupling->enable (_frame);
    _skew_X_By_da_coupling->enable (_frame);
    _skew_Y_By_cx_coupling->enable (_frame);
    _skew_Y_By_cy_coupling->enable (_frame);
    _skew_Y_By_da_coupling->enable (_frame);
  }

  void
  AbstractHomography::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cm11->disable ();
    _cm12->disable ();
    _cm13->disable ();
    _cm14->disable ();

    _cm21->disable ();
    _cm22->disable ();
    _cm23->disable ();
    _cm24->disable ();

    _cm31->disable ();
    _cm32->disable ();
    _cm33->disable ();
    _cm34->disable ();

    _cm41->disable ();
    _cm42->disable ();
    _cm43->disable ();
    _cm44->disable ();

    _tranlateBy_dx_coupling->disable ();
    _tranlateBy_dy_coupling->disable ();
    _scaleBy_cx_coupling->disable ();
    _scaleBy_cy_coupling->disable ();
    _scaleBy_sx_coupling->disable ();
    _scaleBy_sy_coupling->disable ();
    _rotateBy_cx_coupling->disable ();
    _rotateBy_cy_coupling->disable ();
    _rotateBy_da_coupling->disable ();
    _skew_X_By_cx_coupling->disable ();
    _skew_X_By_cy_coupling->disable ();
    _skew_X_By_da_coupling->disable ();
    _skew_Y_By_cx_coupling->disable ();
    _skew_Y_By_cy_coupling->disable ();
    _skew_Y_By_da_coupling->disable ();
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
    //if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
      double m11 = _m11->get_value ();
      double m12 = _m12->get_value ();
      double m13 = _m13->get_value ();
      double m14 = _m14->get_value ();

      double m21 = _m21->get_value ();
      double m22 = _m22->get_value ();
      double m23 = _m23->get_value ();
      double m24 = _m24->get_value ();

      double m31 = _m31->get_value ();
      double m32 = _m32->get_value ();
      double m33 = _m33->get_value ();
      double m34 = _m34->get_value ();

      double m41 = _m41->get_value ();
      double m42 = _m42->get_value ();
      double m43 = _m43->get_value ();
      double m44 = _m44->get_value ();
      Backend::instance ()->load_homography (this, m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43,
                                             m44);
    }
  }

  Process*
  Homography::clone ()
  {
    return new Homography (_m11->get_value (), _m12->get_value (), _m13->get_value (), _m14->get_value (),
                           _m21->get_value (), _m22->get_value (), _m23->get_value (), _m24->get_value (),
                           _m31->get_value (), _m32->get_value (), _m33->get_value (), _m34->get_value (),
                           _m41->get_value (), _m42->get_value (), _m43->get_value (), _m44->get_value ());
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
    double m11 = _m11->get_value ();
    double m12 = _m12->get_value ();
    double m13 = _m13->get_value ();

    double m21 = _m21->get_value ();
    double m22 = _m22->get_value ();
    double m23 = _m23->get_value ();

    double m31 = _m31->get_value ();
    double m32 = _m32->get_value ();
    double m33 = _m33->get_value ();

    Backend::instance ()->load_gradient_homography (this, m11, m12, m13, m21, m22, m23, m31, m32, m33);
  }

  Process*
  GradientHomography::clone ()
  {
    return new GradientHomography (_m11->get_value (), _m12->get_value (), _m13->get_value (), _m21->get_value (),
                                   _m22->get_value (), _m23->get_value (), _m31->get_value (), _m32->get_value (),
                                   _m33->get_value ());
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
    double m11 = _m11->get_value ();
    double m12 = _m12->get_value ();
    //double m13 = _m13->get_value ();

    double m21 = _m21->get_value ();
    double m22 = _m22->get_value ();
    //double m23 = _m23->get_value ();

    double m31 = _m31->get_value ();
    double m32 = _m32->get_value ();
    //double m33 = _m33->get_value ();

    //Backend::instance ()->load_gradient_homography (this, m11, m12, m13, m21, m22, m23, m31, m32, m33);
    //a, b, 0, c, d, 0, e, f, 1)
    Backend::instance ()->load_simple_gradient_transform (this, m11, m12, m21, m22, m31, m32);
  }

  Process*
  SimpleGradientTransform::clone ()
  {
    return new GradientHomography (_m11->get_value (), _m12->get_value (), _m13->get_value (), _m21->get_value (),
                                        _m22->get_value (), _m23->get_value (), _m31->get_value (), _m32->get_value (),
                                        _m33->get_value ());
  }
}

