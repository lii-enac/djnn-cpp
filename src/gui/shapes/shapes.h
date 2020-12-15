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

#include "gui/shapes/ellipse.h"
#include "gui/shapes/text.h"

#include "gen/abstract_prop_rectangle.h"
#include "gen/abstract_prop_circle.h"
#include "gen/abstract_prop_rectangle_clip.h"
#include "gen/abstract_prop_line.h"
#include "gen/abstract_image.h"
#include "gen/abstract_path_image.h"
#include "gen/abstract_data_image.h"

namespace djnn
{
  class Rectangle : public AbstractPropRectangle
  {
  public:
    Rectangle (ParentProcess* parent, const std::string& name, double x, double y, double width, double height, double rx=0, double ry=0);
    Rectangle* clone () override;
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
  };

  class Circle : public AbstractPropCircle
  {
  public:
    Circle (ParentProcess* parent, const std::string& name, double cx, double cy, double r);
    Circle* clone () override;
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
  };

  class Line : public AbstractPropLine
  {
  public:
    Line (ParentProcess* parent, const std::string& name, double x1, double y1, double x2, double y2);
    Line* clone () override;
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
  };

  class RectangleClip : public AbstractPropRectangleClip
  {
  public:
    RectangleClip (ParentProcess* parent, const std::string& name, double x, double y, double width, double height);
    RectangleClip* clone () override;
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
  };

  class PolyPoint : public AbstractGObj
  {
  public:
    PolyPoint (ParentProcess* parent, const std::string& name, double x, double y);
    virtual ~PolyPoint ();
    virtual FatChildProcess* find_child_impl (const std::string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_child_impl("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_child_impl("y"); }
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
    Poly (ParentProcess* parent, const std::string& name, int closed);
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
    Polygon (ParentProcess* parent, const std::string& name) : Poly (parent, name, 1) {};
    virtual ~Polygon () {};
  };

  class Polyline : public Poly
  {
  public:
    Polyline (ParentProcess* parent, const std::string& name) : Poly (parent, name, 0) {};
    virtual ~Polyline () {};
  };

  class PathPoint : public AbstractGObj
  {
  public:
    PathPoint (ParentProcess* parent, const std::string& name, double x, double y);
    virtual ~PathPoint ();
    virtual FatChildProcess* find_child_impl (const std::string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_child_impl("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_child_impl("y"); }
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
    PathMove (ParentProcess* parent, const std::string& name, double x, double y) :
        PathPoint (parent, name, x, y) {}
    void draw () override;
    PathMove* clone () override;
  };

  class PathLine : public PathPoint
  {
  public:
    PathLine (ParentProcess* parent, const std::string& name, double x, double y) :
        PathPoint (parent, name, x, y) {}
    void draw () override;
    PathLine* clone () override;
  };

  class PathQuadratic : public AbstractGObj
  {
  public:
    PathQuadratic (ParentProcess* parent, const std::string& name, double x1, double y1, double x, double y);
    virtual ~PathQuadratic ();
    virtual FatChildProcess* find_child_impl (const std::string&) override;
    AbstractDoubleProperty* x1 () { return (AbstractDoubleProperty*) find_child_impl("x1"); }
    AbstractDoubleProperty* y1 () { return (AbstractDoubleProperty*) find_child_impl("y1"); }
    AbstractDoubleProperty* x ()  { return (AbstractDoubleProperty*) find_child_impl("x"); }
    AbstractDoubleProperty* y ()  { return (AbstractDoubleProperty*) find_child_impl("y"); }
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
    PathCubic (ParentProcess* parent, const std::string& name, double x1, double y1, double x2, double y2, double x, double y);
    virtual ~PathCubic ();
    virtual FatChildProcess* find_child_impl (const std::string&) override;
    AbstractDoubleProperty* x1 () { return (AbstractDoubleProperty*) find_child_impl("x1"); }
    AbstractDoubleProperty* y1 () { return (AbstractDoubleProperty*) find_child_impl("y1"); }
    AbstractDoubleProperty* x2 () { return (AbstractDoubleProperty*) find_child_impl("x2"); }
    AbstractDoubleProperty* y2 () { return (AbstractDoubleProperty*) find_child_impl("y2"); }
    AbstractDoubleProperty* x ()  { return (AbstractDoubleProperty*) find_child_impl("x"); }
    AbstractDoubleProperty* y ()  { return (AbstractDoubleProperty*) find_child_impl("y"); }
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
    PathArc (ParentProcess* parent, const std::string& name,  double rx, double ry, double rotx, double fl, double swfl, double x,
                    double y);
    virtual ~PathArc ();
    virtual FatChildProcess* find_child_impl (const std::string&) override;
    AbstractDoubleProperty* rotx () { return (AbstractDoubleProperty*) find_child_impl("rotx"); }
    AbstractDoubleProperty* fl ()   { return (AbstractDoubleProperty*) find_child_impl("fl"); }
    AbstractDoubleProperty* swfl () { return (AbstractDoubleProperty*) find_child_impl("swfl"); }
    AbstractDoubleProperty* rx ()   { return (AbstractDoubleProperty*) find_child_impl("rx"); }
    AbstractDoubleProperty* ry ()   { return (AbstractDoubleProperty*) find_child_impl("ry"); }
    AbstractDoubleProperty* x ()    { return (AbstractDoubleProperty*) find_child_impl("x"); }
    AbstractDoubleProperty* y ()    { return (AbstractDoubleProperty*) find_child_impl("y"); }
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
    PathClosure (ParentProcess* parent, const std::string& name);
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
    Path (ParentProcess* parent, const std::string& name);
    virtual ~Path ();
    FatProcess* items () { return _items;}
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
    Path* clone () override;
    void set_bounding_box (double x, double y, double w, double h);
    void invalidate_bounding_box () { _bbw->set_value(-1, true); }
    bool is_bounding_box_valid () const { return _bbw->get_value()>=0; }
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
    PathClip (ParentProcess* parent, const std::string& name) :
        Path (parent, name) {}
    virtual ~PathClip () {}
    void draw () override;
    PathClip* clone () override;
  };


  class ImageWatcher;

  class Image : public AbstractPathImage
  {
  public:
    Image (ParentProcess* parent, const std::string& name, std::string path, double x, double y, double w, double h);
    virtual ~Image ();
    void draw () override;
    Image* clone () override;
    virtual FatChildProcess* find_child_impl (const std::string&) override;
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
      DataImage (ParentProcess* parent, const std::string& name, const std::string& data, double x, double y, double w, double h);
      DataImage (ParentProcess* parent, const std::string& name, double x, double y, double w, double h);
      virtual ~DataImage ();
      void draw () override;
      DataImage* clone () override;
      virtual FatChildProcess* find_child_impl (const std::string&) override;
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
    private:
      DataImage * _img;
    };

  class Group : public Container
  {
  public:
    Group (ParentProcess* parent, const std::string& name);
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
    Defs (ParentProcess* parent, const std::string& name);
    virtual ~Defs () override;
    void impl_activate () override;
    void impl_deactivate () override;
    virtual process_type_e get_process_type () const override { return DEFS_T; }
    Defs* clone () override;
  };
} /* namespace djnn */
