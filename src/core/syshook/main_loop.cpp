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
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>

 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "main_loop.h"
#include "cpp-thread.h"
//#include "cpp-mutex.h"
//#include "cpp-mutex-priv.h"

#if DJNN_USE_QT_MAINLOOP
#include "qt/qt_mainloop.h"
#endif

//#include <iostream>
#define DBG std::cerr << __PRETTY_FUNCTION__ << " " << __FILE__ << ":" << __LINE__ << std::endl;

namespace djnn {

    MainLoop MainLoop::_instance;
    //static djnn_mutex_t* own_mutex;
    
    MainLoop&
    MainLoop::instance ()
    {
      static std::atomic_flag onceFlag = ATOMIC_FLAG_INIT;
      if(!onceFlag.test_and_set()) {

        #if DJNN_USE_QT_MAINLOOP
        QtMainloop::build_instance(&_instance);
        #endif

        init_global_mutex();
        ExternalSource::init();

        //own_mutex = create_lock();

        launch_mutex_lock ();
        djnn::get_exclusive_access (DBG_GET); // prevent other external sources from accessing the shared data
      }
      return _instance;
    }

    MainLoop::MainLoop ()
    : _another_source_wants_to_be_mainloop (nullptr)
    {
      set_run_for_ever (); // default mode is forever
    }

    MainLoop::~MainLoop ()
    {
    }

    void
    MainLoop::impl_activate ()
    {
      djnn::release_exclusive_access (DBG_REL);
      launch_mutex_unlock();

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

      launch_mutex_lock (); // reacquire launch mutex
      djnn::get_exclusive_access (DBG_GET); // prevent external source threads to do anything once mainloop is terminated

    }
    void
    MainLoop::impl_deactivate ()
    {
      if (_another_source_wants_to_be_mainloop) {
        _another_source_wants_to_be_mainloop->please_stop ();
      } else {
        //release (own_mutex);
      }
    }

    void 
    MainLoop::set_another_source_wants_to_be_mainloop (ExternalSource *source)
    {
      _another_source_wants_to_be_mainloop = source;
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

    djnn_thread_t * own_thread;

    void
    MainLoop::run_in_own_thread ()
    {
      delete own_thread;
      //auto *
      #if DJNN_USE_BOOST_THREAD || DJNN_USE_BOOST_FIBER || DJNN_USE_STD_THREAD
      own_thread = new djnn_thread_t (&MainLoop::private_run, this); // FIXME: leak
      own_thread->detach(); // FIXME: could be properly joined
      #endif

      #if DJNN_USE_QT_THREAD
      own_thread = QThread::create([this]() { this->MainLoop::private_run(); });
      QObject::connect(own_thread, SIGNAL(finished()), own_thread, SLOT(deleteLater()));
      own_thread->start();
      #endif

      #if DJNN_USE_SDL_THREAD
      own_thread = SDL_CreateThread(SDL_ThreadFunction, "djnn thread", this); // FIXME: leak
      SDL_DetachThread(own_thread); // // FIXME: could be properly joined
      #endif
    }

    void
    MainLoop::private_run ()
    {
      run();
    }

    void
    MainLoop::run ()
    {
      if (is_run_forever ()) {
        while (1) {
          unsigned int duration = 2000; // check from time to time if we need to stop
          djnn::sleep(duration);
          if(get_please_stop()) {
            break;
          }
        }
      } else { /* mainloop run for _duration time */
        djnn::sleep(_duration.count());
        
        // FIXME : should be removed : with mainloop deactivation
        // reset _duration at set_run_for_ever () to avoid mainloop re-activation with _duration already set
        set_run_for_ever (); // reset to default forever mode
      }

      if (_another_source_wants_to_be_mainloop)
        _another_source_wants_to_be_mainloop->please_stop ();
    }

}
