#include <fstream>

#include "core/core-dev.h"

#include "core/ontology/process.h"
#include "core/utils/to_string.h"

#include "core/utils/utils-dev.h" // get_hierarchy_name
#include "core/utils/iostream.h"
#include <iomanip>

namespace djnn {
#ifndef DJNN_NO_DEBUG // still required to link smala programs
                      // DEBUG OPTIONS
int
    _DEBUG_SEE_COLOR_PICKING_VIEW                 = 0,
    _DEBUG_GRAPH_CYCLE_DETECT                     = 0,
    _DEBUG_SEE_ACTIVATION_SEQUENCE                = 0,
    _DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_TIME_US = 1000,
    _DEBUG_SEE_ACTIVATION_SEQUENCE_ONLY_TARGETED  = 0,
    _DEBUG_SEE_ACTIVATION_SEQUENCE_2              = 0,
    _DEBUG_SEE_PROP_SET_VALUE                     = 0,
    _AUTHORIZE_CYCLE                              = 0,
    _DEBUG_SEE_RECOMPUTE_PIXMAP_AND_PAINTEVENT    = 0,
    _DEBUG_SEE_RECOMPUTE_PIXMAP_ONLY              = 0,
    _DEBUG_SEE_COMPONENTS_DESTRUCTION_INFO_LEVEL  = 0,
    _ENABLE_TOUCHES                               = 0,
    _DEBUG_ENABLE_STRESS_TEST                     = 0,
    _STYLUS_EMULATE_MOUSE                         = 0;

const char* _DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_LOCATION = "";

djnnstl::string _SVG_USER_CUSTOM_ATTRS = "";


djnnstl::string cpp_demangle (char const* name);

djnnstl::string
print_process_full_name (const CoreProcess* p)
{
    return cpp_demangle (typeid (*p).name ()) + "- (" +
           (p && p->get_debug_parent () ? p->get_debug_parent ()->get_debug_name () + "/" : "") +
           (p ? p->get_debug_name () : "") + ")";
}

djnnstl::string
extract_filename (const djnnstl::string& s)
{
    std::string::size_type n;
    n                = s.rfind ('/');
    std::string sub1 = s.substr (n + 1);
    // std::cout << sub1 << '\n';
    return sub1;
}

djnnstl::string
extract_code_from_file (const djnnstl::string& filepath, int lineno)
{
    std::ifstream file (filepath); // ouverture du fichier
    djnnstl::string        line;
    int           current_line = 0;
    while (getline (file, line)) {
        current_line++;
        if (current_line == lineno) {
            line.erase (0, line.find_first_not_of (" \t"));
            return line;
        }
    }
    // si on n'a pas trouvé la ligne demandée
    return " line not found in file ! ";
}

djnnstl::string
print_process_fileno (const CoreProcess* p)
{
    return extract_filename (p->debug_info ().filepath) + ":" + djnnstl::to_string (p->debug_info ().lineno);
}

djnnstl::string
print_process_debug_info (const CoreProcess* p)
{
    //return " ---- from " + print_process_fileno (p) + " ---- " + extract_code_from_file (p->debug_info ().filepath, p->debug_info ().lineno);
    return print_process_fileno (p) + ": " + extract_code_from_file (p->debug_info ().filepath, p->debug_info ().lineno);
}

void
debug_activation_sequence_2 (CoreProcess *p)
{
    bool do_debug_log = _DEBUG_SEE_ACTIVATION_SEQUENCE_2;// && debug_info ().lineno != 0;
    extern int debug_index, debug_index_post;
    //int sav_debug_index = debug_index;
    if (do_debug_log) {
        auto const& src_code = print_process_debug_info (p);
        auto const & hn = get_hierarchy_name (p);
        if (!hn.empty()) {
            const int ms = sizeof("move_x") - 1;
            //std::cerr << hn << " "<< hn.size() << " " << ms << " " << hn.substr(0, ms-2) << std::endl;
            if (hn.size()==ms) {
                if (hn.substr(0, ms-2) == "move")
                    do_debug_log = false;
            }
        }
        if (do_debug_log) {
        auto const & tn = cpp_demangle (typeid (*p).name ());
        size_t w = 50;
        std::cerr 
            << std::left
            << std::setw(60) << src_code.substr(0,60)
            << std::setw(15) << " (" + std::to_string(debug_index) + "," + std::to_string(debug_index_post) + ")" " act "
            << std::setw(w) << hn.substr(0, w-3) + (hn.size()>(w-3)?"...":" ")
            << std::setw(w) << tn.substr(0, w-3) + (tn.size()>(w-3)?"...":"")
            //<< __FL__;
            << std::endl;
        ++debug_index;
        }
    }
    if (do_debug_log) {
        //std::cerr << print_process_debug_info (this) << " (was " << sav_debug_index << ")" << std::endl;
        ++debug_index_post;
    }
}

void
debug_prop_set_value (CoreProcess *p)
{
    bool do_debug_log = _DEBUG_SEE_PROP_SET_VALUE;// && debug_info ().lineno != 0;
    extern int debug_index, debug_index_post;
    //int sav_debug_index = debug_index;
    if (do_debug_log) {
        auto const& src_code = print_process_debug_info (p);
        auto const & hn = get_hierarchy_name (p);
        if (!hn.empty()) {
            const int ms = sizeof("move_x") - 1;
            //std::cerr << hn << " "<< hn.size() << " " << ms << " " << hn.substr(0, ms-2) << std::endl;
            if (hn.size()==ms) {
                if (hn.substr(0, ms-2) == "move")
                    do_debug_log = false;
            }
        }
        if (do_debug_log) {
        auto const & tn = cpp_demangle (typeid (*p).name ());
        size_t w = 50;
        std::cerr 
            << std::left
            << std::setw(60) << src_code.substr(0,60)
            << std::setw(15) << " (" + std::to_string(debug_index) + "," + std::to_string(debug_index_post) + ")" " sv "
            << std::setw(w) << hn.substr(0, w-3) + (hn.size()>(w-3)?"...":" ")
            //<< std::setw(w) << tn.substr(0, w-3) + (tn.size()>(w-3)?"...":"")
            //<< __FL__;
            //<< std::endl
            ;
        //++debug_index;
        }
    }
    if (do_debug_log) {
        //std::cerr << print_process_debug_info (this) << " (was " << sav_debug_index << ")" << std::endl;
        //++debug_index_post;
    }
}

#endif
} // namespace djnn