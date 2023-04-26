#pragma once

namespace djnn {

// FIXME: share the following code with other parts of the lib that cast from one type to another
  inline
  int
  s_to_p(const djnnstl::string& s, int)
  {
    return stoi(s);
  }

  inline
  double
  s_to_p(const djnnstl::string& s, double)
  {
    return stof(s);
  }

  inline
  bool
  s_to_p(const djnnstl::string& s, bool)
  {
    //return stoi(s) ? true : false;
    return s=="true" ? true : (s=="false" ? false : stoi(s));
  }

  inline
  const djnnstl::string&
  s_to_p(const djnnstl::string& s, djnnstl::string)
  {
    return s;
  }

  template <class X>
  inline
  djnnstl::string
  p_to_s(const X& x)
  {
    return djnnstl::to_string(x);
  }

  template <>
  inline
  djnnstl::string
  p_to_s<bool>(const bool& x)
  {
    return (x ? "true" : "false");
  }
}