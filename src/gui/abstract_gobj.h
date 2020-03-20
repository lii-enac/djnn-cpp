/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/tree/bool_property.h"
#include "core/tree/int_property.h"
#include "core/tree/double_property.h"
#include "core/tree/text_property.h"

#include <memory>

namespace djnn
{

  class Window;
  extern bool gui_initialized;

  class AbstractGObjImpl {
  public:
    virtual ~AbstractGObjImpl();
  };

  class AbstractGObj : public Process
  {
  public:
    AbstractGObj (Process *parent, const std::string& name) : Process (name), _frame (nullptr), _damaged(notify_none), _impl(nullptr) {
      if (!gui_initialized) warning (this, "Module GUI not initialized");
    }
    virtual ~AbstractGObj () {};
    virtual process_type_e get_cpnt_type () const override { return GOBJ_T; }
    Window*& frame () { return _frame; } 
    //std::weak_ptr<Window>
    //auto frame () { return &*AbstractGObj::_frame.lock (); }
    void update_drawing () override;
    void impl_activate () override;
    void impl_deactivate () override;
    void notify_change (unsigned int nm) override { _damaged |= nm; }
    void reset_damaged (unsigned int nm) { _damaged &= ~nm; }
    void reset_damaged () { _damaged = notify_none; }
    int get_damaged () { return _damaged; }

    AbstractGObjImpl* impl() { return _impl; }
    void set_impl(AbstractGObjImpl* i) { _impl=i; }
    
  protected:
    virtual Process* create_GObj_prop (BoolPropertyProxy **prop, Coupling  **cprop, bool *rawp, const std::string& name, int notify_mask);
    virtual Process* create_GObj_prop (IntPropertyProxy **prop, Coupling  **cprop, int *rawp, const std::string& name, int notify_mask);
    virtual Process* create_GObj_prop (DoublePropertyProxy **prop, Coupling  **cprop, double *rawp, const std::string& name, int notify_mask);
    virtual Process* create_GObj_prop (TextPropertyProxy **prop, Coupling  **cprop, std::string *rawp, const std::string& name, int notify_mask);

  protected:
    void update_frame_if_necessary ();
    Window * _frame;
    //std::weak_ptr<Window> _frame;
    unsigned int _damaged;
    AbstractGObjImpl *_impl;
  };
}
