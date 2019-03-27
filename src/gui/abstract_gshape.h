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

#include "../core/tree/double_property.h"
#include "../core/tree/process.h"
#include <iostream>

namespace djnn
{
  class SVGHolder : public Container
  {
  public:
    SVGHolder (Process *p, const std::string& n) : Container (p, n), _gobj (nullptr) { Process::finalize (); }
    SVGHolder () : Container (), _gobj (nullptr) {}
    virtual ~SVGHolder () {}
    Process* clone () override;
    Process* find_component (const string &path) override;
    void set_gobj (Process* gobj) { _gobj = gobj; }
  private:
    Process* _gobj;
  };

  class AbstractGShape : public AbstractGObj
  {
  public:
    AbstractGShape (Process *p, const std::string& n);
    AbstractGShape ();
    virtual ~AbstractGShape ();
    Process* matrix () { return _matrix; }
    Process* inverted_matrix () { return _inverted_matrix; }
    void set_origin (double x, double y) { _origin_x->set_value (x, true); _origin_y->set_value (y, true); }
    DoubleProperty* origin_x () { return _origin_x; }
    DoubleProperty* origin_y () { return _origin_y; }
    bool has_ui () { return _has_ui; }
    Process* find_component (const string &n) override;

    void pick () override;

  private:
    void init_mouse_ui ();
    Process* _matrix, *_inverted_matrix;
    DoubleProperty *_origin_x, *_origin_y;
    static vector<string> _ui;
    bool _has_ui;
  };

  class Touch : public Process
  {
  public:
    Touch (Process *p, const std::string &n);
    Touch ();
    void activate () override {};
    void deactivate () override {};
    void set_x (double v) { _x->set_value (v, true); }
    void set_y (double v) { _y->set_value (v, true); }
    void set_local_x (double v) { _local_x->set_value (v, true); }
    void set_local_y (double v) { _local_y->set_value (v, true); }
    void set_pressure (double v) { _pressure->set_value (v, true); }
    void set_id (int v) { _id->set_value (v, true); }
    
    AbstractGShape* shape () { return _shape; }
    void set_shape (AbstractGShape *s) { _shape = s; }
    virtual ~Touch ();
  private:
    DoubleProperty *_x,* _y, *_local_x, *_local_y, *_pressure;
    IntProperty *_id;
    AbstractGShape* _shape;
  };

  inline bool is_pickable (AbstractGShape * s) {
      return s->has_ui ();
  }

} /* namespace djnn */
