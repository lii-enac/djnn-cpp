#pragma once

#ifndef __FL__

#ifdef __PRETTY_FUNCTION__
#define __DJNN_FUNCTION__ __PRETTY_FUNCTION__
#else
#define __DJNN_FUNCTION__ __FUNCTION__
#endif

#define __FL__ " " << __DJNN_FUNCTION__ << ":" << __FILE__ << ":" << __LINE__ << std::endl;
#define DBG std::cerr << __DJNN_FUNCTION__ << ":" << __FILE__ << ":" << __LINE__ << std::endl;
#define DBGPROC std::cerr << this << " " << get_name() << " " << __DJNN_FUNCTION__ << ":" << __FILE__ << ":" << __LINE__ << std::endl;
#define UNIMPL { static int done=false; if(!done) {std::cerr << "unimplemented " << __DJNN_FUNCTION__ << ":" << __FILE__ << ":" << __LINE__ << std::endl; done=true;} }
#define DBGVAR(x) #x << ":" << x

#endif
