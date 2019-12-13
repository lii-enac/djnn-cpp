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
 *
 */

#include "style.h"
#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display.h"
#include "display/abstract_display.h"
#include "display/window.h"
#include "core/ontology/coupling.h"

#include <iostream>

namespace djnn
{

  AbstractStyle::AbstractStyle (Process *parent, const string &name) :
      AbstractGObj (parent, name)
  {
  }

  AbstractStyle::~AbstractStyle ()
  {
  }

  void
  FillColor::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_fill_color (this);
    }
  }

  Process*
  FillColor::clone ()
  {
    return new FillColor (nullptr, get_name (), raw_props.r, raw_props.g, raw_props.b);
  }

  void
  OutlineColor::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_color (this);
    }
  }

  Process*
  OutlineColor::clone ()
  {
    return new OutlineColor (nullptr, get_name (),raw_props.r, raw_props.g, raw_props.b);
  }

  void
  NoOutline::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_no_outline ();
    }
  }

  Process*
  NoOutline::clone ()
  {
    return new NoOutline (nullptr, get_name ());
  }

  void
  NoFill::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_no_fill ();
    }
  }

  Process*
  NoFill::clone ()
  {
    return new NoFill (nullptr, get_name ());
  }
  
  void
  OutlineOpacity::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_opacity (this);
    }
  }

  Process*
  OutlineOpacity::clone ()
  {
    return new OutlineOpacity (nullptr, get_name (), raw_props.a);
  }

  void
  FillOpacity::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_fill_opacity (this);
    }
  }

  Process*
  FillOpacity::clone ()
  {
    return new FillOpacity (nullptr, get_name (), raw_props.a);
  }

  void
  DashArray::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_dash_array (this);
    }
  }

  Process*
  DashArray::clone ()
  {
    DashArray* newda = new DashArray (nullptr, get_name ());

    for (auto d : _dash_array) {
      newda->_dash_array.push_back(d);
    }

    return newda;
  }

  void
  NoDashArray::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_no_dash_array ();
    }
  }

  Process*
  NoDashArray::clone ()
  {
    return new NoDashArray (nullptr, get_name ());
  }

  GradientStop::GradientStop (Process *parent, const std::string &name, double r, double g, double b, double a, double offset) :
      AbstractPropGradientStop (parent, name, r, g, b, a, offset)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (parent);
    if (grad == nullptr) {
      cerr << "Parent of gradient stop must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->stops ()->add_child (this, "");
  }

  GradientStop::~GradientStop()
  {
  }

  Process*
  GradientStop::clone ()
  {
    return new GradientStop (nullptr, get_name (), raw_props.r, raw_props.g, raw_props.b, raw_props.a, raw_props.offset);
  }

  void
  GradientStop::draw ()
  {
    Backend::instance ()->load_gradient_stop (this);
  }

  AbstractGradient::AbstractGradient (Process *parent, const std::string &name, int spread, int coords) :
      AbstractPropGradient (parent, name, spread, coords),
      _g (nullptr), _linear (false)
  {
    _transforms = new List (this, "transforms");
    _stops = new List (this, "stops");
  }

  AbstractGradient::~AbstractGradient ()
  {
    delete _transforms;
    delete _stops;
  }

  LinearGradient::LinearGradient (Process *parent, const std::string &name, double x1, double y1, double x2, double y2,
				  djnFillSpread s, djnFillCoords fc) :
      AbstractPropLinearGradient (parent, name, x1, y1, x2, y2, s, fc)
  {
    _linear = true;
    Process::finalize_construction (parent, name);
  }

  LinearGradient::LinearGradient (Process *parent, const std::string &name, double x1, double y1, double x2, double y2, int s,
				  int fc) :
      AbstractPropLinearGradient (parent, name, x1, y1, x2, y2, s, fc)
  {
    _linear = true;
    Process::finalize_construction (parent, name);
  }

  Process*
  LinearGradient::clone ()
  {
    LinearGradient *g = new LinearGradient (nullptr, get_name (), raw_props.x1, raw_props.y1, raw_props.x2, raw_props.y2,
					    AbstractGradient::raw_props.spread, AbstractGradient::raw_props.coords);
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
  }

  void
  LinearGradient::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_linear_gradient (this);
    }
  }

  RefLinearGradient::RefLinearGradient (Process *parent, const string &name, LinearGradient *lg) :
      AbstractStyle (parent, name), _lg (lg)
  {
    activate_children ();
    Process::finalize_construction (parent, name);
  }

  void
  RefLinearGradient::activate_children ()
  {
    _lg->set_activation_flag (ACTIVATION);
    _lg->transforms ()->set_activation_flag (ACTIVATION);
    for (auto t : _lg->transforms ()->children ()) {
      t->set_activation_flag (ACTIVATION);
    }
    _lg->stops ()->set_activation_flag (ACTIVATION);
    for (auto s : _lg->stops ()->children ()) {
      s->set_activation_flag (ACTIVATION);
    }
  }

  void
  RefLinearGradient::impl_activate ()
  {
    AbstractStyle::impl_activate ();
  }

  void
  RefLinearGradient::impl_deactivate ()
  {
    AbstractStyle::impl_deactivate ();
  }

  void
  RefLinearGradient::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_linear_gradient (_lg);
    }
  }

  Process*
  RefLinearGradient::clone ()
  {
    return new RefLinearGradient (nullptr, get_name (), _lg);
  }

  RadialGradient::RadialGradient (Process *parent, const std::string &name, double cx, double cy, double r, double fx,
				  double fy, djnFillSpread s, djnFillCoords fc) :
      AbstractPropRadialGradient (parent, name, cx, cy, r, fx, fy, s, fc)
  {
    Process::finalize_construction (parent, name);
  }

  RadialGradient::RadialGradient (Process *parent, const std::string &name, double cx, double cy, double r, double fx,
				  double fy, int s, int fc) :
      AbstractPropRadialGradient (parent, name, cx, cy, r, fx, fy, s, fc)
  {
    Process::finalize_construction (parent, name);
  }

  RadialGradient::~RadialGradient () {}

  Process*
  RadialGradient::clone ()
  {
    RadialGradient *rg = new RadialGradient (nullptr, get_name (), raw_props.cx, raw_props.cy, raw_props.r, raw_props.fx,
					     raw_props.fy, AbstractGradient::raw_props.spread, AbstractGradient::raw_props.coords);
    for (auto s : _stops->children ()) {
      rg->stops ()->add_child (s->clone (), s->get_name ());
    }
    for (auto t : _transforms->children ()) {
      rg->transforms ()->add_child (t->clone (), t->get_name ());
    }
    return rg;
  }

  void
  RadialGradient::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_radial_gradient (this);
    }
  }

  RefRadialGradient::RefRadialGradient (Process *parent, const string &name, RadialGradient *rg) :
      AbstractStyle (parent, name), _rg (rg)
  {
    activate_children ();
    Process::finalize_construction (parent, name);
  }

  void
  RefRadialGradient::activate_children ()
  {
    _rg->set_activation_flag (ACTIVATION);
    _rg->transforms ()->set_activation_flag (ACTIVATION);
    for (auto t : _rg->transforms ()->children ()) {
      t->set_activation_flag (ACTIVATION);
    }
    _rg->stops ()->set_activation_flag (ACTIVATION);
    for (auto s : _rg->stops ()->children ()) {
      s->set_activation_flag (ACTIVATION);
    }
  }

  void
  RefRadialGradient::impl_activate ()
  {
    AbstractStyle::impl_activate ();
  }

  void
  RefRadialGradient::impl_deactivate ()
  {
    AbstractStyle::impl_deactivate ();
  }

  void
  RefRadialGradient::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_radial_gradient (_rg);
    }
  }

  Process*
  RefRadialGradient::clone ()
  {
    return new RefRadialGradient (nullptr, get_name (), _rg);
  }

  void
  AbstractGradient::update ()
  {
    if ( stops () != (List*) find_component ("stops")) {
      delete _stops;
      _stops = (List*) find_component ("stops");
    }
    if (_transforms != (List*) find_component ("transforms")) {
      delete _transforms;
      _transforms = (List*) find_component ("transforms");
     }
  }

  FontSize::FontSize (Process *parent, const std::string &name, djnLengthUnit unit, double size) :
      AbstractPropFontSize (parent, name, unit, size)
  {
    Process::finalize_construction (parent, name);
  }

  FontSize::FontSize (Process *parent, const std::string &name, int unit, double size) :
      AbstractPropFontSize (parent, name, unit, size)
  {
    Process::finalize_construction (parent, name);
  }


  void
  FontSize::impl_activate ()
  {
    AbstractPropFontSize::impl_activate ();
    Container *c = dynamic_cast<Container*> (get_parent ());
    if (c) {
      c->add_to_context ("FontSize", this);
    }
  }

  void
  FontSize::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_font_size (this);
    }
  }

  Process*
  FontSize::clone ()
  {
    return new FontSize (nullptr, get_name (), raw_props.unit, raw_props.size);
  }

  FontWeight::FontWeight (Process* parent, const std::string &name, int weight) :
      AbstractPropFontWeight (parent, name, weight) 
  {
    Process::finalize_construction (parent, name);
  }

  void
  FontWeight::impl_activate ()
  {
    AbstractPropFontWeight::impl_activate ();
    Container *c = dynamic_cast<Container*> (get_parent ());
    if (c)
      c->add_to_context ("FontWeight", this);
  }

  void
  FontWeight::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_font_weight (this);
    }
  }

  Process*
  FontWeight::clone ()
  {
    return new FontWeight (nullptr, get_name (), raw_props.weight);
  }

  FontStyle::FontStyle (Process* parent, const std::string &name, djnFontSlope style) :
      AbstractPropFontStyle (parent, name, style)
  {
    Process::finalize_construction (parent, name);
  }

  FontStyle::FontStyle (Process* parent, const std::string &name, int style) :
      AbstractPropFontStyle (parent, name, style)
  {
    Process::finalize_construction (parent, name);
  }

  void
  FontStyle::impl_activate ()
  {
    AbstractPropFontStyle::impl_activate ();
    Container *c = dynamic_cast<Container*> (get_parent ());
    if (c)
      c->add_to_context ("FontStyle", this);
  }

  void
  FontStyle::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_font_style (this);
    }
  }

  Process*
  FontStyle::clone ()
  {
    return new FontStyle (nullptr, get_name (), raw_props.style);
  }

  FontFamily::FontFamily (Process* parent, const std::string &name, const std::string &family) :
      AbstractPropFontFamily (parent, name, family)
  {
    Process::finalize_construction (parent, name);
  }

  void
  FontFamily::impl_activate ()
  {
    AbstractPropFontFamily::impl_activate ();
    Container *c = dynamic_cast<Container*> (get_parent ());
    if (c)
      c->add_to_context ("FontFamily", this);
  }

  void
  FontFamily::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_font_family (this);
    }
  }

  Process*
  FontFamily::clone ()
  {
    return new FontFamily (nullptr, get_name (), raw_props.family);
  }
}

