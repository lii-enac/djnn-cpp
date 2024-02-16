#pragma once

#include "core/property/double_property.h"
#include "core/tree/structure_observer.h"
#include "gui/gui-priv.h"
#include "gui/shape/abstract_gshape.h"

namespace djnn {
class List;

class PathPoint : public AbstractGObj {
  public:
    PathPoint (CoreProcess* parent, const string& name, double x, double y);
    virtual ~PathPoint ();
    virtual CoreProcess*    find_child_impl (const string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*)find_child_impl ("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*)find_child_impl ("y"); }
    void                    draw () override = 0;
    // notify polygon ( (grand-grand-)parent polygon-list-point)
    void notify_change (unsigned int nm) override {
        _damaged |= nm;
        get_parent ()->get_parent ()->notify_change (nm);
    }

  protected:
    struct raw_props_t {
        double x, y;
    };
    raw_props_t       raw_props;
    CouplingWithData *_cx, *_cy;
    void              impl_activate () override;
    void              impl_deactivate () override;
};

class PathMove : public PathPoint {
  public:
    PathMove (CoreProcess* parent, const string& name, double x, double y)
        : PathPoint (parent, name, x, y) {}
    void      draw () override;
    PathMove* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
};

class PathLine : public PathPoint {
  public:
    PathLine (CoreProcess* parent, const string& name, double x, double y)
        : PathPoint (parent, name, x, y) {}
    void      draw () override;
    PathLine* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
};

class PathQuadratic : public AbstractGObj {
  public:
    PathQuadratic (CoreProcess* parent, const string& name, double x1, double y1, double x, double y);
    virtual ~PathQuadratic ();
    virtual CoreProcess*    find_child_impl (const string&) override;
    AbstractDoubleProperty* x1 () { return (AbstractDoubleProperty*)find_child_impl ("x1"); }
    AbstractDoubleProperty* y1 () { return (AbstractDoubleProperty*)find_child_impl ("y1"); }
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*)find_child_impl ("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*)find_child_impl ("y"); }
    void                    draw () override;
    PathQuadratic*          impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;

  private:
    struct raw_props_t {
        double x1, y1, x, y;
    };
    raw_props_t       raw_props;
    CouplingWithData *_cx1, *_cy1, *_cx, *_cy;
    void              impl_activate () override;
    void              impl_deactivate () override;
};

class PathCubic : public AbstractGObj {
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
    PathCubic*              impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;

  private:
    struct raw_props_t {
        double x1, y1, x2, y2, x, y;
    };
    raw_props_t       raw_props;
    CouplingWithData *_cx1, *_cy1, *_cx2, *_cy2, *_cx, *_cy;
    void              impl_activate () override;
    void              impl_deactivate () override;
};

class PathArc : public AbstractGObj {
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
    PathArc*                impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;

  private:
    struct raw_props_t {
        double rx, ry, rotx, fl, swfl, x, y;
    };
    raw_props_t       raw_props;
    CouplingWithData *_crx, *_cry, *_crotx, *_cfl, *_cswfl, *_cx, *_cy;
    void              impl_activate () override;
    void              impl_deactivate () override;
};

class PathClosure : public AbstractGObj {
  public:
    PathClosure (CoreProcess* parent, const string& name);
    virtual ~PathClosure () {}
    void         draw () override;
    PathClosure* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;

  private:
    void impl_activate () override {
        AbstractGObj::impl_activate ();
    }
    void impl_deactivate () override {
        AbstractGObj::impl_deactivate ();
    }
};

class Path : public AbstractGShape {
  public:
    Path (CoreProcess* parent, const string& name);
    Path (CoreProcess* parent, const string& name, const string& path_spec);
    virtual ~Path ();

    // HACK to get access directly to GUIstructureHolder of items
    // and use the correct draw function
    List*               items () { return _items; }
    GUIStructureHolder* items_GH () { return _items_GH; }

    void   draw () override;
    void   get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
    // Path* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
    Path* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
    void  set_bounding_box (double x, double y, double w, double h);
    void  invalidate_bounding_box () { _bbw->set_value (-1, true); }
    bool  is_bounding_box_valid () const { return _bbw->get_value () >= 0; }

  protected:
    void                impl_activate () override;
    void                impl_deactivate () override;
    List*               _items;
    FatProcess*         _bounding_box;
    DoubleProperty *    _bbx, *_bby, *_bbw, *_bbh;
    GUIStructureHolder* _items_GH; // HACK to get access directly to GUIstructureHolder of items
};

int parse_path (Path* p, const char* path_spec);
} // namespace djnn
