#pragma once

#include "core/utils/containers.h"

#if DJNN_STRING_FMT

#include <fmt/format.h>
namespace djnn {
 using fmt::to_string;
}

#elif DJNN_STL_STD

namespace djnn {
 using std::to_string;
}

#elif DJNN_STL_EASTL

namespace djnn {
 using eastl::to_string;
}

#endif
