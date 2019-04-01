#include "opengl.h"

char const* gl_error_string(GLenum err)
{
  switch (err)
  {
    // opengl 2 errors (8)
    case GL_NO_ERROR:
      return "GL_NO_ERROR";

    case GL_INVALID_ENUM:
      return "GL_INVALID_ENUM";

    case GL_INVALID_VALUE:
      return "GL_INVALID_VALUE";

    case GL_INVALID_OPERATION:
      return "GL_INVALID_OPERATION";

#ifdef GL_STACK_OVERFLOW
    case GL_STACK_OVERFLOW:
      return "GL_STACK_OVERFLOW";
#endif

#ifdef GL_STACK_UNDERFLOW
    case GL_STACK_UNDERFLOW:
      return "GL_STACK_UNDERFLOW";
#endif

    case GL_OUT_OF_MEMORY:
      return "GL_OUT_OF_MEMORY";

#ifdef GL_TABLE_TOO_LARGE
    case GL_TABLE_TOO_LARGE:
      return "GL_TABLE_TOO_LARGE";
#endif

    // opengl 3 errors (1)
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      return "GL_INVALID_FRAMEBUFFER_OPERATION";

    // gles 2, 3 and gl 4 error are handled by the switch above
    default:
      //assert(!"unknown error");
      return nullptr;
  }
}
