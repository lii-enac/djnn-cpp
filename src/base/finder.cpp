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

#include <stdexcept>

#include "core/core-dev.h" // graph add/remove edge
#include "core/ontology/coupling.h"
#include "core/property/bool_property.h"
#include "core/property/double_property.h"
#include "core/property/int_property.h"
#include "core/serializer/serializer.h"
#include "core/utils/djnn_dynamic_cast.h"
#include "core/utils/error.h"

namespace djnn {
void
Finder::FinderAction::impl_activate ()
{
    Finder* f    = (Finder*)get_parent ();
    string  path = f->_path.get_value ();
    string  key  = f->_key.get_value ();
    if (key.empty ()) {
        warning (this, "finder - \"key\" is empty \n");
        f->_result.set_value ((CoreProcess*)nullptr, true); // ??
        f->_not_found.set_activation_flag (ACTIVATION);
        return;
    }
    CoreProcess* res;
    for (auto c : f->_container->children ()) {
        res = c->find_child_impl (path);
        if (res) {
            if (res->get_process_type () != PROPERTY_T) {
                continue;
            } else {
                switch (((AbstractProperty*)res)->get_property_type ()) {
                case Integer:
                    try {
                        int v = (int)stoi (key);
                        if (v == ((IntProperty*)res)->get_value ()) {
                            f->_result.set_value (c, true);
                            f->_found.set_activation_flag (ACTIVATION);
                            return;
                        }
                    } catch (const std::invalid_argument& ia) {
                        warning (this, "finder - failed to convert the string \"" + key + "\" into an integer property value\n");
                    }
                    break;
                case Boolean:
                    if ((key.compare ("true") == 0 && ((BoolProperty*)res)->get_value () == 1) || (key.compare ("false") == 0 && ((BoolProperty*)res)->get_value () == 0)) {
                        f->_result.set_value (c, true);
                        f->_found.set_activation_flag (ACTIVATION);
                        return;
                    }
                    break;
                case Double:
                    try {
                        double v = (double)stof (key);
                        if (v == ((DoubleProperty*)res)->get_value ()) {
                            f->_result.set_value (c, true);
                            f->_found.set_activation_flag (ACTIVATION);
                            return;
                        }
                    } catch (const std::invalid_argument& ia) {
                        warning (this, "finder - failed to convert the string \"" + key + "\" into a double property value\n");
                    }
                    break;
                case String:
                    if (key.compare (((TextProperty*)res)->get_value ()) == 0) {
                        f->_result.set_value (c, true);
                        f->_found.set_activation_flag (ACTIVATION);
                        return;
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
    f->_result.set_value ((CoreProcess*)nullptr, true);
    f->_not_found.set_activation_flag (ACTIVATION);
}

Finder::Finder (CoreProcess* parent, const string& name, CoreProcess* container, const string& path)
    : FatProcess (name),
      _path (this, "path", path),
      _key (this, "key", ""),
      _result (this, "result", nullptr),
      _found (this, "found"),
      _not_found (this, "notFound"),
      _action (this, "action"),
      _cfind (&_key, ACTIVATION, &_action, ACTIVATION)
{
    _container = djnn_dynamic_cast<Container*> (container);
    if (_container == nullptr) {
        warning (this, "Finder is only applicable to Containers");
        return;
    }
    _cfind.disable ();
    graph_add_edge (&_action, &_result);
    graph_add_edge (&_action, &_found);
    graph_add_edge (&_action, &_not_found);

    finalize_construction (parent, name);
}

Finder::~Finder ()
{
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_action);
    }
    graph_remove_edge (&_action, &_result);
    graph_remove_edge (&_action, &_found);
    graph_remove_edge (&_action, &_not_found);
}

void
Finder::set_parent (CoreProcess* parent)
{
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_action);
    }

    add_state_dependency (parent, &_action);
    FatProcess::set_parent (parent);
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
Finder::serialize (const string& type)
{

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:finder");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("path", _path.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

} // namespace djnn
