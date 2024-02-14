#pragma once
#include "core/utils/containers.h"

namespace djnn {
class CoreProcess;
}

void     djnn_set_value (djnn::CoreProcess* p, const djnnstl::string& v, bool immediate);
mystring operator+ (const mystring&, const std::string&);
// mystring operator+ (const std::string&, const mystring&);
