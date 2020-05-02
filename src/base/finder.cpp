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

#include "core/serializer/serializer.h"
#include "core/ontology/coupling.h"
#include "core/tree/int_property.h"
#include "core/tree/bool_property.h"
#include "core/tree/double_property.h"
#include "core/execution/graph.h"
#include "core/utils/error.h"


namespace djnn
{
  void
  Finder::FinderAction::impl_activate ()
  {
    Finder* f = (Finder*) get_parent ();
    std::string path = f->_path.get_value ();
    std::string key = f->_key.get_value ();
    if (key.empty())
      return;
    FatChildProcess *res;
    for (auto c : f->_container->children ()) {
      res = c->find_child (path);
      if (res) {
        if (res->get_process_type () != PROPERTY_T) {
          f->_result.set_value (c, true);
          f->_found.activate ();
          return;
        } else {
          switch (((AbstractProperty*) res)->get_prop_type ())
            {
            case Integer:
              try {
                int v = (int) stoi (key);
                if (v == ((IntProperty*) res)->get_value ()) {
                  f->_result.set_value (c, true);
                  f->_found.activate ();
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
                f->_result.set_value (c, true);
                f->_found.activate ();
                return;
              }
              break;
            case Double:
              try {
                double v = (double) stof (key);
                if (v == ((DoubleProperty*) res)->get_value ()) {
                  f->_result.set_value (c, true);
                  f->_found.activate ();
                  return;
                }
              }
              catch (const std::invalid_argument& ia) {
                warning (this, "failed to convert the string \"" + key + "\" into a double property value\n");
              }
              break;
            case String:
              if (key.compare (((TextProperty*) res)->get_value ()) == 0) {
                f->_result.set_value (c, true);
                f->_found.activate ();
                return;
              }
              break;
            default:
              break;
            }
        }
      }
    }
    f->_not_found.activate ();
  }

  Finder::Finder (FatProcess *parent, const std::string& name, FatProcess *container, const std::string& path)
  : FatProcess (name),
  _path (this, "path", path),
  _key (this, "key", ""),
  _result (this, "result", nullptr),
  _found (this, "found"),
  _not_found (this, "notFound"),
  _action (this, "action"),
  _cfind (&_key, ACTIVATION, &_action, ACTIVATION)
  {
    _container = dynamic_cast<Container*> (container);
    if (_container == nullptr) {
      warning (this, "Finder is only applicable to Containers");
      return;
    }
    _cfind.disable ();
    Graph::instance ().add_edge (&_key, &_action);
    Graph::instance ().add_edge (&_action, &_result);

    finalize_construction (parent, name);
  }

  Finder::~Finder ()
  {
    remove_state_dependency (get_parent (), &_action);
    Graph::instance ().remove_edge (&_key, &_action);
    Graph::instance ().remove_edge (&_action, &_result);
  }

  void
  Finder::set_parent (FatProcess* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
       remove_state_dependency (get_parent (), &_action);
    }

    add_state_dependency (p, &_action);
    FatProcess::set_parent (p); 
  }

  void
  Finder::impl_activate ()
  {
    _cfind.enable ();
  }

  void
  Finder::impl_deactivate ()
  {
    _cfind.disable ();
  }

#ifndef DJNN_NO_SERIALIZE
  void
  Finder::serialize (const std::string& type)
  {

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:finder");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("path", _path.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);

  }
#endif

}
