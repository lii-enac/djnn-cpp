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

#include "../backend.h"
#include "../abstract_backend.h"
#include "../../display/display.h"
#include "../../display/abstract_display.h"
#include "../../display/window.h"
#include "style.h"
#include "../../core/ontology/coupling.h"

#include <iostream>

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
/*
  AbstractColor::AbstractColor (double r, double g, double b) :
      raw_props{.r=r, .g=g, .b=b},
      _cr (nullptr), _cg (nullptr), _cb (nullptr)
  {
  }

  AbstractColor::AbstractColor (Process *p, const std::string& n, double r, double g, double b) :
      AbstractStyle (p, n),
      raw_props{.r=r, .g=g, .b=b},
      _cr (nullptr), _cg (nullptr), _cb (nullptr)
  {
    Process::finalize_construction (p);
  }

  AbstractColor::~AbstractColor ()
  {
    delete _cr;
    delete _cg;
    delete _cb;

    if (symtable ().empty () == false) {
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
    }
  }

  Process*
  AbstractColor::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="r") {
      coupling=&_cr;
      rawp=&raw_props.r;
      notify_mask = notify_damaged_style;
    } else
    if(name=="g") {
      coupling=&_cg;
      rawp=&raw_props.g;
      notify_mask = notify_damaged_style;
    } else
    if(name=="b") {
      coupling=&_cb;
      rawp=&raw_props.b;
      notify_mask = notify_damaged_style;
    }  else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr; // do not cache
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  AbstractColor::get_properties_values (double &r, double &g, double &b)
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
  }

  void
  AbstractColor::impl_deactivate ()
  {
    if (_cr) _cr->disable ();
    if (_cg) _cg->disable ();
    if (_cb) _cb->disable ();
    AbstractStyle::impl_deactivate ();
  }
*/
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
    return new FillColor (raw_props.r, raw_props.g, raw_props.b);
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
    return new OutlineColor (raw_props.r, raw_props.g, raw_props.b);
  }

  /*
  FillRule::FillRule (Process* p, const std::string &n, djnFillRuleType rule) :
      AbstractStyle (p, n),
      raw_props{.rule=rule},
      _cr (nullptr)
  {
    Process::finalize_construction (p);
  }

  FillRule::FillRule (djnFillRuleType rule) :
      AbstractStyle (),
      raw_props{.rule=rule},
      _cr (nullptr)
  {
  }

  FillRule::FillRule (Process* p, const std::string &n, int rule) :
      AbstractStyle (p, n),
      raw_props{.rule=rule},
      _cr (nullptr)
  {
    Process::finalize_construction (p);
  }

  FillRule::FillRule (int rule) :
      AbstractStyle (),
      raw_props{.rule=rule},
      _cr (nullptr)
  {;
  }

  FillRule::~FillRule ()
  {
    delete _cr;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("rule");
      if (it != symtable ().end ())
        delete it->second;
    }
  }

  Process*
  FillRule::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    int* rawp;
    int notify_mask = notify_none;

    if(name=="rule") {
      coupling=&_cr;
      rawp=&raw_props.rule;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    IntPropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  FillRule::get_properties_values (int &rule)
  {
    rule = raw_props.rule;
  }

  void
  FillRule::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if(_cr) _cr->enable (_frame);
  }

  void
  FillRule::impl_deactivate ()
  {
    AbstractStyle::impl_deactivate ();
    if (_cr) _cr->disable ();
  }


  void
  FillRule::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_fill_rule (this);
    }
  }


  Process*
  FillRule::clone ()
  {
    return new FillRule (raw_props.rule);
  }
  */

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
    return new NoOutline ();
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
    return new NoFill ();
  }
/*
  Texture::Texture (Process* p, const std::string &n, const std::string &path) :
      AbstractStyle (p, n),
      raw_props{.path=path},
      _cp (nullptr)
  {
    Process::finalize_construction (p);
  }

  Texture::Texture (const std::string &path) :
      AbstractStyle (),
      raw_props{.path=path},
      _cp (nullptr)
  {
  }

  Texture::~Texture ()
  {
    delete _cp;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("path");
      if (it != symtable ().end ())
        delete it->second;
    }
  }


  Process*
  Texture::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    string* rawp;
    int notify_mask = notify_none;

    if(name=="path") {
      coupling=&_cp;
      rawp=&raw_props.path;
      notify_mask = notify_damaged_style;
    } else
    return nullptr;
    
    TextPropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  Texture::get_properties_values (string &path)
  {
    path = raw_props.path;
  }

  void
  Texture::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if (_cp) _cp->enable (_frame);
  }

  void
  Texture::impl_deactivate ()
  {
    AbstractStyle::impl_deactivate ();
    if (_cp) _cp->disable ();
  }

  void
  Texture::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_texture (this);
    }
  }

  Process*
  Texture::clone ()
  {
    return new Texture (raw_props.path);
  }
*/
/*
  AbstractOpacity::AbstractOpacity (Process* p, const std::string &n, double alpha) :
      AbstractStyle (p, n),
      raw_props{.alpha=alpha},
      _ca (nullptr)
  {
    Process::finalize_construction (p);
  }

  AbstractOpacity::AbstractOpacity (double alpha) :
      AbstractStyle (),
      raw_props{.alpha=alpha},
      _ca (nullptr)
  {
  }

  AbstractOpacity::~AbstractOpacity ()
  {
    delete _ca;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("a");
      if (it != symtable ().end ())
        delete it->second;
    }
  }

  Process*
  AbstractOpacity::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="a") {
      coupling=&_ca;
      rawp=&raw_props.alpha;
      notify_mask = notify_damaged_style;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  AbstractOpacity::get_properties_values (double &alpha)
  {
    alpha = raw_props.alpha;
  }

  void
  AbstractOpacity::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if (_ca) _ca->enable (_frame);
  }

  void
  AbstractOpacity::impl_deactivate ()
  {
    AbstractStyle::impl_deactivate ();
    if (_ca) _ca->disable ();
  }
*/
  
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
    return new OutlineOpacity (raw_props.alpha);
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
    return new FillOpacity (raw_props.alpha);
  }

/*
  OutlineWidth::OutlineWidth (Process* p, const std::string &n, double width) :
      AbstractStyle (p, n),
      raw_props{.width=width},
      _cw (nullptr)
  {
    Process::finalize_construction (p);
  }

  OutlineWidth::OutlineWidth (double width) :
      AbstractStyle (),
      raw_props{.width=width},
      _cw (nullptr)
  {
  }


  OutlineWidth::~OutlineWidth ()
  {
    delete _cw;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("width");
      if (it != symtable ().end ())
        delete it->second;
    }
  }

  Process*
  OutlineWidth::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="width") {
      coupling=&_cw;
      rawp=&raw_props.width;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  OutlineWidth::get_properties_values (double &width)
  {
    width = raw_props.width;
  }

  void
  OutlineWidth::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if (_cw) _cw->enable (_frame);
  }

  void
  OutlineWidth::impl_deactivate ()
  {
    AbstractStyle::impl_deactivate ();
    if (_cw) _cw->disable ();
  }*/
/*
  void
  OutlineWidth::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_width (this);
    }
  }

  Process*
  OutlineWidth::clone ()
  {
    return new OutlineWidth (raw_props.width);
  }
*/
/*
  OutlineCapStyle::OutlineCapStyle (Process* p, const std::string &n, djnCapStyle cap) :
      AbstractStyle (p, n),
      raw_props{.cap=cap},
      _cc (nullptr)
  {
    Process::finalize_construction (p);
  }

  OutlineCapStyle::OutlineCapStyle (Process* p, const std::string &n, int cap) :
      AbstractStyle (p, n),
      raw_props{.cap=cap},
      _cc (nullptr)
  {
    Process::finalize_construction (p);
  }

  OutlineCapStyle::OutlineCapStyle (djnCapStyle cap) :
      AbstractStyle (),
      raw_props{.cap=cap},
      _cc (nullptr)
  {
  }

  OutlineCapStyle::OutlineCapStyle (int cap) :
      AbstractStyle (),
      raw_props{.cap=cap},
      _cc (nullptr)
  {
  }

  OutlineCapStyle::~OutlineCapStyle ()
  {
    delete _cc;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("cap");
      if (it != symtable ().end ())
        delete it->second;
    }
  }

  Process*
  OutlineCapStyle::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    int* rawp;
    int notify_mask = notify_none;

    if(name=="cap") {
      coupling=&_cc;
      rawp=&raw_props.cap;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    IntPropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  OutlineCapStyle::get_properties_values (int &cap)
  {
    cap = raw_props.cap;
  }

  void
  OutlineCapStyle::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if (_cc) _cc->enable (_frame);
  }

  void
  OutlineCapStyle::impl_deactivate ()
  {
    AbstractStyle::impl_deactivate ();
    if (_cc) _cc->disable ();
  }
*/
  /*
  void
  OutlineCapStyle::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_cap_style (this);
    }
  }

  Process* OutlineCapStyle::clone ()
  {
    return new OutlineCapStyle ((djnCapStyle) raw_props.cap);
  }
  */
/*
  OutlineJoinStyle::OutlineJoinStyle (Process* p, const std::string &n, djnJoinStyle join) :
      AbstractStyle (p, n),
      raw_props{.join=join},
      _cj (nullptr)
  {
    Process::finalize_construction (p);
  }

  OutlineJoinStyle::OutlineJoinStyle (djnJoinStyle join) :
      AbstractStyle (),
      raw_props{.join=join},
      _cj (nullptr)
  {
  }

  OutlineJoinStyle::OutlineJoinStyle (Process* p, const std::string &n, int join) :
      AbstractStyle (p, n),
      raw_props{.join=join},
      _cj (nullptr)
  {
    Process::finalize_construction (p);
  }

  OutlineJoinStyle::OutlineJoinStyle (int join) :
      AbstractStyle (),
      raw_props{.join=join},
      _cj (nullptr)
  {
  }

  OutlineJoinStyle::~OutlineJoinStyle ()
  {
    delete _cj;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("join");
      if (it != symtable ().end ())
        delete it->second;
    }
  }

  Process*
  OutlineJoinStyle::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    int* rawp;
    int notify_mask = notify_none;

    if(name=="join") {
      coupling=&_cj;
      rawp=&raw_props.join;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    IntPropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  OutlineJoinStyle::get_properties_values (int &join)
  {
    join = raw_props.join;
  }

  void
  OutlineJoinStyle::impl_activate ()
  {
    auto _frame = frame ();
    if (_cj) _cj->enable (_frame);
  }

  void
  OutlineJoinStyle::impl_deactivate ()
  {
    AbstractStyle::impl_deactivate ();
    if (_cj) _cj->disable ();
  }
*/
  /*
  void
  OutlineJoinStyle::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_join_style (this);
    }
  }

  Process* OutlineJoinStyle::clone ()
  {
    return new OutlineJoinStyle (raw_props.join);
  }*/
/*
  OutlineMiterLimit::OutlineMiterLimit (Process* p, const std::string &n, int limit) :
      AbstractStyle (p, n),
      raw_props{.limit=limit},
      _cl (nullptr)
  {
    Process::finalize_construction (p);
  }

  OutlineMiterLimit::OutlineMiterLimit (int limit) :
      AbstractStyle (),
      raw_props{.limit=limit},
      _cl (nullptr)
  {
  }

  OutlineMiterLimit::~OutlineMiterLimit ()
  {
    delete _cl;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("limit");
      if (it != symtable ().end ())
        delete it->second;
    }
  }

  Process*
  OutlineMiterLimit::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    int* rawp;
    int notify_mask = notify_none;

    if(name=="limit") {
      coupling=&_cl;
      rawp=&raw_props.limit;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    IntPropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  OutlineMiterLimit::get_properties_values (int &limit)
  {
    limit = raw_props.limit;
  }

  void
  OutlineMiterLimit::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if (_cl) _cl->enable (_frame);
  }

  void
  OutlineMiterLimit::impl_deactivate ()
  {
    AbstractStyle::impl_deactivate ();
    if (_cl) _cl->disable ();
  }
*/
  /*
  void
  OutlineMiterLimit::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_miter_limit (this);
    }
  }

  Process*
  OutlineMiterLimit::clone ()
  {
    return new OutlineMiterLimit (raw_props.limit);
  }*/

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
    DashArray* newda = new DashArray ();

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
    return new NoDashArray ();
  }

/*
  DashOffset::DashOffset (Process* p, const std::string &n, double offset) :
      AbstractStyle (p, n),
      raw_props{.offset=offset},
      _co (nullptr)
  {
    Process::finalize_construction (p);
  }

  DashOffset::DashOffset (double offset) :
      AbstractStyle (),
      raw_props{.offset=offset},
      _co (nullptr)
  {
  }

  DashOffset::~DashOffset ()
  {
    delete _co;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("offset");
      if (it != symtable ().end ())
        delete it->second;
    }
  }

  Process*
  DashOffset::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="offset") {
      coupling=&_co;
      rawp=&raw_props.offset;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  DashOffset::get_properties_values (double &offset)
  {
    offset = raw_props.offset;
  }


  void
  DashOffset::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if (_co) _co->enable (_frame);
  }

  void
  DashOffset::impl_deactivate ()
  {
    AbstractStyle::impl_deactivate ();
    if (_co) _co->disable ();
  }

  void
  DashOffset::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_dash_offset (this);
    }
  }

  Process*
  DashOffset::clone ()
  {
    return new DashOffset (raw_props.offset);
  }
*/

  GradientStop::GradientStop (Process *p, const std::string &n, double r, double g, double b, double a, double offset) :
      //AbstractStyle (p, n),
      AbstractPropGradientStop (p, n, r, g, b, a, offset)
      //raw_props{.r=r, .g=g, .b=b, .a=a, .offset=offset},
      //_cr (nullptr), _cg (nullptr), _cb (nullptr), _ca (nullptr), _co (nullptr)
  {
    AbstractGradient *grad = dynamic_cast<AbstractGradient*> (p);
    if (grad == nullptr) {
      cerr << "Parent of gradient stop must be <LinearGradient|RadialGradient>\n";
      return;
    }
    grad->stops ()->add_child (this, "");
  }

  GradientStop::GradientStop (double r, double g, double b, double a, double offset) :
      //AbstractStyle (),
      AbstractPropGradientStop (r, g, b, a, offset)
      //raw_props{.r=r, .g=g, .b=b, .a=a, .offset=offset},
      //_cr (nullptr), _cg (nullptr), _cb (nullptr), _ca (nullptr), _co (nullptr)
  {
  }

  GradientStop::~GradientStop()
  {
  }

  Process*
  GradientStop::clone ()
  {
    return new GradientStop (raw_props.r, raw_props.g, raw_props.b, raw_props.a, raw_props.offset);
  }
/*
  GradientStop::~GradientStop ()
  {
    delete _cr;
    delete _cg;
    delete _cb;
    delete _ca;
    delete _co;

    if (symtable ().empty () == false) {
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

      it = symtable ().find ("a");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("offset");
      if (it != symtable ().end ())
        delete it->second;
    }
  }

  Process*
  GradientStop::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="r") {
      coupling=&_cr;
      rawp=&raw_props.r;
      notify_mask = notify_damaged_style;
    } else
    if(name=="g") {
      coupling=&_cg;
      rawp=&raw_props.g;
      notify_mask = notify_damaged_style;
    } else
    if(name=="b") {
      coupling=&_cb;
      rawp=&raw_props.b;
      notify_mask = notify_damaged_style;
    } else
    if(name=="a") {
      coupling=&_ca;
      rawp=&raw_props.a;
      notify_mask = notify_damaged_style;
    } else
    if(name=="offset") {
      coupling=&_co;
      rawp=&raw_props.offset;
      notify_mask = notify_damaged_style;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  GradientStop::get_properties_values (double &r, double &g, double &b, double &a, double &offset)
  {
    r = raw_props.r;
    g = raw_props.g;
    b = raw_props.b;
    a = raw_props.a;
    offset = raw_props.offset;
  }

  void
  GradientStop::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if (_cr) _cr->enable (_frame);
    if (_cg) _cg->enable (_frame);
    if (_cb) _cb->enable (_frame);
    if (_ca) _ca->enable (_frame);
    if (_co) _co->enable (_frame);
  }

  void
  GradientStop::impl_deactivate ()
  {
    AbstractStyle::impl_deactivate ();
    if (_cr) _cr->disable ();
    if (_cg) _cg->disable ();
    if (_cb) _cb->disable ();
    if (_ca) _ca->disable ();
    if (_co) _co->disable ();
  }
*/
  void
  GradientStop::draw ()
  {
    Backend::instance ()->load_gradient_stop (this);
  }

  AbstractGradient::AbstractGradient (Process *p, const std::string &n, int spread, int coords) :
      AbstractPropGradient (p, n, spread, coords),
      //raw_props{.spread=spread, .coords=coords},
      //_cs(nullptr), _cc(nullptr),
      _g (nullptr), _linear (false)
  {
    _transforms = new List (this, "transforms");
    _stops = new List (this, "stops");
  }

  AbstractGradient::AbstractGradient (int spread, int coords) :
      AbstractPropGradient (spread, coords),
      //raw_props{.spread=spread, .coords=coords},
      //_cs(nullptr), _cc(nullptr),
      _g (nullptr), _linear (false)
  {
    _transforms = new List (this, "transforms");
    _stops = new List (this, "stops");
  }

  AbstractGradient::~AbstractGradient ()
  {
    // FIXME delete _transforms and _stops ???

/*    delete _cs;
    delete _cc;
    delete _transforms;
    delete _stops;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("spread");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("coords");
      if (it != symtable ().end ())
        delete it->second;
    }
    */
  }

  /*
  Process*
  AbstractGradient::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    int* rawp;
    int notify_mask = notify_none;

    if(name=="spread") {
      coupling=&_cs;
      rawp=&raw_props.spread;
      notify_mask = notify_damaged_style;
    } else
    if(name=="coords") {
      coupling=&_cc;
      rawp=&raw_props.coords;
      notify_mask = notify_damaged_style;
    }  else
    return nullptr;
    
    IntPropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  AbstractGradient::get_properties_values (int &spread, int &coords)
  {
    spread = raw_props.spread;
    coords = raw_props.coords;
  }


  void
  AbstractGradient::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    _stops->activate ();
    _transforms->activate ();
    //auto _frame = frame ();
    if (_cs) _cs->enable (_frame);
    if (_cc) _cc->enable (_frame);
  }

  void
  AbstractGradient::impl_deactivate ()
  {
    AbstractStyle::impl_deactivate ();
    _stops->deactivate ();
    _transforms->deactivate ();
    auto _frame = frame ();
    if (_cs) _cs->enable (_frame);
    if (_cc) _cc->enable (_frame);
  }
  */

  LinearGradient::LinearGradient (Process *p, const std::string &n, double x1, double y1, double x2, double y2,
				  djnFillSpread s, djnFillCoords fc) :
      AbstractPropLinearGradient (p, n, x1, y1, x2, y2, s, fc)
     // raw_props{.x1=x1, .y1=y1, .x2=x2, .y2=y2},
     // _cx1 (nullptr), _cy1 (nullptr), _cx2 (nullptr), _cy2 (nullptr)
  {
    _linear = true;
    Process::finalize_construction (p);
  }

  LinearGradient::LinearGradient (Process *p, const std::string &n, double x1, double y1, double x2, double y2, int s,
				  int fc) :
      AbstractPropLinearGradient (p, n, x1, y1, x2, y2, s, fc)
      //raw_props{.x1=x1, .y1=y1, .x2=x2, .y2=y2},
      //_cx1 (nullptr), _cy1 (nullptr), _cx2 (nullptr), _cy2 (nullptr)
  {
    _linear = true;
    Process::finalize_construction (p);
  }

  LinearGradient::LinearGradient (double x1, double y1, double x2, double y2, int s, int fc) :
      AbstractPropLinearGradient (x1, y1, x2, y2, s, fc)
      //raw_props{.x1=x1, .y1=y1, .x2=x2, .y2=y2},
      //_cx1 (nullptr), _cy1 (nullptr), _cx2 (nullptr), _cy2 (nullptr)
  {
    _linear = true;
  }

  Process*
  LinearGradient::clone ()
  {
    LinearGradient *g = new LinearGradient (raw_props.x1, raw_props.y1, raw_props.x2, raw_props.y2,
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

  /*
  LinearGradient::~LinearGradient ()
  {
    delete _cx1;
    delete _cx2;
    delete _cy1;
    delete _cy2;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("x1");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("y1");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("x2");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("y2");
      if (it != symtable ().end ())
        delete it->second;
    }

  }

  Process*
  LinearGradient::find_component (const string& name)
  {
    Process* res = AbstractGradient::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="x1") {
      coupling=&_cx1;
      rawp=&raw_props.x1;
      notify_mask = notify_damaged_style;
    } else
    if(name=="y1") {
      coupling=&_cy1;
      rawp=&raw_props.y1;
      notify_mask = notify_damaged_style;
    } else
    if(name=="x2") {
      coupling=&_cx2;
      rawp=&raw_props.x2;
      notify_mask = notify_damaged_style;
    } else
    if(name=="y2") {
      coupling=&_cy2;
      rawp=&raw_props.y2;
      notify_mask = notify_damaged_style;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  LinearGradient::get_properties_values (double &x1, double &y1, double &x2, double &y2)
  {
    x1 = raw_props.x1;
    y1 = raw_props.y1;
    x2 = raw_props.x2;
    y2 = raw_props.y2;
  }

  void
  LinearGradient::impl_activate ()
  {
    AbstractGradient::impl_activate ();
    auto _frame = frame ();
    if (_cx1) _cx1->enable (_frame);
    if (_cy1) _cy1->enable (_frame);
    if (_cx2) _cx2->enable (_frame);
    if (_cy2) _cy2->enable (_frame);
  }

  void
  LinearGradient::impl_deactivate ()
  {
    AbstractGradient::impl_deactivate ();
    if (_cx1) _cx1->disable ();
    if (_cy1) _cy1->disable ();
    if (_cx2) _cx2->disable ();
    if (_cy2) _cy2->disable ();
  }
  */
  void
  LinearGradient::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_linear_gradient (this);
    }
  }

  RefLinearGradient::RefLinearGradient (Process *p, const string &n, LinearGradient *lg) :
      AbstractStyle (p, n), _lg (lg)
  {
    activate_children ();
    Process::finalize_construction (p);
  }

  RefLinearGradient::RefLinearGradient (LinearGradient *lg) :
      AbstractStyle (), _lg (lg)
  {
    activate_children ();
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
    return new RefLinearGradient (_lg);
  }

  RadialGradient::RadialGradient (Process *p, const std::string &n, double cx, double cy, double r, double fx,
				  double fy, djnFillSpread s, djnFillCoords fc) :
      AbstractPropRadialGradient (p, n, cx, cy, r, fx, fy, s, fc)
      //raw_props{.cx=cx, .cy=cy, .r=r, .fx=fx, .fy=fy},
      //_ccx (nullptr), _ccy (nullptr), _cr (nullptr), _cfx (nullptr), _cfy (nullptr)
  {
    Process::finalize_construction (p);
  }

  RadialGradient::RadialGradient (Process *p, const std::string &n, double cx, double cy, double r, double fx,
				  double fy, int s, int fc) :
      AbstractPropRadialGradient (p, n, cx, cy, r, fx, fy, s, fc)
      //raw_props{.cx=cx, .cy=cy, .r=r, .fx=fx, .fy=fy},
      //_ccx (nullptr), _ccy (nullptr), _cr (nullptr), _cfx (nullptr), _cfy (nullptr)
  {
    Process::finalize_construction (p);
  }

  RadialGradient::RadialGradient (double cx, double cy, double r, double fx, double fy, int s, int fc) :
      AbstractPropRadialGradient (cx, cy, r, fx, fy, s, fc)
      //raw_props{.cx=cx, .cy=cy, .r=r, .fx=fx, .fy=fy},
      //_ccx (nullptr), _ccy (nullptr), _cr (nullptr), _cfx (nullptr), _cfy (nullptr)
  {
  }

  RadialGradient::~RadialGradient () {}

  Process*
  RadialGradient::clone ()
  {
    RadialGradient *rg = new RadialGradient (raw_props.cx, raw_props.cy, raw_props.r, raw_props.fx,
					     raw_props.fy, AbstractGradient::raw_props.spread, AbstractGradient::raw_props.coords);
    for (auto s : _stops->children ()) {
      rg->stops ()->add_child (s->clone (), s->get_name ());
    }
    for (auto t : _transforms->children ()) {
      rg->transforms ()->add_child (t->clone (), t->get_name ());
    }
    return rg;
  }
/*
  RadialGradient::~RadialGradient ()
  {
    delete _ccx;
    delete _ccy;
    delete _cr;
    delete _cfx;
    delete _cfy;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("cx");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("cy");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("r");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("fx");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("fy");
      if (it != symtable ().end ())
        delete it->second;
    }
  }

  Process*
  RadialGradient::find_component (const string& name)
  {
    Process* res = AbstractGradient::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="cx") {
      coupling=&_ccx;
      rawp=&raw_props.cx;
      notify_mask = notify_damaged_style;
    } else
    if(name=="cy") {
      coupling=&_ccy;
      rawp=&raw_props.cy;
      notify_mask = notify_damaged_style;
    } else
    if(name=="r") {
      coupling=&_cr;
      rawp=&raw_props.r;
      notify_mask = notify_damaged_style;
    } else
    if(name=="fx") {
      coupling=&_cfx;
      rawp=&raw_props.fx;
      notify_mask = notify_damaged_style;
    } else
    if(name=="fy") {
      coupling=&_cfy;
      rawp=&raw_props.fy;
      notify_mask = notify_damaged_style;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  RadialGradient::get_properties_values (double &cx, double &cy, double &r, double &fx, double &fy)
  {
    cx = raw_props.cx;
    cy = raw_props.cy;
    r = raw_props.r;
    fx = raw_props.fx;
    fy = raw_props.fy;
  }

  void
  RadialGradient::impl_activate ()
  {
    AbstractGradient::impl_activate ();
    auto _frame = frame ();
    if (_ccx) _ccx->enable (_frame);
    if (_ccy) _ccy->enable (_frame);
    if (_cr) _cr->enable (_frame);
    if (_cfx) _cfx->enable (_frame);
    if (_cfy) _cfy->enable (_frame);
  }

  void
  RadialGradient::impl_deactivate ()
  {
    AbstractGradient::impl_deactivate ();
    if (_ccx) _ccx->disable ();
    if (_ccy) _ccy->disable ();
    if (_cr) _cr->disable ();
    if (_cfx) _cfx->disable ();
    if (_cfy) _cfy->disable ();
  }
*/
  void
  RadialGradient::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_radial_gradient (this);
    }
  }

  RefRadialGradient::RefRadialGradient (Process *p, const string &n, RadialGradient *rg) :
      AbstractStyle (p, n), _rg (rg)
  {
    activate_children ();
    Process::finalize_construction (p);
  }

  RefRadialGradient::RefRadialGradient (RadialGradient *rg) :
      AbstractStyle (), _rg (rg)
  {
    activate_children ();
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
    return new RefRadialGradient (_rg);
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
/*
  void
  LinearGradient::update ()
  {
    AbstractGradient::update ();
  }

  void
  RadialGradient::update ()
  {
    AbstractGradient::update ();
  }
*/

  FontSize::FontSize (Process *p, const std::string &n, djnLengthUnit unit, double size) :
      AbstractPropFontSize (p, n, unit, size)
      //raw_props{.unit=unit, .size=size},
      //_cu (nullptr), _cs (nullptr)
  {
    Process::finalize_construction (p);
  }

  FontSize::FontSize (djnLengthUnit unit, double size) :
      AbstractPropFontSize (unit, size) 
      //raw_props{.unit=unit, .size=size},
      //_cu (nullptr), _cs (nullptr)
  {
  }

  FontSize::FontSize (Process *p, const std::string &n, int unit, double size) :
      AbstractPropFontSize (p, n, unit, size)
      //raw_props{.unit=unit, .size=size},
      //_cu (nullptr), _cs (nullptr)
  {
    Process::finalize_construction (p);
  }

  FontSize::FontSize (int unit, double size) :
      AbstractPropFontSize (unit, size)
      //raw_props{.unit=unit, .size=size},
      //_cu (nullptr), _cs (nullptr)
  {
  }

/*
  FontSize::~FontSize ()
  {
    delete _cu;
    delete _cs;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("unit");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("size");
      if (it != symtable ().end ())
        delete it->second;
    }
  }

  Process*
  FontSize::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    bool propd = false;
    Coupling ** coupling;
    double* rawpd;
    int* rawpi;
    int notify_mask = notify_none;

    if(name=="unit") {
      coupling=&_cu;
      rawpi=&raw_props.unit;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="size") {
      propd=true;
      coupling=&_cs;
      rawpd=&raw_props.size;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    if (propd) {
      DoublePropertyProxy* prop = nullptr;
      res = create_GObj_prop (&prop, coupling, rawpd, name, notify_mask);
    }
    else {
      IntPropertyProxy* prop = nullptr;
       res = create_GObj_prop (&prop, coupling, rawpi, name, notify_mask);
    }

    return res;
  }

  void
  FontSize::get_properties_values (int &unit, double &size)
  {
    unit = raw_props.unit;
    size = raw_props.size;
  }
*/
  void
  FontSize::impl_activate ()
  {
    AbstractPropFontSize::impl_activate ();
    //if (_cu) _cu->enable (frame);
    //if (_cs) _cs->enable (frame);
    Container *c = dynamic_cast<Container*> (get_parent ());
    if (c) {
      c->add_to_context ("FontSize", this);
    }
  }

  /*void
  FontSize::impl_deactivate ()
  {
    AbstractStyle::impl_deactivate ();
    if (_cu) _cu->disable ();
    if (_cs) _cs->disable ();
  }
*/
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
    return new FontSize (raw_props.unit, raw_props.size);
  }

  FontWeight::FontWeight (Process* p, const std::string &n, int weight) :
      AbstractPropFontWeight (p, n, weight) 
      //raw_props{.weight=weight},
      //_cw (nullptr)
  {
    Process::finalize_construction (p);
  }

  FontWeight::FontWeight (int weight) :
      AbstractPropFontWeight (weight)
      //raw_props{.weight=weight},
      //_cw (nullptr)
  {
  }
/*
  FontWeight::~FontWeight ()
  {
    delete _cw;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("weight");
      if (it != symtable ().end ())
        delete it->second;
    }
  }

  Process*
  FontWeight::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    int * rawp;
    int notify_mask = notify_none;

    if(name=="weight") {
      coupling=&_cw;
      rawp=&raw_props.weight;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    IntPropertyProxy* prop = nullptr;  
    res = create_GObj_prop (&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  FontWeight::get_properties_values (int &weight)
  {
    weight = raw_props.weight;
  }
*/
  void
  FontWeight::impl_activate ()
  {
    AbstractPropFontWeight::impl_activate ();
    //if (_cw) _cw->enable (_frame);
    Container *c = dynamic_cast<Container*> (get_parent ());
    if (c)
      c->add_to_context ("FontWeight", this);
  }

  /*void
  FontWeight::impl_deactivate ()
  {
    AbstractStyle::impl_deactivate ();
    if (_cw) _cw->disable ();
  }
*/
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
    return new FontWeight (raw_props.weight);
  }

  FontStyle::FontStyle (Process* p, const std::string &n, djnFontSlope style) :
      AbstractPropFontStyle (p, n, style)
      //raw_props{.style=style},
      //_cs (nullptr)
  {
    Process::finalize_construction (p);
  }

  FontStyle::FontStyle (djnFontSlope style) :
      AbstractPropFontStyle (style)
      //raw_props{.style=style},
      //_cs (nullptr)
  {
  }

  FontStyle::FontStyle (Process* p, const std::string &n, int style) :
      AbstractPropFontStyle (p, n, style)
      //raw_props{.style=style},
      //_cs (nullptr)
  {
    Process::finalize_construction (p);
  }

  FontStyle::FontStyle (int style) :
      AbstractPropFontStyle (style)
      //raw_props{.style=style},
      //_cs (nullptr)
  {
  }
/*
  FontStyle::~FontStyle ()
  {
    delete _cs;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("style");
      if (it != symtable ().end ())
        delete it->second;
    }
  }

  Process*
  FontStyle::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    int * rawp;
    int notify_mask = notify_none;

    if(name=="style") {
      coupling=&_cs;
      rawp=&raw_props.style;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    IntPropertyProxy* prop = nullptr;  
    res = create_GObj_prop (&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  FontStyle::get_properties_values (int &style)
  {
    style = raw_props.style;
  }
*/
  void
  FontStyle::impl_activate ()
  {
    AbstractPropFontStyle::impl_activate ();
    //if (_cs) _cs->enable (_frame);
    Container *c = dynamic_cast<Container*> (get_parent ());
    if (c)
      c->add_to_context ("FontStyle", this);
  }
/*
  void
  FontStyle::impl_deactivate ()
  {
    AbstractStyle::impl_deactivate ();
    if (_cs) _cs->disable ();
  }
*/
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
    return new FontStyle (raw_props.style);
  }

  FontFamily::FontFamily (Process* p, const std::string &n, const std::string &family) :
      AbstractPropFontFamily (p, n, family)
      //raw_props{.family=family},
      //_cf (nullptr)
  {
    Process::finalize_construction (p);
  }

  FontFamily::FontFamily (const std::string &family) :
      AbstractPropFontFamily (family)
      //raw_props{.family=family},
      //_cf (nullptr)
  {
  }
/*
  FontFamily::~FontFamily ()
  {
    delete _cf;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("family");
      if (it != symtable ().end ())
        delete it->second;
    }
  }

  Process*
  FontFamily::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    string * rawp;
    int notify_mask = notify_none;

    if(name=="family") {
      coupling=&_cf;
      rawp=&raw_props.family;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    TextPropertyProxy* prop = nullptr;  
    res = create_GObj_prop (&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  FontFamily::get_properties_values (string &family)
  {
    family = raw_props.family;
  }
*/
  void
  FontFamily::impl_activate ()
  {
    AbstractPropFontFamily::impl_activate ();
    //if (_cf) _cf->enable (_frame);
    Container *c = dynamic_cast<Container*> (get_parent ());
    if (c)
      c->add_to_context ("FontFamily", this);
  }
/*
  void
  FontFamily::impl_deactivate ()
  {
    AbstractStyle::impl_deactivate ();
    if (_cf) _cf->disable ();
  }
*/
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
    return new FontFamily (raw_props.family);
  }



/*
  TextAnchor::TextAnchor (Process* p, const std::string &n, djnAnchorType anchor) :
      AbstractPropTextAnchor (p, n, anchor)
      //raw_props{.anchor=anchor},
      //_ca (nullptr)
  {
    Process::finalize_construction (p);
  }

  TextAnchor::TextAnchor (djnAnchorType anchor) :
      AbstractPropTextAnchor (anchor) 
      //raw_props{.anchor=anchor},
      //_ca (nullptr)
  {
  }

  TextAnchor::TextAnchor (Process* p, const std::string &n, int anchor) :
      AbstractPropTextAnchor (p, n, anchor), 
      //raw_props{.anchor=anchor},
      //_ca (nullptr)
  {
    Process::finalize_construction (p);
  }

  TextAnchor::TextAnchor (int anchor) :
      AbstractPropTextAnchor (anchor) 
      //raw_props{.anchor=anchor},
      //_ca (nullptr)
  {
  }

  TextAnchor::~TextAnchor ()
  {
    delete _ca;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("anchor");
      if (it != symtable ().end ())
        delete it->second;
    }
  }

  Process*
  TextAnchor::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    int * rawp;
    int notify_mask = notify_none;

    if(name=="anchor") {
      coupling=&_ca;
      rawp=&raw_props.anchor;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    IntPropertyProxy* prop = nullptr;  
    res = create_GObj_prop (&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  TextAnchor::get_properties_values (int &anchor)
  {
    anchor = raw_props.anchor;
  }

  void
  TextAnchor::impl_activate ()
  {
    AbstractPropTextAnchor::impl_activate ();
    //if (_ca) _ca->enable (_frame);
    Container *c = dynamic_cast<Container*> (get_parent ());
    if (c)
      c->add_to_context ("FontFamily", this);
  }

  void
  TextAnchor::impl_deactivate ()
  {
    AbstractStyle::impl_deactivate ();
    if (_ca) _ca->disable ();
  }

  void
  TextAnchor::draw ()
  {
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_text_anchor (this);
    }
  }

  Process*
  TextAnchor::clone ()
  {
    return new TextAnchor (raw_props.anchor);
  }
  */
}

