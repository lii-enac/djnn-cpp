/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include <regex>
#include <unistd.h>

#include "IvyAccess.h"

#include "core/core-dev.h" // graph exec
#include "core/utils/error.h"
#include "core/utils/iostream.h"
#include "core/utils/to_string.h"
#include "exec_env/main_loop.h"
#include "ivy.h"     //TODO add Ivy/ after confinement
#include "ivyloop.h" //TODO add Ivy/ after confinement
#include "utils/debug.h"

// using djnnstl::cout;
// using djnnstl::cerr;
// using djnnstl::endl;

using namespace djnnstl;

// #define __IVY_DEBUG__

static std::map<IvyClientPtr, string> __app_map;

/** regexp function **/

static int
count_regex_groups (const std::string& pattern)
{
    std::regex  re (pattern);
    std::smatch match;

    /* The number of groups is the value of regex.mark_count() */
    int count = re.mark_count ();

    return count;
}

static const char*
_skim_regex (const char* p, int* nb)
{
    enum states
    {
        NORMAL,
        QUOTED,
        SUBEXP,
        BRACKETED
    };
    const char* q            = p;
    enum states state        = NORMAL;
    enum states quoted_state = NORMAL;
    *nb                      = 0;
    while (*q) {
        switch (state) {
        case NORMAL:
            switch (*q) {
            case '\\':
                quoted_state = NORMAL;
                state        = QUOTED;
                break;
            case '/':
                goto done;
            case '(':
                state = SUBEXP;
                (*nb)++;
                break;
            case '[':
                state = BRACKETED;
                break;
            }
            break;
        case QUOTED:
            state = quoted_state;
            break;
        case SUBEXP:
            switch (*q) {
            case '\\':
                quoted_state = SUBEXP;
                state        = QUOTED;
                break;
            case ')':
                state = NORMAL;
                break;
            }
            break;
        case BRACKETED:
            switch (*q) {
            case '\\':
                quoted_state = BRACKETED;
                state        = QUOTED;
                break;
            case ']':
                state = NORMAL;
                break;
            }
            break;
        }
        ++q;
    }

done:
    return q;
}

/** IVY DEBUG MAPPING **/

#ifdef __IVY_DEBUG__
static void
_ivy_debug_mapping (map<string, vector<pair<int, djnn::TextProperty*>>> inmap)
{

    map<string, vector<pair<int, djnn::TextProperty*>>>::iterator mit;
    cout << endl
         << "MAP _in_map:" << endl;
    for (mit = inmap.begin (); mit != inmap.end (); ++mit) {
        cout << mit->first << " => (";
        /* vector */
        vector<pair<int, djnn::TextProperty*>>::iterator vit;
        for (vit = mit->second.begin (); vit != mit->second.end (); ++vit) {
            /* pair */
            cout << "[" << (*vit).first << ", " << (*vit).second << "] ";
        }
        cout << ")" << endl;
    }
    cout << endl;
}
#endif

/** IVY CALLBACK **/

#define STOP_IVY djnn::MainLoop::instance ().is_stopping () || access->should_i_stop ()

static void
_on_ivy_message (IvyClientPtr app, void* user_data, int argc, char** argv)
{
    djnn::IvyAccess::msg_callback_user_data* mcud = reinterpret_cast<djnn::IvyAccess::msg_callback_user_data*> (user_data);

    djnn::IvyAccess* access = mcud->access; // reinterpret_cast<djnn::IvyAccess*>(user_data);
    if (STOP_IVY)
        return;

    djnn::get_exclusive_access (DBG_GET);

    if (STOP_IVY) {
        djnn::release_exclusive_access (DBG_REL);
        return;
    }

    // pair<string, map<string, vector<pair<int, djnn::TextProperty*>>>*>* keypair =
    //(pair<string, map<string, vector<pair<int, djnn::TextProperty*>>>*> *) user_data;
    djnn::IvyAccess::regexp_keypair_t* keypair = mcud->keypair; // reinterpret_cast<djnn::IvyAccess::regexp_keypair_t*>(user_data);
    djnnstl::string                    regexp  = keypair->first;
    // map<string, vector<pair<int, djnn::TextProperty*>>>*
    djnn::IvyAccess::in_map_t*
        in_map = keypair->second;

#ifdef __IVY_DEBUG__
    cout << endl
         << "_on_ivy_message" << endl;
    cout << "regexp: '" << regexp << "'" << endl;
    _ivy_debug_mapping (*in_map);
#endif

    // map<string, vector<pair<int, djnn::TextProperty*>>>::iterator mit;
    //::iterator mit;
    auto mit = in_map->find (regexp);

    if (mit != in_map->end ()) {
        /* the regexp exist in map */
        // vector<pair<int, djnn::TextProperty*>>::iterator vit;
        for (auto vit = mit->second.begin (); vit != mit->second.end (); ++vit) {
            /* pair */
            djnn::TextProperty* txtprop = (*vit).second;
            if ((*vit).first - 1 >= argc) {
                cerr << "Ivy err: regexp position is greater or equal to argc " << (*vit).first << " " << argc << endl;
                continue;
            }
            djnnstl::string msg = argv[(*vit).first - 1]; // index shift is -1 between regexp and argv
            txtprop->set_value (msg, true);
            txtprop->schedule_activation ();
            // GRAPH_EXEC; // methode 1 : per value
        }
    }

#ifdef __IVY_DEBUG__
    cout << "---------------------" << endl;
    cout << "_on_ivy_message - " << endl;
    cout << "argc " << argc << endl;
    for (int i = 0; i < argc; i++) {
        cout << "argv[" << i << "] - " << djnnstl::string (argv[i]) << endl;
    }
    cout << "user_data (pair->first - regexp) : \"" << regexp << "\"" << endl;
    cout << "---------------------" << endl
         << endl;
#endif
    GRAPH_EXEC; // methode 2 : per message
    djnn::release_exclusive_access (DBG_REL);
}

static void
_on_ivy_die (IvyClientPtr app, void* user_data, int event)
{
    djnn::IvyAccess* access = reinterpret_cast<djnn::IvyAccess*> (user_data);

    djnn::get_exclusive_access (DBG_GET);

    if (STOP_IVY) {
        djnn::release_exclusive_access (DBG_GET);
        return;
    }

    access->get_die ().activate (); // notify _die has been receiveds
    GRAPH_EXEC;

    djnn::release_exclusive_access (DBG_GET);
}

static void
_on_ivy_arriving_leaving_agent (IvyClientPtr app, void* user_data, IvyApplicationEvent event)
{
    djnn::IvyAccess* access = reinterpret_cast<djnn::IvyAccess*> (user_data);

    djnn::get_exclusive_access (DBG_GET);

    if (STOP_IVY) {
        djnn::release_exclusive_access (DBG_GET);
        return;
    }

    string appName = IvyGetApplicationName (app);

    if (event == IvyApplicationConnected) {

        // managing _app_map
        auto it = __app_map.find (app);
        if (it == __app_map.end ())
            __app_map[app] = appName;
        else
            djnn_warning (nullptr, "this ivy application IvyClienPtr already exist : " + appName + "\n");

        string appHost = IvyGetApplicationHost (app);
        access->set_arriving (appName);
        access->set_arriving_info ("appName: " + appName + " - FQDN: " + appHost);
    }

    else if (event == IvyApplicationDisconnected) {

        // managing _app_map
        auto it = __app_map.find (app);
        if (it != __app_map.end ()) {
            __app_map.erase (it);
        } else
            djnn_warning (nullptr, "this ivy application IvyClienPtr name does not exist : " + appName + "\n");

        access->set_leaving (appName);
    }

    // debug
    // auto& app_map = access->get_app_map ();
    // std::cerr << "list of app:" << std::endl;
    // for (const auto& pair : app_map) {
    //     std::cout << pair.first << std::endl;
    // }

    GRAPH_EXEC;
    djnn::release_exclusive_access (DBG_GET);
}

namespace djnn {

/****  IVY OUT ACTIONS ****/

void
IvyAccess::IvyOutAction::impl_activate () // coupling_activation_hook ()
{
    IvySendMsg ("%s", _out->get_value ().c_str ());
}

/****  IVY SEND DIE ACTIONS ****/

void
IvyAccess::IvySendDieAction::impl_activate () // coupling_activation_hook ()
{
    const string appName_to_found = _send_die->get_value ();

    /* send "Die" to all ivy application that have these name : _send_die->get_value () */
    bool found = false;
    for (const auto& it : __app_map) {
        if (it.second == appName_to_found) {
            IvySendDieMsg (it.first);
            found = true;
        }
    }

    if (!found)
        djnn__warning (this, "this ivy application \"" + appName_to_found + "\" is unknown \n");
}

/**** IVY ACCESS ****/

IvyAccess::IvyAccess (CoreProcess* parent, const string& name,
                      const string& bus, const string& appname, const string& ready, bool isModel)
    : FatProcess (name, isModel),
      ExternalSource (name),
      _out (this, "out", ""),
      _out_a (this, "out_action", &_out),
      _out_c (&_out, ACTIVATION, &_out_a, ACTIVATION, true),
      _arriving (this, "arriving", ""),
      _arriving_info (this, "arriving_info", ""),
      _leaving (this, "leaving", ""),
      _send_die (this, "send_die", ""),
      _send_die_a (this, "send_die_action", &_send_die),
      _send_die_c (&_send_die, ACTIVATION, &_send_die_a, ACTIVATION, true),
      _die (this, "die", true)

{
    _bus           = bus;
    _appname       = appname;
    _ready_message = ready;

    _out_c.disable ();
    _send_die_c.disable ();

    /* note:
     * c_out in now immediat : 07.2020 - to react each time is activated
     * so we removed the remove_edge (to graph)
     * it works because Ivy can't be stoped
     */
    // graph_add_edge (&_out, &_out_a);

    /* IN is a special child build in IvyAccess::find_child */

    finalize_construction (parent, name);
}

IvyAccess::~IvyAccess ()
{

    /* remove edges */

    /* note:
     * c_out in now immediat : 07.2020 - to react each time is activated
     * so we removed the remove_edge (to graph)
     * it works because Ivy can't be stoped
     */
    // graph_remove_edge (&_out, &_out_a);

    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_out_a);
        remove_state_dependency (get_parent (), &_send_die_a);
    }

    /* erase _cb_regex_vector */

    // cerr << "_cb_regex_vector.size: " << _cb_regex_vector.size () << endl;
    while (!_cb_regex_vector.empty ()) {
        auto st = _cb_regex_vector.back ();
        _cb_regex_vector.pop_back ();
        delete st->keypair; // delete the regexp_keypair_t
        delete st;          // delete the pointer on struct djnn::IvyAccess::msg_callback_user_data
    }
    // cerr << "_cb_regex_vector.size: " << _cb_regex_vector.size () << endl;

    /* erase _in_map */

    // _ivy_debug_mapping (_in_map);
    auto it = _in_map.begin ();
    while (it != _in_map.end ()) {
        // note it->second is a vector of pair <index, *textProperty>
        auto v = it->second;
        while (!v.empty ()) {
            // note v.back () is a pair <index, *textProperty>
            auto p = v.back ();
            // remove and delete the Textproperty
            this->remove_child (p.second);
            delete p.second;
            v.pop_back ();
        }

        _in_map.erase (it);
        it = _in_map.begin ();
    }
    // _ivy_debug_mapping (_in_map);

    /* cleaning symtable */

    /* becareful :
     * - 8 children (plain object) should stay in the symtable: out, out_action, arriving, leaving, arriving_info
     * - IvyAccess is not a Container so we have to take care of the symtable
     */
    auto it_s = symtable ().begin ();
    while (symtable ().size () != 8) {
        if (it_s->first.compare ("out") == 0 || it_s->first.compare ("out_action") == 0 || it_s->first.compare ("arriving_info") == 0 || it_s->first.compare ("arriving") == 0 || it_s->first.compare ("arriving_info") == 0 || it_s->first.compare ("leaving") == 0 || it_s->first.compare ("die") == 0 || it_s->first.compare ("send_die") == 0 || it_s->first.compare ("send_die_action") == 0) {
            it_s++;
        } else {
            delete it_s->second;
            std::string key_to_remove = it_s->first;
            it_s++; // to keep iterator valid
            this->remove_symbol (key_to_remove);
        }
    }

    // DEBUG
    // std::cerr << std::endl << std::endl << std::endl;
    // cerr << "remaining children after symtable cleaning :" << children_size () << endl ;
    // for (auto s : this->symtable ()) {
    //   cerr << s.first << endl;
    // }
}

// note : see impl_activate on stoping Ivy
// void
// IvyAccess::set_parent (CoreProcess* parent)
// {
//   /* in case of re-parenting remove edge dependency in graph */
//   if (get_parent ()) {
//       remove_state_dependency (get_parent (), &_out_a);
//   }

//   add_state_dependency (parent, &_out_a);

//   FatProcess::set_parent (parent);
// }

void
IvyAccess::impl_activate ()
{
    // if ( somehow_activating() )
    /* should never happen for IvyAccess */
    //  return;
    // DBG;
    // MainLoop::instance().add_external_source(this); // FIXME TODO
    ExternalSource::start ();
    ///* launch thread */
    // start_thread ();
}

void
IvyAccess::impl_deactivate ()
{
    // Ivy stop Mainloop
    IvyStop ();

    // ask to stop this ExternalSource
    please_stop ();
}

static void
_before_select (void* data)
{
    IvyAccess* access = reinterpret_cast<IvyAccess*> (data);
    if (STOP_IVY) {
        return;
    }
    djnn::get_exclusive_access (DBG_GET);
    if (access->should_i_stop ())
        return;
    GRAPH_EXEC;
    djnn::release_exclusive_access (DBG_REL);
}

static void
_after_select (void* data)
{
    IvyAccess* access = reinterpret_cast<IvyAccess*> (data);
    if (STOP_IVY) {
        // throw 1; thread leak, to be continued...
    }
}

void
IvyAccess::run ()
{
    set_please_stop (false);
    try {
        IvyInit (_appname.c_str (), _ready_message.c_str (), _on_ivy_arriving_leaving_agent, this, _on_ivy_die, this);

        /* get exclusive_access - before select */
        IvySetBeforeSelectHook (_before_select, this);
        /* release exclusive_access - after select */
        IvySetAfterSelectHook (_after_select, this);

        IvyStart (_bus.c_str ());

        /* enable coupling */
        djnn::get_exclusive_access (DBG_GET);
        _out_c.enable ();
        _send_die_c.enable ();
        djnn::release_exclusive_access (DBG_REL);

        /* note :
         * for now,
         * We can NOT get out of IvyMainLoop:
         * IvyStop has never been implemented...
         *
         */
        try {
            /* start Ivy mainloop */
            IvyMainLoop ();
        } catch (int) {
            DBG;
        }

        // ... hence we will never reach this point...

        /* disable coupling */
        djnn::get_exclusive_access (DBG_GET);
        _out_c.disable ();
        _send_die_c.disable ();
        djnn::release_exclusive_access (DBG_REL);

    } catch (std::exception& e) {
        warning (nullptr, e.what ());
    }
}

CoreProcess*
IvyAccess::find_child_impl (const string& key)
{

#ifdef __IVY_DEBUG__
    cout << endl
         << "---- Ivy find_child key : " << key << "------" << endl;
#endif

    if (key.at (0) == 'i' && key.at (1) == 'n' && key.at (2) == '/') {

        symtable_t::iterator it = find_child_iterator (key.substr (3));
        if (it != children_end ()) {
            /* key exist  - return */
            return it->second;
        }

        /* key do not exist - maybe use internal string as regexp */

        /*
          get sub_string = ...... : in/....../1
          and save index : 1
        */
        int         nb_subexp, len = 0;
        string      full_exp = key.substr (3);
        const char* re_end   = _skim_regex (full_exp.c_str (), &nb_subexp);

        string regexp = full_exp;
        if (re_end && *re_end != '\0') {
            len    = key.find (re_end, 3);
            regexp = key.substr (3, len - 3);
        }
        int index = std::stoi (re_end + 1);

#ifdef __IVY_DEBUG__
        cout << "regexp : \"" << regexp << "\" - full : \"" << full_exp << "\"" << endl;
#endif

        AbstractTextProperty* tmp = dynamic_cast<AbstractTextProperty*> (FatProcess::find_child_impl (regexp));
        if (tmp) {

            string reg_str = tmp->get_value ();

#ifdef __IVY_DEBUG__
            cout << "REPLACE : \"" << regexp << "\" -> \"" << reg_str << "\"" << endl;
#endif

            // take advantage of the first call to generate all child properties for each of the regexp groups
            // else just, answer the initial question
            map<string, vector<pair<int, djnn::TextProperty*>>>::iterator mit;
            mit = _in_map.find (reg_str);
            if (mit == _in_map.end ()) {
                int nb_groups = count_regex_groups (reg_str);
#ifdef __IVY_DEBUG__
                cout << endl
                     << "----------- initialize full regex ------------" << endl;
                cout << "nb groupes: " << nb_groups << endl;
#endif
                for (int i = 1; i <= nb_groups; i++) {
                    string new_bind_to_generate = "in/" + reg_str + "/" + djnnstl::to_string (i);
                    /* key do not exist - but use internal string as regexp --- return*/
                    find_child_impl (new_bind_to_generate);
                }

#ifdef __IVY_DEBUG__
                _ivy_debug_mapping (_in_map);
                cout << "-------------- END initialization -------------" << endl
                     << endl;
#endif
            }

            // found the answer for the question asked
            string new_regexp_to_found = "in/" + reg_str + "/" + djnnstl::to_string (index); // the initial question to answer.

#ifdef __IVY_DEBUG__
            cout << "new_regexp_to_found : " << new_regexp_to_found << endl;
#endif

            return find_child_impl (new_regexp_to_found);

        } else {

            /* key don't exist at all - create it */

            /* add as a new _in child */
            /* and keep track of "/number" */
            TextProperty* newin = new TextProperty (this, full_exp, "");

            /* if it is the first binding on this regexp we had  the callback else nothing */
            map<string, vector<pair<int, djnn::TextProperty*>>>::iterator mit;
            mit = _in_map.find (regexp);
            if (mit == _in_map.end ()) {
                /* the only way for now is to save in a pair <regexp, in_map*>* to keep track on cb */
                auto*                   regexp_keypair = new regexp_keypair_t (regexp, &_in_map);
                msg_callback_user_data* mcud           = new djnn::IvyAccess::msg_callback_user_data{this, regexp_keypair};
                _cb_regex_vector.push_back (mcud);
                IvyBindMsg (_on_ivy_message, mcud, "%s", regexp.c_str ());
            }

            /* register in _in_map */
            _in_map[regexp].push_back (djnnstl::pair (index, newin));

#ifdef __IVY_DEBUG__
            _ivy_debug_mapping (_in_map);
            cout << "nb sub : " << nb_subexp << " endl : \"" << re_end << "\" len : " << len << " index : " << index << endl;
            cout << " regexp : \"" << regexp << "\" - full : \"" << full_exp << "\"" << endl
                 << endl;
#endif

            /* key don't exist at all - return */
            return newin;
        }
    }

    else if (key.compare ("out") == 0)
        return &_out;

    else if (key.compare ("arriving") == 0)
        return &_arriving;

    else if (key.compare ("arriving_info") == 0)
        return &_arriving_info;

    else if (key.compare ("leaving") == 0)
        return &_leaving;

    else if (key.compare ("send_die") == 0)
        return &_send_die;

    else if (key.compare ("die") == 0)
        return &_die;

    else
        return 0;
}

} // namespace djnn
