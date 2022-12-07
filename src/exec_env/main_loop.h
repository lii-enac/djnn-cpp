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
#include "external_source.h"
#include "cpp-chrono.h"

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <algorithm>

namespace djnn
{
    // temporyary hack
    template<typename T>
    struct djnn_atomic {
        #if DJNN_USE_FREERTOS
        using atomic=T;
        #else
        using atomic=std::atomic<T>;
        #endif
    };

  class MainLoop : public FatProcess, ExternalSource
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
    
    void add_background_process (FatProcess *p) { _background_processes.push_back (p); }
    void remove_background_process (FatProcess *p) {  _background_processes.erase (std::remove(_background_processes.begin(), _background_processes.end(), p), _background_processes.end()); }

    void external_run_for_qt5_9_fixme() { run(); }
    void private_run ();
    void please_stop () override;
    bool is_stopping () const { return _is_stopping; }
  protected:

    // FatProcess
    void impl_activate () override;
    void impl_deactivate () override;

    void post_activate () override {};
    void run_in_main_thread ();
    void run_in_own_thread ();
    void join_own_thread ();
    void run () override;

    djnn_atomic<chrono::milliseconds>::atomic _duration;

    // FIXME: hack to reactivate mainloop : used only for unit_test
    //        djnn application only has one mainloop.
    bool pre_activate () override { return true; }

  private:
    // MainLoop should be created *before* any other external-source
    MainLoop ();
    vector<FatProcess*> _background_processes;
    vector<ExternalSource*> _external_sources;

    std::mutex cancel_mutex;
    // we need a condition variable, a mutex is not enough, see https://stackoverflow.com/questions/12551341/when-is-a-condition-variable-needed-isnt-a-mutex-enough
    // The mutex must be locked by the current thread of execution, otherwise, the behavior is undefined. https://en.cppreference.com/w/cpp/thread/timed_mutex/unlock
    // If lock is called by a thread that already owns the mutex, the behavior is undefined: for example, the program may deadlock. https://en.cppreference.com/w/cpp/thread/timed_mutex/lock
    //std::condition_variable_any cv;
    std::condition_variable cv;

    static djnn_atomic<bool>::atomic _is_stopping; // for external sources that can't be stopped easily eg Ivy
  };

}
