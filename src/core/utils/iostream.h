#pragma once
#include <iostream>

#if DJNN_STL_EASTL

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