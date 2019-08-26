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
  Group::Group (Process* p, const string &n) :
      Container (p, n), _gobj (nullptr)
  {
    _gobj = new AbstractGObj (this, "");
    Process::finalize_construction (p);
  }

  Group::Group () :
      Container (), _gobj (nullptr)
  {
    _gobj = new AbstractGObj (this, "");
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
    Container::impl_deactivate ();
    _gobj->impl_deactivate ();
  }

  void
  Group::draw ()
  {
    if (somehow_activating () && DisplayBackend::instance ()->window () == frame ()) {
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
