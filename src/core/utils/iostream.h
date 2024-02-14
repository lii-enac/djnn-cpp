#pragma once

#if !DJNN_STL_STD && !DJNN_STL_EASTL && !DJNN_STL_DJNN
  #define DJNN_STL_STD 1
#endif

#if DJNN_STL_DJNN || DJNN_USE_DJNN_IOSTREAM

  #if ! DJNN_STL_DJNN
  #include <string>
  #endif

namespace djnnstl {
  // this is not how it should be implemented, but it's easier this way

  class ostream_proxy;

  class ostream {
    public:
    virtual ostream_proxy& get_proxy () = 0;
  };
  class cout_ostream : public ostream {
    public:
    ostream_proxy& get_proxy () override;
  };
  class cerr_ostream : public ostream {
    public:
    ostream_proxy& get_proxy () override;
  };

  class iomanip {
    public:
    virtual void do_manip(ostream&) const = 0;
  };
  class endl_iomanip : public iomanip {
    private:
    void do_manip(ostream&) const override;
  };

  extern cout_ostream cout;
  extern cerr_ostream cerr;
  //extern endl_iomanip endl;
  extern char endl;

  template <typename T>
  ostream& operator << (ostream& o, const T& t);

  ostream& operator << (ostream& o, const char*);
  //ostream& operator << (ostream& o, const iomanip& manip);
  //ostream& operator << (ostream& o, iomanip& manip);
  ostream& operator << (ostream& o, const void*);

  template <typename T>
  ostream& operator << (ostream& o, const T* t) { o << (const void*) t; return o; }
  template <typename T>
  ostream& operator << (ostream& o, T* t) { o << (const void*) t; return o; }

  inline ostream& operator << (ostream& o, iomanip& manip)
  {
    manip.do_manip(o);
    return o;
  }

  // ostream& operator << (ostream&, bool);
  // ostream& operator << (ostream&, int);
  // ostream& operator << (ostream&, float);
  // ostream& operator << (ostream&, double);
  // ostream& operator << (ostream&, unsigned long);
  // ostream& operator << (ostream&, const char*);
  // ostream& operator << (ostream&, char);
  // ostream& operator << (ostream&, iomanip&);
  // #if DJNN_STL_DJNN
  // using djnnstl::string;
  // #else
  // using std::string;
  // #endif
  // ostream& operator << (ostream&, const string&);
  
}

#elif DJNN_STL_STD
#include <iostream>

namespace djnnstl {
  using std::cout;
  using std::cerr;
  using std::endl;
}

#elif DJNN_STL_EASTL
#include <iostream>
#include <EASTL/string.h>

namespace eastl {
// https://stackoverflow.com/a/5195657
// In general you should have the insertion operators in the same namespace as the class upon which it operates.

inline
std::ostream&
operator<<(std::ostream& out, const eastl::string& s)
{
  out << s.c_str();
  return out;
}

}


#endif