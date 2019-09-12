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

#include "gen/rectangle.h"
#include "gen/rectangle_clip.h"
#include "gen/circle.h"
#include "gen/ellipse.h"
#include "gen/line.h"
#include "gen/abstract_image.h"


namespace djnn
{

  class Text : public AbstractGShape
  {
    class TextSizeAction : public Action
    {
      friend Text;
      public:
        TextSizeAction (Process *p, const string &n, Text *text) : Action (p, n), _ff (nullptr), _fsz (nullptr), _fs (nullptr), _fw (nullptr), _text (text) {};
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
    Text (Process *p, const std::string& n, double x, double y, const std::string &text);
    Text (Process *p, const std::string& n, double x, double y, double dx, double dy, int dxu, int dyu,
          const std::string &encoding, const std::string &text);
    Text (double x, double y, const std::string &text);
    virtual ~Text ();
    void draw () override;
    Process* clone () override;
    void get_properties_values (double &x, double &y, double &dx, double &dy, double &fsize, int &dxU, int &dyU, int &width, int &height, int &encoding, int &fstyle, int &fweight, string &text, string &ffamily);
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
    void set_parent (Process* p) override;
    struct raw_props_t { double x, y, dx, dy, fsize; int dxU, dyU, width, height, encoding, fstyle, fweight; string text, ffamily;};
    raw_props_t raw_props;    
    Coupling *_cx, *_cy, *_cdx, *_cdy, *_cfsize, 
      *_cdxU, *_cdyU, *_cwidth, *_cheight, *_cencoding,
      *_cfstyle, *_cfweight,
      *_ctext, *_cffamily;

    TextSizeAction *_update_size;
    
    void init_text (double x, double y, double dx, double dy, int dxu, int dyu,
              const std::string &encoding, const std::string &text);
    void impl_activate () override;
    void impl_deactivate () override;
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
    void impl_activate () override;
    void impl_deactivate () override;
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
    void impl_activate () override;
    void impl_deactivate () override;
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
    void impl_activate () override;
    void impl_deactivate () override;
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
    void impl_activate () override;
    void impl_deactivate () override;
  };

  class PathArc : public AbstractGObj
  {
  public:
    PathArc (Process* p, const string &n,  double rx, double ry, double rotx, double fl, double swfl, double x,
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
    PathClosure (Process* p, const string &n);
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
    Path (Process* p, const string &n);
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
    PathClip (Process* p, const string &n) :
        Path (p, n) {}
    PathClip () : 
        Path () {}
    virtual ~PathClip () {}
    void draw () override;
    Process* clone () override;
  };

  class ImageWatcher;

  /*class AbstractImage : public AbstractGShape
  {
  public:
    AbstractImage (Process *p, const std::string& n, const std::string &path, double x, double y, double w, double h);
    AbstractImage (const std::string &path, double x, double y, double w, double h);
    virtual ~AbstractImage ();
    void get_properties_values (std::string &path, double &x, double &y, double &w, double &h);
    void draw () override;
    Process* clone () override;
    virtual Process* find_component (const string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_component("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_component("y"); }
    AbstractDoubleProperty* width () { return (AbstractDoubleProperty*) find_component("width"); }
    AbstractDoubleProperty* height () { return (AbstractDoubleProperty*) find_component("height"); }
    AbstractTextProperty* path () { return (AbstractTextProperty*) find_component("path"); }
    
  private:
    struct raw_props_t { double x,y,width,height; string path; };
    raw_props_t raw_props;
    Coupling *_cx, *_cy, *_cwidth, *_cheight, *_cpath, *_cwatcher;
    void impl_activate () override;
    void impl_deactivate () override;
    void *_cache;
  };*/

  class Image : public AbstractImage
  {
  public:
    Image (Process *p, const std::string& n, std::string path, double x, double y, double w, double h);
    Image (std::string path, double x, double y, double w, double h);
    virtual ~Image ();
    //void get_properties_values (std::string &path, double &x, double &y, double &w, double &h);
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
    Coupling *_cx, *_cy, *_cwidth, *_cheight, *_cpath, *_cwatcher;
    ImageWatcher *_watcher;
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

  class Group : public Container
  {
  public:
    Group (Process *p, const string &n);
    Group ();
    virtual ~Group () override;
    //Window*
    //std::weak_ptr<Window>
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
