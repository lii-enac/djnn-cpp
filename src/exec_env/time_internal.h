#pragma once

#include <chrono>

namespace djnn_internal {

namespace Time {

typedef std::chrono::duration<long long, std::micro> duration;
typedef std::chrono::steady_clock                    clock;
typedef std::chrono::time_point<clock, duration>     time_point;

class Timer {
  public:
    Timer ()
        : _scheduled (false) {}
    virtual ~Timer () {}
    virtual void      do_it (const duration& actualduration) = 0;
    const time_point& get_start_time () const { return _start; }
    const time_point& get_end_time () const { return _end; }
    void              set_start_time (time_point d) { _start = d; }
    void              set_end_time (time_point d) { _end = d; }
    duration          get_duration () const { return _end - _start; }
    bool              is_already_scheduled () const { return _scheduled; }
    void              set_scheduled (bool s) { _scheduled = s; }

  private:
    time_point _start, _end;
    bool       _scheduled;
};

} // namespace Time
} // namespace djnn_internal