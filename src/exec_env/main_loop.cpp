/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "core/utils/algorithm.h"
#include "main_loop.h"
#include "cpp-thread.h"

#if DJNN_USE_QT_MAINLOOP
#include "qt/qt_mainloop.h"
#endif

#if DJNN_USE_FREERTOS_MAINLOOP
#include "freertos/freertos_mainloop.h"
#endif

#include "exec_env/djnn_time_manager.h"

//#include "core/utils/iostream.h"
//#include "utils/debug.h"
//#include "core/utils/error.h"


namespace djnn {

    static MainLoop * _instance;

    // MainLoop is a singleton, put this field into a static variable, and do not include "djnn-thread.h" in mainloop.h
    #if DJNN_USE_BOOST_THREAD || DJNN_USE_BOOST_FIBER || DJNN_USE_STD_THREAD
    static djnn_thread_t own_thread;
    #endif

    #if DJNN_USE_QT_THREAD
    static QThread* own_thread;
    #endif

    #if DJNN_USE_SDL_THREAD
    static SDL_Thread * own_thread; 
    #endif
    
    MainLoop&
    MainLoop::instance ()
    {
      static std::atomic_flag onceFlag = ATOMIC_FLAG_INIT;
      if(!onceFlag.test_and_set()) {
        _instance = new MainLoop ();
        #if DJNN_USE_QT_MAINLOOP
        QtMainloop::build_instance(_instance);
        #endif
        #if DJNN_USE_FREERTOS_MAINLOOP
        FreeRTOSMainloop::build_instance(_instance);
        #endif
        ExternalSource::init();
        launch_mutex_lock ();
      }
      return *_instance;
    }

    MainLoop::MainLoop ()
    : FatProcess ("Mainloop"),
      ExternalSource ("Mainloop"),
      _another_source_wants_to_be_mainloop (nullptr)
    {
      set_run_for_ever (); // default mode
    }

    MainLoop::~MainLoop ()
    {
      please_stop ();
    }

    void
    MainLoop::impl_activate ()
    {
      DjnnTimeManager::instance().update_ref_now();
      _is_stopping = false;

      for (auto es: _external_sources) {
        //std::cerr << "starting " << es->get_name () << __FL__;
        es->start ();
      }
      for (auto p: _background_processes) {
        p->activate ();
      }
      if (_another_source_wants_to_be_mainloop) {
        run_in_own_thread ();
        launch_mutex_unlock();
        CoreProcess::post_activate ();
        _another_source_wants_to_be_mainloop->run (); // should block
      } else {
        launch_mutex_unlock();
        CoreProcess::post_activate ();
        run_in_main_thread (); // should block
      }
      // here, the mainloop is blocking

      // [blocking........]
      
      // starting from here, the mainloop has exited
      _is_stopping = true;
      launch_mutex_lock ();
      if (_another_source_wants_to_be_mainloop) {
        please_stop ();
        join_own_thread ();
      }

      // auto deactivate
      if(somehow_activating()) {
        deactivate ();
      }
    }

    djnn_atomic<bool>::atomic MainLoop::_is_stopping;

    void
    MainLoop::run ()
    {
      { // scope for mutex
        std::unique_lock<std::mutex> l(cancel_mutex); // first lock, get it
        if (is_run_forever ()) {
          //std::cerr << ">> mainloop entering sleep forever" << __FL__;
          cv.wait (l); // second lock, blocks until another thread calls pleaseStop
          //std::cerr << "<< mainloop leaving sleep forever" << __FL__;
        } else {
          //std::cerr << ">> mainloop entering sleep " << DBGVAR(_duration.count()) << __FL__;
          cv.wait_for (l, std::chrono::milliseconds (_duration)); // second lock, blocks until another thread calls pleaseStop
          //std::cerr << "<< mainloop exited sleep " << __FL__;
        }
      }

      // fist tell all external sources to stop...
      for (auto es: _external_sources) {
        es->please_stop ();
      }
      // ...then join them
      for (auto es: _external_sources) {
        es->join ();
      }

      if (_another_source_wants_to_be_mainloop)
        _another_source_wants_to_be_mainloop->please_stop ();
    }

    void
    MainLoop::impl_deactivate ()
    {
      please_stop ();
    }

    void
    MainLoop::please_stop()
    {
      if(get_please_stop ()) return;
      set_please_stop (true);
      cv.notify_one ();
    }

    void
    MainLoop::private_run () // FIXME: useless?
    {
      run();
    }

    void
    MainLoop::run_in_main_thread ()
    {
      private_run ();
    }


    #if DJNN_USE_SDL_THREAD
    static int SDL_ThreadFunction(void* data)
    {
        auto * ml = (MainLoop*) data;
        ml->private_run ();
        return 0;
    }
    #endif

    void
    MainLoop::run_in_own_thread ()
    {
      #if DJNN_USE_BOOST_THREAD || DJNN_USE_BOOST_FIBER || DJNN_USE_STD_THREAD
      own_thread = djnn_thread_t (&MainLoop::private_run, this);
      #endif

      #if DJNN_USE_QT_THREAD
      own_thread = QThread::create([this]() { this->MainLoop::private_run(); });
      own_thread->start();
      #endif

      #if DJNN_USE_SDL_THREAD
      own_thread = SDL_CreateThread(SDL_ThreadFunction, "djnn thread", this); // FIXME: leak
      #endif
    }

    void
    MainLoop::join_own_thread ()
    {
      #if DJNN_USE_BOOST_THREAD || DJNN_USE_BOOST_FIBER || DJNN_USE_STD_THREAD
      //own_thread.join();
      own_thread.detach();
      #endif

      #if DJNN_USE_QT_THREAD
      if (own_thread) own_thread->wait();
      delete own_thread;
      #endif

      #if DJNN_USE_SDL_THREAD
      SDL_WaitThread(own_thread, nullptr);
      #endif
    }

    void 
    MainLoop::set_another_source_wants_to_be_mainloop (ExternalSource *source)
    {
      _another_source_wants_to_be_mainloop = source;
    }

    void
    MainLoop::add_external_source (ExternalSource* es)
    {
      _external_sources.push_back(es);
    }

    void
    MainLoop::remove_external_source (ExternalSource* es)
    {
       auto i = djnnstl::find(_external_sources.begin (), _external_sources.end (), es);
       if (i != _external_sources.end ())
          _external_sources.erase(i);
    }

    void
    MainLoop::add_background_process (CoreProcess *p)
    {
      _background_processes.push_back (p);
    }

    void
    MainLoop::remove_background_process (CoreProcess *p)
    {
      _background_processes.erase (djnnstl::remove(_background_processes.begin(), _background_processes.end(), p), _background_processes.end());
    }


}
