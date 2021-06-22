#pragma once

#include <vector>

namespace djnn {

	extern vector<djnn::string> loadedModules; 

	void init_base ();
    void clear_base ();
}
