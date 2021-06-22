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
 *
 */

#pragma once

#include "gui/abstract_gobj.h"

#include "core/tree/ref_property.h"
#include "core/tree/double_property.h"
#include "core/tree/int_property.h"

namespace djnn
{

  class UI;
  class AbstractGShape : public AbstractGObj
  {
  public:
    AbstractGShape (ParentProcess* parent, const djnn::string& name);
    virtual ~AbstractGShape ();
    FatProcess* matrix () { return _matrix; }
    FatProcess* inverted_matrix () { return _inverted_matrix; }
    void set_origin (double x, double y) { _origin_x->set_value (x, true); _origin_y->set_value (y, true); }
    DoubleProperty* origin_x () { return _origin_x; }
    DoubleProperty* origin_y () { return _origin_y; }
    bool has_ui () { return ui != nullptr; }
    UI* get_ui () { return ui; }
    FatChildProcess* find_child_impl (const djnn::string& n) override;
    void impl_deactivate () override;
    void impl_activate () override;
    void add_style_class (const djnn::string& classname);
    virtual void press (double x, double y) {}
    virtual void move (double x, double y) {}
    void pick () override;
    void pre_draw ();
    void post_draw ();
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    virtual void get_bounding_box (double& x, double& y, double& w, double& h) const;
    virtual double sdf (double x, double y) const;

  protected:
    void init_ui ();
  private:
    FatProcess* _matrix, *_inverted_matrix;
    DoubleProperty *_origin_x, *_origin_y;
    //static vector<djnn::string> _ui;
    UI *ui;
    vector<int> _classes;
  };

  class Touch : public FatProcess
  {
  public:
    Touch (ParentProcess* parent, const djnn::string& name, int id, double init_x, double init_y, double init_pressure);
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
    AbstractGShape* init_shape () { return _shape; }
    AbstractGShape* current_shape () { return (AbstractGShape*)_last_shape->get_value (); }
    void set_init_shape (AbstractGShape *s) { _shape = s; }
    void set_current_shape (AbstractGShape *s) { _last_shape->set_value(s, true); }
    virtual ~Touch ();
  private:
    void init_touch (int id, double init_x, double init_y, double init_pressure);
    DoubleProperty *_init_x,* _init_y, *_move_x,* _move_y, *_local_init_x, *_local_init_y, *_local_move_x, *_local_move_y, *_pressure;
    IntProperty *_id;
    RefProperty *_last_shape;
    AbstractGShape* _shape;
    FatProcess *_move, *_enter, *_leave, *_release;
  };

} /* namespace djnn */
