#pragma once

#include <vector>

namespace djnn {

	extern vector<string> loadedModules; 

	void init_base ();
    void clear_base ();
}
