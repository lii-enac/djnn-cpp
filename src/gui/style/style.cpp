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
 *
 */

#include "../backend.h"
#include "../abstract_backend.h"
#include "style.h"
#include "../../core/control/coupling.h"

namespace djnn
{
  AbstractColor::AbstractColor (Process *p, const std::string& n, double r, double g, double b) :
      AbstractGObj (p, n), _cr (nullptr), _cg (nullptr), _cb (nullptr)
  {
    _r = new DoubleProperty (this, "r", r);
    _g = new DoubleProperty (this, "g", g);
    _b = new DoubleProperty (this, "b", b);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cr = new Coupling (_r, ACTIVATION, update, ACTIVATION);
    _cg = new Coupling (_g, ACTIVATION, update, ACTIVATION);
    _cb = new Coupling (_b, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  void
  AbstractColor::activate ()
  {
    AbstractGObj::activate ();
    _cr->enable (_frame);
    _cg->enable (_frame);
    _cb->enable (_frame);
  }

  void
  AbstractColor::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cr->disable ();
    _cg->disable ();
    _cb->disable ();
  }

  AbstractColor::~AbstractColor ()
  {
    delete _cr;
    delete _cg;
    delete _cb;
    delete _r;
    delete _g;
    delete _b;
  }

  void
  FillColor::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      int r = (int) _r->get_value ();
      int g = (int) _g->get_value ();
      int b = (int) _b->get_value ();
      Backend::instance ()->load_fill_color (r, g, b);
    }
  }

  void
  OutlineColor::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      int r = (int) _r->get_value ();
      int g = (int) _g->get_value ();
      int b = (int) _b->get_value ();
      Backend::instance ()->load_outline_color (r, g, b);
    }
  }

  FillRule::FillRule (Process* p, const std::string &n, djnFillRuleType rule) :
      AbstractGObj (p, n)
  {
    _rule = new IntProperty (this, "rule", rule);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cr = new Coupling (_rule, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  FillRule::FillRule (Process* p, const std::string &n, int rule) :
      AbstractGObj (p, n)
  {
    _rule = new IntProperty (this, "rule", rule);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cr = new Coupling (_rule, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  FillRule::~FillRule ()
  {
    delete _rule;
    delete _cr;
  }

  void
  FillRule::activate ()
  {
    AbstractGObj::activate ();
    _cr->enable (_frame);
  }

  void
  FillRule::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cr->disable ();
  }

  void
  FillRule::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_fill_rule ((djnFillRuleType) (_rule->get_value ()));
    }
  }

  void
  NoOutline::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_no_outline ();
    }
  }

  void
  NoFill::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_no_fill ();
    }
  }

  Texture::Texture (Process* p, const std::string &n, const std::string &path) :
      AbstractGObj (p, n)
  {
    _path = new TextProperty (this, "path", path);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cp = new Coupling (_path, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  Texture::~Texture ()
  {
    delete _cp;
    delete _path;
  }

  void
  Texture::activate ()
  {
    AbstractGObj::activate ();
    _cp->enable (_frame);
  }

  void
  Texture::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cp->disable ();
  }

  void
  Texture::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_texture (_path->get_value ());
    }
  }

  AbstractOpacity::AbstractOpacity (Process* p, const std::string &n, double alpha) :
      AbstractGObj (p, n)
  {
    _alpha = new DoubleProperty (this, "a", alpha);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _ca = new Coupling (_alpha, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  AbstractOpacity::~AbstractOpacity ()
  {
    delete _ca;
    delete _alpha;
  }

  void
  AbstractOpacity::activate ()
  {
    AbstractGObj::activate ();
    _ca->enable (_frame);
  }

  void
  AbstractOpacity::deactivate ()
  {
    AbstractGObj::deactivate ();
    _ca->disable ();
  }

  void
  OutlineOpacity::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_opacity ((float) _alpha->get_value ());
    }
  }
  void
  FillOpacity::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_fill_opacity ((float) _alpha->get_value ());
    }
  }

  OutlineWidth::OutlineWidth (Process* p, const std::string &n, double width) :
      AbstractGObj (p, n)
  {
    _width = new DoubleProperty (this, "width", width);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cw = new Coupling (_width, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  OutlineWidth::~OutlineWidth ()
  {
    delete _cw;
    delete _width;
  }

  void
  OutlineWidth::activate ()
  {
    AbstractGObj::activate ();
    _cw->enable (_frame);
  }

  void
  OutlineWidth::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cw->disable ();
  }

  void
  OutlineWidth::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_width (_width->get_value ());
    }
  }

  OutlineCapStyle::OutlineCapStyle (Process* p, const std::string &n, djnCapStyle cap) :
      AbstractGObj (p, n)
  {
    _cap = new IntProperty (this, "cap", cap);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cc = new Coupling (_cap, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  OutlineCapStyle::OutlineCapStyle (Process* p, const std::string &n, int cap) :
      AbstractGObj (p, n)
  {
    _cap = new IntProperty (this, "cap", cap);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cc = new Coupling (_cap, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  OutlineCapStyle::~OutlineCapStyle ()
  {
    delete _cc;
    delete _cap;
  }

  void
  OutlineCapStyle::activate ()
  {
    AbstractGObj::activate ();
    _cc->enable (_frame);
  }

  void
  OutlineCapStyle::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cc->disable ();
  }

  void
  OutlineCapStyle::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_cap_style ((djnCapStyle) _cap->get_value ());
    }
  }

  OutlineJoinStyle::OutlineJoinStyle (Process* p, const std::string &n, djnJoinStyle join) :
      AbstractGObj (p, n)
  {
    _join = new IntProperty (this, "join", join);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cj = new Coupling (_join, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  OutlineJoinStyle::OutlineJoinStyle (Process* p, const std::string &n, int join) :
      AbstractGObj (p, n)
  {
    _join = new IntProperty (this, "join", join);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cj = new Coupling (_join, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  OutlineJoinStyle::~OutlineJoinStyle ()
  {
    delete _cj;
    delete _join;
  }

  void
  OutlineJoinStyle::activate ()
  {
    AbstractGObj::activate ();
    _cj->enable (_frame);
  }

  void
  OutlineJoinStyle::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cj->disable ();
  }

  void
  OutlineJoinStyle::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_join_style ((djnJoinStyle) _join->get_value ());
    }
  }

  OutlineMiterLimit::OutlineMiterLimit (Process* p, const std::string &n, int limit) :
      AbstractGObj (p, n)
  {
    _limit = new IntProperty (this, "limit", limit);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cl = new Coupling (_limit, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  OutlineMiterLimit::~OutlineMiterLimit ()
  {
    delete _cl;
    delete _limit;
  }

  void
  OutlineMiterLimit::activate ()
  {
    AbstractGObj::activate ();
    _cl->enable (_frame);
  }

  void
  OutlineMiterLimit::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cl->disable ();
  }

  void
  OutlineMiterLimit::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_miter_limit (_limit->get_value ());
    }
  }

  void
  DashArray::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_dash_array (_dash_array);
    }
  }

  void
  NoDashArray::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_no_dash_array ();
    }
  }

  DashOffset::DashOffset (Process* p, const std::string &n, double offset) :
      AbstractGObj (p, n)
  {
    _offset = new DoubleProperty (this, "offset", offset);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _co = new Coupling (_offset, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  DashOffset::~DashOffset ()
  {
    delete _co;
    delete _offset;
  }

  void
  DashOffset::activate ()
  {
    AbstractGObj::activate ();
    _co->enable (_frame);
  }

  void
  DashOffset::deactivate ()
  {
    AbstractGObj::deactivate ();
    _co->disable ();
  }

  void
  DashOffset::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_dash_offset (_offset->get_value ());
    }
  }

  GradientStop::GradientStop (Process *p, const std::string &n, double r, double g, double b, double a, double offset) :
      AbstractGObj (p, n)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (p);
    if (grad == nullptr) {
      cerr << "Parent of gradient stop must be <LinearGradient|RadialGradient>\n";
      return;
    }
    _r = new DoubleProperty (this, "r", r);
    _g = new DoubleProperty (this, "g", g);
    _b = new DoubleProperty (this, "b", b);
    _a = new DoubleProperty (this, "a", a);
    _offset = new DoubleProperty (this, "offset", offset);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cr = new Coupling (_r, ACTIVATION, update, ACTIVATION);
    _cg = new Coupling (_g, ACTIVATION, update, ACTIVATION);
    _cb = new Coupling (_b, ACTIVATION, update, ACTIVATION);
    _ca = new Coupling (_a, ACTIVATION, update, ACTIVATION);
    _co = new Coupling (_offset, ACTIVATION, update, ACTIVATION);
    grad->stops ()->add_child (this, "");
  }

  GradientStop::GradientStop (double r, double g, double b, double a, double offset) :
      AbstractGObj ()
  {
    _r = new DoubleProperty (this, "r", r);
    _g = new DoubleProperty (this, "g", g);
    _b = new DoubleProperty (this, "b", b);
    _a = new DoubleProperty (this, "a", a);
    _offset = new DoubleProperty (this, "offset", offset);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cr = new Coupling (_r, ACTIVATION, update, ACTIVATION);
    _cg = new Coupling (_g, ACTIVATION, update, ACTIVATION);
    _cb = new Coupling (_b, ACTIVATION, update, ACTIVATION);
    _ca = new Coupling (_a, ACTIVATION, update, ACTIVATION);
    _co = new Coupling (_offset, ACTIVATION, update, ACTIVATION);
  }

  Process*
  GradientStop::clone ()
  {
    return new GradientStop (_r->get_value (), _g->get_value (), _b->get_value (), _a->get_value (),
                             _offset->get_value ());
  }

  GradientStop::~GradientStop ()
  {
    delete _cr;
    delete _cg;
    delete _cb;
    delete _ca;
    delete _co;

    delete _r;
    delete _g;
    delete _b;
    delete _a;
    delete _offset;
  }

  void
  GradientStop::activate ()
  {
    AbstractGObj::activate ();
    _cr->enable (_frame);
    _cg->enable (_frame);
    _cb->enable (_frame);
    _ca->enable (_frame);
    _co->enable (_frame);
  }

  void
  GradientStop::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cr->disable ();
    _cg->disable ();
    _cb->disable ();
    _ca->disable ();
    _co->disable ();
  }

  void
  GradientStop::draw ()
  {
    Backend::instance ()->load_gradient_stop (_r->get_value (), _g->get_value (), _b->get_value (), _a->get_value (),
                                              _offset->get_value ());
  }

  AbstractGradient::AbstractGradient (Process *p, const std::string &n, int s, int fc) :
      AbstractGObj (p, n), _g (nullptr), _linear (false)
  {
    _spread = new IntProperty (this, "spread", s);
    _coords = new IntProperty (this, "coords", fc);
    _transforms = new List (this, "transforms");
    _stops = new List (this, "stops");
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cs = new Coupling (_spread, ACTIVATION, update, ACTIVATION);
    _cc = new Coupling (_coords, ACTIVATION, update, ACTIVATION);
  }

  AbstractGradient::AbstractGradient (int s, int fc) :
      AbstractGObj (), _g (nullptr), _linear (false)
  {
    _spread = new IntProperty (this, "spread", s);
    _coords = new IntProperty (this, "coords", fc);
    _transforms = new List (this, "transforms");
    _stops = new List (this, "stops");
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cs = new Coupling (_spread, ACTIVATION, update, ACTIVATION);
    _cc = new Coupling (_coords, ACTIVATION, update, ACTIVATION);
  }

  AbstractGradient::~AbstractGradient ()
  {
    delete _cs;
    delete _cc;
    delete _spread;
    delete _coords;
    delete _transforms;
    //delete _stops;
  }

  void
  AbstractGradient::activate ()
  {
    AbstractGObj::activate ();
    _stops->activation ();
    _transforms->activation ();
    _cs->enable (_frame);
    _cc->enable (_frame);
  }

  void
  AbstractGradient::deactivate ()
  {
    AbstractGObj::deactivate ();
    _stops->deactivation ();
    _transforms->deactivation ();
    _cs->enable (_frame);
    _cc->enable (_frame);
  }

  LinearGradient::LinearGradient (Process *p, const std::string &n, double x1, double y1, double x2, double y2,
                                  djnFillSpread s, djnFillCoords fc) :
      AbstractGradient (p, n, s, fc)
  {
    _linear = true;
    _x1 = new DoubleProperty (this, "x1", x1);
    _y1 = new DoubleProperty (this, "y1", y1);
    _x2 = new DoubleProperty (this, "x2", x2);
    _y2 = new DoubleProperty (this, "y2", y2);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cx1 = new Coupling (_x1, ACTIVATION, update, ACTIVATION);
    _cy1 = new Coupling (_y1, ACTIVATION, update, ACTIVATION);
    _cx2 = new Coupling (_x2, ACTIVATION, update, ACTIVATION);
    _cy2 = new Coupling (_y2, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  LinearGradient::LinearGradient (Process *p, const std::string &n, double x1, double y1, double x2, double y2, int s,
                                  int fc) :
      AbstractGradient (p, n, s, fc)
  {
    _linear = true;
    _x1 = new DoubleProperty (this, "x1", x1);
    _y1 = new DoubleProperty (this, "y1", y1);
    _x2 = new DoubleProperty (this, "x2", x2);
    _y2 = new DoubleProperty (this, "y2", y2);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cx1 = new Coupling (_x1, ACTIVATION, update, ACTIVATION);
    _cy1 = new Coupling (_y1, ACTIVATION, update, ACTIVATION);
    _cx2 = new Coupling (_x2, ACTIVATION, update, ACTIVATION);
    _cy2 = new Coupling (_y2, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  LinearGradient::LinearGradient (double x1, double y1, double x2, double y2, int s, int fc) :
      AbstractGradient (s, fc)
  {
    _x1 = new DoubleProperty (this, "x1", x1);
    _y1 = new DoubleProperty (this, "y1", y1);
    _x2 = new DoubleProperty (this, "x2", x2);
    _y2 = new DoubleProperty (this, "y2", y2);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cx1 = new Coupling (_x1, ACTIVATION, update, ACTIVATION);
    _cy1 = new Coupling (_y1, ACTIVATION, update, ACTIVATION);
    _cx2 = new Coupling (_x2, ACTIVATION, update, ACTIVATION);
    _cy2 = new Coupling (_y2, ACTIVATION, update, ACTIVATION);
  }

  Process*
  LinearGradient::clone ()
  {
    LinearGradient *g = new LinearGradient (_x1->get_value (), _y1->get_value (), _x2->get_value (), _y2->get_value (),
                                            spread ()->get_value (), coords ()->get_value ());
    for (auto s : _stops->children ()) {
      g->stops ()->add_child (s->clone (), "");
    }
    for (auto t : _transforms->children ()) {
      g->transforms ()->add_child (t->clone (), "");
    }
    return g;
  }

  LinearGradient::~LinearGradient ()
  {
    delete _cx1;
    delete _cx2;
    delete _cy1;
    delete _cy2;
    delete _x1;
    delete _y1;
    delete _x2;
    delete _y2;
  }

  void
  LinearGradient::activate ()
  {
    AbstractGradient::activate ();
    _cx1->enable (_frame);
    _cy1->enable (_frame);
    _cx2->enable (_frame);
    _cy2->enable (_frame);
  }

  void
  LinearGradient::deactivate ()
  {
    AbstractGradient::deactivate ();
    _cx1->disable ();
    _cy1->disable ();
    _cx2->disable ();
    _cy2->disable ();
  }

  void
  LinearGradient::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_linear_gradient (this);
    }
  }

  RefLinearGradient::RefLinearGradient (Process *p, const string &n, LinearGradient *lg) :
      AbstractGObj (p, n), _lg (lg)
  {
    _lg->set_activation_flag (activated);
    _lg->transforms ()->set_activation_flag (activated);
    for (auto t : _lg->transforms ()->children ()) {
      t->set_activation_flag (activated);
    }
    _lg->stops ()->set_activation_flag (activated);
    for (auto s : _lg->stops ()->children ()) {
      s->set_activation_flag (activated);
    }
    Process::finalize ();
  }

  void
  RefLinearGradient::activate ()
  {
    AbstractGObj::activate ();
  }

  void
  RefLinearGradient::deactivate ()
  {
    AbstractGObj::deactivate ();
  }

  void
  RefLinearGradient::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_linear_gradient (_lg);
    }
  }

  RadialGradient::RadialGradient (Process *p, const std::string &n, double cx, double cy, double r, double fx,
                                  double fy, djnFillSpread s, djnFillCoords fc) :
      AbstractGradient (p, n, s, fc)
  {
    _cx = new DoubleProperty (this, "cx", cx);
    _cy = new DoubleProperty (this, "cy", cy);
    _r = new DoubleProperty (this, "r", r);
    _fx = new DoubleProperty (this, "fx", fx);
    _fy = new DoubleProperty (this, "fy", fy);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _ccx = new Coupling (_cx, ACTIVATION, update, ACTIVATION);
    _ccy = new Coupling (_cy, ACTIVATION, update, ACTIVATION);
    _cr = new Coupling (_r, ACTIVATION, update, ACTIVATION);
    _cfx = new Coupling (_fx, ACTIVATION, update, ACTIVATION);
    _cfy = new Coupling (_fy, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  RadialGradient::RadialGradient (Process *p, const std::string &n, double cx, double cy, double r, double fx,
                                  double fy, int s, int fc) :
      AbstractGradient (p, n, s, fc)
  {
    _cx = new DoubleProperty (this, "cx", cx);
    _cy = new DoubleProperty (this, "cy", cy);
    _r = new DoubleProperty (this, "r", r);
    _fx = new DoubleProperty (this, "fx", fx);
    _fy = new DoubleProperty (this, "fy", fy);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _ccx = new Coupling (_cx, ACTIVATION, update, ACTIVATION);
    _ccy = new Coupling (_cy, ACTIVATION, update, ACTIVATION);
    _cr = new Coupling (_r, ACTIVATION, update, ACTIVATION);
    _cfx = new Coupling (_fx, ACTIVATION, update, ACTIVATION);
    _cfy = new Coupling (_fy, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  RadialGradient::RadialGradient (double cx, double cy, double r, double fx, double fy, int s, int fc) :
      AbstractGradient (s, fc)
  {
    _cx = new DoubleProperty (this, "cx", cx);
    _cy = new DoubleProperty (this, "cy", cy);
    _r = new DoubleProperty (this, "r", r);
    _fx = new DoubleProperty (this, "fx", fx);
    _fy = new DoubleProperty (this, "fy", fy);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _ccx = new Coupling (_cx, ACTIVATION, update, ACTIVATION);
    _ccy = new Coupling (_cy, ACTIVATION, update, ACTIVATION);
    _cr = new Coupling (_r, ACTIVATION, update, ACTIVATION);
    _cfx = new Coupling (_fx, ACTIVATION, update, ACTIVATION);
    _cfy = new Coupling (_fy, ACTIVATION, update, ACTIVATION);
  }

  Process*
  RadialGradient::clone ()
  {
    RadialGradient *rg = new RadialGradient (_cx->get_value (), _cy->get_value (), _r->get_value (), _fx->get_value (),
                                             _fy->get_value (), spread ()->get_value (), coords ()->get_value ());
    for (auto s : _stops->children ()) {
      rg->stops ()->add_child (s->clone (), "");
    }
    for (auto t : _transforms->children ()) {
      rg->transforms ()->add_child (t->clone (), "");
    }
    return rg;
  }

  RadialGradient::~RadialGradient ()
  {
    delete _ccx;
    delete _ccy;
    delete _cr;
    delete _cfx;
    delete _cfy;
    delete _cx;
    delete _cy;
    delete _fx;
    delete _fy;
    delete _r;
  }

  void
  RadialGradient::activate ()
  {
    AbstractGradient::activate ();
    _ccx->enable (_frame);
    _ccy->enable (_frame);
    _cr->enable (_frame);
    _cfx->enable (_frame);
    _cfy->enable (_frame);
  }

  void
  RadialGradient::deactivate ()
  {
    AbstractGradient::deactivate ();
    _ccx->disable ();
    _ccy->disable ();
    _cr->disable ();
    _cfx->disable ();
    _cfy->disable ();
  }

  void
  RadialGradient::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_radial_gradient (this);
    }
  }

  RefRadialGradient::RefRadialGradient (Process *p, const string &n, RadialGradient *rg) :
      AbstractGObj (p, n), _rg (rg)
  {
    for (auto t : _rg->transforms ()->children ()) {
      t->set_activation_flag (activated);
    }
    _rg->stops ()->set_activation_flag (activated);
    for (auto s : _rg->stops ()->children ()) {
      s->set_activation_flag (activated);
    }
    Process::finalize ();
  }

  void
  RefRadialGradient::activate ()
  {
    AbstractGObj::activate ();
    _rg->set_activation_flag (activated);
    _rg->transforms ()->set_activation_flag (activated);
    _rg->stops ()->set_activation_flag (activated);
  }

  void
  RefRadialGradient::deactivate ()
  {
    AbstractGObj::deactivate ();
    _rg->set_activation_flag (deactivated);
    _rg->transforms ()->set_activation_flag (deactivated);
    _rg->stops ()->set_activation_flag (deactivated);
  }

  void
  RefRadialGradient::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_radial_gradient (_rg);
    }
  }

  FontSize::FontSize (Process *p, const std::string &n, djnLengthUnit unit, double size) :
      AbstractGObj (p, n)
  {
    _unit = new IntProperty (this, "unit", unit);
    _size = new DoubleProperty (this, "size", size);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cu = new Coupling (_unit, ACTIVATION, update, ACTIVATION);
    _cs = new Coupling (_size, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  FontSize::FontSize (Process *p, const std::string &n, int unit, double size) :
      AbstractGObj (p, n)
  {
    _unit = new IntProperty (this, "unit", unit);
    _size = new DoubleProperty (this, "size", size);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cu = new Coupling (_unit, ACTIVATION, update, ACTIVATION);
    _cs = new Coupling (_size, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  FontSize::~FontSize ()
  {
    delete _cu;
    delete _cs;
    delete _unit;
    delete _size;
  }

  void
  FontSize::activate ()
  {
    AbstractGObj::activate ();
    _cu->enable (_frame);
    _cs->enable (_frame);
  }

  void
  FontSize::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cu->disable ();
    _cs->disable ();
  }

  void
  FontSize::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_font_size ((djnLengthUnit) _unit->get_value (), _size->get_value ());
    }
  }

  FontWeight::FontWeight (Process* p, const std::string &n, int weight) :
      AbstractGObj (p, n)
  {
    _weight = new IntProperty (this, "weight", weight);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cw = new Coupling (_weight, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  FontWeight::~FontWeight ()
  {
    delete _cw;
    delete _weight;
  }

  void
  FontWeight::activate ()
  {
    AbstractGObj::activate ();
    _cw->enable (_frame);
  }

  void
  FontWeight::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cw->disable ();
  }

  void
  FontWeight::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_font_weight (_weight->get_value ());
    }
  }

  FontStyle::FontStyle (Process* p, const std::string &n, djnFontSlope style) :
      AbstractGObj (p, n)
  {
    _style = new IntProperty (this, "style", style);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cs = new Coupling (_style, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  FontStyle::FontStyle (Process* p, const std::string &n, int style) :
      AbstractGObj (p, n)
  {
    _style = new IntProperty (this, "style", style);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cs = new Coupling (_style, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  FontStyle::~FontStyle ()
  {
    delete _cs;
    delete _style;
  }

  void
  FontStyle::activate ()
  {
    AbstractGObj::activate ();
    _cs->enable (_frame);
  }

  void
  FontStyle::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cs->disable ();
  }

  void
  FontStyle::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_font_style ((djnFontSlope) _style->get_value ());
    }
  }

  FontFamily::FontFamily (Process* p, const std::string &n, const std::string &family) :
      AbstractGObj (p, n)
  {
    _family = new TextProperty (this, "family", family);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cf = new Coupling (_family, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  FontFamily::~FontFamily ()
  {
    delete _cf;
    delete _family;
  }

  void
  FontFamily::activate ()
  {
    AbstractGObj::activate ();
    _cf->enable (_frame);
  }

  void
  FontFamily::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cf->disable ();
  }

  void
  FontFamily::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_font_family (_family->get_value ());
    }
  }

  TextAnchor::TextAnchor (Process* p, const std::string &n, djnAnchorType anchor) :
      AbstractGObj (p, n)
  {
    _anchor = new IntProperty (this, "anchor", anchor);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _ca = new Coupling (_anchor, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  TextAnchor::TextAnchor (Process* p, const std::string &n, int anchor) :
      AbstractGObj (p, n)
  {
    _anchor = new IntProperty (this, "anchor", anchor);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _ca = new Coupling (_anchor, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  TextAnchor::~TextAnchor ()
  {
    delete _ca;
    delete _anchor;
  }

  void
  TextAnchor::activate ()
  {
    AbstractGObj::activate ();
    _ca->enable (_frame);
  }

  void
  TextAnchor::deactivate ()
  {
    AbstractGObj::deactivate ();
    _ca->disable ();
  }

  void
  TextAnchor::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_text_anchor ((djnAnchorType) _anchor->get_value ());
    }
  }
}

