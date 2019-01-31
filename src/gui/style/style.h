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

#include "../../core/tree/list.h"
#include "../abstract_gobj.h"
#include "../../core/tree/process.h"

#include "style_types.h"

#include <vector>

namespace djnn
{

  class AbstractStyle : public AbstractGObj
  {
  public:
    AbstractStyle (Process *p, const string &n);
    AbstractStyle ();
    virtual ~AbstractStyle ();
  };

  class AbstractColor : public AbstractStyle
  {
  public:
    AbstractColor (Process *p, const std::string& n, double r, double g, double b);
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
    void activate () override;
    void deactivate () override;
  };

  class FillColor : public AbstractColor
  {
  public:
    FillColor (Process *p, const std::string& n, double r, double g, double b) :
        AbstractColor (p, n, r, g, b) {}
    FillColor (double r, double g, double b) :
        AbstractColor (r, g, b) {}
    virtual ~FillColor () {}
    void draw () override;
    Process* clone () override;
  };

  class OutlineColor : public AbstractColor
  {
  public:
    OutlineColor (Process *p, const std::string& n, double r, double g, double b) :
        AbstractColor (p, n, r, g, b) {}
    OutlineColor (double r, double g, double b) :
        AbstractColor (r, g, b) {}
    virtual ~OutlineColor () {}
    void draw () override;
    Process* clone () override;
  };

  class FillRule : public AbstractStyle
  {
  public:
    FillRule (Process *p, const std::string &n, djnFillRuleType rule);
    FillRule (djnFillRuleType rule);
    FillRule (Process *p, const std::string &n, int rule);
    FillRule (int rule);
    virtual ~FillRule ();
    void get_properties_values (int &rule);
    virtual Process* find_component (const string&) override;
    AbstractIntProperty* rule () { return (AbstractIntProperty*) find_component("rule"); }
    //IntProperty* rule () { return _rule;}
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    //IntProperty* _rule;
    struct raw_props_t { int rule; };
    raw_props_t raw_props;
    Coupling* _cr;
  };

  class NoOutline : public AbstractStyle
  {
  public:
    NoOutline (Process *p, const std::string &n) :
        AbstractStyle (p, n) { Process::finalize ();}
    NoOutline () :
        AbstractStyle () {}    
    virtual ~NoOutline () {}
    void activate () override { AbstractStyle::activate ();}
    void deactivate () override { AbstractStyle::deactivate ();}
    void draw () override;
    Process* clone () override;
  };

  class NoFill : public AbstractStyle
  {
  public:
    NoFill (Process *p, const std::string &n) :
        AbstractStyle (p, n) { Process::finalize ();}
    NoFill () :
        AbstractStyle () {}
    virtual ~NoFill () {}
    void activate () override { AbstractStyle::activate ();}
    void deactivate () override { AbstractStyle::deactivate ();}
    void draw () override;
    Process* clone () override;
  };

  class Texture : public AbstractStyle
  {
  public:
    Texture (Process *p, const std::string &n, const std::string &path);
    Texture (const std::string &path);
    virtual ~Texture ();
    void get_properties_values (string &path);
    virtual Process* find_component (const string&) override;
    AbstractTextProperty* path () { return (AbstractTextProperty*) find_component("path"); }
    //TextProperty* path () { return _path;}
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    //TextProperty* _path;
    struct raw_props_t { string path; };
    raw_props_t raw_props;
    Coupling* _cp;
  };

  class AbstractOpacity : public AbstractStyle
  {
  public:
    AbstractOpacity (Process *p, const std::string &n, double alpha);
    AbstractOpacity (double alpha);
    virtual ~AbstractOpacity ();
    void get_properties_values (double &alpha);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* alpha () { return (AbstractDoubleProperty*) find_component("alpha"); }
    //DoubleProperty* alpha () { return _alpha;}
    void activate () override;
    void deactivate () override;
    void draw () override = 0;
  protected:
    //DoubleProperty* _alpha;
    struct raw_props_t { double alpha; };
    raw_props_t raw_props;
    Coupling* _ca;
  };

  class OutlineOpacity : public AbstractOpacity
  {
  public:
    OutlineOpacity (Process *p, const std::string &n, double alpha) :
        AbstractOpacity (p, n, alpha) {}
    OutlineOpacity (double alpha) :
        AbstractOpacity (alpha) {}
    virtual ~OutlineOpacity () {};
    void draw () override;
    Process* clone () override;
  };

  class FillOpacity : public AbstractOpacity
  {
  public:
    FillOpacity (Process *p, const std::string &n, double alpha) :
        AbstractOpacity (p, n, alpha) {}
    FillOpacity (double alpha) :
        AbstractOpacity (alpha) {}
    virtual ~FillOpacity () {};
    void draw () override;
    Process* clone () override;
  };

  class OutlineWidth : public AbstractStyle
  {
  public:
    OutlineWidth (Process *p, const std::string &n, double width);
    OutlineWidth (double width);
    virtual ~OutlineWidth ();
    void get_properties_values (double &width);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* width () { return (AbstractDoubleProperty*) find_component("width"); }
    //DoubleProperty* width () { return _width;};
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    //DoubleProperty* _width;
    struct raw_props_t { double width; };
    raw_props_t raw_props;
    Coupling* _cw;
  };

  class OutlineCapStyle : public AbstractStyle
  {
  public:
    OutlineCapStyle (Process *p, const std::string &n, djnCapStyle cap);
    OutlineCapStyle (djnCapStyle cap);
    OutlineCapStyle (Process *p, const std::string &n, int cap);
    OutlineCapStyle (int cap);
    virtual ~OutlineCapStyle ();
    void get_properties_values (int &cap);
    virtual Process* find_component (const string&) override;
    AbstractIntProperty* cap () { return (AbstractIntProperty*) find_component("cap"); }
    //IntProperty* cap () { return _cap;};
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    //IntProperty* _cap;
    struct raw_props_t { int cap; };
    raw_props_t raw_props;
    Coupling* _cc;
  };

  class OutlineJoinStyle : public AbstractStyle
  {
  public:
    OutlineJoinStyle (Process *p, const std::string &n, djnJoinStyle join);
    OutlineJoinStyle (djnJoinStyle join);
    OutlineJoinStyle (Process *p, const std::string &n, int join);
    OutlineJoinStyle (int join);
    virtual ~OutlineJoinStyle ();
    void get_properties_values (int &join);
    virtual Process* find_component (const string&) override;
    AbstractIntProperty* join () { return (AbstractIntProperty*) find_component("join"); }
    //IntProperty* join () { return _join;};
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    //IntProperty* _join;
    struct raw_props_t { int join; };
    raw_props_t raw_props;
    Coupling* _cj;
  };

  class OutlineMiterLimit : public AbstractStyle
  {
  public:
    OutlineMiterLimit (Process *p, const std::string &n, int limit);
    OutlineMiterLimit (int limit);
    virtual ~OutlineMiterLimit ();
    void get_properties_values (int &limit);
    virtual Process* find_component (const string&) override;
    AbstractIntProperty* limit () { return (AbstractIntProperty*) find_component("limit"); }
    //IntProperty* limit () { return _limit;};
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    //IntProperty* _limit;
    struct raw_props_t { int limit; };
    raw_props_t raw_props;
    Coupling* _cl;
  };

  class DashArray : public AbstractStyle
  {
  public:
    DashArray (Process *p, const std::string &n) :
        AbstractStyle (p, n) { Process::finalize ();}
    DashArray () :
        AbstractStyle () {}
    virtual ~DashArray () { _dash_array.clear ();}
    const vector<double>& dash_array () const { return _dash_array;}
    void add_sub_pattern (double dash, double space)
    {
      _dash_array.push_back (dash);
      _dash_array.push_back (space);
    }
    void activate () override { AbstractStyle::activate ();}
    void deactivate () override { AbstractStyle::deactivate ();}
    void draw () override;
    Process* clone () override;
  private:
    vector<double> _dash_array;
  };

  class NoDashArray : public AbstractStyle
  {
  public:
    NoDashArray (Process *p, const std::string &n) :
        AbstractStyle (p, n) { Process::finalize ();}
    NoDashArray () :
        AbstractStyle () {}
    virtual ~NoDashArray () {}
    void activate () override { AbstractStyle::activate ();}
    void deactivate () override { AbstractStyle::deactivate ();}
    void draw () override;
    Process* clone () override;
  };

  class DashOffset : public AbstractStyle
  {
  public:
    DashOffset (Process *p, const std::string &n, double offset);
    DashOffset (double offset);
    virtual ~DashOffset ();
    void get_properties_values (double &offset);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* offset () { return (AbstractDoubleProperty*) find_component("offset"); }
    //DoubleProperty* offset () { return _offset;};
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    struct raw_props_t { double offset; };
    raw_props_t raw_props;
    //DoubleProperty* _offset;
    Coupling* _co;
  };

  class GradientStop : public AbstractStyle
  {
  public:
    GradientStop (Process *p, const std::string &n, double r, double g, double b, double a, double offset);
    GradientStop (double r, double g, double b, double a, double offset);
    virtual ~GradientStop ();
    void get_properties_values (double &r, double &g, double &b, double &a, double &offset);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* r () { return (AbstractDoubleProperty*) find_component("r"); }
    AbstractDoubleProperty* g () { return (AbstractDoubleProperty*) find_component("g"); }
    AbstractDoubleProperty* b () { return (AbstractDoubleProperty*) find_component("b"); }
    AbstractDoubleProperty* a () { return (AbstractDoubleProperty*) find_component("a"); }
    AbstractDoubleProperty* offset () { return (AbstractDoubleProperty*) find_component("offset"); }
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    //DoubleProperty *_r, *_g, *_b, *_a, *_offset;
    struct raw_props_t { double r; double g; double b; double a;  double offset; };
    raw_props_t raw_props;
    Coupling *_cr, *_cg, *_cb, *_ca, *_co;
  };

  class AbstractGradient : public AbstractStyle
  {
  public:
    AbstractGradient (Process *p, const std::string &n, int spread, int coords);
    AbstractGradient (int spread, int coords);
    AbstractGradient (Process *p, const std::string &n);
    virtual ~AbstractGradient ();
    void get_properties_values (int &spread, int &coords);
    virtual Process* find_component (const string&) override;
    AbstractIntProperty* spread () { return (AbstractIntProperty*) find_component("spread"); }
    AbstractIntProperty* coords () { return (AbstractIntProperty*) find_component("coords"); }
    void activate () override;
    void deactivate () override;
    virtual void update ();
    virtual void draw () override = 0;
    List* transforms () { return _transforms;}
    List* stops () { return _stops;}
    // IntProperty* spread () { return _spread;}
    // IntProperty* coords () { return _coords;}
    bool is_linear () { return _linear;}
  protected:
    //IntProperty *_spread, *_coords;
    struct raw_props_t { int spread; int coords; };
    raw_props_t raw_props;
    Coupling *_cs, *_cc;
    List *_stops, *_transforms;
    AbstractGradient *_g;
    int _linear;
  };

  class LinearGradient : public AbstractGradient
  {
  public:
    LinearGradient (Process *p, const std::string &n, double x1, double y1, double x2, double y2,
                    djnFillSpread s, djnFillCoords fc);
    LinearGradient (Process *p, const std::string &n, double x1, double y1, double x2, double y2,
                    int s, int fc);
    LinearGradient (double x1, double y1, double x2, double y2, int s, int fc);
    LinearGradient (Process *p, const std::string &n, LinearGradient *lg);
    virtual ~LinearGradient ();
    void get_properties_values (double &x1, double &y1, double &x2, double &y2);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* x1 () { return (AbstractDoubleProperty*) find_component("x1"); }
    AbstractDoubleProperty* y1 () { return (AbstractDoubleProperty*) find_component("y1"); }
    AbstractDoubleProperty* x2 () { return (AbstractDoubleProperty*) find_component("x2"); }
    AbstractDoubleProperty* y2 () { return (AbstractDoubleProperty*) find_component("y2"); }
    void activate () override;
    void deactivate () override;
    void update () override;
    void draw () override;
    Process* clone () override;
    // DoubleProperty* x1 () { return _x1;}
    // DoubleProperty* y1 () { return _y1;}
    // DoubleProperty* x2 () { return _x2;}
    // DoubleProperty* y2 () { return _y2;}
  private:
    //DoubleProperty *_x1, *_y1, *_x2, *_y2;
    struct raw_props_t { double x1; double y1; double x2; double y2; };
    raw_props_t raw_props;
    Coupling *_cx1, *_cy1, *_cx2, *_cy2;
  };

  class RefLinearGradient : public AbstractStyle
  {
  public:
    RefLinearGradient (Process *p, const std::string &n, LinearGradient *lg);
    RefLinearGradient (LinearGradient *lg);
    virtual ~RefLinearGradient () {};
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    void activate_children ();
    LinearGradient *_lg;
  };

  class RadialGradient : public AbstractGradient
  {
  public:
    RadialGradient (Process *p, const std::string &n, double cx, double cy, double r, double fx, double fy,
                    djnFillSpread s, djnFillCoords fc);
    RadialGradient (Process *p, const std::string &n, double cx, double cy, double r, double fx, double fy,
                        int s, int fc);
    RadialGradient (double cx, double cy, double r, double fx, double fy, int s, int fc);
    RadialGradient (Process *p, const std::string &n, RadialGradient *rg);
    virtual ~RadialGradient ();
    void get_properties_values (double &cx, double &cy, double &r, double &fx, double &fy);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* cx () { return (AbstractDoubleProperty*) find_component("cx"); }
    AbstractDoubleProperty* cy () { return (AbstractDoubleProperty*) find_component("cy"); }
    AbstractDoubleProperty* r () { return (AbstractDoubleProperty*) find_component("r"); }
    AbstractDoubleProperty* fx () { return (AbstractDoubleProperty*) find_component("fx"); }
    AbstractDoubleProperty* fy () { return (AbstractDoubleProperty*) find_component("fy"); }
    void update () override;
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
    // DoubleProperty* cx () { return _cx;}
    // DoubleProperty* cy () { return _cy;}
    // DoubleProperty* r () { return _r;}
    // DoubleProperty* fx () { return _fx;}
    // DoubleProperty* fy () { return _fy;}
  private:
    //DoubleProperty *_cx, *_cy, *_r, *_fx, *_fy;
    struct raw_props_t { double cx; double cy; double r; double fx; double fy; };
    raw_props_t raw_props;
    Coupling *_ccx, *_ccy, *_cr, *_cfx, *_cfy;
  };

  class RefRadialGradient : public AbstractStyle
  {
  public:
    RefRadialGradient (Process *p, const std::string &n, RadialGradient *rg);
    RefRadialGradient (RadialGradient *rg);
    virtual ~RefRadialGradient () {};
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    void activate_children ();
    RadialGradient* _rg;
  };

  class FontSize : public AbstractStyle
  {
  public:
    FontSize (Process *p, const std::string &n, djnLengthUnit unit, double size);
    FontSize (djnLengthUnit unit, double size);
    FontSize (Process *p, const std::string &n, int unit, double size);
    FontSize (int unit, double size);
    virtual ~FontSize ();
    void get_properties_values (int &unit, double &size);
    virtual Process* find_component (const string&) override;
    AbstractIntProperty* unit () { return (AbstractIntProperty*) find_component("unit"); }
    AbstractDoubleProperty* size () { return (AbstractDoubleProperty*) find_component("size"); }
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
    //DoubleProperty* size () { return _size; }
  private:
    struct raw_props_t { int unit; double size; };
    raw_props_t raw_props;
    // IntProperty *_unit;
    // DoubleProperty *_size;
    Coupling *_cu, *_cs;
  };

  class FontWeight : public AbstractStyle
  {
  public:
    FontWeight (Process *p, const std::string &n, int weight);
    FontWeight (int weight);
    virtual ~FontWeight ();
    void get_properties_values (int &weight);
    virtual Process* find_component (const string&) override;
    AbstractIntProperty* weight () { return (AbstractIntProperty*) find_component("weight"); }
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
    //IntProperty* weight () { return _weight; }
  private:
    struct raw_props_t { int weight; };
    raw_props_t raw_props;
    //IntProperty* _weight;
    Coupling* _cw;
  };

  class FontStyle : public AbstractStyle
  {
  public:
    FontStyle (Process *p, const std::string &n, djnFontSlope style);
    FontStyle (djnFontSlope style);
    FontStyle (Process *p, const std::string &n, int style);
    FontStyle (int style);
    virtual ~FontStyle ();
    void get_properties_values (int &style);
    virtual Process* find_component (const string&) override;
    AbstractIntProperty* style () { return (AbstractIntProperty*) find_component("style"); }
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
    //IntProperty* style () { return _style; }
  private:
    //IntProperty* _style;
    struct raw_props_t { int style; };
    raw_props_t raw_props;
    Coupling* _cs;
  };

  class FontFamily : public AbstractStyle
  {
  public:
    FontFamily (Process *p, const std::string &n, const std::string &family);
    FontFamily (const std::string &family);
    virtual ~FontFamily ();
    void get_properties_values (string &family);
    virtual Process* find_component (const string&) override;
    AbstractTextProperty* family () { return (AbstractTextProperty*) find_component("family"); }
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
    //TextProperty* family () { return _family; }
  private:
    //TextProperty* _family;
    struct raw_props_t { string family; };
    raw_props_t raw_props;
    Coupling* _cf;
  };

  class TextAnchor : public AbstractStyle
  {
  public:
    TextAnchor (Process *p, const std::string &n, djnAnchorType anchor);
    TextAnchor (djnAnchorType anchor);
    TextAnchor (Process *p, const std::string &n, int anchor);
    TextAnchor (int anchor);
    virtual ~TextAnchor ();
    void get_properties_values (int &anchor);
    virtual Process* find_component (const string&) override;
    AbstractIntProperty* anchor () { return (AbstractIntProperty*) find_component("anchor"); }
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    //IntProperty* _anchor;
    struct raw_props_t { int anchor; };
    raw_props_t raw_props;
    Coupling* _ca;
  };

  class RGBToLCHConverter : public Process
  {
  private:
    class ConverterAction : public Process {
      public:
        ConverterAction (RGBToLCHConverter *p, const std::string &n);
        virtual ~ConverterAction () {}
        void activate () override;
        void deactivate () override {};
      private:
        RGBToLCHConverter *_p;
    };
  public:
    RGBToLCHConverter (Process *p, const std::string &n);
    virtual ~RGBToLCHConverter ();
    void activate () override;
    void deactivate () override;
  private:
    IntProperty *_r, *_g, *_b;
    DoubleProperty *_l, *_c, *_h;
    ConverterAction *_action;
    Coupling *_cr, *_cg, *_cb;
  };

  class LCHToRGBConverter : public Process
  {
  private:
    class ConverterAction : public Process {
      public:
        ConverterAction (LCHToRGBConverter *p, const std::string &n);
        virtual ~ConverterAction () {}
        void activate () override;
        void deactivate () override {};
      private:
        LCHToRGBConverter *_p;
    };
  public:
    LCHToRGBConverter (Process *p, const std::string &n);
    virtual ~LCHToRGBConverter ();
    void activate () override;
    void deactivate () override;
  private:
    IntProperty *_r, *_g, *_b;
    DoubleProperty *_l, *_c, *_h;
    ConverterAction *_action;
    Coupling *_cl, *_cc, *_ch;
  };
}

