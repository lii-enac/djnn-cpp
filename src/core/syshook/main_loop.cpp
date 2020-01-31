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

#if DJNN_USE_QT_MAINLOOP
#include "qt/qt_mainloop.h"
#endif

#include <iostream>
#include "utils/debug.h"

#include "core/syshook/djnn_time_manager.h"

namespace djnn {

    MainLoop MainLoop::_instance;
    
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

        launch_mutex_lock ();
        djnn::get_exclusive_access (DBG_GET); // prevent other external sources from accessing the shared data
      }
      return _instance;
    }

    MainLoop::MainLoop ()
    : Process ("Mainloop"),
      _another_source_wants_to_be_mainloop (nullptr)
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

      for (auto es: _external_sources) es->start ();
      launch_mutex_unlock();

      //std::cerr << "-----------" << __FL__;
      //DjnnTimeManager::instance().debug();
      //std::cerr << "starting mainloop with TimeManager firstDelta " << DjnnTimeManager::instance().getFirstDelta() << __FL__; 

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
      for (auto es: _external_sources) es->please_stop ();
      djnn::get_exclusive_access (DBG_GET); // prevent external source threads to do anything once mainloop is terminated
    }

    void
    MainLoop::impl_deactivate ()
    {
      if (_another_source_wants_to_be_mainloop) {
        _another_source_wants_to_be_mainloop->please_stop ();
      } else {
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

    void
    MainLoop::run_in_own_thread ()
    {
      #if DJNN_USE_BOOST_THREAD || DJNN_USE_BOOST_FIBER || DJNN_USE_STD_THREAD
      auto own_thread = djnn_thread_t (&MainLoop::private_run, this);
      own_thread.detach(); // FIXME: could be properly joined
      // since the thread is detached, own_thread object does not own it anymore and can be safely destroyed
      #endif

      #if DJNN_USE_QT_THREAD
      auto *
      own_thread = QThread::create([this]() { this->MainLoop::private_run(); });
      QObject::connect(own_thread, SIGNAL(finished()), own_thread, SLOT(deleteLater()));
      own_thread->start();
      #endif

      #if DJNN_USE_SDL_THREAD
      auto *
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
          //std::cerr << ">> mainloop ****forever***** entering sleep " << chrono::milliseconds(_duration).count() << "ms " << __FL__;
          djnn::sleep(duration);
          //std::cerr << "<< mainloop forever exit sleep " << chrono::milliseconds(_duration).count() << "ms " <<  __FL__;
          if(get_please_stop()) {
            break;
          }
        }
      } else { /* mainloop run for _duration time */
        auto ddd = chrono::milliseconds(_duration);
        //std::cerr << ">> mainloop duration entering sleep " << ddd.count () << "ms " <<  __FL__;
        djnn::sleep(ddd.count ());
        //std::cerr << "<< mainloop duration exit sleep " << ddd.count() << "ms " <<  __FL__;
        
        // FIXME : should be removed : with mainloop deactivation
        // reset _duration at set_run_for_ever () to avoid mainloop re-activation with _duration already set
        set_run_for_ever (); // reset to default forever mode
      }

      if (_another_source_wants_to_be_mainloop)
        _another_source_wants_to_be_mainloop->please_stop ();
    }

    void
    MainLoop::add_external_source (ExternalSource* es)
    {
      _external_sources.push_back(es);
    }

    void
    MainLoop::remove_external_source (ExternalSource* es)
    {
      _external_sources.erase(std::find(std::begin(_external_sources), std::end(_external_sources), es));
    }

}
