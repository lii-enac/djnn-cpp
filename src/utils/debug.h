#pragma once

#ifndef __FL__

#ifndef __DJNN__FUNCTION__
    #ifdef __PRETTY_FUNCTION__
        #define __DJNN_FUNCTION__ __PRETTY_FUNCTION__
    #else
        #define __DJNN_FUNCTION__ __FUNCTION__
    #endif
#endif

#define __FL__ " " << __DJNN_FUNCTION__ << ":" << __FILE__ << ":" << __LINE__ << std::endl;
#ifndef DBG
#define DBG std::cerr << __DJNN_FUNCTION__ << ":" << __FILE__ << ":" << __LINE__ << std::endl;
#endif
#define DBGPROC std::cerr << this << " " << get_name() << " " << __DJNN_FUNCTION__ << ":" << __FILE__ << ":" << __LINE__ << std::endl;
#define UNIMPL { static int done=false; if(!done) {std::cerr << "unimplemented " << __DJNN_FUNCTION__ << ":" << __FILE__ << ":" << __LINE__ << std::endl; done=true;} }
#define DEPR { static int done=false; if(!done) {std::cerr << "deprecated " << __DJNN_FUNCTION__ << ":" << __FILE__ << ":" << __LINE__ << std::endl; done=true;} }

#define DBGVAR(x) #x << ":" << x

#endif

/* DEBUG DISPLAY INFORMATION */
#define _DEBUG_SEE_COLOR_PICKING_VIEW  0
