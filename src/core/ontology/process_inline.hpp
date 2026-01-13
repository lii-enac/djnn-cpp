#pragma once

#include <iomanip>

namespace djnn {

// -----------------------------------------------------------------------
// behavior: main activation API


#if defined(DJNN_NO_DEBUG)


INLINE
void
CoreProcess::activate ()
{
    if (pre_activate ()) {
        impl_activate ();
        post_activate ();
    }
}


#else

INLINE
void
CoreProcess::activate ()
{
    bool do_debug_log = _DEBUG_SEE_ACTIVATION_SEQUENCE_2 && debug_info ().lineno != 0;
    if (pre_activate ()) {
        extern int debug_index, debug_index_post;
        //int sav_debug_index = debug_index;
        if (do_debug_log) {
            auto const& src_code = print_process_debug_info (this);
            auto const & hn = get_hierarchy_name (this);
            if (!hn.empty()) {
                const int ms = sizeof("move_x") - 1;
                //std::cerr << hn << " "<< hn.size() << " " << ms << " " << hn.substr(0, ms-2) << std::endl;
                if (hn.size()==ms) {
                    if (hn.substr(0, ms-2) == "move")
                        do_debug_log = false;
                }
            }
            if (do_debug_log) {
            auto const & tn = cpp_demangle (typeid (*this).name ());
            size_t w = 50;
            std::cerr 
                << std::left
                << std::setw(60) << src_code.substr(0,60)
                << std::setw(15) << " (" + std::to_string(debug_index) + "," + std::to_string(debug_index_post) + ")" " act "
                << std::setw(w) << hn.substr(0, w-3) + (hn.size()>(w-3)?"...":" ")
                << std::setw(w) << tn.substr(0, w-3) + (tn.size()>(w-3)?"...":"")
                << __FL__;
                //<< std::endl;
            ++debug_index;
            }
        }
        impl_activate ();
        post_activate ();
        if (do_debug_log) {
            //std::cerr << print_process_debug_info (this) << " (was " << sav_debug_index << ")" << std::endl;
            ++debug_index_post;
        }
    }
}
#endif


INLINE
void
CoreProcess::deactivate ()
{
    if (pre_deactivate ()) {
        impl_deactivate ();
        post_deactivate ();
    }
}

INLINE
bool
CoreProcess::pre_activate ()
{
    /* no activation if :
     * 1 - already activated
     * 2 - is activating
     */

    if (get_activation_state () != DEACTIVATED) {
        return false;
    }
    set_activation_state (ACTIVATING);
    return true;
}

INLINE
bool
FatProcess::pre_activate ()
{
    /* no activation if :
     * 3 - the parent exists and is stopped
     */
    if (CoreProcess * p = get_parent (); p && !p->somehow_activating ()) {
        return false;
    }

    // no need to check all of the activations in the hierarchy of parents,
    // since any activation/deactivation should have been up/downstreamed (?)
    
    return CoreProcess::pre_activate ();
}

INLINE
void
CoreProcess::post_activate ()
{
    notify_activation ();
    set_activation_state (ACTIVATED);
}

INLINE
void
CoreProcess::post_activate_auto_deactivate ()
{
    // TODO: inline function in process.h
    CoreProcess::post_activate ();
    deactivate ();
}

INLINE
bool
CoreProcess::pre_deactivate ()
{
    if (get_activation_state () != ACTIVATED) {
        return false;
    }
    set_activation_state (DEACTIVATING);
    return true;
}

INLINE
void
CoreProcess::post_deactivate ()
{
    notify_deactivation ();
    set_activation_state (DEACTIVATED);
    // set_activation_flag (NONE_ACTIVATION); // handled in Graph::exec ()
}

void graph_schedule_activation(CoreProcess*);
void graph_schedule_deletion(CoreProcess*);

void graph_add_in_activation(Vertex*);

INLINE
void
CoreProcess::schedule_activation ()
{
    graph_schedule_activation (this);
}

INLINE
void
CoreProcess::schedule_deletion ()
{
    graph_schedule_deletion (this);
}

extern
void
notify (const CoreProcess::couplings_t& couplings);

INLINE
void
CoreProcess::notify_activation ()
{
    notify (get_activation_couplings ());
}

INLINE
void
CoreProcess::notify_deactivation ()
{
    notify (get_deactivation_couplings ());
}

INLINE
void
CoreProcess::set_activation_flag (activation_flag_e VALUE)
{
    // only if flags are different
    if (static_cast<activation_flag_e> (get_bitset (ACTIVATION_FLAG_MASK, ACTIVATION_FLAG_SHIFT)) != VALUE) {
#if !_EXEC_FULL_ORDERED_VERTICES
        // if the process has a vertex and has something to do // could be simplified?
        if (_vertex && (VALUE != NONE_ACTIVATION)) {
            graph_add_in_activation (_vertex);
        }
#endif
        set_bitset (ACTIVATION_FLAG_MASK, ACTIVATION_FLAG_SHIFT, static_cast<unsigned int> (VALUE));
    }
}

// pseudo, graph-less coupling for efficiency reasons in gui
INLINE
void
FatProcess::notify_change (unsigned int notify_mask_)
{
    if (auto* p = get_parent ())
        p->notify_change (notify_mask_);
}

}