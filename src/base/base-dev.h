#pragma once

#include <vector>

namespace djnn {

	extern vector<std::string> loadedModules; 

	void init_base ();
    void clear_base ();
}
