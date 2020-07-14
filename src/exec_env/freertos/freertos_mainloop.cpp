#include "exec_env/freertos/freertos_mainloop.h"
#include "exec_env/main_loop.h"
#include "exec_env/global_mutex.h"


extern "C" {
	// specific to crazyflie?
	void workerBlock();
	void workerHandleOneEvent();
}

namespace djnn {

	FreeRTOSMainloop* FreeRTOSMainloop::_instance;

	void
	FreeRTOSMainloop::build_instance (MainLoop * ml)
	{
		_instance = new FreeRTOSMainloop (ml);
	}

	FreeRTOSMainloop&
	FreeRTOSMainloop::instance ()
	{
		return *(_instance);
	}

	FreeRTOSMainloop::FreeRTOSMainloop (MainLoop * ml) :
	ExternalSource ("FreeRTOS"),
      _please_exec (false), already_awaken(false)
	{
		(*ml).set_another_source_wants_to_be_mainloop (this);
	}

	void
	FreeRTOSMainloop::please_stop ()
	{
	}

	void
	FreeRTOSMainloop::run ()
	{
		while (true) {
			workerBlock ();
			djnn::get_exclusive_access (DBG_GET);
			workerHandleOneEvent ();
		  	GRAPH_EXEC;
		  	djnn::release_exclusive_access (DBG_REL);
		}
	}

	void
	FreeRTOSMainloop::wakeup ()
	{ //DBG;
	}
}

