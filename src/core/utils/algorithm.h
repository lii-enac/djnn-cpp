#pragma once

#if !DJNN_STL_STD && !DJNN_STL_EASTL && !DJNN_STL_DJNN
#define DJNN_STL_STD 1
#endif

#if DJNN_STL_STD
#include <algorithm>
#include <limits>
namespace djnnstl {
using std::find;
using std::max;
using std::min;
using std::numeric_limits;
using std::remove;
using std::remove_if;
using std::sort;
using std::stable_sort;
using std::transform;
} // namespace djnnstl
#elif DJNN_STL_EASTL
#include <EASTL/algorithm.h>
#include <EASTL/sort.h>
namespace djnnstl {
using eastl::stable_sort;
}
#elif DJNN_STL_DJNN
#include <algorithm>
#include <limits>
namespace djnnstl {
using std::max;
using std::min;
using std::stable_sort;
// using djnnstl::remove;
using std::find;
using std::numeric_limits;
using std::remove;
using std::remove_if;
using std::sort;
using std::transform;
} // namespace djnnstl
#endif
