#pragma once

#include "core/property/double_property.h"
#include "gui/shape/abstract_gshape.h"
#include "core/tree/structure_observer.h"
#include "gui/gui-priv.h"

namespace djnn
{
  class List;
  
  class PolyPoint : public AbstractGObj
  {
  public:
    PolyPoint (ParentProcess* parent, const string& name, double x, double y);
    virtual ~PolyPoint ();
    virtual FatChildProcess* find_child_impl (const string&) override;
    AbstractDoubleProperty* x () { return (AbstractDoubleProperty*) find_child_impl("x"); }
    AbstractDoubleProperty* y () { return (AbstractDoubleProperty*) find_child_impl("y"); }
    void draw () override;
    PolyPoint* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
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
    Poly (ParentProcess* parent, const string& name, int closed);
    virtual ~Poly ();

    // HACK to get access directly to GUIstructureHolder of points
    // and use the correct draw function
    List* points () { return _points;} 
    GUIStructureHolder* points_GH () { return _points_GH;}

    bool closed () { return _closed;}
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
    Poly* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
    void set_bounding_box (double x, double y, double w, double h);
  protected:
    void impl_activate () override;
    void impl_deactivate () override;
  private:
    List *_points;
    FatProcess* _bounding_box;
    DoubleProperty *_bbx, *_bby, *_bbw, *_bbh;
    bool _closed;
    GUIStructureHolder* _points_GH ; // HACK to get access directly to GUIstructureHolder of points
  };

  class Polygon : public Poly
  {
  public:
    Polygon (ParentProcess* parent, const string& name) : Poly (parent, name, 1) {};
    virtual ~Polygon () {};
  };

  class Polyline : public Poly
  {
  public:
    Polyline (ParentProcess* parent, const string& name) : Poly (parent, name, 0) {};
    virtual ~Polyline () {};
  };
}