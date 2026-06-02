#pragma once

#if !defined(DJNN_NO_DEBUG)
#include <iomanip>
#include "core/utils/iostream.h"
#include "core/utils/utils-dev.h"

namespace djnn {
djnnstl::string print_process_debug_info (const CoreProcess* p);
}

#endif

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

extern void debug_activation_sequence_2(CoreProcess *p);

INLINE
void
CoreProcess::activate ()
{
    if (pre_activate ()) {
        debug_activation_sequence_2 (this);
        impl_activate ();
        post_activate ();
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
     * 3 - is deactivating
     */

    if (get_activation_state () != DEACTIVATED) {
        return false;
    }
    set_activation_state (ACTIVATING);
    return true;
}

INLINE
bool
ChildProcess::parent_somehow_deactivating () const
{
    if (auto * p = get_parent (); p)
        return p->somehow_deactivating ();
    else
        return false;
}

INLINE
bool
FatProcess::pre_activate ()
{
    /* no activation if :
     * 4 - the parent exists and is stopped
     */
    if (parent_somehow_deactivating ()) return false;

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