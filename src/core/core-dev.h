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
 *
 */
#pragma once

#include "core/enums.h"
#include "core/xml/xml-dev.h"

namespace djnn {
class CoreProcess;
using Process = CoreProcess;

void init_core ();
void clear_core ();
void graph_add_edge (CoreProcess* src, CoreProcess* dst);
void graph_remove_edge (CoreProcess* src, CoreProcess* dst);
void graph_check_order (CoreProcess* p1, CoreProcess* p2);
void graph_exec ();
} // namespace djnn

// #define DBG_GRAPH_EXE
// #define GRAPH_EXEC { DBG_GRAPH_EXE; djnn::graph_exec (); }

namespace djnn {
#ifndef DJNN_NO_DEBUG // still required to link smala programs
                      // DEBUG OPTIONS
extern int
    _DEBUG_SEE_COLOR_PICKING_VIEW,
    _DEBUG_GRAPH_CYCLE_DETECT,
    _DEBUG_SEE_ACTIVATION_SEQUENCE,
    _DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_TIME_US,
    _DEBUG_SEE_ACTIVATION_SEQUENCE_ONLY_TARGETED,
    _AUTHORIZE_CYCLE,
    _DEBUG_SEE_RECOMPUTE_PIXMAP_AND_PAINTEVENT,
    _DEBUG_SEE_RECOMPUTE_PIXMAP_ONLY,
    _DEBUG_SEE_COMPONENTS_DESTRUCTION_INFO_LEVEL,
    _ENABLE_TOUCHES,
    _DEBUG_ENABLE_STRESS_TEST;

extern const char* _DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_LOCATION;
#endif
} // namespace djnn
