#pragma once

#include "core/utils/containers.h"

#if DJNN_STRING_FMT

#include <fmt/format.h>
namespace djnnstl {
using fmt::to_string;
}

#elif DJNN_STL_STD

// namespace djnnstl {
//  using std::to_string;
// }

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
