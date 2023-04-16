#include "core/utils/iostream.h"


#if DJNN_STL_DJNN || DJNN_USE_DJNN_IOSTREAM

#include <iostream>
#include "iostream.h"

namespace djnnstl {
  class ostream_proxy {
    public:
    ostream_proxy(std::ostream& o) : ostream(o) {}
    std::ostream& ostream;
  };

  ostream_proxy cout_proxy(std::cout);
  ostream_proxy cerr_proxy(std::cerr);

  ostream_proxy& cout_ostream::get_proxy ()
  {
    return cout_proxy;
  }
  ostream_proxy& cerr_ostream::get_proxy ()
  {
    return cerr_proxy;
  }

  void endl_iomanip::do_manip(ostream& o) const
  {
    o.get_proxy().ostream << std::endl;
  }

  cout_ostream cout;
  cerr_ostream cerr;
  //endl_iomanip endl;
  char endl = '\n';


  std::ostream&
  operator << (std::ostream& o, const iomanip & manip)
  {
    //manip.do_manip(ostream_proxy(o));
    o << std::endl;
    return o;
  }

  template <typename T>
  ostream& operator << (ostream& o, const T& t)
  {
    o.get_proxy().ostream << t;
    return o;
  }


  ostream& operator << (ostream& o, const void* t)
  {
    o.get_proxy().ostream << t;
    return o;
  }

  ostream& operator << (ostream& o, const char* t)
  {
    o.get_proxy().ostream << t;
    return o;
  }

  // template <typename T>
  // ostream& operator << (ostream& o, T* t)
  // {
  //   o.get_proxy().ostream << t;
  //   return o;
  // }
  
  // template <typename T>
  // ostream& operator << (ostream& o, const T* t)
  // {
  //   o.get_proxy().ostream << t;
  //   return o;
  // }


  

  template ostream& operator << (ostream&, const bool&);
  template ostream& operator << (ostream&, const int&);
  template ostream& operator << (ostream&, const float&);
  template ostream& operator << (ostream&, const double&);
  template ostream& operator << (ostream&, const unsigned long&);
  //template ostream& operator << (ostream&, const char*);
  template ostream& operator << (ostream&, const char&);
  template ostream& operator << (ostream& o, const iomanip& manip);

  #if DJNN_STL_DJNN
  using djnnstl::string;
  #else
  using std::string;
  #endif
  template ostream& operator << (ostream&, const string&);
  
  // template <> ostream& operator << (ostream& o, const iomanip& manip)
  // {
  //   manip.do_manip(o);
  //   return o;
  // }

  // ostream& operator << (ostream& o, iomanip& manip)
  // {
  //   manip.do_manip(o);
  //   return o;
  // }


}

#endif
