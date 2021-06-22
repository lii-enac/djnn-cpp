#pragma once

#include <string>
#include <vector>
#include <map>
#include <list>

namespace djnnstl = std;


// #include <EASTL/string.h>
// #include <EASTL/vector.h>
// #include <EASTL/map.h>
// #include <EASTL/list.h>

// namespace djnnstl = eastl;

// extern int stoi(const eastl::string&);
// extern int stoi(const eastl::string&, size_t* idx, int base = 10);
// extern double stof(const eastl::string&);
// extern double stof(const eastl::string&, size_t* idx, int base = 10);

namespace djnn {
  using djnnstl::string;
  using djnnstl::vector;
  using djnnstl::list;
  using djnnstl::map;
  using djnnstl::pair;
}



