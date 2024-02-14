#pragma once

#ifndef GET_CHILD
#define GET_CHILD(type, parent, path)                                                                                                                      \
    auto* path = dynamic_cast<type*> (parent->find_child (#path));                                                                                         \
    if (!path) {                                                                                                                                           \
        fprintf (stderr, "ERROR - there is NO " #type " \"%s\" child in process \"%s\" - %s:%s:%d\n\n", #path, #parent, __FUNCTION__, __FILE__, __LINE__); \
        exit (-1);                                                                                                                                         \
    }
#endif
// ex: GET_CHILD (TextProperty, data, layer_name);
// is equivalent to:
// auto * layer_name = dynamic_cast<djnn::TextProperty*>(data->find("layer_name")); (this will create a djnn TextProperty* variable 'name')

#define GET_CHILD_VAR2(varname, type, parent, path)                                                                                                        \
    varname = dynamic_cast<djnn::type*> (parent->find_child (#path));                                                                                      \
    if (!varname) {                                                                                                                                        \
        fprintf (stderr, "ERROR - there is NO " #type " \"%s\" child in process \"%s\" - %s:%s:%d\n\n", #path, #parent, __FUNCTION__, __FILE__, __LINE__); \
        exit (-1);                                                                                                                                         \
    }
#ifndef GET_CHILD_VAR
#define GET_CHILD_VAR(varname, type, parent, path) auto* GET_CHILD_VAR2 (varname, type, parent, path)
#endif
// ex: GET_CHILD_VAR (name, TextProperty, data, g/layer_name);
// is equivalent to:
// auto * name = dynamic_cast<djnn::TextProperty*>(data->find("g/layer_name")); (this will create a djnn TextProperty* variable 'name')

#ifndef GET_CHILD_VALUE
#define GET_CHILD_VALUE(varname, type, parent, path)                                                                                                       \
    auto* child_##varname = dynamic_cast<djnn::Abstract##type##Property*> (parent->find_child (#path));                                                    \
    if (!child_##varname) {                                                                                                                                \
        fprintf (stderr, "ERROR - there is NO " #type " \"%s\" child in process \"%s\" - %s:%s:%d\n\n", #path, #parent, __FUNCTION__, __FILE__, __LINE__); \
        exit (-1);                                                                                                                                         \
    };                                                                                                                                                     \
    const auto& varname = child_##varname->get_value ();
#endif
#ifndef GET_CHILD_VALUE2
#define GET_CHILD_VALUE2(varname, type, parent, path)                                                                                                      \
    if (auto* child_##parent = dynamic_cast<djnn::Abstract##type##Property*> (parent->find_child (#path))) {                                               \
        varname = child_##parent->get_value ();                                                                                                            \
    } else {                                                                                                                                               \
        fprintf (stderr, "ERROR - there is NO " #type " \"%s\" child in process \"%s\" - %s:%s:%d\n\n", #path, #parent, __FUNCTION__, __FILE__, __LINE__); \
        exit (-1);                                                                                                                                         \
    }
#endif
// ex: GET_CHILD_VALUE (name, Text, data, layer_name);
// is equivalent to:
// const auto& name = dynamic_cast<djnn::TextProperty*>(data->find_child("layer_name"))->get_value(); (this will create a "string name" with the value of the property)

#ifndef SET_CHILD_VALUE
#define SET_CHILD_VALUE(type, parent, path, value, propagate)                                                                                                  \
    {                                                                                                                                                          \
        auto* var = dynamic_cast<djnn::Abstract##type##Property*> (parent->find_child (#path));                                                                \
        if (!var) {                                                                                                                                            \
            fprintf (stderr, "ERROR - there is NO " #type " \"%s\" child in process \"%s\" - %s:%s:%d\n\n", #path, #parent, __FUNCTION__, __FILE__, __LINE__); \
            exit (-1);                                                                                                                                         \
        }                                                                                                                                                      \
        var->set_value (value, propagate);                                                                                                                     \
    }
#endif
// ex: SET_CHILD_VALUE(djnn::Text, data, img/path, new_path, true);
// this will set the content of the existing variable new_path to the "img/path" child of data, as a djnn::TextProperty
