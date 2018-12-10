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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
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
    Rectangle (Process *p, const std::string& n, double x, double y, double w, double h, double rx,
               double ry);
    Rectangle (double x, double y, double w, double h, double rx, double ry);
    virtual ~Rectangle ();
    void draw () override;
    Process* clone () override;
    DoubleProperty* x () { return _x;}
    DoubleProperty* y () { return _y;}
    DoubleProperty* width () { return _width;}
    DoubleProperty* height () { return _height;}
    DoubleProperty* rx () { return _rx;}
    DoubleProperty* ry () { return _ry;}
    void get_properties_values (double &x, double &y, double &w, double &h, double &rx, double &ry);
  private:
    DoubleProperty *_x;
    DoubleProperty *_y;
    DoubleProperty *_width;
    DoubleProperty *_height;
    DoubleProperty *_rx;
    DoubleProperty *_ry;
    Coupling *_cx, *_cy, *_cwidth, *_cheight, *_crx, *_cry;
    void init_rectangle (double x, double y, double w, double h, double rx, double ry);
    void  activate () override;
    void  deactivate () override;
  };

  class Text : public AbstractGShape
  {
    class TextSizeAction : public Process
    { friend Text;
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
    DoubleProperty* x () { return _x;}
    DoubleProperty* y () { return _y;}
    DoubleProperty* dx () { return _dx;}
    DoubleProperty* dy () { return _dy;}
    IntProperty*    dxU () { return _dxU;}
    IntProperty*    dyU () { return _dyU;}
    IntProperty*    width () { return _width;}
    void set_width (double width) { _width->set_value (width, true);}
    IntProperty*    height () { return _height;}
    void set_height (double height) { _height->set_value (height, true);}
    IntProperty*    encoding () { return _encoding;}
    TextProperty*   text (){ return _text;}
  private:
    DoubleProperty *_x;
    DoubleProperty *_y;
    DoubleProperty *_dx;
    DoubleProperty *_dy;
    IntProperty *_dxU;
    IntProperty *_dyU;
    IntProperty *_width;
    IntProperty *_height;
    IntProperty *_encoding;
    TextProperty *_text;
    Coupling *_cx, *_cy, *_ctext, *_ctext_size, *_cffamily, *_cfsize, *_cfstyle, *_cfweight;
    TextProperty *_ffamily;
    DoubleProperty *_fsize;
    IntProperty *_fstyle, *_fweight;
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
    DoubleProperty* x () { return _x;}
    DoubleProperty* y () { return _y;}
    DoubleProperty* width () { return _width;}
    DoubleProperty* height () { return _height;}
    void get_properties_values (double &x, double &y, double &w, double &h);
  private:
    DoubleProperty *_x;
    DoubleProperty *_y;
    DoubleProperty *_width;
    DoubleProperty *_height;
    Coupling *_cx, *_cy, *_cwidth, *_cheight;
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
    DoubleProperty* x1 () { return _x1;}
    DoubleProperty* y1 () { return _y1;}
    DoubleProperty* x2 () { return _x2;}
    DoubleProperty* y2 () { return _y2;}
    void get_properties_values (double &x1, double &y1, double &x2, double &y2);
  private:
    DoubleProperty *_x1;
    DoubleProperty *_y1;
    DoubleProperty *_x2;
    DoubleProperty *_y2;
    Coupling *_cx1, *_cy1, *_cx2, *_cy2;
    void init_line (double x1, double y1, double x2, double y2);
    void activate () override;
    void deactivate () override;
  };

  class PolyPoint : public AbstractGObj
  {
  public:
    PolyPoint (double x, double y);
    PolyPoint (Process* p, const string &n, double x, double y);
    virtual ~PolyPoint ();
    DoubleProperty* x () { return _x;}
    DoubleProperty* y () { return _y;}
    void draw () override;
    Process* clone () override;
  private:
    DoubleProperty *_x, *_y;
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
    PathPoint (Process* p, const string &n, double x, double y);
    PathPoint (double x, double y);
    virtual ~PathPoint ();
    DoubleProperty* x () { return _x;}
    DoubleProperty* y () { return _y;}
    virtual void draw () override = 0;
  protected:
    DoubleProperty *_x, *_y;
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
    DoubleProperty* x () { return _x;}
    DoubleProperty* y () { return _y;}
    DoubleProperty* x1 () { return _x1;}
    DoubleProperty* y1 () { return _y1;}
    void draw () override;
    Process* clone () override;
  private:
    DoubleProperty *_x, *_y, *_x1, *_y1;
    Coupling *_cx, *_cy, *_cx1, *_cy1;
    void activate () override;
    void deactivate () override;
  };

  class PathCubic : public AbstractGObj
  {
  public:
    PathCubic (Process* p, const string &n, double x1, double y1, double x2, double y2, double x, double y);
    PathCubic (double x1, double y1, double x2, double y2, double x, double y);
    virtual ~PathCubic ();
    DoubleProperty* x () { return _x;}
    DoubleProperty* y () { return _y;}
    DoubleProperty* x1 () { return _x1;}
    DoubleProperty* y1 () { return _y1;}
    DoubleProperty* x2 () { return _x2;}
    DoubleProperty* y2 () { return _y2;}
    void draw () override;
    Process* clone () override;
  private:
    DoubleProperty *_x, *_y, *_x1, *_y1, *_x2, *_y2;
    Coupling *_cx, *_cy, *_cx1, *_cy1, *_cx2, *_cy2;
    void activate () override;
    void deactivate () override;
  };

  class PathArc : public AbstractGObj
  {
  public:
    PathArc (Process* p, const string &n, double rotx, double fl, double swfl, double rx, double ry, double x,
             double y);
    PathArc (double rotx, double fl, double swfl, double rx, double ry, double x,
                 double y);
    virtual ~PathArc ();
    DoubleProperty* x () { return _x;}
    DoubleProperty* y () { return _y;}
    DoubleProperty* rotx () {return _rotx;}
    DoubleProperty* fl () { return _fl;}
    DoubleProperty* swfl () { return _swfl;}
    DoubleProperty* rx () { return _rx;}
    DoubleProperty* ry () { return _ry;}
    void draw () override;
    Process* clone () override;
  private:
    DoubleProperty *_x, *_y, *_rotx, *_fl, *_swfl, *_rx, *_ry;
    Coupling *_cx, *_cy, *_crotx, *_cfl, *_cswfl, *_crx, *_cry;
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
    DoubleProperty* x () { return _x;}
    DoubleProperty* y () { return _y;}
    DoubleProperty* width () { return _width;}
    DoubleProperty* height () { return _height;}
    TextProperty* path () { return _path;}
    void* cache () { return _cache;}
    void set_cache (void * cache) { _cache = cache;}
    bool invalid_cache () { return _invalid_cache;}
    void set_invalid_cache (bool v) { _invalid_cache = v;}
  private:
    DoubleProperty *_x;
    DoubleProperty *_y;
    DoubleProperty *_width;
    DoubleProperty *_height;
    TextProperty *_path;
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

  class Ellipse : public AbstractGShape
  {
  public:
    Ellipse (Process *p, const std::string& n, double cx, double cy, double rx, double ry);
    Ellipse (double cx, double cy, double rx, double ry);
    virtual ~Ellipse ();
    void draw () override;
    Process* clone () override;
    DoubleProperty* cx () { return _cx;}
    DoubleProperty* cy () { return _cy;}
    DoubleProperty* rx () { return _rx;}
    DoubleProperty* ry () { return _ry;}
    void get_properties_values (double &cx, double &cy, double &rx, double &ry);
  private:
    DoubleProperty *_cx, *_cy, *_rx, *_ry;
    Coupling *_ccx, *_ccy, *_crx, *_cry;
    void init_ellipse (double cx, double cy, double rx, double ry);
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
    DoubleProperty* cx () { return _cx;}
    DoubleProperty* cy () { return _cy;}
    DoubleProperty* r ()  { return _r; }
    void get_properties_values (double &cx, double &cy, double &r);
  private:
    DoubleProperty *_cx;
    DoubleProperty *_cy;
    DoubleProperty *_r;
    Coupling *_ccx, *_ccy, *_cr;
    void init_circle (double cx, double cy, double r);
    void activate () override;
    void deactivate () override;
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
