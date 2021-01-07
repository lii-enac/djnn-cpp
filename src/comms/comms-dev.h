#pragma once

namespace djnn {

	extern std::vector<std::string> loadedModules; 

	void init_comms ();
	void clear_comms ();

}
