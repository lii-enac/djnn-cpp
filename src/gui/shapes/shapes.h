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
#include "gui/abstract_gshape.h"

#include "core/tree/int_property.h"
#include "core/tree/text_property.h"
#include "gui/style/style.h"

#include "gen/abstract_prop_rectangle.h"
#include "gen/abstract_prop_circle.h"
#include "gen/rectangle_clip.h"
#include "gen/ellipse.h"
#include "gen/line.h"
#include "gen/abstract_image.h"
#include "gen/abstract_path_image.h"
#include "gen/abstract_data_image.h"

namespace djnn
{
  typedef void* FontMetricsImpl ;

  class Rectangle : public AbstractPropRectangle
  {
  public:
    Rectangle (Process *parent, const std::string& name, double x, double y, double width, double height, double rx=0, double ry=0);
    Rectangle (double x, double y, double width, double height, double rx=0, double ry=0);
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
  };

  class Circle : public AbstractPropCircle
  {
  public:
    Circle (Process *parent, const std::string& name, double cx, double cy, double r);
    Circle (double cx, double cy, double r);
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
  };

  class Text : public AbstractGShape
  {
    class TextSizeAction : public Action
    {
      friend Text;
      public:
        TextSizeAction (Process *parent, const string &name, Text *text) : Action (parent, name), _ff (nullptr), _fsz (nullptr), _fs (nullptr), _fw (nullptr), _text (text) {};
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
    Text (Process *parent, const std::string& name, double x, double y, const std::string &text);
    Text (Process *parent, const std::string& name, double x, double y, double dx, double dy, int dxu, int dyu,
          const std::string &encoding, const std::string &text);
    Text (double x, double y, const std::string &text);
    virtual ~Text ();
    void draw () override;
    Process* clone () override;
    void get_properties_values (double &x, double &y, double &dx, double &dy, int &dxU, int &dyU, int &width, int &height, int &encoding, string &text);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_component("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_component("y"); }
    AbstractDoubleProperty* dx () { return (AbstractDoubleProperty*) find_component("dx"); }
    AbstractDoubleProperty* dy () { return (AbstractDoubleProperty*) find_component("dy"); }
    AbstractIntProperty*    dxU () { return (AbstractIntProperty*) find_component("dxU"); }
    AbstractIntProperty*    dyU () { return (AbstractIntProperty*) find_component("dyU"); }
    AbstractIntProperty*    encoding () { return (AbstractIntProperty*) find_component("encoding"); }
    IntProperty*  width () { return &_width; }
    IntProperty*  height () { return &_height; }
    TextProperty* text () { return &_text;}
    void set_width (double width) { _width.set_value (width, true); }
    void set_height (double height) { _height.set_value (height, true); }
    const string& get_raw_text () { return _text.get_value (); }
    double get_cursor_from_index (int index);
    std::pair<double,int> get_cursor_from_local_x (double pos);
    FontMetricsImpl get_font_metrics () { return _fm; };
    void set_font_metrics (FontMetricsImpl *fm) { _fm = fm; }
  private:
    void set_parent (Process* p) override;
    struct raw_props_t { double x, y, dx, dy; int dxU, dyU, encoding, fstyle, fweight;};
    raw_props_t raw_props;    
    Coupling *_cx, *_cy, *_cdx, *_cdy, *_cfsize, 
      *_cdxU, *_cdyU, *_cencoding,
      *_cfstyle, *_cfweight, *_cffamily;

    /* implementation */
    FontMetricsImpl _fm;

    /* no need proxy because always create */
    TextSizeAction _update_size;
    IntProperty _width;
    IntProperty _height;
    TextProperty _text;
    Coupling _cupdate_size, _ctext;

    void impl_activate () override;
    void impl_deactivate () override;
  };

  class PolyPoint : public AbstractGObj
  {
  public:
    PolyPoint (double x, double y);
    PolyPoint (Process* parent, const string &name, double x, double y);
    virtual ~PolyPoint ();
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_component("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_component("y"); }
    void draw () override;
    Process* clone () override;
    // notify polygon ( (grand-grand-)parent polygon-list-point)
    void notify_change (unsigned int nm) override { _damaged |= nm; get_parent ()->get_parent()->notify_change (nm); }
  protected:
    struct raw_props_t { double x,y; };
    raw_props_t raw_props;
    Coupling *_cx, *_cy;
    void impl_activate () override;
    void impl_deactivate () override;
  };

  class Poly : public AbstractGShape
  {
  public:
    Poly (int closed);
    Poly (Process* parent, const string &name, int closed);
    virtual ~Poly ();
    Process* points () { return _points;}
    bool closed () { return _closed;}
    void draw () override;
    Process* clone () override;
    void set_bounding_box (double x, double y, double w, double h);
  protected:
    void impl_activate () override;
    void impl_deactivate () override;
  private:
    List *_points;
    Process* _bounding_box;
    DoubleProperty *_bbx, *_bby, *_bbw, *_bbh;
    bool _closed;
  };

  class Polygon : public Poly
  {
  public:
    Polygon () : Poly (1) {};
    Polygon (Process* parent, const string &name) : Poly (parent, name, 1) {};
    virtual ~Polygon () {};
  };

  class Polyline : public Poly
  {
  public:
    Polyline () : Poly (0) {};
    Polyline (Process* parent, const string &name) : Poly (parent, name, 0) {};
    virtual ~Polyline () {};
  };

  class PathPoint : public AbstractGObj
  {
  public:
    PathPoint (double x, double y);
    PathPoint (Process* parent, const string &name, double x, double y);
    virtual ~PathPoint ();
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_component("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_component("y"); }
    void draw () override = 0;
    // notify polygon ( (grand-grand-)parent polygon-list-point)
    void notify_change (unsigned int nm) override { _damaged |= nm; get_parent ()->get_parent()->notify_change (nm); }
  protected:
    struct raw_props_t { double x,y; };
    raw_props_t raw_props;
    Coupling *_cx, *_cy;
    void impl_activate () override;
    void impl_deactivate () override;
  };

  class PathMove : public PathPoint
  {
  public:
    PathMove (Process* parent, const string &name, double x, double y) :
        PathPoint (parent, name, x, y) {}
    PathMove (double x, double y) :
            PathPoint (x, y) {}
    void draw () override;
    Process* clone () override;
  };

  class PathLine : public PathPoint
  {
  public:
    PathLine (Process* parent, const string &name, double x, double y) :
        PathPoint (parent, name, x, y) {}
    PathLine (double x, double y) :
        PathPoint (x, y) {}
    void draw () override;
    Process* clone () override;
  };

  class PathQuadratic : public AbstractGObj
  {
  public:
    PathQuadratic (Process* parent, const string &name, double x1, double y1, double x, double y);
    PathQuadratic (double x1, double y1, double x, double y);
    virtual ~PathQuadratic ();
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* x1 () { return (AbstractDoubleProperty*) find_component("x1"); }
    AbstractDoubleProperty* y1 () { return (AbstractDoubleProperty*) find_component("y1"); }
    AbstractDoubleProperty* x ()  { return (AbstractDoubleProperty*) find_component("x"); }
    AbstractDoubleProperty* y ()  { return (AbstractDoubleProperty*) find_component("y"); }
    void draw () override;
    Process* clone () override;
  private:
    struct raw_props_t { double x1,y1,x,y; };
    raw_props_t raw_props;
    Coupling *_cx1, *_cy1, *_cx, *_cy;
    void impl_activate () override;
    void impl_deactivate () override;
  };

  class PathCubic : public AbstractGObj
  {
  public:
    PathCubic (Process* parent, const string &name, double x1, double y1, double x2, double y2, double x, double y);
    PathCubic (double x1, double y1, double x2, double y2, double x, double y);
    virtual ~PathCubic ();
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* x1 () { return (AbstractDoubleProperty*) find_component("x1"); }
    AbstractDoubleProperty* y1 () { return (AbstractDoubleProperty*) find_component("y1"); }
    AbstractDoubleProperty* x2 () { return (AbstractDoubleProperty*) find_component("x2"); }
    AbstractDoubleProperty* y2 () { return (AbstractDoubleProperty*) find_component("y2"); }
    AbstractDoubleProperty* x ()  { return (AbstractDoubleProperty*) find_component("x"); }
    AbstractDoubleProperty* y ()  { return (AbstractDoubleProperty*) find_component("y"); }
    void draw () override;
    Process* clone () override;
  private:
    struct raw_props_t { double x1,y1,x2,y2,x,y; };
    raw_props_t raw_props;
    Coupling *_cx1, *_cy1, *_cx2, *_cy2, *_cx, *_cy;
    void impl_activate () override;
    void impl_deactivate () override;
  };

  class PathArc : public AbstractGObj
  {
  public:
    PathArc (Process* parent, const string &name,  double rx, double ry, double rotx, double fl, double swfl, double x,
                    double y);
    PathArc ( double rx, double ry, double rotx, double fl, double swfl, double x,
                    double y);
    virtual ~PathArc ();
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* rotx () { return (AbstractDoubleProperty*) find_component("rotx"); }
    AbstractDoubleProperty* fl ()   { return (AbstractDoubleProperty*) find_component("fl"); }
    AbstractDoubleProperty* swfl () { return (AbstractDoubleProperty*) find_component("swfl"); }
    AbstractDoubleProperty* rx ()   { return (AbstractDoubleProperty*) find_component("rx"); }
    AbstractDoubleProperty* ry ()   { return (AbstractDoubleProperty*) find_component("ry"); }
    AbstractDoubleProperty* x ()    { return (AbstractDoubleProperty*) find_component("x"); }
    AbstractDoubleProperty* y ()    { return (AbstractDoubleProperty*) find_component("y"); }
    void draw () override;
    Process* clone () override;
  private:
    struct raw_props_t { double rx,ry,rotx,fl,swfl,x,y; };
    raw_props_t raw_props;
    Coupling *_crx, *_cry, *_crotx, *_cfl, *_cswfl, *_cx, *_cy;
    void impl_activate () override;
    void impl_deactivate () override;
  };

  class PathClosure : public AbstractGObj
  {
  public:
    PathClosure (Process* parent, const string &name);
    PathClosure () : AbstractGObj () {};
    virtual ~PathClosure () {}
    void draw () override;
    Process* clone () override;
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
    Path ();
    Path (Process* parent, const string &name);
    virtual ~Path ();
    Process* items () { return _items;}
    void draw () override;
    Process* clone () override;
    void set_bounding_box (double x, double y, double w, double h);
  protected:
    void impl_activate () override;
    void impl_deactivate () override;
    List *_items;
    Process* _bounding_box;
    DoubleProperty *_bbx, *_bby, *_bbw, *_bbh;
  };

  class PathClip : public Path
  {
  public:
    PathClip (Process* parent, const string &name) :
        Path (parent, name) {}
    PathClip () : 
        Path () {}
    virtual ~PathClip () {}
    void draw () override;
    Process* clone () override;
  };


  class ImageWatcher;

  class Image : public AbstractPathImage
  {
  public:
    Image (Process *parent, const std::string& name, std::string path, double x, double y, double w, double h);
    Image (std::string path, double x, double y, double w, double h);
    virtual ~Image ();
    void draw () override;
    Process* clone () override;
    virtual Process* find_component (const string&) override;
    void* cache () { return _cache;}
    void set_cache (void * cache) { _cache = cache;}
    bool invalid_cache () { return _invalid_cache;}
    void set_invalid_cache (bool v) { _invalid_cache = v;}
  private:
    Coupling *_cwatcher;
    ImageWatcher *_watcher;
    void impl_activate () override;
    void impl_deactivate () override;
    void *_cache;
    bool _invalid_cache;
  };

  class DataImageWatcher;
  class DataImage : public AbstractDataImage
    {
    public:
      DataImage (Process *parent, const std::string& name, std::string data, double x, double y, double w, double h);
      DataImage (std::string path, double x, double y, double w, double h);
      virtual ~DataImage ();
      void draw () override;
      Process* clone () override;
      virtual Process* find_component (const string&) override;
      void* cache () { return _cache;}
      void set_cache (void * cache) { _cache = cache;}
      bool invalid_cache () { return _invalid_cache;}
      void set_invalid_cache (bool v) { _invalid_cache = v;}
    private:
      Coupling *_cwatcher;
      DataImageWatcher *_watcher;
      void impl_activate () override;
      void impl_deactivate () override;
      void *_cache;
      bool _invalid_cache;
    };

  class ImageWatcher : public Process
  {
  public:
    ImageWatcher (Image *i) :
        Process (), _img (i) {}
    virtual ~ImageWatcher () {}
    void impl_activate () override
    {
      _img->set_invalid_cache (true);
    }
    void impl_deactivate () override {};
  private:
    Image * _img;
  };

  class DataImageWatcher : public Process
    {
    public:
      DataImageWatcher (DataImage *i) :
          Process (), _img (i) {}
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
    Group (Process *parent, const string &name);
    Group ();
    virtual ~Group () override;
    auto
      frame () { return _gobj->frame ();}
    void impl_activate () override;
    void impl_deactivate () override;
    void draw () override;
    Process* clone () override;
  protected:
    AbstractGObj *_gobj;
  };
} /* namespace djnn */
