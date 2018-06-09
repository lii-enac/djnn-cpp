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
    bool has_ui () { return _has_ui; }
    Process* find_component (const string &n) override;
    
  private:
    void init_mouse_ui ();
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
    AbstractGShape* shape () { return _shape; }
    void set_shape (AbstractGShape *s) { _shape = s; }
    virtual ~Touch ();
  private:
    DoubleProperty *_x,* _y;
    AbstractGShape* _shape;
  };

  inline bool is_pickable (AbstractGShape * s) {
      return s->has_ui ();
  }

} /* namespace djnn */
