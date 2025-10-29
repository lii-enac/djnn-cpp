#pragma once

#include "core/utils/containers.h"

#if DJNN_STRING_FMT

#include <fmt/format.h>
namespace djnnstl {
using fmt::to_string;
}

#elif DJNN_STL_STD

namespace djnnstl {
 //using std::to_string;

 inline string to_string(bool v) { return std::to_string(v); }
 inline string to_string(int v) { return std::to_string(v); }
 inline string to_string(long v) { return std::to_string(v); }
 inline string to_string(long long v) { return std::to_string(v); }
 inline string to_string(unsigned int v) { return std::to_string(v); }
 inline string to_string(unsigned long v) { return std::to_string(v); }
 inline string to_string(unsigned long long v) { return std::to_string(v); }

 string to_string(double); // 2-digit only

}

#elif DJNN_STL_EASTL

namespace djnnstl {
using eastl::to_string;
}

#elif DJNN_STL_DJNN

namespace djnnstl {
string to_string (bool);
// string stoi(bool);
} // namespace djnnstl

#endif
