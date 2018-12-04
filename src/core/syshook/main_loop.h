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
    void add_background_process (Process *p) { _background_processes.push_back (p); }
  protected:

    // Process
    void
    activate ();

    void
    deactivate ();

    void
    run_in_main_thread ();

    void
    run_in_own_thread ();

    void
    run ();

    std::mutex own_mutex;
    std::chrono::milliseconds _duration;

  private:
    static MainLoop *_instance;
    static std::once_flag onceFlag;
    // MainLoop should be created *before* any other external-source (is activated ?) -- or not ?
    MainLoop ();
    vector<Process*> _background_processes;
  };

}
