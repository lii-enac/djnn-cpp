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

#pragma once

// #include <vector> // pch

#include "style_types.h"

#include "gui/abstract_gobj.h"
#include "core/ontology/process.h"

#include "abstract_style.h"

#include "gen/fill_rule.h"
#include "gen/texture.h"
#include "gen/outline_width.h"
#include "gen/outline_cap_style.h"
#include "gen/outline_join_style.h"
#include "gen/outline_miter_limit.h"
#include "gen/dash_offset.h"
#include "gen/abstract_prop_gradient_stop.h"
#include "gen/text_anchor.h"

#include "color.h"
#include "gen/abstract_opacity.h"

#include "gen/abstract_prop_font_family.h"
#include "gen/abstract_prop_font_size.h"
#include "gen/abstract_prop_font_style.h"
#include "gen/abstract_prop_font_weight.h"

#include "style_sheet.h"
namespace djnn
{

  class FillColor : public AbstractColor
  {
  public:
    FillColor (ParentProcess* parent, const std::string& name, int r, int g, int b) :
        AbstractColor (parent, name, r, g, b) { finalize_construction (parent, name); }
    FillColor (ParentProcess* parent, const std::string& name, int v) :
        AbstractColor (parent, name, v) { finalize_construction (parent, name); }
    virtual ~FillColor () {}
    void draw () override;
    FillColor* clone () override;
  };

  class OutlineColor : public AbstractColor
  {
  public:
    OutlineColor (ParentProcess* parent, const std::string& name, int r, int g, int b) :
        AbstractColor (parent, name, r, g, b) { finalize_construction (parent, name); }
    OutlineColor (ParentProcess* parent, const std::string& name, int v) :
         AbstractColor (parent, name, v) { finalize_construction (parent, name); }
    virtual ~OutlineColor () {}
    void draw () override;
    OutlineColor* clone () override;
  };

  class NoOutline : public AbstractStyle
  {
  public:
    NoOutline (ParentProcess* parent, const std::string& name) :
        AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~NoOutline () {}
    void impl_activate () override { AbstractStyle::impl_activate ();}
    void impl_deactivate () override { AbstractStyle::impl_deactivate ();}
    void draw () override;
    NoOutline* clone () override;
  };

  class NoFill : public AbstractStyle
  {
  public:
    NoFill (ParentProcess* parent, const std::string& name) :
        AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~NoFill () {}
    void impl_activate () override { AbstractStyle::impl_activate ();}
    void impl_deactivate () override { AbstractStyle::impl_deactivate ();}
    void draw () override;
    NoFill* clone () override;
  };

  class OutlineOpacity : public AbstractOpacity
  {
  public:
    OutlineOpacity (ParentProcess* parent, const std::string& name, double alpha) :
        AbstractOpacity (parent, name, alpha) { finalize_construction (parent, name); }
    virtual ~OutlineOpacity () {}
    void draw () override;
    OutlineOpacity* clone () override;
  };

  class FillOpacity : public AbstractOpacity
  {
  public:
    FillOpacity (ParentProcess* parent, const std::string& name, double alpha) :
        AbstractOpacity (parent, name, alpha) { finalize_construction (parent, name); }
    virtual ~FillOpacity () {}
    void draw () override;
    FillOpacity* clone () override;
  };


  class DashArray : public AbstractStyle
  {
  public:
    DashArray (ParentProcess* parent, const std::string& name) :
        AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~DashArray () { _dash_array.clear ();}
    const std::vector<double>& dash_array () const { return _dash_array;}
    void add_sub_pattern (double dash, double space)
    {
      _dash_array.push_back (dash);
      _dash_array.push_back (space);
    }
    void impl_activate () override { AbstractStyle::impl_activate ();}
    void impl_deactivate () override { AbstractStyle::impl_deactivate ();}
    void draw () override;
    DashArray* clone () override;
  private:
    std::vector<double> _dash_array;
  };

  class NoDashArray : public AbstractStyle
  {
  public:
    NoDashArray (ParentProcess* parent, const std::string& name) :
        AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~NoDashArray () {}
    void impl_activate () override { AbstractStyle::impl_activate ();}
    void impl_deactivate () override { AbstractStyle::impl_deactivate ();}
    void draw () override;
    NoDashArray* clone () override;
  };


}


namespace djnn {
  class GradientStop : public AbstractPropGradientStop
  {
  public:
    GradientStop (ParentProcess* parent, const std::string& name, double r, double g, double b, double a, double offset);
    virtual ~GradientStop ();
    void draw () override;
    GradientStop* clone () override;
  };
  
}

#include "gradient.h"

namespace djnn {
  class FontSize : public AbstractPropFontSize
  {
  public:
    FontSize (ParentProcess* parent, const std::string& name, djnLengthUnit unit, double size);
    FontSize (ParentProcess* parent, const std::string& name, int unit, double size);
    void impl_activate () override;
    void draw () override;
    FontSize* clone () override;
  private:
  };

  class FontWeight : public AbstractPropFontWeight // AbstractStyle
  {
  public:
    FontWeight (ParentProcess* parent, const std::string& name, int weight);
    void impl_activate () override;
    void draw () override;
    FontWeight* clone () override;
  };

  class FontStyle : public AbstractPropFontStyle // AbstractStyle
  {
  public:
    FontStyle (ParentProcess* parent, const std::string& name, djnFontSlope style);
    FontStyle (ParentProcess* parent, const std::string& name, int style);
    void impl_activate () override;
    void draw () override;
    FontStyle* clone () override;
  };

  class FontFamily : public AbstractPropFontFamily // AbstractStyle
  {
  public:
    FontFamily (ParentProcess* parent, const std::string& name, const std::string& family);
    void impl_activate () override;
    void draw () override;
    FontFamily* clone () override;
  };

  class RGBToLCHConverter : public FatProcess
  {
  private:
    class ConverterAction : public Action {
      public:
        ConverterAction (RGBToLCHConverter *p, const std::string& n);
        virtual ~ConverterAction () {}
        void impl_activate () override;
      private:
        RGBToLCHConverter *_p;
    };
  public:
    RGBToLCHConverter (ParentProcess* parent, const std::string& name);
    virtual ~RGBToLCHConverter ();
    void impl_activate () override;
    void impl_deactivate () override;
  private:
    void set_parent (ParentProcess* parent) override;
    IntProperty *_r, *_g, *_b;
    DoubleProperty *_l, *_c, *_h;
    ConverterAction *_action;
    Coupling *_cr, *_cg, *_cb;
  };

  class LCHToRGBConverter : public FatProcess
  {
  private:
    class ConverterAction : public Action {
      public:
        ConverterAction (LCHToRGBConverter *p, const std::string& n);
        virtual ~ConverterAction () {}
        void impl_activate () override;
      private:
        LCHToRGBConverter *_p;
    };
  public:
    LCHToRGBConverter (ParentProcess* parent, const std::string& name);
    virtual ~LCHToRGBConverter ();
    void impl_activate () override;
    void impl_deactivate () override;
  private:
    void set_parent (ParentProcess* parent) override;
    IntProperty *_r, *_g, *_b;
    DoubleProperty *_l, *_c, *_h;
    ConverterAction *_action;
    Coupling *_cl, *_cc, *_ch;
  };
}

