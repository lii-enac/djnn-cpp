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

namespace djnn
{
  Group::Group (Process* p, const string &n) :
      Container (p, n), _gobj (nullptr)
  {
    _gobj = new AbstractGObj (this, "");
    Process::finalize ();
  }

  Group::Group () :
      Container (), _gobj (nullptr)
  {
    _gobj = new AbstractGObj (this, "");
  }

  Group::~Group ()
  {
    if (_gobj) {delete _gobj; _gobj = nullptr;}
  }

  void
  Group::activate ()
  {
    Container::activate ();
    _gobj->activate ();
  }

  void
  Group::deactivate ()
  {
    Container::deactivate ();
    _gobj->deactivate ();
  }

  void
  Group::draw ()
  {
    //if (_activation_state <= activated && Backend::instance ()->window () == frame ()) {
    if (somehow_activating () && Backend::instance ()->window () == frame ()) {
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
