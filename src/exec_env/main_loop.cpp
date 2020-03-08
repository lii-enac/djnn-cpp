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

#include "main_loop.h"
#include "cpp-thread.h"

#if DJNN_USE_QT_MAINLOOP
#include "qt/qt_mainloop.h"
#endif

#include <iostream>
#include <algorithm>
#include "utils/debug.h"

#include "exec_env/djnn_time_manager.h"

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
        ExternalSource::init();
        launch_mutex_lock ();
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
      please_stop ();
    }

    void
    MainLoop::impl_activate ()
    { //DBG;
      DjnnTimeManager::instance().update_ref_now();

      _is_stopping = false;

      for (auto es: _external_sources) es->start ();
      for (auto p: _background_processes) {
        p->activate ();
      }
      
      if (_another_source_wants_to_be_mainloop) {
        run_in_own_thread ();
        set_activation_state (ACTIVATED);
        launch_mutex_unlock();
        _another_source_wants_to_be_mainloop->run (); // should block
      } else {
        launch_mutex_unlock();
        run_in_main_thread (); // should block
      }
      // up here, the mainloop is blocking

      _is_stopping = true;
      // starting from here, the mainloop has exited
      launch_mutex_lock (); // reacquire launch mutex

      if (_another_source_wants_to_be_mainloop) {
        please_stop ();
        join_own_thread ();
      }
    }

    std::atomic<bool> MainLoop::_is_stopping;

    void
    MainLoop::run ()
    {
      cancel_mutex.lock(); // first lock, get it
      if (is_run_forever ()) {
        //std::cerr << ">> mainloop entering sleep forever" << __FL__;
        //cancel_mutex.lock(); // second lock, blocks until another thread calls pleaseStop
        cv.wait(cancel_mutex);
        //std::cerr << "<< mainloop leaving sleep forever" << __FL__;
      } else {
        //std::cerr << ">> mainloop entering sleep " << DBGVAR(_duration.load().count()) << __FL__;
        //bool timer_cancelled =
        //cancel_mutex.try_lock_for(std::chrono::milliseconds (_duration)); // second lock, blocks for ddd ms unless it's unlocked elsewhere
        cv.wait_for(cancel_mutex, std::chrono::milliseconds (_duration));
        //std::cerr << "<< mainloop exited sleep " << DBGVAR(timer_cancelled) << __FL__;
      }
      cancel_mutex.unlock(); // unlock first lock

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
      //cancel_mutex.unlock();
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
      //auto
      own_thread = djnn_thread_t (&MainLoop::private_run, this);
      //own_thread.detach(); // FIXME: could be properly joined
      // since the thread is detached, own_thread object does not own it anymore and can be safely destroyed
      #endif

      #if DJNN_USE_QT_THREAD
      //auto *
      own_thread = QThread::create([this]() { this->MainLoop::private_run(); });
      QObject::connect(own_thread, SIGNAL(finished()), own_thread, SLOT(deleteLater()));
      own_thread->start();
      #endif

      #if DJNN_USE_SDL_THREAD
      //auto *
      own_thread = SDL_CreateThread(SDL_ThreadFunction, "djnn thread", this); // FIXME: leak
      //SDL_DetachThread(own_thread); // // FIXME: could be properly joined
      #endif
    }

    void
    MainLoop::join_own_thread ()
    {
      //std::cerr << ">> " << __FL__;
      #if DJNN_USE_BOOST_THREAD || DJNN_USE_BOOST_FIBER || DJNN_USE_STD_THREAD
      own_thread.join();
      #endif

      #if DJNN_USE_QT_THREAD
      own_thread->wait();
      #endif

      #if DJNN_USE_SDL_THREAD
      SDL_WaitThread(own_thread, nullptr);
      #endif
      //std::cerr << "<< " << __FL__;
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
       auto i = std::find(_external_sources.begin (), _external_sources.end (), es);
       if (i != _external_sources.end ())
          _external_sources.erase(i);
    }

}
