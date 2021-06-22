#pragma once

namespace djnn {

	extern vector<djnn::string> loadedModules; 

	void init_comms ();
	void clear_comms ();

}
