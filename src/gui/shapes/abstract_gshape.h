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

#include "core/ontology/process.h"
#include "core/tree/ref_property.h"
#include "core/tree/double_property.h"
#include "core/tree/component.h"

#include "display/window.h"

namespace djnn
{
  class SVGHolder : public Container
  {
  public:
    SVGHolder (ParentProcess* parent, const std::string& name) : Container (parent, name), _gobj (nullptr) { finalize_construction (parent, name); }
    virtual ~SVGHolder () {}
    SVGHolder* clone () override;
    FatChildProcess* find_child_impl (const std::string& path) override;
    void set_gobj (FatProcess* gobj) { _gobj = gobj; }
    FatProcess* get_gobj () { return _gobj; }
  private:
    FatProcess* _gobj;
  };

  class UI {
  public:
    UI (ParentProcess* parent, FatProcess *f);
    bool is_pickable () { return pickable->get_value (); }
    //void activate (FatProcess* frame) { cpick->enable (frame); }
    void activate (FatProcess* frame) { 
      /* 
         if the frame was null in the UI constructor cpick was not correctly initialized
         we have to re-set the coupling dst to the actual frame.
      */
      auto* f = dynamic_cast<Window*> (frame);
      if (f) {
        cpick->set_dst (f->damaged ());
        cpick->enable (); 
      }
    }
    void deactivate () { cpick->disable (); }
    virtual ~UI ();
    friend class Picking;
  private:
    BoolProperty *pickable;
    CouplingWithData *cpick;
    DoubleProperty *move_x, *move_y, *press_x, *press_y, *local_move_x, *local_move_y, *local_press_x, *local_press_y;
    DoubleProperty *mouse_press_x, *mouse_press_y, *mouse_move_x, *mouse_move_y;
    DoubleProperty *mouse_local_press_x, *mouse_local_press_y, *mouse_local_move_x, *mouse_local_move_y;
    FatProcess *press, *left, *right, *middle, *move, *release, *enter, *leave, *mouse, *mouse_press, *mouse_release, *mouse_move, *mouse_enter, *mouse_leave, *touches;
    FatProcess *left_press, *left_release, *right_press, *right_release, *middle_press, *middle_release;
    ParentProcess* parent;
  };

  class AbstractGShape : public AbstractGObj
  {
  public:
    AbstractGShape (ParentProcess* parent, const std::string& name);
    virtual ~AbstractGShape ();
    FatProcess* matrix () { return _matrix; }
    FatProcess* inverted_matrix () { return _inverted_matrix; }
    void set_origin (double x, double y) { _origin_x->set_value (x, true); _origin_y->set_value (y, true); }
    DoubleProperty* origin_x () { return _origin_x; }
    DoubleProperty* origin_y () { return _origin_y; }
    bool has_ui () { return ui != nullptr; }
    UI* get_ui () { return ui; }
    FatChildProcess* find_child_impl (const std::string& n) override;
    void impl_deactivate () override;
    void impl_activate () override;
    void add_style_class (const std::string& classname);
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
    static std::vector<std::string> _ui;
    UI *ui;
    std::vector<int> _classes;
  };

  class Touch : public FatProcess
  {
  public:
    Touch (ParentProcess* parent, const std::string& name, int id, double init_x, double init_y, double init_pressure);
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

  inline bool is_pickable (AbstractGShape * s) {
      return s->has_ui () && s->get_ui ()->is_pickable ();
  }

} /* namespace djnn */
