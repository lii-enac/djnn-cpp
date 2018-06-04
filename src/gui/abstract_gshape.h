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

  class Touch : public Process
  {
  public:
    Touch (Process *p, const std::string &n);
    Touch ();
    virtual ~Touch ();
  private:
    DoubleProperty *_x,* _y;
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

  inline bool is_pickable (AbstractGShape * s) {
      return s->has_ui ();
  }

} /* namespace djnn */
