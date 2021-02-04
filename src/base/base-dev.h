#pragma once

#include <vector>

namespace djnn {

	extern std::vector<std::string> loadedModules; 

	void init_base ();
    void clear_base ();
}
