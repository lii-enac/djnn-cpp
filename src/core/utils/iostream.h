#pragma once
#include <iostream>

inline
std::ostream& operator<<(std::ostream& out, const djnn::string& s)
{
  out << s.c_str();
  return out;
}