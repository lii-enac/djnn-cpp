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

  AbstractStyle::AbstractStyle (Process *p, const string &n) :
      AbstractGObj (p, n)
  {
  }

  AbstractStyle::AbstractStyle ()
  {
  }

  AbstractStyle::~AbstractStyle ()
  {
  }

  AbstractColor::AbstractColor (double r, double g, double b) :
      _cr (nullptr), _cg (nullptr), _cb (nullptr)
  {
    _r = new DoubleProperty (this, "r", r);
    _g = new DoubleProperty (this, "g", g);
    _b = new DoubleProperty (this, "b", b);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cr = new Coupling (_r, ACTIVATION, update, ACTIVATION);
    _cr->disable ();
    _cg = new Coupling (_g, ACTIVATION, update, ACTIVATION);
    _cg->disable ();
    _cb = new Coupling (_b, ACTIVATION, update, ACTIVATION);
    _cb->disable ();
  }

  AbstractColor::AbstractColor (Process *p, const std::string& n, double r, double g, double b) :
      AbstractStyle (p, n), _cr (nullptr), _cg (nullptr), _cb (nullptr)
  {
    _r = new DoubleProperty (this, "r", r);
    _g = new DoubleProperty (this, "g", g);
    _b = new DoubleProperty (this, "b", b);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cr = new Coupling (_r, ACTIVATION, update, ACTIVATION);
    _cr->disable ();
    _cg = new Coupling (_g, ACTIVATION, update, ACTIVATION);
    _cg->disable ();
    _cb = new Coupling (_b, ACTIVATION, update, ACTIVATION);
    _cb->disable ();
    Process::finalize ();
  }

  void
  AbstractColor::activate ()
  {
    AbstractStyle::activate ();
    _cr->enable (_frame);
    _cg->enable (_frame);
    _cb->enable (_frame);
  }

  void
  AbstractColor::deactivate ()
  {
    AbstractStyle::deactivate ();
    _cr->disable ();
    _cg->disable ();
    _cb->disable ();
  }

  AbstractColor::~AbstractColor ()
  {
    if (_cr) { delete _cr; _cr = nullptr;}
    if (_cg) { delete _cg; _cg = nullptr;}
    if (_cb) { delete _cb; _cb = nullptr;}
    if (_r) { delete _r; _r = nullptr;}
    if (_g) { delete _g; _g = nullptr;}
    if (_b) { delete _b; _b = nullptr;}
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

  Process*
  FillColor::clone ()
  {
    return new FillColor (_r->get_value (), _g->get_value (), _b->get_value ());
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

  Process*
  OutlineColor::clone ()
  {
    return new OutlineColor (_r->get_value (), _g->get_value (), _b->get_value ());
  }

  FillRule::FillRule (Process* p, const std::string &n, djnFillRuleType rule) :
      AbstractStyle (p, n)
  {
    _rule = new IntProperty (this, "rule", rule);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cr = new Coupling (_rule, ACTIVATION, update, ACTIVATION);
    _cr->disable ();
    Process::finalize ();
  }

  FillRule::FillRule (djnFillRuleType rule) :
      AbstractStyle ()
  {
    _rule = new IntProperty (this, "rule", rule);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cr = new Coupling (_rule, ACTIVATION, update, ACTIVATION);
    _cr->disable ();
  }

  FillRule::FillRule (Process* p, const std::string &n, int rule) :
      AbstractStyle (p, n)
  {
    _rule = new IntProperty (this, "rule", rule);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cr = new Coupling (_rule, ACTIVATION, update, ACTIVATION);
    _cr->disable ();
    Process::finalize ();
  }

  FillRule::FillRule (int rule) :
      AbstractStyle ()
  {
    _rule = new IntProperty (this, "rule", rule);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cr = new Coupling (_rule, ACTIVATION, update, ACTIVATION);
    _cr->disable ();
  }

  FillRule::~FillRule ()
  {
    if (_cr) { delete _cr; _cr = nullptr;}
    if (_rule) { delete _rule; _rule = nullptr;}
  }

  void
  FillRule::activate ()
  {
    AbstractStyle::activate ();
    _cr->enable (_frame);
  }

  void
  FillRule::deactivate ()
  {
    AbstractStyle::deactivate ();
    _cr->disable ();
  }

  void
  FillRule::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_fill_rule ((djnFillRuleType) (_rule->get_value ()));
    }
  }

  Process*
  FillRule::clone ()
  {
    return new FillRule (_rule->get_value ());
  }

  void
  NoOutline::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_no_outline ();
    }
  }

  Process*
  NoOutline::clone ()
  {
    return new NoOutline ();
  }

  void
  NoFill::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_no_fill ();
    }
  }

  Process*
  NoFill::clone ()
  {
    return new NoFill ();
  }

  Texture::Texture (Process* p, const std::string &n, const std::string &path) :
      AbstractStyle (p, n)
  {
    _path = new TextProperty (this, "path", path);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cp = new Coupling (_path, ACTIVATION, update, ACTIVATION);
    _cp->disable ();
    Process::finalize ();
  }

  Texture::Texture (const std::string &path) :
      AbstractStyle ()
  {
    _path = new TextProperty (this, "path", path);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cp = new Coupling (_path, ACTIVATION, update, ACTIVATION);
    _cp->disable ();
  }

  Texture::~Texture ()
  {
    if (_cp) { delete _cp; _cp = nullptr;}
    if (_path) { delete _path; _path = nullptr;}
  }

  void
  Texture::activate ()
  {
    AbstractStyle::activate ();
    _cp->enable (_frame);
  }

  void
  Texture::deactivate ()
  {
    AbstractStyle::deactivate ();
    _cp->disable ();
  }

  void
  Texture::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_texture (_path->get_value ());
    }
  }

  Process*
  Texture::clone ()
  {
    return new Texture (_path->get_value ());
  }

  AbstractOpacity::AbstractOpacity (Process* p, const std::string &n, double alpha) :
      AbstractStyle (p, n)
  {
    _alpha = new DoubleProperty (this, "a", alpha);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _ca = new Coupling (_alpha, ACTIVATION, update, ACTIVATION);
    _ca->disable ();
    Process::finalize ();
  }

  AbstractOpacity::AbstractOpacity (double alpha) :
      AbstractStyle ()
  {
    _alpha = new DoubleProperty (this, "a", alpha);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _ca = new Coupling (_alpha, ACTIVATION, update, ACTIVATION);
    _ca->disable ();
  }

  AbstractOpacity::~AbstractOpacity ()
  {
    if (_ca) { delete _ca; _ca = nullptr;}
    if (_alpha) { delete _alpha; _alpha = nullptr;}
  }

  void
  AbstractOpacity::activate ()
  {
    AbstractStyle::activate ();
    _ca->enable (_frame);
  }

  void
  AbstractOpacity::deactivate ()
  {
    AbstractStyle::deactivate ();
    _ca->disable ();
  }

  void
  OutlineOpacity::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_opacity ((float) _alpha->get_value ());
    }
  }

  Process*
  OutlineOpacity::clone ()
  {
    return new OutlineOpacity (_alpha->get_value ());
  }

  void
  FillOpacity::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_fill_opacity ((float) _alpha->get_value ());
    }
  }

  Process*
  FillOpacity::clone ()
  {
    return new FillOpacity (_alpha->get_value ());
  }

  OutlineWidth::OutlineWidth (Process* p, const std::string &n, double width) :
      AbstractStyle (p, n)
  {
    _width = new DoubleProperty (this, "width", width);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cw = new Coupling (_width, ACTIVATION, update, ACTIVATION);
    _cw->disable ();
    Process::finalize ();
  }

  OutlineWidth::OutlineWidth (double width) :
      AbstractStyle ()
  {
    _width = new DoubleProperty (this, "width", width);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cw = new Coupling (_width, ACTIVATION, update, ACTIVATION);
    _cw->disable ();
  }

  OutlineWidth::~OutlineWidth ()
  {
    if (_cw) { delete _cw; _cw = nullptr;}
    if (_width) { delete _width; _width = nullptr;}
  }

  void
  OutlineWidth::activate ()
  {
    AbstractStyle::activate ();
    _cw->enable (_frame);
  }

  void
  OutlineWidth::deactivate ()
  {
    AbstractStyle::deactivate ();
    _cw->disable ();
  }

  void
  OutlineWidth::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_width (_width->get_value ());
    }
  }

  Process*
  OutlineWidth::clone ()
  {
    return new OutlineWidth (_width->get_value ());
  }

  OutlineCapStyle::OutlineCapStyle (Process* p, const std::string &n, djnCapStyle cap) :
      AbstractStyle (p, n)
  {
    _cap = new IntProperty (this, "cap", cap);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cc = new Coupling (_cap, ACTIVATION, update, ACTIVATION);
    _cc->disable ();
    Process::finalize ();
  }

  OutlineCapStyle::OutlineCapStyle (Process* p, const std::string &n, int cap) :
      AbstractStyle (p, n)
  {
    _cap = new IntProperty (this, "cap", cap);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cc = new Coupling (_cap, ACTIVATION, update, ACTIVATION);
    _cc->disable ();
    Process::finalize ();
  }

  OutlineCapStyle::OutlineCapStyle (djnCapStyle cap) :
      AbstractStyle ()
  {
    _cap = new IntProperty (this, "cap", cap);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cc = new Coupling (_cap, ACTIVATION, update, ACTIVATION);
    _cc->disable ();
  }

  OutlineCapStyle::OutlineCapStyle (int cap) :
      AbstractStyle ()
  {
    _cap = new IntProperty (this, "cap", cap);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cc = new Coupling (_cap, ACTIVATION, update, ACTIVATION);
    _cc->disable ();
  }

  OutlineCapStyle::~OutlineCapStyle ()
  {
    if (_cc) { delete _cc; _cc = nullptr;}
    if (_cap) { delete _cap; _cap = nullptr;}
  }

  void
  OutlineCapStyle::activate ()
  {
    AbstractStyle::activate ();
    _cc->enable (_frame);
  }

  void
  OutlineCapStyle::deactivate ()
  {
    AbstractStyle::deactivate ();
    _cc->disable ();
  }

  void
  OutlineCapStyle::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_cap_style ((djnCapStyle) _cap->get_value ());
    }
  }

  Process* OutlineCapStyle::clone ()
  {
    return new OutlineCapStyle (_cap->get_value ());
  }

  OutlineJoinStyle::OutlineJoinStyle (Process* p, const std::string &n, djnJoinStyle join) :
      AbstractStyle (p, n)
  {
    _join = new IntProperty (this, "join", join);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cj = new Coupling (_join, ACTIVATION, update, ACTIVATION);
    _cj->disable ();
    Process::finalize ();
  }

  OutlineJoinStyle::OutlineJoinStyle (djnJoinStyle join) :
      AbstractStyle ()
  {
    _join = new IntProperty (this, "join", join);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cj = new Coupling (_join, ACTIVATION, update, ACTIVATION);
    _cj->disable ();
  }

  OutlineJoinStyle::OutlineJoinStyle (Process* p, const std::string &n, int join) :
      AbstractStyle (p, n)
  {
    _join = new IntProperty (this, "join", join);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cj = new Coupling (_join, ACTIVATION, update, ACTIVATION);
    _cj->disable ();
    Process::finalize ();
  }

  OutlineJoinStyle::OutlineJoinStyle (int join) :
      AbstractStyle ()
  {
    _join = new IntProperty (this, "join", join);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cj = new Coupling (_join, ACTIVATION, update, ACTIVATION);
    _cj->disable ();
  }

  OutlineJoinStyle::~OutlineJoinStyle ()
  {
    if (_cj) { delete _cj; _cj = nullptr;}
    if (_join) { delete _join; _join = nullptr;}
  }

  void
  OutlineJoinStyle::activate ()
  {
    AbstractStyle::activate ();
    _cj->enable (_frame);
  }

  void
  OutlineJoinStyle::deactivate ()
  {
    AbstractStyle::deactivate ();
    _cj->disable ();
  }

  void
  OutlineJoinStyle::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_join_style ((djnJoinStyle) _join->get_value ());
    }
  }

  Process* OutlineJoinStyle::clone ()
  {
    return new OutlineJoinStyle (_join->get_value ());
  }

  OutlineMiterLimit::OutlineMiterLimit (Process* p, const std::string &n, int limit) :
      AbstractStyle (p, n)
  {
    _limit = new IntProperty (this, "limit", limit);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cl = new Coupling (_limit, ACTIVATION, update, ACTIVATION);
    _cl->disable ();
    Process::finalize ();
  }

  OutlineMiterLimit::OutlineMiterLimit (int limit) :
      AbstractStyle ()
  {
    _limit = new IntProperty (this, "limit", limit);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cl = new Coupling (_limit, ACTIVATION, update, ACTIVATION);
    _cl->disable ();
  }

  OutlineMiterLimit::~OutlineMiterLimit ()
  {
    if (_cl) { delete _cl; _cl = nullptr;}
    if (_limit) { delete _limit; _limit = nullptr;}
  }

  void
  OutlineMiterLimit::activate ()
  {
    AbstractStyle::activate ();
    _cl->enable (_frame);
  }

  void
  OutlineMiterLimit::deactivate ()
  {
    AbstractStyle::deactivate ();
    _cl->disable ();
  }

  void
  OutlineMiterLimit::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_miter_limit (_limit->get_value ());
    }
  }

  Process*
  OutlineMiterLimit::clone ()
  {
    return new OutlineMiterLimit (_limit->get_value ());
  }

  void
  DashArray::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_dash_array (_dash_array);
    }
  }

  Process* DashArray::clone ()
  {
    DashArray* newda = new DashArray ();

    for (auto d : _dash_array) {
      newda->_dash_array.push_back(d);
    }

    return newda;
  }

  void
  NoDashArray::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_no_dash_array ();
    }
  }

  Process*
  NoDashArray::clone ()
  {
    return new NoDashArray ();
  }

  DashOffset::DashOffset (Process* p, const std::string &n, double offset) :
      AbstractStyle (p, n)
  {
    _offset = new DoubleProperty (this, "offset", offset);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _co = new Coupling (_offset, ACTIVATION, update, ACTIVATION);
    _co->disable ();
    Process::finalize ();
  }

  DashOffset::DashOffset (double offset) :
      AbstractStyle ()
  {
    _offset = new DoubleProperty (this, "offset", offset);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _co = new Coupling (_offset, ACTIVATION, update, ACTIVATION);
    _co->disable ();
  }

  DashOffset::~DashOffset ()
  {
    if (_co) { delete _co; _co = nullptr;}
    if (_offset) { delete _offset; _offset = nullptr;}
  }

  void
  DashOffset::activate ()
  {
    AbstractStyle::activate ();
    _co->enable (_frame);
  }

  void
  DashOffset::deactivate ()
  {
    AbstractStyle::deactivate ();
    _co->disable ();
  }

  void
  DashOffset::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_dash_offset (_offset->get_value ());
    }
  }

  Process*
  DashOffset::clone ()
  {
    return new DashOffset (_offset->get_value ());
  }

  GradientStop::GradientStop (Process *p, const std::string &n, double r, double g, double b, double a, double offset) :
      AbstractStyle (p, n)
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
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cr = new Coupling (_r, ACTIVATION, update, ACTIVATION);
    _cr->disable ();
    _cg = new Coupling (_g, ACTIVATION, update, ACTIVATION);
    _cg->disable ();
    _cb = new Coupling (_b, ACTIVATION, update, ACTIVATION);
    _cb->disable ();
    _ca = new Coupling (_a, ACTIVATION, update, ACTIVATION);
    _ca->disable ();
    _co = new Coupling (_offset, ACTIVATION, update, ACTIVATION);
    _co->disable ();
    grad->stops ()->add_child (this, "");
  }

  GradientStop::GradientStop (double r, double g, double b, double a, double offset) :
      AbstractStyle ()
  {
    _r = new DoubleProperty (this, "r", r);
    _g = new DoubleProperty (this, "g", g);
    _b = new DoubleProperty (this, "b", b);
    _a = new DoubleProperty (this, "a", a);
    _offset = new DoubleProperty (this, "offset", offset);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cr = new Coupling (_r, ACTIVATION, update, ACTIVATION);
    _cr->disable ();
    _cg = new Coupling (_g, ACTIVATION, update, ACTIVATION);
    _cg->disable ();
    _cb = new Coupling (_b, ACTIVATION, update, ACTIVATION);
    _cb->disable ();
    _ca = new Coupling (_a, ACTIVATION, update, ACTIVATION);
    _ca->disable ();
    _co = new Coupling (_offset, ACTIVATION, update, ACTIVATION);
    _co->disable ();
  }

  Process*
  GradientStop::clone ()
  {
    return new GradientStop (_r->get_value (), _g->get_value (), _b->get_value (), _a->get_value (),
			     _offset->get_value ());
  }

  GradientStop::~GradientStop ()
  {
    if (_cr) { delete _cr; _cr = nullptr;}
    if (_cg) { delete _cg; _cg = nullptr;}
    if (_cb) { delete _cb; _cb = nullptr;}
    if (_ca) { delete _ca; _ca = nullptr;}
    if (_co) { delete _co; _co = nullptr;}

    if (_r) { delete _r; _r = nullptr;}
    if (_g) { delete _g; _g = nullptr;}
    if (_b) { delete _b; _b = nullptr;}
    if (_a) { delete _a; _a = nullptr;}
    if (_offset) { delete _offset; _offset = nullptr;}
  }

  void
  GradientStop::activate ()
  {
    AbstractStyle::activate ();
    _cr->enable (_frame);
    _cg->enable (_frame);
    _cb->enable (_frame);
    _ca->enable (_frame);
    _co->enable (_frame);
  }

  void
  GradientStop::deactivate ()
  {
    AbstractStyle::deactivate ();
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
      AbstractStyle (p, n), _g (nullptr), _linear (false)
  {
    _spread = new IntProperty (this, "spread", s);
    _coords = new IntProperty (this, "coords", fc);
    _transforms = new List (this, "transforms");
    _stops = new List (this, "stops");
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cs = new Coupling (_spread, ACTIVATION, update, ACTIVATION);
    _cs->disable ();
    _cc = new Coupling (_coords, ACTIVATION, update, ACTIVATION);
    _cc->disable ();
  }

  AbstractGradient::AbstractGradient (int s, int fc) :
      AbstractStyle (), _g (nullptr), _linear (false)
  {
    _spread = new IntProperty (this, "spread", s);
    _coords = new IntProperty (this, "coords", fc);
    _transforms = new List (this, "transforms");
    _stops = new List (this, "stops");
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cs = new Coupling (_spread, ACTIVATION, update, ACTIVATION);
    _cs->disable ();
    _cc = new Coupling (_coords, ACTIVATION, update, ACTIVATION);
    _cc->disable ();
  }

  AbstractGradient::~AbstractGradient ()
  {
    if (_cs) { delete _cs; _cs = nullptr;}
    if (_cc) { delete _cc; _cc = nullptr;}
    if (_spread) { delete _spread; _spread = nullptr;}
    if (_coords) { delete _coords; _coords = nullptr;}
    if (_transforms) { delete _transforms; _transforms = nullptr;}
    if (_stops) { delete _stops; _stops = nullptr;}
  }

  void
  AbstractGradient::activate ()
  {
    AbstractStyle::activate ();
    _stops->activation ();
    _transforms->activation ();
    _cs->enable (_frame);
    _cc->enable (_frame);
  }

  void
  AbstractGradient::deactivate ()
  {
    AbstractStyle::deactivate ();
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
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cx1 = new Coupling (_x1, ACTIVATION, update, ACTIVATION);
    _cx1->disable ();
    _cy1 = new Coupling (_y1, ACTIVATION, update, ACTIVATION);
    _cy1->disable ();
    _cx2 = new Coupling (_x2, ACTIVATION, update, ACTIVATION);
    _cx2->disable ();
    _cy2 = new Coupling (_y2, ACTIVATION, update, ACTIVATION);
    _cy2->disable ();
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
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cx1 = new Coupling (_x1, ACTIVATION, update, ACTIVATION);
    _cx1->disable ();
    _cy1 = new Coupling (_y1, ACTIVATION, update, ACTIVATION);
    _cy1->disable ();
    _cx2 = new Coupling (_x2, ACTIVATION, update, ACTIVATION);
    _cx2->disable ();
    _cy2 = new Coupling (_y2, ACTIVATION, update, ACTIVATION);
    _cy2->disable ();
    Process::finalize ();
  }

  LinearGradient::LinearGradient (double x1, double y1, double x2, double y2, int s, int fc) :
      AbstractGradient (s, fc)
  {
    _x1 = new DoubleProperty (this, "x1", x1);
    _y1 = new DoubleProperty (this, "y1", y1);
    _x2 = new DoubleProperty (this, "x2", x2);
    _y2 = new DoubleProperty (this, "y2", y2);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cx1 = new Coupling (_x1, ACTIVATION, update, ACTIVATION);
    _cx1->disable ();
    _cy1 = new Coupling (_y1, ACTIVATION, update, ACTIVATION);
    _cy1->disable ();
    _cx2 = new Coupling (_x2, ACTIVATION, update, ACTIVATION);
    _cx2->disable ();
    _cy2 = new Coupling (_y2, ACTIVATION, update, ACTIVATION);
    _cy2->disable ();
  }

  Process*
  LinearGradient::clone ()
  {
    LinearGradient *g = new LinearGradient (_x1->get_value (), _y1->get_value (), _x2->get_value (), _y2->get_value (),
					    spread ()->get_value (), coords ()->get_value ());
    for (auto s : _stops->children ()) {
      g->stops ()->add_child (s->clone (), s->get_name ());
    }
    for (auto t : _transforms->children ()) {
      g->transforms ()->add_child (t->clone (), t->get_name ());
    }
    return g;
  }

  LinearGradient::~LinearGradient ()
  {
    if (_cx1) { delete _cx1; _cx1 = nullptr;}
    if (_cx2) { delete _cx2; _cx2 = nullptr;}
    if (_cy1) { delete _cy1; _cy1 = nullptr;}
    if (_cy2) { delete _cy2; _cy2 = nullptr;}
    
    if (_x1) { delete _x1; _x1 = nullptr;}
    if (_y1) { delete _y1; _y1 = nullptr;}
    if (_x2) { delete _x2; _x2 = nullptr;}
    if (_y2) { delete _y2; _y2 = nullptr;}
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
      AbstractStyle (p, n), _lg (lg)
  {
    activate_children ();
    Process::finalize ();
  }

  RefLinearGradient::RefLinearGradient (LinearGradient *lg) :
      AbstractStyle (), _lg (lg)
  {
    activate_children ();
    Process::finalize ();
  }

  void
  RefLinearGradient::activate_children ()
  {
    _lg->request_activation ();
    _lg->transforms ()->request_activation ();
    for (auto t : _lg->transforms ()->children ()) {
      t->request_activation ();
    }
    _lg->stops ()->request_activation ();
    for (auto s : _lg->stops ()->children ()) {
      s->request_activation ();
    }
  }

  void
  RefLinearGradient::activate ()
  {
    AbstractStyle::activate ();
  }

  void
  RefLinearGradient::deactivate ()
  {
    AbstractStyle::deactivate ();
  }

  void
  RefLinearGradient::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_linear_gradient (_lg);
    }
  }

  Process*
  RefLinearGradient::clone ()
  {
    return new RefLinearGradient (_lg);
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
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _ccx = new Coupling (_cx, ACTIVATION, update, ACTIVATION);
    _ccx->disable ();
    _ccy = new Coupling (_cy, ACTIVATION, update, ACTIVATION);
    _ccy->disable ();
    _cr = new Coupling (_r, ACTIVATION, update, ACTIVATION);
    _cr->disable ();
    _cfx = new Coupling (_fx, ACTIVATION, update, ACTIVATION);
    _cfx->disable ();
    _cfy = new Coupling (_fy, ACTIVATION, update, ACTIVATION);
    _cfy->disable ();
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
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _ccx = new Coupling (_cx, ACTIVATION, update, ACTIVATION);
    _ccx->disable ();
    _ccy = new Coupling (_cy, ACTIVATION, update, ACTIVATION);
    _ccy->disable ();
    _cr = new Coupling (_r, ACTIVATION, update, ACTIVATION);
    _cr->disable ();
    _cfx = new Coupling (_fx, ACTIVATION, update, ACTIVATION);
    _cfx->disable ();
    _cfy = new Coupling (_fy, ACTIVATION, update, ACTIVATION);
    _cfy->disable ();
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
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _ccx = new Coupling (_cx, ACTIVATION, update, ACTIVATION);
    _ccx->disable ();
    _ccy = new Coupling (_cy, ACTIVATION, update, ACTIVATION);
    _ccy->disable ();
    _cr = new Coupling (_r, ACTIVATION, update, ACTIVATION);
    _cr->disable ();
    _cfx = new Coupling (_fx, ACTIVATION, update, ACTIVATION);
    _cfx->disable ();
    _cfy = new Coupling (_fy, ACTIVATION, update, ACTIVATION);
    _cfy->disable ();
  }

  Process*
  RadialGradient::clone ()
  {
    RadialGradient *rg = new RadialGradient (_cx->get_value (), _cy->get_value (), _r->get_value (), _fx->get_value (),
					     _fy->get_value (), spread ()->get_value (), coords ()->get_value ());
    for (auto s : _stops->children ()) {
      rg->stops ()->add_child (s->clone (), s->get_name ());
    }
    for (auto t : _transforms->children ()) {
      rg->transforms ()->add_child (t->clone (), t->get_name ());
    }
    return rg;
  }

  RadialGradient::~RadialGradient ()
  {
    if (_ccx) { delete _ccx; _ccx = nullptr;}
    if (_ccy) { delete _ccy; _ccy = nullptr;}
    if (_cr) { delete _cr; _cr = nullptr;}
    if (_cfx) { delete _cfx; _cfx = nullptr;}
    if (_cfy) { delete _cfy; _cfy = nullptr;}
    
    if (_cx) { delete _cx; _cx = nullptr;}
    if (_cy) { delete _cy; _cy = nullptr;}
    if (_fx) { delete _fx; _fx = nullptr;}
    if (_fy) { delete _fy; _fy = nullptr;}
    if (_r) { delete _r; _r = nullptr;}
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
      AbstractStyle (p, n), _rg (rg)
  {
    activate_children ();
    Process::finalize ();
  }

  RefRadialGradient::RefRadialGradient (RadialGradient *rg) :
      AbstractStyle (), _rg (rg)
  {
    activate_children ();
    Process::finalize ();
  }

  void
  RefRadialGradient::activate_children ()
  {
    _rg->request_activation ();
    _rg->transforms ()->request_activation ();
    for (auto t : _rg->transforms ()->children ()) {
      t->request_activation ();
    }
    _rg->stops ()->request_activation ();
    for (auto s : _rg->stops ()->children ()) {
      s->request_activation ();
    }
  }

  void
  RefRadialGradient::activate ()
  {
    AbstractStyle::activate ();
  }

  void
  RefRadialGradient::deactivate ()
  {
    AbstractStyle::deactivate ();
  }

  void
  RefRadialGradient::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_radial_gradient (_rg);
    }
  }

  Process*
  RefRadialGradient::clone ()
  {
    return new RefRadialGradient (_rg);
  }

  void
  AbstractGradient::update ()
  {
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    if (_spread != (IntProperty*) find_component ("spread")) {
      delete _cs;
      delete _spread;
      _spread = (IntProperty*) find_component ("spread");
      _cs = new Coupling (_spread, ACTIVATION, update, ACTIVATION);
    }
    if (_coords != (IntProperty*) find_component ("coords")) {
      delete _cc;
      delete _coords;
      _coords = (IntProperty*) find_component ("coords");
      _cc = new Coupling (_coords, ACTIVATION, update, ACTIVATION);
    }
    if (_stops != (List*) find_component ("stops")) {
      delete _stops;
      _stops = (List*) find_component ("stops");
    }
    if (_transforms != (List*) find_component ("transforms")) {
      delete _transforms;
      _transforms = (List*) find_component ("transforms");
    }
  }

  void
  LinearGradient::update ()
  {
    AbstractGradient::update ();
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    if (_x1 != (DoubleProperty*) find_component ("x1")) {
      delete _cx1;
      delete _x1;
      _x1 = (DoubleProperty*) find_component ("x1");
      _cx1 = new Coupling (_x1, ACTIVATION, update, ACTIVATION);
    }
    if (_x2 != (DoubleProperty*) find_component ("x2")) {
      delete _cx2;
      delete _x2;
      _x2 = (DoubleProperty*) find_component ("x2");
      _cx2 = new Coupling (_x2, ACTIVATION, update, ACTIVATION);
    }
    if (_y1 != (DoubleProperty*) find_component ("y1")) {
      delete _cy1;
      delete _y1;
      _y1 = (DoubleProperty*) find_component ("y1");
      _cy1 = new Coupling (_y1, ACTIVATION, update, ACTIVATION);
    }
    if (_y2 != (DoubleProperty*) find_component ("y2")) {
      delete _cy2;
      delete _y2;
      _y2 = (DoubleProperty*) find_component ("y2");
      _cy2 = new Coupling (_y2, ACTIVATION, update, ACTIVATION);
    }
  }

  void
  RadialGradient::update ()
  {
    AbstractGradient::update ();
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    if (_cx != (DoubleProperty*) find_component ("cx")) {
      delete _ccx;
      delete _cx;
      _cx = (DoubleProperty*) find_component ("cx");
      _ccx = new Coupling (_cx, ACTIVATION, update, ACTIVATION);
    }
    if (_cy != (DoubleProperty*) find_component ("cy")) {
      delete _ccy;
      delete _cy;
      _cy = (DoubleProperty*) find_component ("cy");
      _ccy = new Coupling (_cy, ACTIVATION, update, ACTIVATION);
    }
    if (_r != (DoubleProperty*) find_component ("r")) {
      delete _cr;
      delete _r;
      _r = (DoubleProperty*) find_component ("r");
      _cr = new Coupling (_r, ACTIVATION, update, ACTIVATION);
    }
    if (_fx != (DoubleProperty*) find_component ("fx")) {
      delete _cfx;
      delete _fx;
      _fx = (DoubleProperty*) find_component ("fx");
      _cfx = new Coupling (_fx, ACTIVATION, update, ACTIVATION);
    }
    if (_fy != (DoubleProperty*) find_component ("fy")) {
      delete _cfy;
      delete _fy;
      _fy = (DoubleProperty*) find_component ("fy");
      _cfy = new Coupling (_fy, ACTIVATION, update, ACTIVATION);
    }
  }

  FontSize::FontSize (Process *p, const std::string &n, djnLengthUnit unit, double size) :
      AbstractStyle (p, n)
  {
    _unit = new IntProperty (this, "unit", unit);
    _size = new DoubleProperty (this, "size", size);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cu = new Coupling (_unit, ACTIVATION, update, ACTIVATION);
    _cu->disable ();
    _cs = new Coupling (_size, ACTIVATION, update, ACTIVATION);
    _cs->disable ();
    Process::finalize ();
  }

  FontSize::FontSize (djnLengthUnit unit, double size) :
      AbstractStyle ()
  {
    _unit = new IntProperty (this, "unit", unit);
    _size = new DoubleProperty (this, "size", size);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cu = new Coupling (_unit, ACTIVATION, update, ACTIVATION);
    _cu->disable ();
    _cs = new Coupling (_size, ACTIVATION, update, ACTIVATION);
    _cs->disable ();
  }

  FontSize::FontSize (Process *p, const std::string &n, int unit, double size) :
      AbstractStyle (p, n)
  {
    _unit = new IntProperty (this, "unit", unit);
    _size = new DoubleProperty (this, "size", size);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cu = new Coupling (_unit, ACTIVATION, update, ACTIVATION);
    _cu->disable ();
    _cs = new Coupling (_size, ACTIVATION, update, ACTIVATION);
    _cs->disable ();
    Process::finalize ();
  }

  FontSize::FontSize (int unit, double size) :
      AbstractStyle ()
  {
    _unit = new IntProperty (this, "unit", unit);
    _size = new DoubleProperty (this, "size", size);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cu = new Coupling (_unit, ACTIVATION, update, ACTIVATION);
    _cu->disable ();
    _cs = new Coupling (_size, ACTIVATION, update, ACTIVATION);
    _cs->disable ();
  }

  FontSize::~FontSize ()
  {
    if (_cu) { delete _cu; _cu = nullptr;}
    if (_cs) { delete _cs; _cs = nullptr;}
    
    if (_unit) { delete _unit; _unit = nullptr;}
    if (_size) { delete _size; _size = nullptr;}
  }

  void
  FontSize::activate ()
  {
    AbstractStyle::activate ();
    _cu->enable (_frame);
    _cs->enable (_frame);
    Container *c = dynamic_cast<Container*> (_parent);
    if (c) {
      c->add_to_context ("FontSize", this);
    }
  }

  void
  FontSize::deactivate ()
  {
    AbstractStyle::deactivate ();
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

  Process*
  FontSize::clone ()
  {
    return new FontSize (_unit->get_value (), _size->get_value ());
  }

  FontWeight::FontWeight (Process* p, const std::string &n, int weight) :
      AbstractStyle (p, n)
  {
    _weight = new IntProperty (this, "weight", weight);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cw = new Coupling (_weight, ACTIVATION, update, ACTIVATION);
    _cw->disable ();
    Process::finalize ();
  }

  FontWeight::FontWeight (int weight) :
      AbstractStyle ()
  {
    _weight = new IntProperty (this, "weight", weight);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cw = new Coupling (_weight, ACTIVATION, update, ACTIVATION);
    _cw->disable ();
  }

  FontWeight::~FontWeight ()
  {
    if (_cw) { delete _cw; _cw = nullptr;}
    if (_weight) { delete _weight; _weight = nullptr;}
  }

  void
  FontWeight::activate ()
  {
    AbstractStyle::activate ();
    _cw->enable (_frame);
    Container *c = dynamic_cast<Container*> (_parent);
    if (c)
      c->add_to_context ("FontWeight", this);
  }

  void
  FontWeight::deactivate ()
  {
    AbstractStyle::deactivate ();
    _cw->disable ();
  }

  void
  FontWeight::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_font_weight (_weight->get_value ());
    }
  }

  Process*
  FontWeight::clone ()
  {
    return new FontWeight (_weight->get_value ());
  }

  FontStyle::FontStyle (Process* p, const std::string &n, djnFontSlope style) :
      AbstractStyle (p, n)
  {
    _style = new IntProperty (this, "style", style);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cs = new Coupling (_style, ACTIVATION, update, ACTIVATION);
    _cs->disable ();
    Process::finalize ();
  }

  FontStyle::FontStyle (djnFontSlope style) :
      AbstractStyle ()
  {
    _style = new IntProperty (this, "style", style);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cs = new Coupling (_style, ACTIVATION, update, ACTIVATION);
    _cs->disable ();
  }

  FontStyle::FontStyle (Process* p, const std::string &n, int style) :
      AbstractStyle (p, n)
  {
    _style = new IntProperty (this, "style", style);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cs = new Coupling (_style, ACTIVATION, update, ACTIVATION);
    _cs->disable ();
    Process::finalize ();
  }

  FontStyle::FontStyle (int style) :
      AbstractStyle ()
  {
    _style = new IntProperty (this, "style", style);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cs = new Coupling (_style, ACTIVATION, update, ACTIVATION);
    _cs->disable ();
  }

  FontStyle::~FontStyle ()
  {
    if (_cs) { delete _cs; _cs = nullptr;}
    if (_style) { delete _style; _style = nullptr;}
  }

  void
  FontStyle::activate ()
  {
    AbstractStyle::activate ();
    _cs->enable (_frame);
    Container *c = dynamic_cast<Container*> (_parent);
    if (c)
      c->add_to_context ("FontStyle", this);
  }

  void
  FontStyle::deactivate ()
  {
    AbstractStyle::deactivate ();
    _cs->disable ();
  }

  void
  FontStyle::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_font_style ((djnFontSlope) _style->get_value ());
    }
  }

  Process*
  FontStyle::clone ()
  {
    return new FontStyle (_style->get_value ());
  }

  FontFamily::FontFamily (Process* p, const std::string &n, const std::string &family) :
      AbstractStyle (p, n)
  {
    _family = new TextProperty (this, "family", family);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cf = new Coupling (_family, ACTIVATION, update, ACTIVATION);
    _cf->disable ();
    Process::finalize ();
  }

  FontFamily::FontFamily (const std::string &family) :
      AbstractStyle ()
  {
    _family = new TextProperty (this, "family", family);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cf = new Coupling (_family, ACTIVATION, update, ACTIVATION);
    _cf->disable ();
  }

  FontFamily::~FontFamily ()
  {
    if (_cf) { delete _cf; _cf = nullptr;}
    if (_family) { delete _family; _family = nullptr;}
  }

  void
  FontFamily::activate ()
  {
    AbstractStyle::activate ();
    _cf->enable (_frame);
    Container *c = dynamic_cast<Container*> (_parent);
    if (c)
      c->add_to_context ("FontFamily", this);
  }

  void
  FontFamily::deactivate ()
  {
    AbstractStyle::deactivate ();
    _cf->disable ();
  }

  void
  FontFamily::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_font_family (_family->get_value ());
    }
  }

  Process*
  FontFamily::clone ()
  {
    return new FontFamily (_family->get_value ());
  }

  TextAnchor::TextAnchor (Process* p, const std::string &n, djnAnchorType anchor) :
      AbstractStyle (p, n)
  {
    _anchor = new IntProperty (this, "anchor", anchor);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _ca = new Coupling (_anchor, ACTIVATION, update, ACTIVATION);
    _ca->disable ();
    Process::finalize ();
  }

  TextAnchor::TextAnchor (djnAnchorType anchor) :
      AbstractStyle ()
  {
    _anchor = new IntProperty (this, "anchor", anchor);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _ca = new Coupling (_anchor, ACTIVATION, update, ACTIVATION);
    _ca->disable ();
  }

  TextAnchor::TextAnchor (Process* p, const std::string &n, int anchor) :
      AbstractStyle (p, n)
  {
    _anchor = new IntProperty (this, "anchor", anchor);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _ca = new Coupling (_anchor, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  TextAnchor::TextAnchor (int anchor) :
      AbstractStyle ()
  {
    _anchor = new IntProperty (this, "anchor", anchor);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _ca = new Coupling (_anchor, ACTIVATION, update, ACTIVATION);
  }

  TextAnchor::~TextAnchor ()
  {
    if (_ca) { delete _ca; _ca = nullptr;}
    if (_anchor) { delete _anchor; _anchor = nullptr;}
  }

  void
  TextAnchor::activate ()
  {
    AbstractStyle::activate ();
    _ca->enable (_frame);
  }

  void
  TextAnchor::deactivate ()
  {
    AbstractStyle::deactivate ();
    _ca->disable ();
  }

  void
  TextAnchor::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->load_text_anchor ((djnAnchorType) _anchor->get_value ());
    }
  }

  Process*
  TextAnchor::clone ()
  {
    return new TextAnchor (_anchor->get_value ());
  }
}

