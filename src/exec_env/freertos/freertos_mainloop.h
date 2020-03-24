#pragma once

#include "exec_env/external_source.h"
#include "core/execution/graph.h"

namespace djnn {
	class MainLoop;

	class FreeRTOSMainloop : public ExternalSource {
  
	 public:
	    static void build_instance (MainLoop*);
	    static FreeRTOSMainloop& instance ();
	    static void clear ();
	    virtual void please_stop () override ;
	    void run() override;
	    void wakeup ();
	    void set_please_exec (bool exec) { _please_exec = exec; }
	    //void add_listener (QtMainloopListener* mll) { _mlls.push_back (mll); }
	    //void remove_listener (QtMainloopListener* mll) { _mlls.erase (remove (_mlls.begin (), _mlls.end (), mll), _mlls.end ()); }
	  
	  private:
	    static FreeRTOSMainloop* _instance;
	    FreeRTOSMainloop (MainLoop*);
	    bool _please_exec;
	    //void slot_for_awake ();
	    //void slot_for_about_to_block ();
	    //std::vector<QtMainloopListener*> _mlls;
	    bool already_awaken;
	    //int argc;
	    //char **argv;
	  };
}