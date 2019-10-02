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
#define DBG std::cerr << __PRETTY_FUNCTION__ << " " << __FILE__ << ":" << __LINE__ << std::endl;

namespace djnn {

    MainLoop* MainLoop::_instance;
    std::once_flag MainLoop::onceFlag;
    MainLoop&
    MainLoop::instance ()
    {
      //DBG;
      std::call_once (MainLoop::onceFlag, [] () {
        //DBG;
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
    {//DBG;
      //std::cerr << __PRETTY_FUNCTION__ << " " << this << std::endl;
      set_run_for_ever (); // default mode is forever
      //std::cerr <<"mainloop::lock "<< __LINE__ <<std::endl;
      launch_mutex_lock ();
      //DBG;
      djnn::get_exclusive_access (DBG_GET);
    }

    MainLoop::~MainLoop ()
    {
      //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << this->get_name() << std::endl;
    }

    void
    MainLoop::impl_activate ()
    {
      //std::cerr << std::endl << __PRETTY_FUNCTION__ << " " << this << " " << this->get_name() << std::endl;
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

      //std::cerr <<"mainloop::lock "<< __LINE__ << std::endl;
      launch_mutex_lock (); // reacquire launch mutex
      djnn::get_exclusive_access (DBG_GET); // prevent external source threads to do anything once mainloop is terminated

    }
    void
    MainLoop::impl_deactivate ()
    {
      //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << this->get_name() << std::endl << std::endl;
      if (_another_source_wants_to_be_mainloop) {
        _another_source_wants_to_be_mainloop->please_stop ();
      } else {
        own_mutex.unlock ();
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
      auto * th = new djnn_thread_t (&MainLoop::private_run, this); // FIXME: leak
      th->detach(); // FIXME: could be properly joined
      #endif

      #if DJNN_USE_QT_THREAD
      //DBG;
      auto * th = QThread::create([this]() { this->MainLoop::private_run(); });
      QObject::connect(th, SIGNAL(finished()), th, SLOT(deleteLater()));
      th->start();
      #endif

      #if DJNN_USE_SDL_THREAD
      auto * th = SDL_CreateThread(SDL_ThreadFunction, "djnn thread", this); // FIXME: leak
      SDL_DetachThread(th); // // FIXME: could be properly joined
      #endif
    }

    void
    MainLoop::private_run ()
    {
      run();
    }

    void
    MainLoop::run ()
    {//DBG;
      ///std::cerr <<"mainloop::unlock "<< __LINE__ <<std::endl;
      
      if (is_run_forever ()) {
        //DBG;
        //own_mutex.lock (); // 1st lock: success
        //own_mutex.lock (); // 2nd lock: blocks forever
        while (1) {
          unsigned int duration = 2000;

          // check from time to time if we need to stop
          djnn::sleep(duration);

          if(get_please_stop()) {
            break;
          }
        }
        
      } else { /* mainloop run for _duration time */
        //std::
        //boost::
        djnn::sleep(_duration.count());
        
        // FIXME : should be removed : with mainloop deactivation
        // reset _duration at set_run_for_ever () to avoid mainloop re-activation with _duration already set
        set_run_for_ever (); // reset to default forever mode

        //DBG;
      }

      if (_another_source_wants_to_be_mainloop)
        _another_source_wants_to_be_mainloop->please_stop ();

      
      
      //std::cerr <<"mainloop finished running, mainloop::lock"<<std::endl;
      //djnn::get_exclusive_access (DBG_GET); // prevent external source thread to do anything once mainloop is terminated
      //launch_mutex_lock (); // reacquire launch mutex
    }


}
