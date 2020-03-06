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

#include "core/ontology/process.h"
#include "syshook.h"
#include "external_source.h"
#include "cpp-thread.h"
#include <mutex>
//#include "cpp-chrono.h"
//#include "cpp-mutex.h"

#include <atomic>

namespace djnn
{
  class MainLoop : public Process, ExternalSource
  {

  public:
    ~MainLoop ();
    static MainLoop& instance ();

    void set_run_for_ever () {
      _duration = chrono::milliseconds(-1);
    }

    bool is_run_forever () const {
      return chrono::milliseconds(_duration) == chrono::milliseconds(-1);
    }
    
    void set_run_for (int milliseconds) {
      _duration = chrono::milliseconds(milliseconds);
    }

    void set_run_for (std::chrono::milliseconds d) {
      _duration = chrono::milliseconds(d.count());
    }

    void set_run_for (std::chrono::seconds d) {
      _duration = chrono::seconds(d.count());
    }

#if DJNN_USE_BOOST_CHRONO
    void set_run_for (boost::chrono::milliseconds d) {
      _duration = d;
    }

    void set_run_for (boost::chrono::seconds d) {
      _duration = boost::chrono::duration_cast<boost::chrono::milliseconds>(d);
    }
#endif

    void set_another_source_wants_to_be_mainloop (ExternalSource *);
    ExternalSource * _another_source_wants_to_be_mainloop;

    void add_external_source (ExternalSource*);
    void remove_external_source (ExternalSource*);
    
    void add_background_process (Process *p) { _background_processes.push_back (p); }

    void external_run_for_qt5_9_fixme() { run(); }
    void private_run ();
    void please_stop () override;
    bool is_stopping () const { return _is_stopping; }
  protected:

    // Process
    void impl_activate () override;
    void impl_deactivate () override;

    void run_in_main_thread ();
    void run_in_own_thread ();
    void join_own_thread ();
    void run () override;

    std::atomic<chrono::milliseconds> _duration;

    // FIXME: hack to reactivate mainloop : used only for unit_test
    //        djnn application only has one mainloop.
    bool pre_activate () override { return true; }

  private:
    static MainLoop _instance;

    #if DJNN_USE_BOOST_THREAD || DJNN_USE_BOOST_FIBER || DJNN_USE_STD_THREAD
    djnn_thread_t own_thread;
    #endif

    #if DJNN_USE_QT_THREAD
    QThread* own_thread;
    #endif

    #if DJNN_USE_SDL_THREAD
    SDL_Thread * own_thread; 
    #endif


    // MainLoop should be created *before* any other external-source
    MainLoop ();
    vector<Process*> _background_processes;
    vector<ExternalSource*> _external_sources;
    std::timed_mutex cancel_mutex;
    static std::atomic<bool> _is_stopping; // for external sources that can't be stopped easily eg Ivy 
  };

}
