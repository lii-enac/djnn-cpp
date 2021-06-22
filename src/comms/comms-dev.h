#pragma once

namespace djnn {

	extern vector<std::string> loadedModules; 

	void init_comms ();
	void clear_comms ();

}
