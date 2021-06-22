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

namespace djnn {
    class CoreProcess;
    using Process = CoreProcess;

    void init_core ();
    void clear_core ();
    void graph_add_edge (CoreProcess* src, CoreProcess* dst);
    void graph_remove_edge (CoreProcess* src, CoreProcess* dst);
    void graph_check_order (CoreProcess* p1, CoreProcess* p2);
    void graph_exec ();
}

#if !defined(DJNN_NO_DEBUG) || !defined(DJNN_NO_SERIALIZE)
#include "core/utils/iostream.h"
#endif
//#define DBGG std::cerr << "'" << __FUNCTION__ << " calling graph exec " << __FILE__ << ":" << __LINE__ << std::endl;
#define DBG_GRAPH_EXE
#define GRAPH_EXEC { DBG_GRAPH_EXE; djnn::graph_exec (); }
