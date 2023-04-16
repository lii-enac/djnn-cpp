#pragma once

#if !DJNN_STL_STD && !DJNN_STL_EASTL && !DJNN_STL_DJNN
  #define DJNN_STL_STD 1
#endif

#if DJNN_STL_STD
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
#elif DJNN_STL_DJNN


namespace djnnstl {
  class ostream {
    public:
  };

  class iomanip {
    public:
  };

  ostream cout, cerr;
  iomanip endl;

  ostream& operator << (ostream&, bool);
  ostream& operator << (ostream&, int);
  ostream& operator << (ostream&, float);
  ostream& operator << (ostream&, double);
  ostream& operator << (ostream&, unsigned long);
  ostream& operator << (ostream&, iomanip&);
  class string;
  ostream& operator << (ostream&, const string&);
  
}


#endif