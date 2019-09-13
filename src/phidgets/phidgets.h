/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once
#include "../core/ontology/process.h"

namespace djnn {

	extern std::vector<string> loadedModules;
	extern Process *Phidgets;
	extern Process *InterfaceKits;
 	
 	void init_phidgets ();
  
}