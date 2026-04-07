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

#include "core/ontology/coupling.h"
#include "core/property/bool_property.h"
#include "core/property/double_property.h"
#include "core/property/int_property.h"
#include "core/utils/containers/string.h"

namespace djnn {

//extern const djnnstl::vector<djnnstl::string> __ui_interface;
extern const djnnstl::map<djnnstl::string, bool> __ui_interface;
class List;
class Component;

class UI {
  public:
    UI (CoreProcess* parent, FatProcess* f);
    bool is_pickable () { return pickable->get_value (); }
    void activate (FatProcess* frame);
    void deactivate ();
    virtual ~UI ();

    CoreProcess*       get_parent () { return _parent; }
    const CoreProcess* get_parent () const { return _parent; }
    CoreProcess*       find_child_impl (const djnnstl::string& path);

  private:
    CoreProcess*      _parent;
    BoolProperty*     pickable;
    CouplingWithData* cpick;


#define EVENT(ev, type, parent) \
  public: \
    type*     ev () { return _##ev; } \
    /*type*     ev ();*/ \
  private: \
    type*     _##ev;
#include "ui_event/ui_event_parent.h"
#include "ui_event/ui_event.h"
#undef EVENT


#define EVENT(ev, type, parent) \
    public: \
      DoubleProperty* ev##_x () { return _##ev##_x; } \
      DoubleProperty* ev##_y () { return _##ev##_y; } \
      /*DoubleProperty* ev##_x ();*/ \
      /*DoubleProperty* ev##_y ();*/ \
    private: \
      DoubleProperty* _##ev##_x; \
      DoubleProperty* _##ev##_y;
#include "ui_event/ui_event_xy.h"
#include "ui_event/ui_event_xy_local.h"
#undef EVENT


#define EVENT(ev, type, parent) \
  public: \
    type*     ev () { return _##ev; } \
    /*type*     ev ();*/ \
  private: \
    type*     _##ev;

  EVENT(wheel_dx, DoubleProperty, wheel)
  EVENT(wheel_dy, DoubleProperty, wheel)
  EVENT(stylus_pen_pressure, DoubleProperty, parent)
  EVENT(stylus_eraser_pressure, DoubleProperty, parent)

#undef EVENT
};
} // namespace djnn
