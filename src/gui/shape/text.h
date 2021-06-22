#pragma once

#include "core/control/action.h"
#include "core/tree/double_property.h"
#include "core/tree/int_property.h"
#include "core/tree/text_property.h"
#include "core/tree/ref_property.h"
#include "core/tree/list.h"
//#include "gen/abstract_prop_text.h"
#include "gui/shape/abstract_gshape.h"

#include "gui/style/text.h" // FontFamily FontSize FontWeight

namespace djnn {
  typedef void* FontMetricsImpl ;

  class Text : public AbstractGShape
  {
    class TextSizeAction : public Action
    {
      friend Text;
      public:
        TextSizeAction (ParentProcess* parent, const djnn::string& name, Text *text) : Action (parent, name), _ff (nullptr), _fsz (nullptr), _fs (nullptr), _fw (nullptr), _text (text) {};
        ~TextSizeAction () {}
        void impl_activate () override;
      private:
        FontFamily*_ff;
        FontSize* _fsz;
        FontStyle* _fs;
        FontWeight* _fw;
        Text* _text;
    };
  public:
    Text (ParentProcess* parent, const djnn::string& name, double x, double y, const djnn::string& text);
    Text (ParentProcess* parent, const djnn::string& name, double x, double y, double dx, double dy, int dxu, int dyu,
          const djnn::string& encoding, const djnn::string& text);
    virtual ~Text ();
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
    Text* clone () override;
    void get_properties_values (double &x, double &y, double &dx, double &dy, int &dxU, int &dyU, int &width, int &height, int &encoding, djnn::string &text);
    virtual FatChildProcess* find_child_impl (const djnn::string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_child_impl("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_child_impl("y"); }
    AbstractDoubleProperty* dx () { return (AbstractDoubleProperty*) find_child_impl("dx"); }
    AbstractDoubleProperty* dy () { return (AbstractDoubleProperty*) find_child_impl("dy"); }
    AbstractIntProperty*    dxU () { return (AbstractIntProperty*) find_child_impl("dxU"); }
    AbstractIntProperty*    dyU () { return (AbstractIntProperty*) find_child_impl("dyU"); }
    AbstractIntProperty*    encoding () { return (AbstractIntProperty*) find_child_impl("encoding"); }
    IntProperty*  width () { return &_width; }
    IntProperty*  height () { return &_height; }
    AbstractTextProperty* text () { return &_text;}
    void set_width (double width) { _width.set_value (width, true); }
    void set_height (double height) { _height.set_value (height, true); }
    const djnn::string& get_raw_text () { return _text.get_value (); }
    double get_cursor_from_index (int index);
    pair<double,int> get_cursor_from_local_x (double pos);
    FontMetricsImpl get_font_metrics () { return _fm; };
    void set_font_metrics (FontMetricsImpl *fm) { _fm = fm; }
  private:
    void set_parent (ParentProcess* parent) override;
    struct raw_props_t { double x, y, dx, dy; int dxU, dyU, encoding; djnn::string text; };
    raw_props_t raw_props;    
    CouplingWithData *_cx, *_cy, *_cdx, *_cdy, *_cfsize,
      *_cdxU, *_cdyU, *_cencoding, *_cfstyle, *_cfweight,
      *_cffamily;

    /* implementation */
    FontMetricsImpl _fm;

    /* no proxy needed because those are always created */
    TextSizeAction _update_size;
    IntProperty _width;
    IntProperty _height;

    // text must notify_geometry when changed, hence it's a proxy
    TextPropertyProxy _text; 
    CouplingWithData _cupdate_size, _ctext;

    void impl_activate () override;
    void impl_deactivate () override;
  };
}
