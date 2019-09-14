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
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "main_loop.h"
#include "cpp-thread.h"

#if DJNN_USE_QT_MAINLOOP
#include "qt/qt_mainloop.h"
#endif

#include <iostream>
#define DBG std::cerr << __FUNCTION__ << " " << __FILE__ << ":" << __LINE__ << std::endl;

namespace djnn {

    MainLoop* MainLoop::_instance;
    std::once_flag MainLoop::onceFlag;
    MainLoop&
    MainLoop::instance ()
    {
      std::call_once (MainLoop::onceFlag, [] () {
        init_global_mutex();
        ExternalSource::init();
        _instance = new MainLoop();
        #if DJNN_USE_QT_MAINLOOP
        QtMainloop::build_instance(_instance);
        #endif
      });

      return *(_instance);
    }

    MainLoop::MainLoop ()
    : _another_source_wants_to_be_mainloop (nullptr)
    {
      set_run_for_ever ();
      launch_mutex_lock();
    }

    void
    MainLoop::impl_activate ()
    {
      for (auto p: _background_processes) {
        p->activate ();
      }
      if (_another_source_wants_to_be_mainloop) {
        run_in_own_thread ();
        set_activation_state (ACTIVATED);
        _another_source_wants_to_be_mainloop->run ();
      } else {
        run_in_main_thread ();
      }
    }

    void 
    MainLoop::set_another_source_wants_to_be_mainloop (ExternalSource *source)
    {
      _another_source_wants_to_be_mainloop = source;
    }

    void
    MainLoop::impl_deactivate ()
    {
      if (_another_source_wants_to_be_mainloop) {
        _another_source_wants_to_be_mainloop->please_stop ();
      } else {
        own_mutex.unlock ();
      }
    }

    void
    MainLoop::run_in_main_thread ()
    {
      launch_mutex_unlock();
      run ();
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
      //auto * th =
      new djnn_thread_t (&MainLoop::run, this);
      #endif

      #if DJNN_USE_QT_THREAD
      auto * th = QThread::create([this]() { this->MainLoop::run(); });
      QObject::connect(th, SIGNAL(finished()), th, SLOT(deleteLater()));
      th->start();
      #endif

      #if DJNN_USE_SDL_THREAD
      SDL_CreateThread(SDL_ThreadFunction, "djnn thread", this);
      #endif
    }


    void
    MainLoop::run ()
    {
      if (is_run_forever ()) {
        //DBG;
        //own_mutex.lock (); // 1st lock: success
        //own_mutex.lock (); // 2nd lock: blocks forever
        while (1) {
          unsigned int duration = 2000;
          #if DJNN_USE_SDL_THREAD
          SDL_Delay(duration);
          #elif DJNN_USE_QT_THREAD && (QT_VERSION < QT_VERSION_CHECK(5,10,0))
          QThread::currentThread()->wait(duration);
          #else
          this_thread::sleep_for(chrono::seconds(duration));
          #endif
          if(get_please_stop()) {
            break;
          }
        }
        
      } else { /* mainloop run for _duration time */
        //std::
        //boost::
        #if DJNN_USE_SDL_THREAD
        SDL_Delay(_duration.count());
        #elif DJNN_USE_QT_THREAD && (QT_VERSION < QT_VERSION_CHECK(5,10,0))
        QThread::currentThread()->wait(_duration.count());
        #else
        this_thread::sleep_for (_duration);
        #endif
        
        // FIXME : should be removed : with mainloop deactivation
        // reset _duration at set_run_for_ever () to avoid mainloop re-activation with _duration already set
        set_run_for_ever ();

        //DBG;
      }
      if (_another_source_wants_to_be_mainloop)
        _another_source_wants_to_be_mainloop->please_stop ();

      launch_mutex_lock(); // reacquire launch mutex
    }


}
