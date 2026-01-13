#pragma once

#if !defined(DJNN_NO_DEBUG) || !defined(DJNN_NO_SERIALIZE)
#include "core/utils/iostream.h"
#include "core/core-dev.h"

//#define DBG_ACT_SEQ if (_DEBUG_SEE_ACTIVATION_SEQUENCE) cerr << -1 << " - triggers ------ " << print_process_full_name (this) << " value:" << get_value() << "\t\t" << print_process_debug_info (this) << endl;
//#define DBG_ACT_SEQ if (_DEBUG_SEE_ACTIVATION_SEQUENCE) cerr << print_process_debug_info (this) << "\t\t\t --- " << print_process_full_name (this) << " set value to: °" << get_value() << "°" << endl;
#define DBG_ACT_SEQ if (_DEBUG_SEE_ACTIVATION_SEQUENCE) cerr << "\t\t\t --- " << print_process_full_name (this) << " set value to: °" << get_value() << "°" << endl;

#else
#define DBG_ACT_SEQ
#endif
