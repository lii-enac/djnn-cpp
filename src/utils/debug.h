#pragma once

#ifdef __PRETTY_FUNCTION__
#define __DJNN_FUNCTION__ __PRETTY_FUNCTION__
#else
#define __DJNN_FUNCTION__ __FUNCTION__
#endif

#define __FL__ " " << __DJNN_FUNCTION__ << ":" << __FILE__ << ":" << __LINE__ << std::endl;
#define DBG std::cerr << __DJNN_FUNCTION__ << ":" << __FILE__ << ":" << __LINE__ << std::endl;
#define UNIMPL { static int done=false; if(!done) {std::cerr << "unimplemented " << __DJNN_FUNCTION__ << ":" << __FILE__ << ":" << __LINE__ << std::endl; done=true;} }

