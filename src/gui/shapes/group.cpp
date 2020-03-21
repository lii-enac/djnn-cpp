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
#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display.h"
#include "display/abstract_display.h"

#include "core/execution/graph.h"


namespace djnn
{
  Group::Group (Process* parent, const std::string& n) :
      Container (parent, n), _gobj (nullptr)
  {
    _gobj = new AbstractGObj (this, "_gobj");
    
    /* AbstractGObj do not have a call to finalize_construction */
    /* we have to add_child it here */
    _gobj->set_parent (this);
    

    Process::finalize_construction (parent, n);

    /* and set state_dependency here AFTER setting the parent to "this" */
    _gobj->set_state_dependency (this->state_dependency ());
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
    Group* newg = new Group (nullptr, get_name ());

    for (auto c : _children) {
      Process* child = c->clone ();
      if (child != nullptr)
        newg->add_child (child, this->find_child_name(c));
    }

    return newg;
  }
}
