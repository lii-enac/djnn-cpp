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

#include <atomic>

namespace djnn
{

  class ExternalSource
  {
  public:
    ExternalSource ();
    virtual ~ExternalSource ();

    virtual void please_stop ();

    void private_run();

    static thread_local std::atomic<bool> thread_local_cancelled;
    std::atomic<bool> *cancelled;

  protected:
    virtual void set_please_stop (bool v) { _please_stop = v; }
    virtual bool get_please_stop () const { return _please_stop; }

    virtual void start_thread();
    virtual void run() = 0;
    friend class MainLoop;

    static void init();

  protected:
    static void launch_mutex_lock();
    static void launch_mutex_unlock();

    class Impl;
    Impl * _impl;

  private:
    std::atomic<bool> _please_stop;
  };

}
