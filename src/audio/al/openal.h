#pragma once

#if defined(__APPLE__) && !defined(__EMSCRIPTEN__)

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#else

#include <AL/al.h>
#include <AL/alc.h>

#endif
