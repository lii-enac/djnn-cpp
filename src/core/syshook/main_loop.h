/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *      Mahieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "syshook.h"
#include "../tree/process.h"
#include "external_source.h"

#include "cpp-chrono.h"

namespace djnn
{
  class MainLoop : public Process, ExternalSource
  {

  public:
    static MainLoop& instance ();

    void set_run_for_ever () {
      _duration = chrono::milliseconds(-1);
    }

    bool is_run_forever () const {
      return _duration == chrono::milliseconds(-1);
    }
    
    void set_run_for (chrono::milliseconds d) {
      _duration = d;
    }
    void set_run_for (chrono::seconds d) {
      _duration = chrono::duration_cast<chrono::milliseconds>(d);
    }

    void set_another_source_wants_to_be_mainloop (ExternalSource *);
    ExternalSource * _another_source_wants_to_be_mainloop;
    
    void add_background_process (Process *p) { _background_processes.push_back (p); }

    void external_run_for_qt5_9_fixme() { run(); }
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
    chrono::milliseconds _duration;

  private:
    static MainLoop *_instance;
    static std::once_flag onceFlag;
    // MainLoop should be created *before* any other external-source (is activated ?) -- or not ?
    MainLoop ();
    vector<Process*> _background_processes;
  };

}
