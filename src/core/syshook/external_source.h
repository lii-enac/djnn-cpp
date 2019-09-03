/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

#include <mutex>

namespace djnn
{
  //extern thread_local bool _please_stop;

  class ExternalSource
  {
  public:
    ExternalSource ();
    virtual ~ExternalSource ();

    //virtual void please_stop () { set_please_stop (true); }
    virtual void please_stop (); //{ _please_stop = true; _thread.interrupt(); }
    //virtual void activate_from_mainloop () {}

    void private_run();

  protected:
    //virtual void set_please_stop (bool v) { _please_stop.store (v); }
    //virtual bool get_please_stop () const { return _please_stop.load (); }
    virtual void set_please_stop (bool v) { _please_stop = v; }
    //virtual bool get_please_stop () const { return _please_stop; }
    //virtual void set_please_stop (bool v) { _thread.stop(); }
    //virtual bool get_please_stop () const { return _thread.stopping(); }
    virtual bool get_please_stop () const { return _please_stop; }

    virtual void start_thread();
    virtual void run() = 0;
    friend class MainLoop;

    static void init();

    //std::thread _thread;

    //interruptible_thread _thread;

  protected:
    static void launch_mutex_lock();
    static void launch_mutex_unlock();

    void thread_terminated();

    class Impl;
    Impl * _impl;

  private:
    std::atomic_bool _please_stop;
   //bool _please_stop;
    
  };



}
