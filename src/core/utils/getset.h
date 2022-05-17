#pragma once

#define GET_CHILD(varname, type, parent, name) auto * varname = dynamic_cast<type*>(parent->find_child(name)); if (!varname) { fprintf (stderr, "no " #type " \"%s\" child in process %s:%s:%d\n", name, __FUNCTION__,__FILE__,__LINE__);  exit(-1); }
// ex: GET_CHILD (name, djnn::TextProperty, data, "layer_name");
// is equivalent to:
// auto * name = dynamic_cast<djnn::TextProperty*>(data->find("layer_name")); (this will create a djnn TextProperty* variable 'name')

#define GET_CHILD_VALUE(varname, type, parent, name) auto * child_##varname = dynamic_cast<type##Property*>(parent->find_child(name)); if (!child_##varname) { fprintf (stderr, "no " #type " \"%s\" child in process %s:%s:%d\n", name, __FUNCTION__,__FILE__,__LINE__);  exit(-1); }; auto varname = child_##varname->get_value ();
// ex: GET_CHILD_VALUE (name, djnn::Text, data, "layer_name");
// is equivalent to:
// auto name = dynamic_cast<djnn::TextProperty*>(data->find_child("layer_name"))->get_value(); (this will create a "string name" with the value of the property)

#define SET_CHILD_VALUE(type, parent, name, value, propagate) { auto * var = dynamic_cast<type##Property*>(parent->find_child(name)); if (!var) { fprintf (stderr, "no " #type " \"%s\" child in process %s:%s:%d\n", name, __FUNCTION__,__FILE__,__LINE__);  exit(-1); } var->set_value(value, propagate); }
// ex: SET_CHILD_VALUE(djnn::Text, data, "img/path", new_path, true);
// this will set the content of the existing variable new_path to the "img/path" child of data, as a djnn::TextProperty
