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

#include "abstract_gobj.h"

#include "../core/ontology/process.h"
#include "../core/tree/ref_property.h"
#include "../core/tree/double_property.h"
#include "../core/tree/component.h"

namespace djnn
{
  class SVGHolder : public Container
  {
  public:
    SVGHolder (Process *parent, const std::string& name) : Container (parent, name), _gobj (nullptr) { Process::finalize_construction (parent, name); }
    SVGHolder () : Container (), _gobj (nullptr) {}
    virtual ~SVGHolder () {}
    Process* clone () override;
    Process* find_component (const string &path) override;
    void set_gobj (Process* gobj) { _gobj = gobj; }
    Process* get_gobj () { return _gobj; }
  private:
    Process* _gobj;
  };

  class UI {
  public:
    UI (Process *p);
    virtual ~UI ();
    friend class Picking;
  private:
    DoubleProperty *move_x, *move_y, *press_x, *press_y, *local_move_x, *local_move_y, *local_press_x, *local_press_y;
    DoubleProperty *mouse_press_x, *mouse_press_y, *mouse_move_x, *mouse_move_y;
    DoubleProperty *mouse_local_press_x, *mouse_local_press_y, *mouse_local_move_x, *mouse_local_move_y;
    Process *press, *move, *release, *enter, *leave, *mouse, *mouse_press, *mouse_release, *mouse_move, *mouse_enter, *mouse_leave, *touches;
    Process *parent;
  };

  class AbstractGShape : public AbstractGObj
  {
  public:
    AbstractGShape (Process *parent, const std::string& name);
    AbstractGShape ();
    virtual ~AbstractGShape ();
    Process* matrix () { return _matrix; }
    Process* inverted_matrix () { return _inverted_matrix; }
    void set_origin (double x, double y) { _origin_x->set_value (x, true); _origin_y->set_value (y, true); }
    DoubleProperty* origin_x () { return _origin_x; }
    DoubleProperty* origin_y () { return _origin_y; }
    bool has_ui () { return ui != nullptr; }
    UI* get_ui () { return ui; }
    Process* find_component (const string &n) override;
    void impl_deactivate () override;

    void pick () override;

  private:
    void init_ui ();
    Process* _matrix, *_inverted_matrix;
    DoubleProperty *_origin_x, *_origin_y;
    static vector<string> _ui;
    UI *ui;
  };

  class Touch : public Process
  {
  public:
    Touch (Process *parent, const std::string &name, int id, double init_x, double init_y, double init_pressure);
    Touch ();
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
    Process* get_move () { return _move; }
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
    Process *_move, *_enter, *_leave, *_release;
  };

  inline bool is_pickable (AbstractGShape * s) {
      return s->has_ui ();
  }

} /* namespace djnn */
