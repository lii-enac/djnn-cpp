/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2026)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */
#pragma once

#include "core/property/double_property.h"
#include "core/control/action.h"
#include "gui/gui-priv.h"
#include "gui/shape/abstract_gshape.h"

namespace djnn {
class List;

class SubPath : public AbstractGObj {
  public:
    SubPath (CoreProcess* parent, const string& name) : AbstractGObj(parent, name) {}
    virtual void            get_bounding_box (double& x, double& y, double& w, double& h) const { x=-1; y=-1; w=-1; h=-1; }
    void                    draw () override = 0;
    virtual double          sdf (double px, double py) const; //{ return INFINITY; }
    //virtual void get_xy(double & x, double & y) const = 0;
};

class SubPathWithSingleCoord : public SubPath {
  public:
    SubPathWithSingleCoord (CoreProcess* parent, const string& name, double x, double y);
    virtual ~SubPathWithSingleCoord ();
    virtual CoreProcess*    find_child_impl (const string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*)find_child_impl ("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*)find_child_impl ("y"); }
    //void get_xy(double & x, double & y) const override { x=raw_props.x; y = raw_props.y; }

    // useless?
    // notify polygon ( (grand-)parent: polygon-list-point)
    // void notify_change (unsigned int nm) override {
    //     _damaged |= nm;
    //     get_parent ()->get_parent ()->notify_change (nm);
    // }

  protected:
    struct raw_props_t {
        double x, y;
    };
    raw_props_t       raw_props;
    CouplingWithData *_cx, *_cy;
    void              impl_activate () override;
    void              impl_deactivate () override;
};

class PathMove : public SubPathWithSingleCoord {
  public:
    PathMove (CoreProcess* parent, const string& name, double x, double y)
        : SubPathWithSingleCoord (parent, name, x, y) {}
    void      draw () override;
    void      get_bounding_box (double& x, double& y, double& w, double& h) const override { x=raw_props.x; y=raw_props.y; w=0; h=0;}
    PathMove* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
};

class PathLine : public SubPathWithSingleCoord {
  public:
    PathLine (CoreProcess* parent, const string& name, double x, double y)
        : SubPathWithSingleCoord (parent, name, x, y) {}
    void      draw () override;
    void      get_bounding_box (double& x, double& y, double& w, double& h) const override { x=0; y=0; w=raw_props.x; h=raw_props.y; }
    double    sdf (double px, double py) const override;
    PathLine* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
};

class PathQuadratic : public SubPath {
  public:
    PathQuadratic (CoreProcess* parent, const string& name, double x1, double y1, double x, double y);
    virtual ~PathQuadratic ();
    virtual CoreProcess*    find_child_impl (const string&) override;
    AbstractDoubleProperty* x1 () { return (AbstractDoubleProperty*)find_child_impl ("x1"); }
    AbstractDoubleProperty* y1 () { return (AbstractDoubleProperty*)find_child_impl ("y1"); }
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*)find_child_impl ("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*)find_child_impl ("y"); }
    void                    draw () override;
    void                    get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double                  sdf (double px, double py) const override;
    //void get_xy(double & x, double & y) const override { x=raw_props.x; y = raw_props.y; }
    PathQuadratic*          impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;

  private:
    struct raw_props_t {
        double x1, y1, x, y;
    };
    raw_props_t       raw_props;
    CouplingWithData *_cx1, *_cy1, *_cx, *_cy;
    void              impl_activate () override;
    void              impl_deactivate () override;
};

class PathCubic : public SubPath {
  public:
    PathCubic (CoreProcess* parent, const string& name, double x1, double y1, double x2, double y2, double x, double y);
    virtual ~PathCubic ();
    virtual CoreProcess*    find_child_impl (const string&) override;
    AbstractDoubleProperty* x1 () { return (AbstractDoubleProperty*)find_child_impl ("x1"); }
    AbstractDoubleProperty* y1 () { return (AbstractDoubleProperty*)find_child_impl ("y1"); }
    AbstractDoubleProperty* x2 () { return (AbstractDoubleProperty*)find_child_impl ("x2"); }
    AbstractDoubleProperty* y2 () { return (AbstractDoubleProperty*)find_child_impl ("y2"); }
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*)find_child_impl ("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*)find_child_impl ("y"); }
    void                    draw () override;
    void                    get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double                  sdf (double px, double py) const override;
    //void get_xy(double & x, double & y) const override { x=raw_props.x; y = raw_props.y; }
    PathCubic*              impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;

  private:
    struct raw_props_t {
        double x1, y1, x2, y2, x, y;
    };
    raw_props_t       raw_props;
    CouplingWithData *_cx1, *_cy1, *_cx2, *_cy2, *_cx, *_cy;
    void              impl_activate () override;
    void              impl_deactivate () override;
};

class PathArc : public SubPath {
  public:
    PathArc (CoreProcess* parent, const string& name, double rx, double ry, double rotx, double fl, double swfl, double x,
             double y);
    virtual ~PathArc ();
    virtual CoreProcess*    find_child_impl (const string&) override;
    AbstractDoubleProperty* rotx () { return (AbstractDoubleProperty*)find_child_impl ("rotx"); }
    AbstractDoubleProperty* fl () { return (AbstractDoubleProperty*)find_child_impl ("fl"); }
    AbstractDoubleProperty* swfl () { return (AbstractDoubleProperty*)find_child_impl ("swfl"); }
    AbstractDoubleProperty* rx () { return (AbstractDoubleProperty*)find_child_impl ("rx"); }
    AbstractDoubleProperty* ry () { return (AbstractDoubleProperty*)find_child_impl ("ry"); }
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*)find_child_impl ("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*)find_child_impl ("y"); }
    void                    draw () override;
    void                    get_bounding_box (double& x_, double& y_, double& w_, double& h_) const override;
    double                  sdf (double px, double py) const override;
    //void get_xy(double & x, double & y) const override { x=raw_props.x; y = raw_props.y; }
    PathArc*                impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;

  private:
    struct raw_props_t {
        double rx, ry, rotx, fl, swfl, x, y;
    };
    raw_props_t       raw_props;
    CouplingWithData *_crx, *_cry, *_crotx, *_cfl, *_cswfl, *_cx, *_cy;
    void              impl_activate () override;
    void              impl_deactivate () override;
};

class PathClosure : public SubPath {
  public:
    PathClosure (CoreProcess* parent, const string& name);
    virtual ~PathClosure () {}
    void         draw () override;
    PathClosure* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;

  private:
    void impl_activate () override {
        AbstractGObj::impl_activate ();
    }
    void impl_deactivate () override {
        AbstractGObj::impl_deactivate ();
    }
};

class Path : public AbstractGShape {
    class SpecUpdateAction : public Action {
      public:
        SpecUpdateAction (CoreProcess* parent, const string& name)
            : Action (parent, name) {};

        virtual ~SpecUpdateAction () {}
        void impl_activate () override;
    };

  public:
    Path (CoreProcess* parent, const string& name);
    Path (CoreProcess* parent, const string& name, const string& path_spec);
    virtual ~Path ();

    List*               items () { return _items; }
    TextProperty&       spec () { return _spec; }

    void   draw () override;
    void   get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
    Path* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
    void  set_bounding_box (double x, double y, double w, double h);
    void  invalidate_bounding_box () { _bbw->set_value (-1, true); }
    bool  is_bounding_box_valid () const { return _bbw->get_value () >= 0; }

  protected:
    void                impl_activate () override;
    void                impl_deactivate () override;
    List*               _items;
    FatProcess*         _bounding_box;
    DoubleProperty *    _bbx, *_bby, *_bbw, *_bbh;

    TextProperty     _spec;
    SpecUpdateAction _updateaction;
    Coupling         _c_update;
};

int parse_path (Path* p, const char* path_spec);
} // namespace djnn
