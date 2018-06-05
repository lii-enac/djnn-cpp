/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

#include "syshook.h"
#include "../tree/process.h"
#include "external_source.h"

#include <thread>
#include <mutex>
#include <iostream>

#ifdef __MINGW32__
#include <mingw.thread.h>
#include <mingw.mutex.h>
#endif

namespace djnn
{

  class MainLoop : public Process, ExternalSource
  {

  public:
    static MainLoop& instance ();
    void set_run_for_ever () {
      _duration = -1s;
    }

    bool is_run_forever () const {
      return _duration == -1s;
    }
    
    void set_run_for (std::chrono::milliseconds duration) {
      _duration = duration;
    }

    static ExternalSource * another_source_wants_to_be_mainloop;

  protected:

    // Process
    void
    activate ()
    {
      if (another_source_wants_to_be_mainloop) {
        run_in_own_thread ();
        another_source_wants_to_be_mainloop->activate_from_mainloop ();
      } else {
        run_in_main_thread ();
      }
    }

    void
    deactivate ()
    {
      if (another_source_wants_to_be_mainloop) {
        another_source_wants_to_be_mainloop->please_stop ();
      } else {
        own_mutex.unlock ();
      }
    }

    void
    run_in_main_thread ()
    {
      run ();
    }

    void
    run_in_own_thread ()
    {
      new std::thread (&MainLoop::run, this);
    }

    void
    run ()
    {
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

    std::mutex own_mutex;
    std::chrono::milliseconds _duration;

  private:
    static std::shared_ptr<MainLoop> _instance;
    static std::once_flag onceFlag;
    // MainLoop should be created *before* any other external-source (is activated ?) -- or not ?
    MainLoop ()
    {
      set_run_for_ever ();
      djnn::get_exclusive_access (DBG_GET); // get hand to prevent any other thread from launching
    }
  };

}
