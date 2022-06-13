#pragma once

#if !DJNN_STL_STD && !DJNN_STL_EASTL
    #define DJNN_STL_STD 1
#endif

#if DJNN_STL_STD

#include <string>
#include <vector>
#include <map>
#include <list>

namespace djnnstl = std;

#elif DJNN_STL_EASTL

#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <EASTL/map.h>
#include <EASTL/list.h>

namespace djnnstl = eastl;

extern int stoi(const eastl::string&);
extern int stoi(const eastl::string&, size_t* idx, int base = 10);
extern double stof(const eastl::string&);
extern double stof(const eastl::string&, size_t* idx, int base = 10);

#endif

namespace djnn {
  using djnnstl::string;
  using djnnstl::vector;
  using djnnstl::list;
  using djnnstl::map;
  using djnnstl::pair;
  using djnnstl::make_pair;
}
