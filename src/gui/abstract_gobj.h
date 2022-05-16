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
#include "core/property/bool_property.h"
#include "core/property/int_property.h"
#include "core/property/double_property.h"
#include "core/property/text_property.h"

//#include <memory>

namespace djnn
{

  class Window;
  class Layer;
  extern bool gui_initialized;

  class AbstractGObjImpl {
  public:
    virtual ~AbstractGObjImpl();
  };

  class AbstractGObj : public FatProcess
  {
  public:
    AbstractGObj (ParentProcess* parent, const string& name) : FatProcess (name), _frame (nullptr), _damaged(notify_none), _impl(nullptr) {
      //if (!gui_initialized) warning (this, "Module GUI not initialized");
    }
    virtual ~AbstractGObj () {};
    virtual process_type_e get_process_type () const override { return GOBJ_T; }
    Window*& get_frame () { return _frame; }
    void set_frame (Window*& frame) { _frame = frame; }
    Layer* find_layer ();
    //std::weak_ptr<Window>
    //auto frame () { return &*AbstractGObj::_frame.lock (); }
    void update_drawing () override;
    void impl_activate () override;
    void impl_deactivate () override;
    void notify_change (unsigned int nm) override { _damaged |= nm; FatProcess::notify_change (nm); }
    void reset_damaged (unsigned int nm) { _damaged &= ~nm; }
    void reset_damaged () { _damaged = notify_none; }
    int get_damaged () { return _damaged; }

    AbstractGObjImpl* impl() { return _impl; }
    void set_impl(AbstractGObjImpl* i) { _impl=i; }
    
  protected:
    virtual FatProcess* create_GObj_prop (BoolPropertyProxy **prop, CouplingWithData **cprop, bool *rawp, const string& name, int notify_mask);
    virtual FatProcess* create_GObj_prop (IntPropertyProxy **prop, CouplingWithData **cprop, int *rawp, const string& name, int notify_mask);
    virtual FatProcess* create_GObj_prop (DoublePropertyProxy **prop, CouplingWithData **cprop, double *rawp, const string& name, int notify_mask);
    virtual FatProcess* create_GObj_prop (TextPropertyProxy **prop, CouplingWithData **cprop, string *rawp, const string& name, int notify_mask);
    virtual FatProcess* create_GObj_prop (TextPropertyProxy **prop, CouplingWithData **cprop, string **rawp, const string& name, int notify_mask);

    void create_Gobj_update_coupling (CoreProcess **prop, CouplingWithData **cprop);

    void impl_clone_properties (CoreProcess* clone, map<CoreProcess*, CoreProcess*>& origs_clones);

  protected:
    void update_frame_if_necessary ();
    Window * _frame;
    //std::weak_ptr<Window> _frame;
    unsigned int _damaged;
    AbstractGObjImpl *_impl;
  };

  Window* find_frame (CoreProcess* obj);
  void enable (Coupling* c, CoreProcess* dst);
  void remove_edge (Coupling *c);
  void disable (Coupling *c);
}
