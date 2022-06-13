/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2021)
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

#include "display/pickui.h"

#include "gui/abstract_gobj.h"

#include "core/property/ref_property.h"
#include "core/property/double_property.h"
#include "core/property/int_property.h"

namespace djnn
{

  class AbstractGShape : public AbstractGObj, public PickUI
  {
  public:
    AbstractGShape (ParentProcess* parent, const string& name, int z_order = 0);
    virtual ~AbstractGShape ();
    FatProcess* matrix () { return _matrix; }
    FatProcess* inverted_matrix () { return _inverted_matrix; }
    void set_origin (double x, double y) { _origin_x->set_value (x, true); _origin_y->set_value (y, true); }
    DoubleProperty* origin_x () { return _origin_x; }
    DoubleProperty* origin_y () { return _origin_y; }
    FatChildProcess* find_child_impl (const string& n) override;
    void impl_deactivate () override;
    void impl_activate () override;
    void add_style_class (const string& classname);
    virtual void press (double x, double y) {}
    virtual void move (double x, double y) {}
    void pick () override;
    void pre_draw ();
    void post_draw ();
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    virtual void get_bounding_box (double& x, double& y, double& w, double& h) const;
    virtual double sdf (double x, double y) const;
    void set_mouse_local_coords (double x, double y, bool is_move) override;
    int z_order () { return _z.get_value (); }
  protected:
    void init_ui ();
  private:
    FatProcess* _matrix, *_inverted_matrix;
    DoubleProperty *_origin_x, *_origin_y;
    IntProperty _z;
    vector<int> _classes;
    CouplingWithData* _c_z_prop;
  };

  inline bool is_pickable (AbstractGShape * s) {
     return s->has_ui () && s->ui ()->is_pickable ();
  }

  class Touch : public FatProcess
  {
  public:
    Touch (ParentProcess* parent, const string& name, int id, double init_x, double init_y, double init_pressure);
    void impl_activate () override {};
    void impl_deactivate () override {};
    void set_init_x (double v) { _init_x->set_value (v, true); }
    void set_init_y (double v) { _init_y->set_value (v, true); }
    void set_move_x (double v) { _move_x->set_value (v, true); }
    void set_move_y (double v) { _move_y->set_value (v, true); }
    void set_local_init_x (double v) { _local_init_x->set_value (v, true); }
    void set_local_init_y (double v) { _local_init_y->set_value (v, true); }
    void set_local_move_x (double v) { _local_move_x->set_value (v, true); }
    void set_local_move_y (double v) { _local_move_y->set_value (v, true); }
    void set_pressure (double v) { _pressure->set_value (v, true); }
    void set_id (int v) { _id->set_value (v, true); }
    int get_id () { return _id->get_value (); }
    double get_move_x () { return _move_x->get_value(); }
    double get_move_y () { return _move_y->get_value(); }
    FatProcess* get_move () { return _move; }
    void leave () { _leave->activate (); }
    void release () { _release->activate (); }
    void enter () { _enter->activate (); }
    PickUI* init_shape () { return _shape; }
    PickUI* current_shape () { return dynamic_cast<PickUI*>(_last_shape->get_value ()); }
    void set_init_shape (PickUI *s) { _shape = s; }
    void set_touch_local_coords (PickUI *t, double x, double y, bool is_move);
    void set_current_shape (PickUI *s) {
      CoreProcess *ss = dynamic_cast<CoreProcess*> (s);
      if (ss)
        _last_shape->set_value((CoreProcess*)ss, true); 
    }
    virtual ~Touch ();
  private:
    void init_touch (int id, double init_x, double init_y, double init_pressure);
    DoubleProperty *_init_x,* _init_y, *_move_x,* _move_y, *_local_init_x, *_local_init_y, *_local_move_x, *_local_move_y, *_pressure;
    IntProperty *_id;
    RefProperty *_last_shape;
    PickUI* _shape;
    FatProcess *_move, *_enter, *_leave, *_release;
  };

} /* namespace djnn */
