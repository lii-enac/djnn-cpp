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
#include <string>

namespace djnn
{

  class ExternalSource
  {
  public:
    ExternalSource (const djnn::string& name);
    virtual ~ExternalSource ();

    virtual void start ();
    virtual void please_stop ();
    virtual void join();

    void private_run();
    const djnn::string& get_name () const { return _name; }

    std::atomic<std::atomic<bool>*> cancelled;

    bool should_i_stop () const;

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

    djnn::string _name;

  private:
    std::atomic<bool> _please_stop;
  };


  // not in class ExternalSource as static anymore, because of mingw ld or binutils bug
  extern
#ifndef __EMSCRIPTEN__
    thread_local
#endif
    std::atomic<bool> thread_local_cancelled;

}
