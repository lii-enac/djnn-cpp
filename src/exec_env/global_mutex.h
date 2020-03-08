#pragma once

#include "core/core.h"

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)

#define DBG_GET (">> GET " __FILE__ ":" LINE_STRING)
#define DBG_REL ("<< REL " __FILE__ ":" LINE_STRING)

namespace djnn
{
  void init_global_mutex ();

  void get_exclusive_access(const char* debug);
  void release_exclusive_access(const char* debug);

  void start (Process *c);
  void stop (Process *c);
}
