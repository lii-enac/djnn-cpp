/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2022)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 */

#pragma once

#include "core/ontology/coupling.h"
#include "core/ontology/process.h"
#include "core/property/text_property.h"
#include "gui/shape/abstract_gshape.h"

namespace djnn {

class SimpleText : public AbstractGShape {
  public:
    SimpleText (CoreProcess* parent, const string& name, double x, double y, const string& text);
    virtual ~SimpleText ();
    void                 impl_activate () override;
    void                 impl_deactivate () override;
    void                 set_parent (CoreProcess* p) override;
    string               get_content () { return raw_props.text; }
    void                 set_content (string& new_content) { _text.set_value (new_content, true); };
    double               get_x () { return raw_props.x; }
    double               get_y () { return raw_props.y; }
    void                 set_x (double x) { raw_props.x = x; }
    void                 set_y (double y) { raw_props.y = y; }
    void                 get_properties_values (double& x, double& y, string& text);
    virtual CoreProcess* find_child_impl (const string&) override;

  private:
    struct raw_props_t {
        double x, y;
        string text;
    };
    raw_props_t       raw_props;
    TextPropertyProxy _text;
    CouplingWithData *_cx, *_cy;
    CouplingWithData  _ctext;
    bool              _init;
};

typedef size_t coord_t;
coord_t        next_index (const djnnstl::string& str, coord_t idx);
coord_t        previous_index (const djnnstl::string& str, coord_t idx);
bool           isspace (const char& c);
bool           isdelimiter (const char& c);
bool           is_starting_word (const djnnstl::string& str, coord_t i);
coord_t        first_word (const djnnstl::string& str, coord_t idx);
coord_t        next_word (const djnnstl::string& str, coord_t idx);
coord_t        previous_word (const djnnstl::string& str, coord_t idx);
} // namespace djnn
