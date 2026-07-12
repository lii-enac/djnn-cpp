/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2023-2024)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
#pragma once

namespace djnn {
enum property_type_e {
    Boolean,
    Integer,
    Double,
    String,
    Reference,
    Remote,
    UserDefined
};

class CoreProcess;

CoreProcess* getRef (CoreProcess* p);
void         setRef (CoreProcess* p, CoreProcess* v);
void         activate (CoreProcess* p);
void         deactivate (CoreProcess* p);
void*        get_native_user_data (CoreProcess* native_action);
void         graph_exec ();
#define DBG_GRAPH_EXE
#define GRAPH_EXEC           \
    {                        \
        DBG_GRAPH_EXE;       \
        djnn::graph_exec (); \
    }
} // namespace djnn
