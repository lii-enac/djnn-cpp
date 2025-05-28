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

#include <stdexcept>

#include "text.h"

#include "core/core-dev.h" // graph add/remove edge
#include "core/property/text_property.h"
#include "core/serializer/serializer.h"
#include "core/utils/error.h"
#include "regex.h"

namespace djnn {

void
TextPrinter::init ()
{
    c_input.disable ();
}

TextPrinter::~TextPrinter ()
{
}

void
TextPrinter::TextPrinterAction::impl_activate ()
{
    loginfonofl (_input->get_value ());
}

TextPrinter::TextPrinter (CoreProcess* parent, const string& name)
    : FatProcess (name),
      _input (this, "input", ""),
      _action (this, get_name () + "_action", &_input),
      c_input (&_input, ACTIVATION, &_action, ACTIVATION)
{
    init ();
    finalize_construction (parent, name);
}

#ifndef DJNN_NO_SERIALIZE
void
TextPrinter::serialize (const string& type)
{

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:textprinter");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

template <>
const char name_info<TextCatenatorAction>::left[] = "head";
template <>
const char name_info<TextCatenatorAction>::right[] = "tail";
template <>
const char name_info<TextCatenatorAction>::serialize[] = "textcatenator";

template <>
const char name_info<TextComparatorAction>::left[] = "left";
template <>
const char name_info<TextComparatorAction>::right[] = "right";
template <>
const char name_info<TextComparatorAction>::serialize[] = "textcomparator";

template <>
const char name_info<TextContainerAction>::left[] = "left";
template <>
const char name_info<TextContainerAction>::right[] = "right";
template <>
const char name_info<TextContainerAction>::serialize[] = "textcontainer";

void
DoubleFormatter::init (double initial, int decimal)
{
    _c_input.disable ();
    _c_decimal.disable ();
    graph_add_edge (&_action, &_output);
}

DoubleFormatter::~DoubleFormatter ()
{
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_action);
    }
    graph_remove_edge (&_action, &_output);
}

void
DoubleFormatter::set_parent (CoreProcess* parent)
{
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_action);
    }

    add_state_dependency (parent, &_action);

    FatProcess::set_parent (parent);
}

DoubleFormatter::DoubleFormatter (CoreProcess* parent, const string& name, double initial, int decimal)
    : FatProcess (name),
      _input (this, "input", initial),
      _decimal (this, "decimal", decimal),
      _output (this, "output", ""),
      _action (this, "action", *this),
      _c_input (&_input, ACTIVATION, &_action, ACTIVATION),
      _c_decimal (&_decimal, ACTIVATION, &_action, ACTIVATION)
{
    init (initial, decimal);
    finalize_construction (parent, name);
}

void
DoubleFormatter::impl_activate ()
{
    _c_input.enable ();
    _c_decimal.enable ();
    _action.activate ();
}

void
DoubleFormatter::impl_deactivate ()
{
    _c_input.disable ();
    _c_decimal.disable ();
    _action.deactivate ();
}

#ifndef DJNN_NO_SERIALIZE
void
DoubleFormatter::serialize (const string& type)
{

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:doubleformatter");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("initial", _input.get_value ());
    AbstractSerializer::serializer->int_attribute ("decimal", _decimal.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

TextAccumulator::TextAccumulator (CoreProcess* parent, const string& name, const string& init)
    : FatProcess (name),
      _input (this, "input", ""),
      _state (this, "state", init),
      _del (this, "delete"),
      _acc_action (this, "acc_action", *this),
      _del_action (this, "del_action", *this),
      _c_acc (&_input, ACTIVATION, &_acc_action, ACTIVATION),
      _c_del (&_del, ACTIVATION, &_del_action, ACTIVATION)
{
    _c_acc.disable ();
    _c_del.disable ();
    graph_add_edge (&_acc_action, &_state);
    graph_add_edge (&_del_action, &_state);
    finalize_construction (parent, name);
}

TextAccumulator::~TextAccumulator ()
{
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_acc_action);
        remove_state_dependency (get_parent (), &_del_action);
    }
    graph_remove_edge (&_acc_action, &_state);
    graph_remove_edge (&_del_action, &_state);
}

void
TextAccumulator::set_parent (CoreProcess* parent)
{
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_acc_action);
        remove_state_dependency (get_parent (), &_del_action);
    }

    add_state_dependency (parent, &_acc_action);
    add_state_dependency (parent, &_del_action);

    FatProcess::set_parent (parent);
}

void
TextAccumulator::impl_activate ()
{
    _c_acc.enable ();
    _c_del.enable ();
}

void
TextAccumulator::impl_deactivate ()
{
    _c_acc.disable ();
    _c_del.disable ();
}

#ifndef DJNN_NO_SERIALIZE
void
TextAccumulator::serialize (const string& type)
{
    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:text-accumulator");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("initial", _state.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

void
Regex::RegexAction::impl_activate ()
{
    std::match_results<string::const_iterator> match;
    auto&                                      input = _reg._input.get_value ();
#if DJNN_STL_STD
    if (std::regex_search (input, match, _reg._regex) && match.size () > 1) {
#elif DJNN_STL_EASTL
    if (std::regex_search (input.begin (), input.end (), match, _reg._regex) && match.size () > 1) {
#elif DJNN_STL_DJNN
    if (std::regex_search (input.begin (), input.end (), match, _reg._regex) && match.size () > 1) {
#endif
        // activate all entries before the general match [0]
        for (size_t i = 1; i < match.size (); i++) {
            if (!match.str (i).empty ()) {
                AbstractTextProperty* p   = dynamic_cast<AbstractTextProperty*> (_parent->find_child_impl (std::to_string (i)));
                const string          res = match.str (i).c_str (); // FIXME copy of string even if no EASTL (or not, if optimized?)
                p->set_value (res, true);
            }
        }
        // then activate the match [0]
        auto it = _reg._in_map.find (0);
        if (it != _reg._in_map.end () && !match.str (0).empty ()) {
            const string res = match.str (0).c_str (); // FIXME copy of string even if no EASTL (or not, if optimized?)
            it->second->set_value (res, true);
        }
        static_cast<Regex*>(_parent)->matched()->set_value(true, true);
    }
    else {
        static_cast<Regex*>(_parent)->matched()->set_value(false, true);
    }
}

Regex::Regex (CoreProcess* parent, const string& name, const string& reg)
    : FatProcess (name),
      _input (this, "input", ""),
      _init (this, "init", reg),
      _matched (this, "matched", false),
      _regex (reg.c_str ()),
      _reg_action (this, "reg_action", *this),
      _c_reg (&_input, ACTIVATION, &_reg_action, ACTIVATION)
{
    _c_reg.disable ();
    graph_add_edge (&_input, &_reg_action);
    graph_add_edge (&_reg_action, &_matched);
    finalize_construction (parent, name);
}

Regex::~Regex ()
{
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_reg_action);
    }
    graph_remove_edge (&_reg_action, &_matched);
    graph_remove_edge (&_input, &_reg_action);

    for (auto p : _in_map) {
        graph_remove_edge (&_reg_action, p.second);
        delete p.second;
    }
}

FatProcess*
Regex::find_child_impl (const string& key)
{

    if (key.compare ("input") == 0)
        return &_input;
    if (key.compare ("init") == 0)
        return &_init;
    if (key.compare ("matched") == 0)
        return &_matched;

    try {
        int i = stoi (key, nullptr);

        map<int, TextProperty*>::iterator it = _in_map.find (i);
        if (it != _in_map.end ()) {
            /* key exist  - return */
            return it->second;
        } else {
            /* key doesn't exist */
            TextProperty* newin = new TextProperty (this, key, "");
            graph_add_edge (&_reg_action, newin);
            _in_map[i] = newin;
            return newin;
        }

    } catch (const std::invalid_argument& ia) {
        // std::cerr << "Regex - find_child - Invalid argument (not \"input\" or NAN): " << key << std::endl;
        return nullptr;
    }
}

void
Regex::set_parent (CoreProcess* parent)
{
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_reg_action);
    }

    add_state_dependency (parent, &_reg_action);

    FatProcess::set_parent (parent);
}

void
Regex::impl_activate ()
{
    _c_reg.enable ();
}

void
Regex::impl_deactivate ()
{
    _c_reg.disable ();
}

#ifndef DJNN_NO_SERIALIZE
void
Regex::serialize (const string& type)
{
    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:regex");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("regex", _init.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

} // namespace djnn
