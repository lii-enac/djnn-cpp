//
// Created by Conversy on 20/04/15.
//

#pragma once

#include <iostream>

// #define __FL__ " " __FILE__ ":" << __LINE__ << std::endl;
// #define __EFL__ std::cerr << __FUNCTION__ << " " << __FILE__ ":" << __LINE__ << std::endl;
// #define DBG __EFL__
// #define attr(a) #a ":" << a << " "

#ifdef __APPLE__
// get rid of glu deprecation on macosx
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#ifdef __APPLE__
#pragma GCC diagnostic pop
#endif

char const* gl_error_string(GLenum err);

#define CHKGL {int err = glGetError(); if(err) { std::cerr << gl_error_string(err) << __FL__; }}
//#define CHKGL

#define GENGL
#define DELGL

//#define GENGL { std::cerr << "GENGL" << __FL__; }
//#define DELGL { std::cerr << "DELGL" << __FL__; }

