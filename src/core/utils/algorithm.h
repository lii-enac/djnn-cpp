#pragma once

#if !DJNN_STL_STD && !DJNN_STL_EASTL && !DJNN_STL_DJNN
  #define DJNN_STL_STD 1
#endif

#if DJNN_STL_STD
#include <algorithm>
#include <limits>
namespace djnn {
  using std::stable_sort;
  using std::max;
  using std::min;
}
#elif DJNN_STL_EASTL
#include <EASTL/algorithm.h>
#include <EASTL/sort.h>
namespace djnn {
  using eastl::stable_sort;
}
#elif DJNN_STL_DJNN
#include <algorithm>
#include <limits>
namespace djnn {
  using std::stable_sort;
  using std::max;
  using std::min;
  using std::remove;
  using std::sort;
}
#endif
