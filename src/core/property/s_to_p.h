#pragma once

namespace djnn {

// FIXME: share the following code with other parts of the lib that cast from one type to another
  inline
  int
  s_to_p(const string& s, int)
  {
    return stoi(s);
  }

  inline
  double
  s_to_p(const string& s, double)
  {
    return stof(s);
  }

  inline
  bool
  s_to_p(const string& s, bool)
  {
    //return stoi(s) ? true : false;
    return s=="true" ? true : (s=="false" ? false : stoi(s));
  }

  inline
  const string&
  s_to_p(const string& s, string)
  {
    return s;
  }

  template <class X>
  inline
  string
  p_to_s(const X& x)
  {
    return to_string(x);
  }

  template <>
  inline
  string
  p_to_s<bool>(const bool& x)
  {
    return (x ? "true" : "false");
  }
}