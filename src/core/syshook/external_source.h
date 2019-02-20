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

namespace djnn
{
  

  #if 0
  // form https://stackoverflow.com/questions/13893060/i-want-to-kill-a-stdthread-using-its-thread-object
  // Synopsis
  class interrupt_thread_exception;
  class interruptible_thread;
  void check_for_interrupt();

  // Interrupt exception
  class interrupt_thread_exception: public virtual std::exception {
  public:
      virtual char const* what() const noexcept(true) override { return "interrupt"; }
  }; // class interrupt_thread_exception

  // Interruptible thread
  class interruptible_thread {
  public:
      friend void check_for_interrupt();

      template <typename Function, typename... Args>
      interruptible_thread(Function&& fun, Args&&... args):
      _flag(false),
          _thread([](std::atomic_bool& f, Function&& fun, Args&&... args) {
                      _flag_ref = &f; fun(std::forward<Args>(args)...);
                  },
                  _flag,
                  std::forward<Function>(fun),
                  std::forward<Args>(args)...)
      {}

      bool stopping() const { return _flag.load(); }

      void stop() { _flag.store(true); }

  private:
      static thread_local std::atomic_bool* _flag_ref;

      std::atomic_bool _flag;
      std::thread _thread;
  }; // class interruptible_thread

  // Interruption checker
  inline void check_for_interrupt() noexcept(false) {
      if (not interruptible_thread::_flag_ref) { return; }
      if (not interruptible_thread::_flag_ref->load()) { return; }

      throw interrupt_thread_exception();
  } // check_for_interrupt
#endif

  //extern thread_local bool _please_stop;

  class ExternalSource
  {
  public:
    ExternalSource ();
    virtual ~ExternalSource ();

    //virtual void please_stop () { set_please_stop (true); }
    virtual void please_stop (); //{ _please_stop = true; _thread.interrupt(); }
    //virtual void activate_from_mainloop () {}

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

    //std::thread _thread;

    //interruptible_thread _thread;

  protected:
    static void launch_mutex_lock();
    static void launch_mutex_unlock();


    class Impl;
    Impl * _impl;

  private:
    void private_run();
    //std::atomic_bool
    bool _please_stop;
    
  };



}
