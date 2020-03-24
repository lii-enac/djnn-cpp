#include "exec_env/freertos/freertos_mainloop.h"
#include "exec_env/main_loop.h"
#include "exec_env/global_mutex.h"

#include <mutex>

extern "C" {
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
	//	_qapp->exec ();
		while(true) {
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

	/*void
	FreeRTOSMainloopListener::slot_for_about_to_block () {}

	void
	FreeRTOSMainloop::slot_for_about_to_block ()
	{
		//DBG;
		if (_please_exec) {
		  //DBG;
		  djnn::get_exclusive_access (DBG_GET);
		  GRAPH_EXEC;
		  djnn::release_exclusive_access (DBG_REL);
		  _please_exec = false;
		}

		for (auto mll : _mlls) mll->slot_for_about_to_block();

		already_awaken = false;
	}*/

	void
	FreeRTOSMainloop::slot_for_awake ()
	{
		if(already_awaken) {
		  return;
		}

		if (!get_please_stop ()) {
		  // now qt can call event method on windows
		  already_awaken = true;
		} else {
		  //_qapp->quit ();
		}
	}
}