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

#include "../../core/tree/list.h"
#include "../abstract_gshape.h"

#include "../../core/tree/int_property.h"
#include "../../core/tree/text_property.h"
#include "../style/style.h"

namespace djnn
{
  class Rectangle : public AbstractGShape
  {
  public:
    Rectangle (Process *p, const std::string& n, double x, double y, double w, double h, double rx, double ry);
    Rectangle (double x, double y, double w, double h, double rx, double ry);
    virtual ~Rectangle ();
    void draw () override;
    Process* clone () override;
    void get_properties_values (double &x, double &y, double &w, double &h, double &rx, double &ry);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_component("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_component("y"); }
    AbstractDoubleProperty* width () { return (AbstractDoubleProperty*) find_component("width"); }
    AbstractDoubleProperty* height () { return (AbstractDoubleProperty*) find_component("height"); }
    AbstractDoubleProperty* rx () { return (AbstractDoubleProperty*) find_component("rx"); }
    AbstractDoubleProperty* ry () { return (AbstractDoubleProperty*) find_component("ry"); }

  private:
    struct raw_props_t { double x,y,width,height,rx,ry; };
    raw_props_t raw_props;
    Coupling *_cx, *_cy, *_cwidth, *_cheight, *_crx, *_cry;
    void activate () override;
    void deactivate () override;
  };
  
  class Circle : public AbstractGShape
  {
  public:
    Circle (Process *p, const std::string& n, double cx, double cy, double r);
    Circle (double cx, double cy, double r);
    virtual ~Circle ();
    void draw () override;
    Process* clone () override;
    void get_properties_values (double &cx, double &cy, double &r);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* cx () { return (AbstractDoubleProperty*) find_component("cx"); }
    AbstractDoubleProperty* cy () { return (AbstractDoubleProperty*) find_component("cy"); }
    AbstractDoubleProperty* r ()  { return (AbstractDoubleProperty*) find_component("r"); }
  private:
    struct raw_props_t { double cx, cy, r; };
    raw_props_t raw_props;
    Coupling *_ccx, *_ccy, *_cr;
    void activate () override;
    void deactivate () override;
  };

  class Ellipse : public AbstractGShape
  {
  public:
    Ellipse (Process *p, const std::string& n, double cx, double cy, double rx, double ry);
    Ellipse (double cx, double cy, double rx, double ry);
    virtual ~Ellipse ();
    void draw () override;
    Process* clone () override;
    void get_properties_values (double &cx, double &cy, double &rx, double &ry);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* cx () { return (AbstractDoubleProperty*) find_component("cx"); }
    AbstractDoubleProperty* cy () { return (AbstractDoubleProperty*) find_component("cy"); }
    AbstractDoubleProperty* rx () { return (AbstractDoubleProperty*) find_component("rx"); }
    AbstractDoubleProperty* ry () { return (AbstractDoubleProperty*) find_component("ry"); }
  private:
    struct raw_props_t { double cx, cy, rx, ry; };
    raw_props_t raw_props;
    Coupling *_ccx, *_ccy, *_crx, *_cry;
    void activate () override;
    void deactivate () override;
  };

  class Line : public AbstractGShape
  {
  public:
    Line (Process *p, const std::string& n, double x1, double y1, double x2, double y2);
    Line (double x1, double y1, double x2, double y2);
    virtual ~Line ();
    void draw () override;
    Process* clone () override;
    void get_properties_values (double &x1, double &y1, double &x2, double &y2);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* x1 () { return (AbstractDoubleProperty*) find_component("x1"); }
    AbstractDoubleProperty* y1 () { return (AbstractDoubleProperty*) find_component("y1"); }
    AbstractDoubleProperty* x2 () { return (AbstractDoubleProperty*) find_component("x2"); }
    AbstractDoubleProperty* y2 () { return (AbstractDoubleProperty*) find_component("y2"); }
  private:
    struct raw_props_t { double x1, y1, x2, y2; };
    raw_props_t raw_props;
    Coupling *_cx1, *_cy1, *_cx2, *_cy2;
    void activate () override;
    void deactivate () override;
  };

  class Text : public AbstractGShape
  {
    class TextSizeAction : public Process
    {
      friend Text;
      public:
        TextSizeAction (Process *p, const string &n, Text *text) : Process (p, n), _ff (nullptr), _fsz (nullptr), _fs (nullptr), _fw (nullptr), _text (text) {};
        ~TextSizeAction () {}
        void activate () override;
        void deactivate () override {};
      private:
        FontFamily*_ff;
        FontSize* _fsz;
        FontStyle* _fs;
        FontWeight* _fw;
        Text* _text;
    };
  public:
    Text (Process *p, const std::string& n, double x, double y, const std::string &text);
    Text (Process *p, const std::string& n, double x, double y, double dx, double dy, int dxu, int dyu,
          const std::string &encoding, const std::string &text);
    Text (double x, double y, const std::string &text);
    virtual ~Text ();
    void draw () override;
    Process* clone () override;
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_component("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_component("y"); }
    AbstractDoubleProperty* dx () { return (AbstractDoubleProperty*) find_component("dx"); }
    AbstractDoubleProperty* dy () { return (AbstractDoubleProperty*) find_component("dy"); }
    AbstractDoubleProperty* fsize () { return (AbstractDoubleProperty*) find_component("fsize"); }
    AbstractIntProperty*    dxU () { return (AbstractIntProperty*) find_component("dxU"); }
    AbstractIntProperty*    dyU () { return (AbstractIntProperty*) find_component("dyU"); }
    AbstractIntProperty*    width () { return (AbstractIntProperty*) find_component("width"); }
    AbstractIntProperty*    height () { return (AbstractIntProperty*) find_component("height"); }
    AbstractIntProperty*    encoding () { return (AbstractIntProperty*) find_component("encoding"); }
    AbstractIntProperty*    fweight () { return (AbstractIntProperty*) find_component("fweight"); }
    AbstractIntProperty*    fstyle () { return (AbstractIntProperty*) find_component("fstyle");}
    AbstractTextProperty*   text () { return (AbstractTextProperty*) find_component("text"); }
    AbstractTextProperty*   ffamily () { return (AbstractTextProperty*) find_component("ffamily"); }
    void set_width (double width) { this->width ()->set_value (width, true); }
    void set_height (double height) { this->height ()->set_value (height, true); }
  private:
    struct raw_props_t { double x, y, dx, dy, fsize; int dxU, dyU, width, height, encoding, fstyle, fweight; string text, ffamily;};
    raw_props_t raw_props;    
    Coupling *_cx, *_cy, *_cdx, *_cdy, *_cfsize, 
      *_cdxU, *_cdyU, *_cwidth, *_cheight, *_cencoding,
      *_cfstyle, *_cfweight,
      *_ctext, *_cffamily;

    TextSizeAction *_update_size;
    
    void init_text (double x, double y, double dx, double dy, int dxu, int dyu,
              const std::string &encoding, const std::string &text);
    void activate () override;
    void deactivate () override;
  };

  class RectangleClip : public AbstractGShape
  {
  public:
    RectangleClip (Process *p, const std::string& n, double x, double y, double w, double h);
    RectangleClip (double x, double y, double w, double h);
    virtual ~RectangleClip ();
    void draw () override;
    Process* clone () override;
    void get_properties_values (double &x, double &y, double &w, double &h);
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_component("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_component("y"); }
    AbstractDoubleProperty* width () { return (AbstractDoubleProperty*) find_component("width"); }
    AbstractDoubleProperty* height () { return (AbstractDoubleProperty*) find_component("height"); }
  private:
    struct raw_props_t { double x,y,width,height; };
    raw_props_t raw_props;
    Coupling *_cx, *_cy, *_cwidth, *_cheight;
    void activate () override;
    void deactivate () override;
  };

  class PolyPoint : public AbstractGObj
  {
  public:
    PolyPoint (double x, double y);
    PolyPoint (Process* p, const string &n, double x, double y);
    virtual ~PolyPoint ();
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_component("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_component("y"); }
    void draw () override;
    Process* clone () override;
    // notify polygon ( (grand-grand-)parent polygon-list-point)
    void notify_change (unsigned int nm) override { _damaged |= nm; _parent->get_parent()->notify_change (nm); }
  protected:
    struct raw_props_t { double x,y; };
    raw_props_t raw_props;
    Coupling *_cx, *_cy;
    void activate () override;
    void deactivate () override;
  };

  class Poly : public AbstractGShape
  {
  public:
    Poly (int closed);
    Poly (Process* p, const string &n, int closed);
    virtual ~Poly ();
    Process* points () { return _points;}
    bool closed () { return _closed;}
    void draw () override;
    Process* clone () override;
    void set_bounding_box (double x, double y, double w, double h);
  protected:
    void activate () override;
    void deactivate () override;
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
    Polygon (Process* p, const string &n) : Poly (p, n, 1) {};
    virtual ~Polygon () {};
  };

  class Polyline : public Poly
  {
  public:
    Polyline () : Poly (0) {};
    Polyline (Process* p, const string &n) : Poly (p, n, 0) {};
    virtual ~Polyline () {};
  };

  class PathPoint : public AbstractGObj
  {
  public:
    PathPoint (double x, double y);
    PathPoint (Process* p, const string &n, double x, double y);
    virtual ~PathPoint ();
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_component("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_component("y"); }
    void draw () override = 0;
    // notify polygon ( (grand-grand-)parent polygon-list-point)
    void notify_change (unsigned int nm) override { _damaged |= nm; _parent->get_parent()->notify_change (nm); }
  protected:
    struct raw_props_t { double x,y; };
    raw_props_t raw_props;
    Coupling *_cx, *_cy;
    void activate () override;
    void deactivate () override;
  };

  class PathMove : public PathPoint
  {
  public:
    PathMove (Process* p, const string &n, double x, double y) :
        PathPoint (p, n, x, y) {}
    PathMove (double x, double y) :
            PathPoint (x, y) {}
    void draw () override;
    Process* clone () override;
  };

  class PathLine : public PathPoint
  {
  public:
    PathLine (Process* p, const string &n, double x, double y) :
        PathPoint (p, n, x, y) {}
    PathLine (double x, double y) :
        PathPoint (x, y) {}
    void draw () override;
    Process* clone () override;
  };

  class PathQuadratic : public AbstractGObj
  {
  public:
    PathQuadratic (Process* p, const string &n, double x1, double y1, double x, double y);
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
    void activate () override;
    void deactivate () override;
  };

  class PathCubic : public AbstractGObj
  {
  public:
    PathCubic (Process* p, const string &n, double x1, double y1, double x2, double y2, double x, double y);
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
    DoublePropertyProxy *_x1, *_y1, *_x2, *_y2, *_x, *_y;
    Coupling *_cx1, *_cy1, *_cx2, *_cy2, *_cx, *_cy;
    void activate () override;
    void deactivate () override;
  };

  class PathArc : public AbstractGObj
  {
  public:
    PathArc (Process* p, const string &n, double rotx, double fl, double swfl, double rx, double ry, double x, double y);
    PathArc (double rotx, double fl, double swfl, double rx, double ry, double x, double y);
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
    struct raw_props_t { double rotx,roty,fl,swfl,rx,ry,x,y; };
    raw_props_t raw_props;
    Coupling *_crotx, *_cfl, *_cswfl, *_crx, *_cry, *_cx, *_cy;
    void activate () override;
    void deactivate () override;
  };

  class PathClosure : public AbstractGObj
  {
  public:
    PathClosure (Process* p, const string &n);
    PathClosure () : AbstractGObj () {};
    virtual ~PathClosure () {}
    void draw () override;
    Process* clone () override;
  private:
    void activate () override
    {
      AbstractGObj::activate ();
    }
    void deactivate () override
    {
      AbstractGObj::deactivate ();
    }
  };

  class Path : public djnn::AbstractGShape
  {
  public:
    Path ();
    Path (Process* p, const string &n);
    virtual ~Path ();
    Process* items () { return _items;}
    void draw () override;
    Process* clone () override;
    void set_bounding_box (double x, double y, double w, double h);
  protected:
    void activate () override;
    void deactivate () override;
    List *_items;
    Process* _bounding_box;
    DoubleProperty *_bbx, *_bby, *_bbw, *_bbh;
  };

  class PathClip : public Path
  {
  public:
    PathClip (Process* p, const string &n) :
        Path (p, n) {}
    PathClip () : 
        Path () {}
    virtual ~PathClip () {}
    void draw () override;
    Process* clone () override;
  };

  class ImageWatcher;

  class Image : public AbstractGShape
  {
  public:
    Image (Process *p, const std::string& n, const std::string &path, double x, double y, double w, double h);
    Image (const std::string &path, double x, double y, double w, double h);
    virtual ~Image ();
    void draw () override;
    Process* clone () override;
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_component("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_component("y"); }
    AbstractDoubleProperty* width () { return (AbstractDoubleProperty*) find_component("width"); }
    AbstractDoubleProperty* height () { return (AbstractDoubleProperty*) find_component("height"); }
    AbstractTextProperty* path () { return (AbstractTextProperty*) find_component("path"); }
    void* cache () { return _cache;}
    void set_cache (void * cache) { _cache = cache;}
    bool invalid_cache () { return _invalid_cache;}
    void set_invalid_cache (bool v) { _invalid_cache = v;}
  private:
    struct raw_props_t { double x,y,width,height; string path; };
    raw_props_t raw_props;
    Coupling *_cx, *_cy, *_cwidth, *_cheight, *_cpath;
    ImageWatcher *_watcher;
    void activate () override;
    void deactivate () override;
    void *_cache;
    bool _invalid_cache;
  };

  class ImageWatcher : public Process
  {
  public:
    ImageWatcher (Image *i) :
        Process (), _img (i) {}
    virtual ~ImageWatcher () {}
    void activate () override
    {
      _img->set_invalid_cache (true);
    }
    void deactivate () override {};
  private:
    Image * _img;
  };

  class Group : public Container
  {
  public:
    Group (Process *p, const string &n);
    Group ();
    virtual ~Group () override;
    Window* frame () { return _gobj->frame ();}
    void activate () override;
    void deactivate () override;
    void draw () override;
    Process* clone () override;
  protected:
    AbstractGObj *_gobj;
  };
} /* namespace djnn */
