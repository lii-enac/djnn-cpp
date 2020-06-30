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

#include "arithmetic.h"
#include "trigonometry.h"
#include "geometry.h"
#include "math_functions.h"
#include "clock.h"
#include "connector.h"
#include "fsm.h"
#include "switch.h"
#include "text.h"
#include "logic.h"
#include "log.h"
#include "counter.h"
#include "finder.h"
#include "algorithm.h"
#include "switch_list.h"
#include "switch_range.h"
#include "dictionary.h"
#include "deref.h"
#include "native_async_action.h"
#include "process_handler.h"

namespace djnn {

	extern std::vector<std::string> loadedModules; 

	void init_base ();

    using Connector = Connector;
    using PausedConnector = SimplePausedConnector;

}
