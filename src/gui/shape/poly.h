#pragma once

#include "core/property/double_property.h"
#include "core/tree/structure_observer.h"
#include "gui/gui-priv.h"
#include "gui/shape/abstract_gshape.h"

namespace djnn {
class List;

class PolyPoint : public AbstractGObj {
  public:
    PolyPoint (CoreProcess* parent, const string& name, double x, double y);
    virtual ~PolyPoint ();
    virtual CoreProcess*    find_child_impl (const string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*)find_child_impl ("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*)find_child_impl ("y"); }
    void                    draw () override;
    PolyPoint*              impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
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

class Poly : public AbstractGShape {
  public:
    Poly (CoreProcess* parent, const string& name, int closed);
    virtual ~Poly ();

    List*               points () { return _points; }

    bool   closed () { return _closed; }
    void   draw () override;
    void   get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
    Poly*  impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
    void   set_bounding_box (double x, double y, double w, double h);

  protected:
    void impl_activate () override;
    void impl_deactivate () override;

  private:
    List*               _points;
    FatProcess*         _bounding_box;
    DoubleProperty *    _bbx, *_bby, *_bbw, *_bbh;
    bool                _closed;
};

class Polygon : public Poly {
  public:
    Polygon (CoreProcess* parent, const string& name)
        : Poly (parent, name, 1) {};
    virtual ~Polygon () {};
};

class Polyline : public Poly {
  public:
    Polyline (CoreProcess* parent, const string& name)
        : Poly (parent, name, 0) {};
    virtual ~Polyline () {};
};
} // namespace djnn