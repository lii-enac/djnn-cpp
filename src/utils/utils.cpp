/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2026)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "utils.h"

#include "core/serializer/serializer.h"

// #include <boost/core/demangle.hpp>
#include <typeinfo>

#include "core/utils/iostream.h"
#include "core/utils/utils-dev.h" // demangle

#if _DEBUG_SEE_CREATION_DESTRUCTION_ORDER

#include <mutex>
#include <iomanip> // Pour std::setw
// #include <boost/core/demangle.hpp>
#include "base/connector.h"
#include "base/fsm.h"
#include "base/operators.h"
#include "base/switch.h"
#include "core/control/binding.h"
#include "core/control/native_expression_action.h"
#include "gui/shape/abstract_gshape.h"
#include "gui/style/abstract_style.h"
#include "gui/style/style.h"
#include "gui/transformation/transformations.h"
#endif

using namespace djnnstl;

namespace djnn {
using djnnstl::cerr;
using djnnstl::endl;

static bool __module_initialized = false;

void
init_utils ()
{

    if (__module_initialized == false) {

        __module_initialized = true;

        djnn::loadedModules.push_back ("utils");
    }
}

void
run_stats (CoreProcess* p)
{
    int i = 0;
    depth_first_traverse (p, [&i] (CoreProcess* p) { ++i; });
    cerr << endl
         << "count items: " << i << endl;

    map<string, int> num_by_type;
    int              num_no_coupling            = 0;
    int              num_one_coupling           = 0;
    int              num_more_than_one_coupling = 0;
    int              size                       = 0;
    depth_first_traverse (p, [&] (CoreProcess* p) -> void {
        ++num_by_type[cpp_demangle (typeid (*p).name ())];
        num_no_coupling += !p->has_coupling ();
        num_one_coupling += p->get_activation_couplings ().size () + p->get_deactivation_couplings ().size () == 1 ? 1 : 0;
        num_more_than_one_coupling += p->get_activation_couplings ().size () + p->get_deactivation_couplings ().size () > 1 ? 1 : 0;
        size += sizeof (*p);
    });
    for (auto& item : num_by_type) {
        cerr << "type: " << item.first << " count: " << item.second << endl;
    }
    cerr << "num_no_coupling: " << num_no_coupling << " - " << (num_no_coupling * 100) / i << "%" << endl;
    cerr << "num_one_couplings: " << num_one_coupling << " - " << (num_one_coupling * 100) / i << "%" << endl;
    cerr << "num_more_than_one_couplings: " << num_more_than_one_coupling << " - " << (num_more_than_one_coupling * 100) / i << "%" << endl;
    cerr << "total mem size (*p): " << size << endl
         << endl;
}

#if _DEBUG_SEE_CREATION_DESTRUCTION_ORDER

extern list<pair<CoreProcess*, long int>>              __dbg_creation_stat_order;
extern vector<string>                                  __dbg_destruction_stat_order;
extern map<const CoreProcess*, std::pair<string, int>> parentless_names;

// Thread safety for destruction stats
std::mutex __dbg_destruction_mutex;
// Prevent unbounded memory growth
const size_t MAX_DEBUG_ENTRIES = 10000;

static long int last_creation_id       = -1;
static size_t   last_destruction_index = 0;

void
display_creation_stats ()
{
    int size = __dbg_creation_stat_order.size ();

    map<string, int> num_by_type;
    int              num_no_coupling            = 0;
    int              num_one_coupling           = 0;
    int              num_more_than_one_coupling = 0;
    int              num_total_coupling         = 0;
    int              num_no_children            = 0;
    int              num_properties             = 0;
    int              num_bindings               = 0;
    int              num_connectors             = 0;
    int              num_math_expr              = 0;
    int              num_FSM                    = 0;
    int              num_FSMState               = 0;
    int              num_FSMTransition          = 0;
    int              num_switches               = 0;
    int              num_switchState            = 0;
    int              num_gshapes                = 0;
    int              num_gstype                 = 0;
    int              num_gtransform             = 0;

    // --- CREATION STATS ---
    bool has_new_creations = false;
    int  new_creations     = 0;

    for (const auto& pair : __dbg_creation_stat_order) {
        long int current_id = pair.second;
        if (current_id > last_creation_id) {
            has_new_creations = true;
            new_creations++;
            auto* p = pair.first;
            if (!p)
                continue;

            cerr << "\033[1;34m"; // Blue color for new creations
            cerr << "[" << current_id << "] \t- "
                 << "[" << p << "] - \t"
                 << cpp_demangle (typeid (*p).name ())
                 << " - "
                 << (p->get_parent () ? p->get_parent ()->get_name () : "")
                 << "/" << p->get_debug_name ()
                 << "\033[0m" << endl;
        }
    }

    if (!has_new_creations) {
        new_creations = 0;
    } else {
        if (!__dbg_creation_stat_order.empty ()) {
            last_creation_id = __dbg_creation_stat_order.back ().second;
        }
    }

    // --- DESTRUCTION STATS ---
    {
        std::lock_guard<std::mutex> lock (__dbg_destruction_mutex);
        int                         new_destructions = 0;

        if (last_destruction_index < __dbg_destruction_stat_order.size ()) {
            new_destructions = __dbg_destruction_stat_order.size () - last_destruction_index;
        }

        if (__dbg_destruction_stat_order.size () > MAX_DEBUG_ENTRIES) {
            size_t erase_count = __dbg_destruction_stat_order.size () - MAX_DEBUG_ENTRIES / 2;
            __dbg_destruction_stat_order.erase (
                __dbg_destruction_stat_order.begin (),
                __dbg_destruction_stat_order.begin () + erase_count);
            last_destruction_index = 0;
            new_destructions       = __dbg_destruction_stat_order.size ();
        }

        if (last_destruction_index < __dbg_destruction_stat_order.size ()) {
            for (size_t i = last_destruction_index; i < __dbg_destruction_stat_order.size (); ++i) {
                const auto& s = __dbg_destruction_stat_order[i];
                if (s.empty () || s.data () == nullptr) {
                    cerr << "\033[1;31m[INVALID STRING]\033[0m" << endl;
                    continue;
                }
                cerr << "\033[1;31m" << s << "\033[0m" << endl; // Red color for destructions
            }
            last_destruction_index = __dbg_destruction_stat_order.size ();
        } else {
            new_destructions = 0;
        }

        // Display new counters
        cerr << "\033[1;38;5;208m"; // Orange color
        cerr << "[NEW] creation = " << new_creations << endl;
        cerr << "[NEW] destruction = " << new_destructions << "\033[0m" << endl;
    }

    // OPTIONAL --- PARENTLESS NAMES ---
    // cerr << "\033[1;35m" << endl;
    // cerr << "-- Parentless_name map - " << parentless_names.size() << " -- " << endl;
    // if (!parentless_names.empty()) {
    //     cerr << "Warning - parentless_names is not EMPTY!!" << endl;
    //     for (const auto& [key, pair_value] : parentless_names) {
    //         if (!key)
    //             continue; // Skip null pointers
    //         cerr << '[' << key << "] = "
    //              << pair_value.second << " - \""
    //              << pair_value.first << "\"\n";
    //     }
    // } else {
    //     cerr << "OK - Parentless_Name is EMPTY";
    // }
    // cerr << "\033[0m" << endl;

    // --- STATISTICS CALCULATION (unchanged but safer) ---
    for (auto pair : __dbg_creation_stat_order) {
        auto* p = pair.first;
        if (!p)
            continue; // Skip null pointers

        try {
            // Type counting
            string type_name = cpp_demangle (typeid (*p).name ());
            num_by_type[type_name]++;

            // Coupling stats
            int coupling_count = 0;
            try {
                coupling_count = p->get_activation_couplings ().size () +
                                 p->get_deactivation_couplings ().size ();
                if (coupling_count == 0)
                    num_no_coupling++;
                else if (coupling_count == 1)
                    num_one_coupling++;
                else
                    num_more_than_one_coupling++;
                num_total_coupling += coupling_count;
            } catch (...) {
            }

            // Children count
            try {
                num_no_children += p->children_size () > 0 ? 0 : 1;
            } catch (...) {
            }

            // Category counting with null checks
            try {
                num_properties += dynamic_cast<AbstractProperty*> (p) ? 1 : 0;
            } catch (...) {
            }
            try {
                num_bindings += dynamic_cast<Binding*> (p) ? 1 : 0;
            } catch (...) {
            }
            try {
                num_connectors += dynamic_cast<Connector*> (p) ? 1 : 0;
            } catch (...) {
            }
            try {
                num_math_expr += dynamic_cast<UnaryOperatorCommon*> (p) ? 1 : 0;
            } catch (...) {
            }
            try {
                num_math_expr += dynamic_cast<NativeExpressionAction*> (p) ? 1 : 0;
            } catch (...) {
            }
            try {
                num_FSM += dynamic_cast<FSM*> (p) ? 1 : 0;
            } catch (...) {
            }
            try {
                num_FSMState += dynamic_cast<FSMState*> (p) ? 1 : 0;
            } catch (...) {
            }
            try {
                num_FSMTransition += dynamic_cast<FSMTransition*> (p) ? 1 : 0;
            } catch (...) {
            }
            try {
                num_switches += dynamic_cast<Switch*> (p) ? 1 : 0;
            } catch (...) {
            }

            // Special case for switch states
            try {
                if (auto container = dynamic_cast<Container*> (p)) {
                    if (container->get_parent () && dynamic_cast<Switch*> (container->get_parent ())) {
                        num_switchState++;
                    }
                }
            } catch (...) {
            }

            try {
                num_gshapes += dynamic_cast<AbstractGShape*> (p) ? 1 : 0;
            } catch (...) {
            }
            try {
                num_gstype += dynamic_cast<AbstractStyle*> (p) ? 1 : 0;
            } catch (...) {
            }
            try {
                num_gtransform += dynamic_cast<AbstractTransformation*> (p) ? 1 : 0;
            } catch (...) {
            }
        } catch (...) {
            // Skip invalid process entries
            continue;
        }
    }

    // --- STATISTICS OUTPUT (sorted by count, descending) ---
    cerr << "\033[1;32m" << endl; // Green color for summary
    cerr << "-- ALL TYPE SUMMARY (sorted by count, descending) -- " << endl;

    // Copy elements to a vector for sorting
    std::vector<std::pair<std::string, int>> sorted_types (num_by_type.begin (), num_by_type.end ());

    // Sort by count in descending order
    std::sort (
        sorted_types.begin (),
        sorted_types.end (),
        [] (const auto& a, const auto& b) {
            return a.second > b.second; // Descending sort
        });

    // Find the maximum type name length for alignment
    size_t max_type_length = 0;
    for (const auto& item : sorted_types) {
        if (item.first.length () > max_type_length) {
            max_type_length = item.first.length ();
        }
    }

    // Display sorted with format "count: XX   type: TypeName"
    for (const auto& item : sorted_types) {
        cerr << "count: " << std::setw (4) << std::right << item.second << "   "
             << "type: " << std::left << std::setw (max_type_length + 2) << item.first << endl;
    }

    cerr << endl;

    // --- Display global statistics (aligned) ---
    const int label_width = 25; // Fixed width for labels
    cerr << std::left;          // Left-align all labels

    // Processes and children
    cerr << std::setw (label_width) << "# Processes:" << size << endl;
    cerr << std::setw (label_width) << "# Childless processes:" << num_no_children << endl;
    cerr << endl;

    // Object categories
    cerr << std::setw (label_width) << "# Properties:" << num_properties << endl;
    cerr << std::setw (label_width) << "# Bindings:" << num_bindings << endl;
    cerr << std::setw (label_width) << "# Connectors:" << num_connectors << endl;
    cerr << std::setw (label_width) << "# Math/Expressions:" << num_math_expr << endl;
    cerr << std::setw (label_width) << "# FSMs:" << num_FSM << endl;
    cerr << std::setw (label_width) << "# FSM States:" << num_FSMState << endl;
    cerr << std::setw (label_width) << "# FSM Transitions:" << num_FSMTransition << endl;
    cerr << std::setw (label_width) << "# Switches:" << num_switches << endl;
    cerr << std::setw (label_width) << "# Switch States:" << num_switchState << endl;
    cerr << std::setw (label_width) << "# Shapes:" << num_gshapes << endl;
    cerr << std::setw (label_width) << "# Styles:" << num_gstype << endl;
    cerr << std::setw (label_width) << "# Transformations:" << num_gtransform << endl;
    cerr << endl;

    // Couplings (with percentages)
    size          = __dbg_creation_stat_order.size ();
    int safe_size = (size > 0) ? size : 1; // Avoid division by zero

    cerr << std::setw (label_width) << "# Total couplings:" << num_total_coupling << endl;
    cerr << std::setw (label_width) << "# No couplings:" << num_no_coupling << " \t- "
         << std::setw (3) << (num_no_coupling * 100) / safe_size << "%" << endl;
    cerr << std::setw (label_width) << "# Single coupling:" << num_one_coupling << " \t- "
         << std::setw (3) << (num_one_coupling * 100) / safe_size << "%" << endl;
    cerr << std::setw (label_width) << "# Multiple couplings:" << num_more_than_one_coupling << " \t- "
         << std::setw (3) << (num_more_than_one_coupling * 100) / safe_size << "%" << endl;

    cerr << "\033[0m" << endl;
}

#else
void
display_creation_stats ()
{
    cerr << "WARNING - this is display_creation_stats stub - you maybe forget to activate _DEBUG_SEE_CREATION_DESTRUCTION_ORDER in process.h" << endl;
}
#endif //_DEBUG_SEE_CREATION_DESTRUCTION_ORDER
} // namespace djnn
