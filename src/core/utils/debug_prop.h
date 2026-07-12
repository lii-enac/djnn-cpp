/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2026)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
#pragma once

#if !defined(DJNN_NO_DEBUG) || !defined(DJNN_NO_SERIALIZE)
#include "core/utils/iostream.h"
#include "core/core-dev.h"
#endif

namespace djnn {
extern bool _OUTSIDE_GRAPH_EXE;
}

#if !defined(DJNN_NO_DEBUG)
//#define DBG_ACT_SEQ if (_DEBUG_SEE_PROP_SET_VALUE) cerr << -1 << " - triggers ------ " << print_process_full_name (this) << " value:" << get_value() << "\t\t" << print_process_debug_info (this) << endl;
//#define DBG_ACT_SEQ if (_DEBUG_SEE_PROP_SET_VALUE) cerr << print_process_debug_info (this) << "\t\t\t --- " << print_process_full_name (this) << " set value to: °" << get_value() << "°" << endl;
//#define DBG_ACT_SEQ if (_DEBUG_SEE_PROP_SET_VALUE) cerr << "\t\t\t --- " << print_process_full_name (this) << " set value from: °" << v << "° to: °" << get_value() << "°" << " propagate:" << propagate << endl;
#define DBG_ACT_SEQ if (_DEBUG_SEE_PROP_SET_VALUE) { debug_prop_set_value (this, _OUTSIDE_GRAPH_EXE); djnnstl::cerr << "set value from: °" << get_value() << "° to: °" << v << "°" << " propagate:" << propagate << djnnstl::endl; }
#else
#define DBG_ACT_SEQ
#endif
