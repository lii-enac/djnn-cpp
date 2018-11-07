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
    DoubleProperty* r () { return _r;}
    DoubleProperty* g () { return _g;}
    DoubleProperty* b () { return _b;}
  protected:
    DoubleProperty *_r, *_g, *_b;
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
    IntProperty* rule () { return _rule;}
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    IntProperty* _rule;
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
    TextProperty* path () { return _path;}
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    TextProperty* _path;
    Coupling* _cp;
  };

  class AbstractOpacity : public AbstractStyle
  {
  public:
    AbstractOpacity (Process *p, const std::string &n, double alpha);
    AbstractOpacity (double alpha);
    virtual ~AbstractOpacity ();
    DoubleProperty* alpha () { return _alpha;}
    void activate () override;
    void deactivate () override;
    void draw () override = 0;
  protected:
    DoubleProperty* _alpha;
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
    DoubleProperty* width () { return _width;};
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    DoubleProperty* _width;
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
    IntProperty* cap () { return _cap;};
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    IntProperty* _cap;
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
    IntProperty* join () { return _join;};
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    IntProperty* _join;
    Coupling* _cj;
  };

  class OutlineMiterLimit : public AbstractStyle
  {
  public:
    OutlineMiterLimit (Process *p, const std::string &n, int limit);
    OutlineMiterLimit (int limit);
    virtual ~OutlineMiterLimit ();
    IntProperty* limit () { return _limit;};
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    IntProperty* _limit;
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
    vector<double> dash_array () { return _dash_array;}
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
    DoubleProperty* offset () { return _offset;};
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    DoubleProperty* _offset;
    Coupling* _co;
  };

  class GradientStop : public AbstractStyle
  {
  public:
    GradientStop (Process *p, const std::string &n, double r, double g, double b, double a, double offset);
    GradientStop (double r, double g, double b, double a, double offset);
    virtual ~GradientStop ();
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    DoubleProperty *_r, *_g, *_b, *_a, *_offset;
    Coupling *_cr, *_cg, *_cb, *_ca, *_co;
  };

  class AbstractGradient : public AbstractStyle
  {
  public:
    AbstractGradient (Process *p, const std::string &n, int s, int fc);
    AbstractGradient (int s, int fc);
    AbstractGradient (Process *p, const std::string &n);
    virtual ~AbstractGradient ();
    void activate () override;
    void deactivate () override;
    virtual void update ();
    virtual void draw () override = 0;
    List* transforms () { return _transforms;}
    List* stops () { return _stops;}
    IntProperty* spread () { return _spread;}
    IntProperty* coords () { return _coords;}
    bool is_linear () { return _linear;}
  protected:
    IntProperty *_spread, *_coords;
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
    void activate () override;
    void deactivate () override;
    void update () override;
    void draw () override;
    Process* clone () override;
    DoubleProperty* x1 () { return _x1;}
    DoubleProperty* y1 () { return _y1;}
    DoubleProperty* x2 () { return _x2;}
    DoubleProperty* y2 () { return _y2;}
  private:
    DoubleProperty *_x1, *_y1, *_x2, *_y2;
    Coupling *_cx1, *_cy1, *_cx2, *_cy2;
  };

  class RefLinearGradient : public AbstractStyle
  {
  public:
    RefLinearGradient (Process *p, const std::string &n, LinearGradient *lg);
    virtual ~RefLinearGradient () {};
    void activate () override;
    void deactivate () override;
    void draw () override;
  private:
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
    void update () override;
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
    DoubleProperty* cx () { return _cx;}
    DoubleProperty* cy () { return _cy;}
    DoubleProperty* r () { return _r;}
    DoubleProperty* fx () { return _fx;}
    DoubleProperty* fy () { return _fy;}
  private:
    DoubleProperty *_cx, *_cy, *_r, *_fx, *_fy;
    Coupling *_ccx, *_ccy, *_cr, *_cfx, *_cfy;
  };

  class RefRadialGradient : public AbstractStyle
  {
  public:
    RefRadialGradient (Process *p, const std::string &n, RadialGradient *rg);
    virtual ~RefRadialGradient () {};
    void activate () override;
    void deactivate () override;
    void draw () override;
  private:
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
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
    DoubleProperty* size () { return _size; }
  private:
    IntProperty *_unit;
    DoubleProperty *_size;
    Coupling *_cu, *_cs;
  };

  class FontWeight : public AbstractStyle
  {
  public:
    FontWeight (Process *p, const std::string &n, int weight);
    FontWeight (int weight);
    virtual ~FontWeight ();
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
    IntProperty* weight () { return _weight; }
  private:
    IntProperty* _weight;
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
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
    IntProperty* style () { return _style; }
  private:
    IntProperty* _style;
    Coupling* _cs;
  };

  class FontFamily : public AbstractStyle
  {
  public:
    FontFamily (Process *p, const std::string &n, const std::string &family);
    FontFamily (const std::string &family);
    virtual ~FontFamily ();
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
    TextProperty* family () { return _family; }
  private:
    TextProperty* _family;
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
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  private:
    IntProperty* _anchor;
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

