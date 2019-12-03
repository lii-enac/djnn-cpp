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

#pragma once

#include "style_types.h"

#include "core/tree/list.h"
#include "gui/abstract_gobj.h"
#include "core/ontology/process.h"

#include <vector>


namespace djnn
{

  class AbstractStyle : public AbstractGObj
  {
  public:
    AbstractStyle (Process *parent, const string &name);
    AbstractStyle ();
    virtual ~AbstractStyle ();
  };
}


#include "gen/fill_rule.h"
#include "gen/texture.h"
#include "gen/outline_width.h"
#include "gen/outline_cap_style.h"
#include "gen/outline_join_style.h"
#include "gen/outline_miter_limit.h"
#include "gen/dash_offset.h"
#include "gen/abstract_prop_gradient_stop.h"
#include "gen/text_anchor.h"

#include "gen/abstract_color.h"
#include "gen/abstract_opacity.h"

#include "gen/abstract_prop_font_family.h"
#include "gen/abstract_prop_font_size.h"
#include "gen/abstract_prop_font_style.h"
#include "gen/abstract_prop_font_weight.h"

namespace djnn
{

  /*
  class AbstractColor : public AbstractStyle
  {
  public:
    AbstractColor (Process *parent, const std::string& name, double r, double g, double b);
    AbstractColor (double r, double g, double b);
    virtual ~AbstractColor ();
    virtual void draw () override = 0;
    void get_properties_values (double &r, double &g, double &b);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* r () { return (AbstractDoubleProperty*) find_component("r"); }
    AbstractDoubleProperty* g () { return (AbstractDoubleProperty*) find_component("g"); }
    AbstractDoubleProperty* b () { return (AbstractDoubleProperty*) find_component("b"); }

  protected:
    struct raw_props_t { double r, g, b; };
    raw_props_t raw_props;
    Coupling *_cr, *_cg, *_cb;
    void impl_activate () override;
    void impl_deactivate () override;
  };
  */

  class FillColor : public AbstractColor
  {
  public:
    FillColor (Process *parent, const std::string& name, int r, int g, int b) :
        AbstractColor (parent, name, r, g, b) { Process::finalize_construction (parent, name); }
    FillColor (Process *parent, const std::string& name, int v) :
        AbstractColor (parent, name, v) { Process::finalize_construction (parent, name); }
    FillColor (int r, int g, int b) :
        AbstractColor (r, g, b) {}
    virtual ~FillColor () {}
    void draw () override;
    Process* clone () override;
  };

  class OutlineColor : public AbstractColor
  {
  public:
    OutlineColor (Process *parent, const std::string& name, int r, int g, int b) :
        AbstractColor (parent, name, r, g, b) { Process::finalize_construction (parent, name); }
    OutlineColor (Process *parent, const std::string& name, int v) :
         AbstractColor (parent, name, v) { Process::finalize_construction (parent, name); }
    OutlineColor (int r, int g, int b) :
        AbstractColor (r, g, b) {}
    virtual ~OutlineColor () {}
    void draw () override;
    Process* clone () override;
  };

  class NoOutline : public AbstractStyle
  {
  public:
    NoOutline (Process *parent, const std::string &name) :
        AbstractStyle (parent, name) { Process::finalize_construction (parent, name); }
    NoOutline () :
        AbstractStyle () {}    
    virtual ~NoOutline () {}
    void impl_activate () override { AbstractStyle::impl_activate ();}
    void impl_deactivate () override { AbstractStyle::impl_deactivate ();}
    void draw () override;
    Process* clone () override;
  };

  class NoFill : public AbstractStyle
  {
  public:
    NoFill (Process *parent, const std::string &name) :
        AbstractStyle (parent, name) { Process::finalize_construction (parent, name); }
    NoFill () :
        AbstractStyle () {}
    virtual ~NoFill () {}
    void impl_activate () override { AbstractStyle::impl_activate ();}
    void impl_deactivate () override { AbstractStyle::impl_deactivate ();}
    void draw () override;
    Process* clone () override;
  };

/*
  class FillRule : public AbstractStyle
  {
  public:
    FillRule (Process *parent, const std::string &name, djnFillRuleType rule);
    FillRule (djnFillRuleType rule);
    FillRule (Process *parent, const std::string &name, int rule);
    FillRule (int rule);
    virtual ~FillRule ();
    void get_properties_values (int &rule);
    virtual Process* find_component (const string&) override;
    AbstractIntProperty* rule () { return (AbstractIntProperty*) find_component("rule"); }
    void impl_activate () override;
    void impl_deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    struct raw_props_t { int rule; };
    raw_props_t raw_props;
    Coupling* _cr;
  };

  
  class Texture : public AbstractStyle
  {
  public:
    Texture (Process *parent, const std::string &name, const std::string &path);
    Texture (const std::string &path);
    virtual ~Texture ();
    void get_properties_values (string &path);
    virtual Process* find_component (const string&) override;
    AbstractTextProperty* path () { return (AbstractTextProperty*) find_component("path"); }
    void impl_activate () override;
    void impl_deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    struct raw_props_t { string path; };
    raw_props_t raw_props;
    Coupling* _cp;
  };
*/
  /*
  class AbstractOpacity : public AbstractStyle
  {
  public:
    AbstractOpacity (Process *parent, const std::string &name, double alpha);
    AbstractOpacity (double alpha);
    virtual ~AbstractOpacity ();
    void get_properties_values (double &alpha);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* alpha () { return (AbstractDoubleProperty*) find_component("a"); }
    void impl_activate () override;
    void impl_deactivate () override;
    void draw () override = 0;
  protected:
    struct raw_props_t { double alpha; };
    raw_props_t raw_props;
    Coupling* _ca;
  };
*/
  class OutlineOpacity : public AbstractOpacity
  {
  public:
    OutlineOpacity (Process *parent, const std::string &name, double alpha) :
        AbstractOpacity (parent, name, alpha) { Process::finalize_construction (parent, name); }
    OutlineOpacity (double alpha) :
        AbstractOpacity (alpha) {}
    virtual ~OutlineOpacity () {}
    void draw () override;
    Process* clone () override;
  };

  class FillOpacity : public AbstractOpacity
  {
  public:
    FillOpacity (Process *parent, const std::string &name, double alpha) :
        AbstractOpacity (parent, name, alpha) { Process::finalize_construction (parent, name); }
    FillOpacity (double alpha) :
        AbstractOpacity (alpha) {}
    virtual ~FillOpacity () {}
    void draw () override;
    Process* clone () override;
  };

/*
  class OutlineWidth : public AbstractStyle
  {
  public:
    OutlineWidth (Process *parent, const std::string &name, double width);
    OutlineWidth (double width);
    virtual ~OutlineWidth ();
    void get_properties_values (double &width);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* width () { return (AbstractDoubleProperty*) find_component("width"); }
    void impl_activate () override;
    void impl_deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    struct raw_props_t { double width; };
    raw_props_t raw_props;
    Coupling* _cw;
  };

  class OutlineCapStyle : public AbstractStyle
  {
  public:
    OutlineCapStyle (Process *parent, const std::string &name, djnCapStyle cap);
    OutlineCapStyle (djnCapStyle cap);
    OutlineCapStyle (Process *parent, const std::string &name, int cap);
    OutlineCapStyle (int cap);
    virtual ~OutlineCapStyle ();
    void get_properties_values (int &cap);
    virtual Process* find_component (const string&) override;
    AbstractIntProperty* cap () { return (AbstractIntProperty*) find_component("cap"); }
    void impl_activate () override;
    void impl_deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    struct raw_props_t { int cap; };
    raw_props_t raw_props;
    Coupling* _cc;
  };

  class OutlineJoinStyle : public AbstractStyle
  {
  public:
    OutlineJoinStyle (Process *parent, const std::string &name, djnJoinStyle join);
    OutlineJoinStyle (djnJoinStyle join);
    OutlineJoinStyle (Process *parent, const std::string &name, int join);
    OutlineJoinStyle (int join);
    virtual ~OutlineJoinStyle ();
    void get_properties_values (int &join);
    virtual Process* find_component (const string&) override;
    AbstractIntProperty* join () { return (AbstractIntProperty*) find_component("join"); }
    void impl_activate () override;
    void impl_deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    struct raw_props_t { int join; };
    raw_props_t raw_props;
    Coupling* _cj;
  };

  class OutlineMiterLimit : public AbstractStyle
  {
  public:
    OutlineMiterLimit (Process *parent, const std::string &name, int limit);
    OutlineMiterLimit (int limit);
    virtual ~OutlineMiterLimit ();
    void get_properties_values (int &limit);
    virtual Process* find_component (const string&) override;
    AbstractIntProperty* limit () { return (AbstractIntProperty*) find_component("limit"); }
    void impl_activate () override;
    void impl_deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    struct raw_props_t { int limit; };
    raw_props_t raw_props;
    Coupling* _cl;
  };
*/
  class DashArray : public AbstractStyle
  {
  public:
    DashArray (Process *parent, const std::string &name) :
        AbstractStyle (parent, name) { Process::finalize_construction (parent, name); }
    DashArray () :
        AbstractStyle () {}
    virtual ~DashArray () { _dash_array.clear ();}
    const vector<double>& dash_array () const { return _dash_array;}
    void add_sub_pattern (double dash, double space)
    {
      _dash_array.push_back (dash);
      _dash_array.push_back (space);
    }
    void impl_activate () override { AbstractStyle::impl_activate ();}
    void impl_deactivate () override { AbstractStyle::impl_deactivate ();}
    void draw () override;
    Process* clone () override;
  private:
    vector<double> _dash_array;
  };

  class NoDashArray : public AbstractStyle
  {
  public:
    NoDashArray (Process *parent, const std::string &name) :
        AbstractStyle (parent, name) { Process::finalize_construction (parent, name); }
    NoDashArray () :
        AbstractStyle () {}
    virtual ~NoDashArray () {}
    void impl_activate () override { AbstractStyle::impl_activate ();}
    void impl_deactivate () override { AbstractStyle::impl_deactivate ();}
    void draw () override;
    Process* clone () override;
  };

/*
  class DashOffset : public AbstractStyle
  {
  public:
    DashOffset (Process *parent, const std::string &name, double offset);
    DashOffset (double offset);
    virtual ~DashOffset ();
    void get_properties_values (double &offset);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* offset () { return (AbstractDoubleProperty*) find_component("offset"); }
    void impl_activate () override;
    void impl_deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    struct raw_props_t { double offset; };
    raw_props_t raw_props;
    Coupling* _co;
  };
*/
}

//#include "gen/abstract_prop_gradient_stop.h"

namespace djnn {
  class GradientStop : public AbstractPropGradientStop
  {
  public:
    GradientStop (Process *parent, const std::string &name, double r, double g, double b, double a, double offset);
    GradientStop (double r, double g, double b, double a, double offset);
    virtual ~GradientStop ();
    // void get_properties_values (double &r, double &g, double &b, double &a, double &offset);
    // virtual Process* find_component (const string&) override;
    // AbstractDoubleProperty* r () { return (AbstractDoubleProperty*) find_component("r"); }
    // AbstractDoubleProperty* g () { return (AbstractDoubleProperty*) find_component("g"); }
    // AbstractDoubleProperty* b () { return (AbstractDoubleProperty*) find_component("b"); }
    // AbstractDoubleProperty* a () { return (AbstractDoubleProperty*) find_component("a"); }
    // AbstractDoubleProperty* offset () { return (AbstractDoubleProperty*) find_component("offset"); }
    // void impl_activate () override;
    // void impl_deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    // struct raw_props_t { double r; double g; double b; double a;  double offset; };
    // raw_props_t raw_props;
    // Coupling *_cr, *_cg, *_cb, *_ca, *_co;
  };
  

  /*class AbstractGradient : public AbstractStyle
  {
  public:
    AbstractGradient (Process *parent, const std::string &name, int spread, int coords);
    AbstractGradient (int spread, int coords);
    AbstractGradient (Process *parent, const std::string &name);
    virtual ~AbstractGradient ();
    void get_properties_values (int &spread, int &coords);
    virtual Process* find_component (const string&) override;
    AbstractIntProperty* spread () { return (AbstractIntProperty*) find_component("spread"); }
    AbstractIntProperty* coords () { return (AbstractIntProperty*) find_component("coords"); }
    void impl_activate () override;
    void impl_deactivate () override;
    virtual void update ();
    virtual void draw () override = 0;
    List* transforms () { return _transforms;}
    List* stops () { return _stops;}
    bool is_linear () { return _linear;}
  protected:
    struct raw_props_t { int spread; int coords; };
    raw_props_t raw_props;
    Coupling *_cs, *_cc;
    List *_stops, *_transforms;
    AbstractGradient *_g;
    int _linear;
  };*/
}

#include "gen/abstract_prop_gradient.h"

namespace djnn {

  class AbstractGradient : public AbstractPropGradient
  {
  public:
    AbstractGradient (Process *parent, const std::string &name, int spread, int coords);
    AbstractGradient (int spread, int coords);
    AbstractGradient (Process *parent, const std::string &name);
    virtual ~AbstractGradient ();
    // void get_properties_values (int &spread, int &coords);
    // virtual Process* find_component (const string&) override;
    // AbstractIntProperty* spread () { return (AbstractIntProperty*) find_component("spread"); }
    // AbstractIntProperty* coords () { return (AbstractIntProperty*) find_component("coords"); }
    // void impl_activate () override;
    // void impl_deactivate () override;
    virtual void update ();
    // virtual void draw () override = 0;
    List* transforms () { return _transforms;}
    List* stops () { return _stops;}
    bool is_linear () { return _linear;}
  protected:
    // struct raw_props_t { int spread; int coords; };
    // raw_props_t raw_props;
    // Coupling *_cs, *_cc;
    List *_stops, *_transforms;
    AbstractGradient *_g;
    int _linear;
  };
}

#include "gen/abstract_prop_linear_gradient.h"
#include "gen/abstract_prop_radial_gradient.h"

namespace djnn {

  /*
  class LinearGradient : public AbstractGradient
  {
  public:
    LinearGradient (Process *parent, const std::string &name, double x1, double y1, double x2, double y2,
                    djnFillSpread s, djnFillCoords fc);
    LinearGradient (Process *parent, const std::string &name, double x1, double y1, double x2, double y2,
                    int s, int fc);
    LinearGradient (double x1, double y1, double x2, double y2, int s, int fc);
    LinearGradient (Process *parent, const std::string &name, LinearGradient *lg);
    virtual ~LinearGradient ();
    void get_properties_values (double &x1, double &y1, double &x2, double &y2);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* x1 () { return (AbstractDoubleProperty*) find_component("x1"); }
    AbstractDoubleProperty* y1 () { return (AbstractDoubleProperty*) find_component("y1"); }
    AbstractDoubleProperty* x2 () { return (AbstractDoubleProperty*) find_component("x2"); }
    AbstractDoubleProperty* y2 () { return (AbstractDoubleProperty*) find_component("y2"); }
    void impl_activate () override;
    void impl_deactivate () override;
    void update () override;
    void draw () override;
    Process* clone () override;
  private:
    struct raw_props_t { double x1; double y1; double x2; double y2; };
    raw_props_t raw_props;
    Coupling *_cx1, *_cy1, *_cx2, *_cy2;
  };
  */


  class LinearGradient : public AbstractPropLinearGradient
  {
  public:
    LinearGradient (Process *parent, const std::string &name, double x1, double y1, double x2, double y2,
                    djnFillSpread s, djnFillCoords fc);
    LinearGradient (Process *parent, const std::string &name, double x1, double y1, double x2, double y2,
                    int s, int fc);
    LinearGradient (double x1, double y1, double x2, double y2, int s, int fc);
    LinearGradient (Process *parent, const std::string &name, LinearGradient *lg);
    virtual ~LinearGradient ();
    // void get_properties_values (double &x1, double &y1, double &x2, double &y2);
    // virtual Process* find_component (const string&) override;
    // AbstractDoubleProperty* x1 () { return (AbstractDoubleProperty*) find_component("x1"); }
    // AbstractDoubleProperty* y1 () { return (AbstractDoubleProperty*) find_component("y1"); }
    // AbstractDoubleProperty* x2 () { return (AbstractDoubleProperty*) find_component("x2"); }
    // AbstractDoubleProperty* y2 () { return (AbstractDoubleProperty*) find_component("y2"); }
    // void impl_activate () override;
    // void impl_deactivate () override;
    // void update () override;
    void draw () override;
    Process* clone () override;
  private:
    // struct raw_props_t { double x1; double y1; double x2; double y2; };
    // raw_props_t raw_props;
    // Coupling *_cx1, *_cy1, *_cx2, *_cy2;
  };


  class RefLinearGradient : public AbstractStyle
  {
  public:
    RefLinearGradient (Process *parent, const std::string &name, LinearGradient *lg);
    RefLinearGradient (LinearGradient *lg);
    virtual ~RefLinearGradient () {};
    void impl_activate () override;
    void impl_deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    void activate_children ();
    LinearGradient *_lg;
  };

  

  class RadialGradient : public AbstractPropRadialGradient
  {
  public:
    RadialGradient (Process *parent, const std::string &name, double cx, double cy, double r, double fx, double fy,
                    djnFillSpread s, djnFillCoords fc);
    RadialGradient (Process *parent, const std::string &name, double cx, double cy, double r, double fx, double fy,
                        int s, int fc);
    RadialGradient (double cx, double cy, double r, double fx, double fy, int s, int fc);
    RadialGradient (Process *parent, const std::string &name, RadialGradient *rg);
    virtual ~RadialGradient ();
    // void get_properties_values (double &cx, double &cy, double &r, double &fx, double &fy);
    // virtual Process* find_component (const string&) override;
    // AbstractDoubleProperty* cx () { return (AbstractDoubleProperty*) find_component("cx"); }
    // AbstractDoubleProperty* cy () { return (AbstractDoubleProperty*) find_component("cy"); }
    // AbstractDoubleProperty* r () { return (AbstractDoubleProperty*) find_component("r"); }
    // AbstractDoubleProperty* fx () { return (AbstractDoubleProperty*) find_component("fx"); }
    // AbstractDoubleProperty* fy () { return (AbstractDoubleProperty*) find_component("fy"); }
    // void update () override;
    // void impl_activate () override;
    // void impl_deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    // struct raw_props_t { double cx; double cy; double r; double fx; double fy; };
    // raw_props_t raw_props;
    // Coupling *_ccx, *_ccy, *_cr, *_cfx, *_cfy;
  };
  

  class RefRadialGradient : public AbstractStyle
  {
  public:
    RefRadialGradient (Process *parent, const std::string &name, RadialGradient *rg);
    RefRadialGradient (RadialGradient *rg);
    virtual ~RefRadialGradient () {};
    void impl_activate () override;
    void impl_deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    void activate_children ();
    RadialGradient* _rg;
  };

  
  class FontSize : public AbstractPropFontSize
  {
  public:
    FontSize (Process *parent, const std::string &name, djnLengthUnit unit, double size);
    FontSize (djnLengthUnit unit, double size);
    FontSize (Process *parent, const std::string &name, int unit, double size);
    FontSize (int unit, double size);
    // virtual ~FontSize ();
    // void get_properties_values (int &unit, double &size);
    // virtual Process* find_component (const string&) override;
    // AbstractIntProperty* unit () { return (AbstractIntProperty*) find_component("unit"); }
    // AbstractDoubleProperty* size () { return (AbstractDoubleProperty*) find_component("size"); }
    void impl_activate () override;
    //void impl_deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    //struct raw_props_t { int unit; double size; };
    //raw_props_t raw_props;
    //Coupling *_cu, *_cs;
  };

  class FontWeight : public AbstractPropFontWeight // AbstractStyle
  {
  public:
    FontWeight (Process *parent, const std::string &name, int weight);
    FontWeight (int weight);
    //virtual ~FontWeight ();
    // void get_properties_values (int &weight);
    // virtual Process* find_component (const string&) override;
    // AbstractIntProperty* weight () { return (AbstractIntProperty*) find_component("weight"); }
    void impl_activate () override;
    //void impl_deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    //struct raw_props_t { int weight; };
    //raw_props_t raw_props;
    //Coupling* _cw;
  };

  class FontStyle : public AbstractPropFontStyle // AbstractStyle
  {
  public:
    FontStyle (Process *parent, const std::string &name, djnFontSlope style);
    FontStyle (djnFontSlope style);
    FontStyle (Process *parent, const std::string &name, int style);
    FontStyle (int style);
    // virtual ~FontStyle ();
    // void get_properties_values (int &style);
    // virtual Process* find_component (const string&) override;
    // AbstractIntProperty* style () { return (AbstractIntProperty*) find_component("style"); }
    void impl_activate () override;
    // void impl_deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    //struct raw_props_t { int style; };
    //raw_props_t raw_props;
    //Coupling* _cs;
  };

  class FontFamily : public AbstractPropFontFamily // AbstractStyle
  {
  public:
    FontFamily (Process *parent, const std::string &name, const std::string &family);
    FontFamily (const std::string &family);
    // virtual ~FontFamily ();
    // void get_properties_values (string &family);
    // virtual Process* find_component (const string&) override;
    // AbstractTextProperty* family () { return (AbstractTextProperty*) find_component("family"); }
    void impl_activate () override;
    // void impl_deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    struct raw_props_t { string family; };
    raw_props_t raw_props;
    //Coupling* _cf;
  };
/*
  class TextAnchor : public AbstractPropTextAnchor // AbstractStyle
  {
  public:
    TextAnchor (Process *parent, const std::string &name, djnAnchorType anchor);
    TextAnchor (djnAnchorType anchor);
    TextAnchor (Process *parent, const std::string &name, int anchor);
    TextAnchor (int anchor);
    // virtual ~TextAnchor ();
    // void get_properties_values (int &anchor);
    // virtual Process* find_component (const string&) override;
    // AbstractIntProperty* anchor () { return (AbstractIntProperty*) find_component("anchor"); }
    void impl_activate () override;
    // void impl_deactivate () override;
    // void draw () override;
    Process* clone () override;
  private:
    //struct raw_props_t { int anchor; };
    //raw_props_t raw_props;
    //Coupling* _ca;
  };
  */

  class RGBToLCHConverter : public Process
  {
  private:
    class ConverterAction : public Action {
      public:
        ConverterAction (RGBToLCHConverter *p, const std::string &n);
        virtual ~ConverterAction () {}
        void impl_activate () override;
        void impl_deactivate () override {};
      private:
        RGBToLCHConverter *_p;
    };
  public:
    RGBToLCHConverter (Process *parent, const std::string &name);
    virtual ~RGBToLCHConverter ();
    void impl_activate () override;
    void impl_deactivate () override;
  private:
    void set_parent (Process* p) override;
    IntProperty *_r, *_g, *_b;
    DoubleProperty *_l, *_c, *_h;
    ConverterAction *_action;
    Coupling *_cr, *_cg, *_cb;
  };

  class LCHToRGBConverter : public Process
  {
  private:
    class ConverterAction : public Action {
      public:
        ConverterAction (LCHToRGBConverter *p, const std::string &n);
        virtual ~ConverterAction () {}
        void impl_activate () override;
        void impl_deactivate () override {};
      private:
        LCHToRGBConverter *_p;
    };
  public:
    LCHToRGBConverter (Process *parent, const std::string &name);
    virtual ~LCHToRGBConverter ();
    void impl_activate () override;
    void impl_deactivate () override;
  private:
    void set_parent (Process* p) override;
    IntProperty *_r, *_g, *_b;
    DoubleProperty *_l, *_c, *_h;
    ConverterAction *_action;
    Coupling *_cl, *_cc, *_ch;
  };
}

