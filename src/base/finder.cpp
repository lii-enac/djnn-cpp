/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "finder.h"

#include "../core/serializer/serializer.h"
#include "../core/ontology/coupling.h"
#include "../core/tree/int_property.h"
#include "../core/tree/bool_property.h"
#include "../core/tree/double_property.h"
#include "../core/execution/graph.h"
#include "../core/utils/error.h"

#include <iostream>

namespace djnn
{
  void
  Finder::FinderAction::impl_activate ()
  {
    Finder* f = (Finder*) _parent;
    string path = f->_path->get_value ();
    string key = f->_key->get_value ();
    if (key.empty())
      return;
    Process *res;
    for (auto c : f->_container->children ()) {
      res = c->find_component (path);
      if (res) {
        if (res->get_cpnt_type () != PROPERTY_T) {
          f->_result->set_value (c, true);
          f->_found->activate ();
          return;
        } else {
          switch (((AbstractProperty*) res)->get_prop_type ())
            {
            case Integer:
              try {
                int v = (int) stoi (key);
                if (v == ((IntProperty*) res)->get_value ()) {
                  f->_result->set_value (c, true);
                  f->_found->activate ();
                  return;
                }
              }
              catch (const std::invalid_argument& ia) {
                warning (this, "failed to convert the string \"" + key + "\" into an integer property value\n");
              }
              break;
            case Boolean:
              if ((key.compare ("true") == 0 && ((BoolProperty*) res)->get_value () == 1)
                  || (key.compare ("false") == 0 && ((BoolProperty*) res)->get_value () == 0)) {
                f->_result->set_value (c, true);
                f->_found->activate ();
                return;
              }
              break;
            case Double:
              try {
                double v = (double) stof (key);
                if (v == ((DoubleProperty*) res)->get_value ()) {
                  f->_result->set_value (c, true);
                  f->_found->activate ();
                  return;
                }
              }
              catch (const std::invalid_argument& ia) {
                warning (this, "failed to convert the string \"" + key + "\" into a double property value\n");
              }
              break;
            case String:
              if (key.compare (((TextProperty*) res)->get_value ()) == 0) {
                f->_result->set_value (c, true);
                f->_found->activate ();
                return;
              }
              break;
            default:
              break;
            }
        }
      }
    }
    f->_not_found->activate ();
  }

  Finder::Finder (Process *p, const string &n, Process *container, const string &path) :
      Process (p, n)
  {
    _container = dynamic_cast<Container*> (container);
    if (_container == nullptr) {
      warning (this, "Finder is only applicable to Containers");
      return;
    }
    _path = new TextProperty (this, "path", path);
    _key = new TextProperty (this, "key", "");
    _result = new RefProperty (this, "result", nullptr);
    _found = new Spike (this, "found");
    _not_found = new Spike (this, "notFound");
    _action = new FinderAction (this, "action");
    _cfind = new Coupling (_key, ACTIVATION, _action, ACTIVATION);
    _cfind->disable ();
    Graph::instance ().add_edge (_key, _action);
    Graph::instance ().add_edge (_action, _result);

    add_state_dependency (_parent, _action);
    Process::finalize_construction ();
  }

  Finder::~Finder ()
  {
    remove_state_dependency (_parent, _action);
    Graph::instance ().remove_edge (_key, _action);
    Graph::instance ().remove_edge (_action, _result);
    delete _cfind;
    delete _path;
    delete _key;
    delete _found;
    delete _not_found;
    delete _action;
  }

  void
  Finder::impl_activate ()
  {
    _cfind->enable ();
  }

  void
  Finder::impl_deactivate ()
  {
    _cfind->disable ();
  }

  void
  Finder::serialize (const string& type)
  {

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:finder");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->text_attribute ("path", _path->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);

  }
}
