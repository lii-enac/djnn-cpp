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

namespace djnn {

    MainLoop::MainLoop ()
    {
      set_run_for_ever ();
      
      launch_mutex_lock();
    }

    void
    MainLoop::activate ()
    {
      for (auto p: _background_processes) {
        p->activation ();
      }
      if (_another_source_wants_to_be_mainloop) {
        //djnn::get_exclusive_access (DBG_GET);
        run_in_own_thread ();
        //another_source_wants_to_be_mainloop->activate_from_mainloop ();
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
    MainLoop::deactivate ()
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

    void
    MainLoop::run_in_own_thread ()
    {
      #if DJNN_USE_BOOST_THREAD || DJNN_USE_BOOST_FIBER || DJNN_USE_STD_THREAD
      //auto * th =
      new djnn_thread_t (&MainLoop::run, this);
      #endif

      #if DJNN_USE_QT_THREAD
      //auto * th = QThread::create([this]() { this->ExternalSource::run(); });
      auto * th = QThread::create([this]() { this->MainLoop::run(); });
      QObject::connect(th, SIGNAL(finished()), th, SLOT(deleteLater()));
      th->start();
      #endif
    }

#define DBG std::cerr << __FUNCTION__ << " " << __FILE__ << ":" << __LINE__ << std::endl;

    void
    MainLoop::run ()
    {
      //DBG;

      if (is_run_forever ()) {
        //DBG;
        //own_mutex.lock (); // 1st lock: success
        //own_mutex.lock (); // 2nd lock: blocks forever
        while (1) this_thread::sleep_for(chrono::seconds(20000));
      } else {
        //DBG;
        //std::
        //boost::
        this_thread::sleep_for (chrono::milliseconds(_duration));
        //DBG;
      }
      if (_another_source_wants_to_be_mainloop)
        _another_source_wants_to_be_mainloop->please_stop ();
    }


}
