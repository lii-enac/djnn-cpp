#include "main_loop.h"

namespace djnn {

    // MainLoop should be created *before* any other external-source (is activated ?) -- or not ?
    MainLoop::MainLoop ()
    {
      set_run_for_ever ();
      djnn::get_exclusive_access (DBG_GET); // get hand to prevent any other thread from launching
    }


// Process
    void
    MainLoop::activate ()
    {
      if (another_source_wants_to_be_mainloop) {
        //djnn::get_exclusive_access (DBG_GET);
        run_in_own_thread ();
        //another_source_wants_to_be_mainloop->activate_from_mainloop ();
        another_source_wants_to_be_mainloop->private_run ();
      } else {
        run_in_main_thread ();
      }
    }

    void
    MainLoop::deactivate ()
    {
      if (another_source_wants_to_be_mainloop) {
        another_source_wants_to_be_mainloop->please_stop ();
      } else {
        own_mutex.unlock ();
      }
    }

    void
    MainLoop::run_in_main_thread ()
    {
      private_run ();
    }

    void
    MainLoop::run_in_own_thread ()
    {
      new std::thread (&MainLoop::run, this);
    }

    void
    MainLoop::run ()
    {
      //private_run();
      djnn::release_exclusive_access (DBG_REL); // launch other threads

      if (is_run_forever ()) {
        own_mutex.lock (); // 1st lock: success
        own_mutex.lock (); // 2nd lock: blocks forever
      } else {
        std::this_thread::sleep_for (_duration);
      }
      if (another_source_wants_to_be_mainloop)
        another_source_wants_to_be_mainloop->please_stop ();
    }


}
