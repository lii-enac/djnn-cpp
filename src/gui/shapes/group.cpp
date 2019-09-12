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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "shapes.h"
#include "../backend.h"
#include "../abstract_backend.h"
#include "../../display/display.h"
#include "../../display/abstract_display.h"

namespace djnn
{
  Group::Group (Process* parent, const string &n) :
      Container (parent, n), _gobj (nullptr)
  {
    _gobj = new AbstractGObj (this, "_gobj");
    
    /* AbstractGObj do not have a call to finalize_construction */
    /* we have to add_child it here */
    _gobj->set_parent (this);
    

    Process::finalize_construction (parent);

    /* and set state_dependency here AFTER setting the parent to "this" */
    _gobj->state_dependency (this->state_dependency ());
  }

  Group::Group () :
      Container (), _gobj (nullptr)
  {
    _gobj = new AbstractGObj (this, "_gobj");
    /* AbstractGObj do not have a call to finalize_construction */
    /* we have to add_child it here */
    _gobj->set_parent (this);
    _gobj->state_dependency (this->state_dependency ());
  }

  Group::~Group ()
  {
    delete _gobj;
  }

  void
  Group::impl_activate ()
  {
    Container::impl_activate ();
    _gobj->impl_activate ();
  }

  void
  Group::impl_deactivate ()
  {
    _gobj->impl_deactivate ();
    Container::impl_deactivate ();
  }

  void
  Group::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Container::draw ();
    }
  }

  Process* 
  Group::clone () 
  {
    Group* newg = new Group ();

    for (auto c : _children) {
      newg->add_child (c->clone (), this->find_component_name(c));
    }

    return newg;
  }
}
