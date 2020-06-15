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

#include "core/tree/list.h"
#include "gui/shapes/abstract_gshape.h"

#include "core/tree/int_property.h"
#include "core/tree/text_property.h"
#include "gui/style/style.h"

#include "gen/abstract_prop_rectangle.h"
#include "gen/abstract_prop_circle.h"
#include "gen/abstract_prop_ellipse.h"
#include "gen/abstract_prop_rectangle_clip.h"
#include "gen/abstract_prop_line.h"
#include "gen/abstract_image.h"
#include "gen/abstract_path_image.h"
#include "gen/abstract_data_image.h"

namespace djnn
{
  typedef void* FontMetricsImpl ;

  class Rectangle : public AbstractPropRectangle
  {
  public:
    Rectangle (FatProcess *parent, const std::string& name, double x, double y, double width, double height, double rx=0, double ry=0);
    Rectangle* clone () override;
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
  };

  class Circle : public AbstractPropCircle
  {
  public:
    Circle (FatProcess *parent, const std::string& name, double cx, double cy, double r);
    Circle* clone () override;
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
  };

  class Ellipse : public AbstractPropEllipse
  {
  public:
    Ellipse (FatProcess *parent, const std::string& name, double cx, double cy, double rx, double ry);
    Ellipse* clone () override;
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
  };

  class Line : public AbstractPropLine
  {
  public:
    Line (FatProcess *parent, const std::string& name, double x1, double y1, double x2, double y2);
    Line* clone () override;
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
  };

  class RectangleClip : public AbstractPropRectangleClip
  {
  public:
    RectangleClip (FatProcess *parent, const std::string& name, double x, double y, double width, double height);
    RectangleClip* clone () override;
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
  };

  class Text : public AbstractGShape
  {
    class TextSizeAction : public Action
    {
      friend Text;
      public:
        TextSizeAction (FatProcess *parent, const std::string& name, Text *text) : Action (parent, name), _ff (nullptr), _fsz (nullptr), _fs (nullptr), _fw (nullptr), _text (text) {};
        ~TextSizeAction () {}
        void impl_activate () override;
        void impl_deactivate () override {};
      private:
        FontFamily*_ff;
        FontSize* _fsz;
        FontStyle* _fs;
        FontWeight* _fw;
        Text* _text;
    };
  public:
    Text (FatProcess *parent, const std::string& name, double x, double y, const std::string& text);
    Text (FatProcess *parent, const std::string& name, double x, double y, double dx, double dy, int dxu, int dyu,
          const std::string& encoding, const std::string& text);
    virtual ~Text ();
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
    Text* clone () override;
    void get_properties_values (double &x, double &y, double &dx, double &dy, int &dxU, int &dyU, int &width, int &height, int &encoding, std::string &text);
    virtual ChildProcess* find_child (const std::string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_child("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_child("y"); }
    AbstractDoubleProperty* dx () { return (AbstractDoubleProperty*) find_child("dx"); }
    AbstractDoubleProperty* dy () { return (AbstractDoubleProperty*) find_child("dy"); }
    AbstractIntProperty*    dxU () { return (AbstractIntProperty*) find_child("dxU"); }
    AbstractIntProperty*    dyU () { return (AbstractIntProperty*) find_child("dyU"); }
    AbstractIntProperty*    encoding () { return (AbstractIntProperty*) find_child("encoding"); }
    IntProperty*  width () { return &_width; }
    IntProperty*  height () { return &_height; }
    AbstractTextProperty* text () { return &_text;}
    void set_width (double width) { _width.set_value (width, true); }
    void set_height (double height) { _height.set_value (height, true); }
    const std::string& get_raw_text () { return _text.get_value (); }
    double get_cursor_from_index (int index);
    std::pair<double,int> get_cursor_from_local_x (double pos);
    FontMetricsImpl get_font_metrics () { return _fm; };
    void set_font_metrics (FontMetricsImpl *fm) { _fm = fm; }
  private:
    void set_parent (FatProcess* p) override;
    struct raw_props_t { double x, y, dx, dy; int dxU, dyU, encoding; std::string text; };
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

  class PolyPoint : public AbstractGObj
  {
  public:
    PolyPoint (FatProcess* parent, const std::string& name, double x, double y);
    virtual ~PolyPoint ();
    virtual ChildProcess* find_child (const std::string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_child("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_child("y"); }
    void draw () override;
    PolyPoint* clone () override;
    // notify polygon ( (grand-grand-)parent polygon-list-point)
    void notify_change (unsigned int nm) override { _damaged |= nm; get_parent ()->get_parent()->notify_change (nm); }
  protected:
    struct raw_props_t { double x,y; };
    raw_props_t raw_props;
    CouplingWithData *_cx, *_cy;
    void impl_activate () override;
    void impl_deactivate () override;
  };

  class Poly : public AbstractGShape
  {
  public:
    Poly (FatProcess* parent, const std::string& name, int closed);
    virtual ~Poly ();
    FatProcess* points () { return _points;}
    bool closed () { return _closed;}
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
    Poly* clone () override;
    void set_bounding_box (double x, double y, double w, double h);
  protected:
    void impl_activate () override;
    void impl_deactivate () override;
  private:
    List *_points;
    FatProcess* _bounding_box;
    DoubleProperty *_bbx, *_bby, *_bbw, *_bbh;
    bool _closed;
  };

  class Polygon : public Poly
  {
  public:
    Polygon (FatProcess* parent, const std::string& name) : Poly (parent, name, 1) {};
    virtual ~Polygon () {};
  };

  class Polyline : public Poly
  {
  public:
    Polyline (FatProcess* parent, const std::string& name) : Poly (parent, name, 0) {};
    virtual ~Polyline () {};
  };

  class PathPoint : public AbstractGObj
  {
  public:
    PathPoint (FatProcess* parent, const std::string& name, double x, double y);
    virtual ~PathPoint ();
    virtual ChildProcess* find_child (const std::string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_child("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_child("y"); }
    void draw () override = 0;
    // notify polygon ( (grand-grand-)parent polygon-list-point)
    void notify_change (unsigned int nm) override { _damaged |= nm; get_parent ()->get_parent()->notify_change (nm); }
  protected:
    struct raw_props_t { double x,y; };
    raw_props_t raw_props;
    CouplingWithData *_cx, *_cy;
    void impl_activate () override;
    void impl_deactivate () override;
  };

  class PathMove : public PathPoint
  {
  public:
    PathMove (FatProcess* parent, const std::string& name, double x, double y) :
        PathPoint (parent, name, x, y) {}
    void draw () override;
    PathMove* clone () override;
  };

  class PathLine : public PathPoint
  {
  public:
    PathLine (FatProcess* parent, const std::string& name, double x, double y) :
        PathPoint (parent, name, x, y) {}
    void draw () override;
    PathLine* clone () override;
  };

  class PathQuadratic : public AbstractGObj
  {
  public:
    PathQuadratic (FatProcess* parent, const std::string& name, double x1, double y1, double x, double y);
    virtual ~PathQuadratic ();
    virtual ChildProcess* find_child (const std::string&) override;
    AbstractDoubleProperty* x1 () { return (AbstractDoubleProperty*) find_child("x1"); }
    AbstractDoubleProperty* y1 () { return (AbstractDoubleProperty*) find_child("y1"); }
    AbstractDoubleProperty* x ()  { return (AbstractDoubleProperty*) find_child("x"); }
    AbstractDoubleProperty* y ()  { return (AbstractDoubleProperty*) find_child("y"); }
    void draw () override;
    PathQuadratic* clone () override;
  private:
    struct raw_props_t { double x1,y1,x,y; };
    raw_props_t raw_props;
    CouplingWithData *_cx1, *_cy1, *_cx, *_cy;
    void impl_activate () override;
    void impl_deactivate () override;
  };

  class PathCubic : public AbstractGObj
  {
  public:
    PathCubic (FatProcess* parent, const std::string& name, double x1, double y1, double x2, double y2, double x, double y);
    virtual ~PathCubic ();
    virtual ChildProcess* find_child (const std::string&) override;
    AbstractDoubleProperty* x1 () { return (AbstractDoubleProperty*) find_child("x1"); }
    AbstractDoubleProperty* y1 () { return (AbstractDoubleProperty*) find_child("y1"); }
    AbstractDoubleProperty* x2 () { return (AbstractDoubleProperty*) find_child("x2"); }
    AbstractDoubleProperty* y2 () { return (AbstractDoubleProperty*) find_child("y2"); }
    AbstractDoubleProperty* x ()  { return (AbstractDoubleProperty*) find_child("x"); }
    AbstractDoubleProperty* y ()  { return (AbstractDoubleProperty*) find_child("y"); }
    void draw () override;
    PathCubic* clone () override;
  private:
    struct raw_props_t { double x1,y1,x2,y2,x,y; };
    raw_props_t raw_props;
    CouplingWithData *_cx1, *_cy1, *_cx2, *_cy2, *_cx, *_cy;
    void impl_activate () override;
    void impl_deactivate () override;
  };

  class PathArc : public AbstractGObj
  {
  public:
    PathArc (FatProcess* parent, const std::string& name,  double rx, double ry, double rotx, double fl, double swfl, double x,
                    double y);
    virtual ~PathArc ();
    virtual ChildProcess* find_child (const std::string&) override;
    AbstractDoubleProperty* rotx () { return (AbstractDoubleProperty*) find_child("rotx"); }
    AbstractDoubleProperty* fl ()   { return (AbstractDoubleProperty*) find_child("fl"); }
    AbstractDoubleProperty* swfl () { return (AbstractDoubleProperty*) find_child("swfl"); }
    AbstractDoubleProperty* rx ()   { return (AbstractDoubleProperty*) find_child("rx"); }
    AbstractDoubleProperty* ry ()   { return (AbstractDoubleProperty*) find_child("ry"); }
    AbstractDoubleProperty* x ()    { return (AbstractDoubleProperty*) find_child("x"); }
    AbstractDoubleProperty* y ()    { return (AbstractDoubleProperty*) find_child("y"); }
    void draw () override;
    PathArc* clone () override;
  private:
    struct raw_props_t { double rx,ry,rotx,fl,swfl,x,y; };
    raw_props_t raw_props;
    CouplingWithData *_crx, *_cry, *_crotx, *_cfl, *_cswfl, *_cx, *_cy;
    void impl_activate () override;
    void impl_deactivate () override;
  };

  class PathClosure : public AbstractGObj
  {
  public:
    PathClosure (FatProcess* parent, const std::string& name);
    virtual ~PathClosure () {}
    void draw () override;
    PathClosure* clone () override;
  private:
    void impl_activate () override
    {
      AbstractGObj::impl_activate ();
    }
    void impl_deactivate () override
    {
      AbstractGObj::impl_deactivate ();
    }
  };

  class Path : public AbstractGShape
  {
  public:
    Path (FatProcess* parent, const std::string& name);
    virtual ~Path ();
    FatProcess* items () { return _items;}
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
    Path* clone () override;
    void set_bounding_box (double x, double y, double w, double h);
  protected:
    void impl_activate () override;
    void impl_deactivate () override;
    List *_items;
    FatProcess* _bounding_box;
    DoubleProperty *_bbx, *_bby, *_bbw, *_bbh;
  };

  class PathClip : public Path
  {
  public:
    PathClip (FatProcess* parent, const std::string& name) :
        Path (parent, name) {}
    virtual ~PathClip () {}
    void draw () override;
    PathClip* clone () override;
  };


  class ImageWatcher;

  class Image : public AbstractPathImage
  {
  public:
    Image (FatProcess *parent, const std::string& name, std::string path, double x, double y, double w, double h);
    virtual ~Image ();
    void draw () override;
    Image* clone () override;
    virtual ChildProcess* find_child (const std::string&) override;
    void* cache () { return _cache;}
    void set_cache (void * cache) { _cache = cache;}
    bool invalid_cache () { return _invalid_cache;}
    void set_invalid_cache (bool v) { _invalid_cache = v;}
    void press (double x, double y) override;
    void move (double x, double y) override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
  private:
    Coupling *_cwatcher;
    ImageWatcher *_watcher;
    IntProperty _pixel;
    void impl_activate () override;
    void impl_deactivate () override;
    void *_cache;
    bool _invalid_cache;
  };

  class DataImageWatcher;
  class DataImage : public AbstractDataImage
    {
    public:
      DataImage (FatProcess *parent, const std::string& name, const std::string& data, double x, double y, double w, double h);
      DataImage (FatProcess *parent, const std::string& name, double x, double y, double w, double h);
      virtual ~DataImage ();
      void draw () override;
      DataImage* clone () override;
      virtual ChildProcess* find_child (const std::string&) override;
      void* cache () { return _cache;}
      void set_cache (void * cache) { _cache = cache;}
      bool invalid_cache () { return _invalid_cache;}
      void set_invalid_cache (bool v) { _invalid_cache = v;}
      //void get_bounding_box (double& x, double& y, double& w, double& h) const override;
      //double sdf (double x, double y) const override;
    private:
      Coupling *_cwatcher;
      DataImageWatcher *_watcher;
      void impl_activate () override;
      void impl_deactivate () override;
      void *_cache;
      bool _invalid_cache;
    };

  class ImageWatcher : public Action
  {
  public:
    ImageWatcher (Image *i) :
        Action (i, "ImageWatcher"), _img (i) {}
    virtual ~ImageWatcher () {}
    void impl_activate () override
    {
      _img->set_invalid_cache (true);
    }
    void impl_deactivate () override {};
  private:
    Image * _img;
  };

  class DataImageWatcher : public Action
    {
    public:
      DataImageWatcher (DataImage *i) :
          Action (i, "DataImageWatcher"), _img (i) {}
      virtual ~DataImageWatcher () {}
      void impl_activate () override
      {
        _img->set_invalid_cache (true);
      }
      void impl_deactivate () override {};
    private:
      DataImage * _img;
    };

  class Group : public Container
  {
  public:
    Group (FatProcess *parent, const std::string& name);
    virtual ~Group () override;
    auto
      get_frame () { return _gobj->get_frame ();}
    void impl_activate () override;
    void impl_deactivate () override;
    void draw () override;
    Group* clone () override;
  protected:
    AbstractGObj *_gobj;
  };

  class Defs : public Container
  {
  public:
    Defs (FatProcess *parent, const std::string& name);
    virtual ~Defs () override;
    void impl_activate () override;
    void impl_deactivate () override;
    virtual process_type_e get_process_type () const override { return DEFS_T; }
    Defs* clone () override;
  };
} /* namespace djnn */
