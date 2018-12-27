#pragma once

#if defined(__WIN32__)
#define DJNN_USE_QTHREAD 1
#else
#define DJNN_USE_BOOST_THREAD 1
#define DJNN_USE_CPP_THREAD 0
#endif


#if DJNN_USE_BOOST_THREAD
#include <boost/thread/thread.hpp>
namespace djnn {

namespace this_thread = boost::this_thread;
typedef boost::thread djnn_thread_t;
}
#endif

#if DJNN_USE_STD_THREAD
#include <thread>
namespace djnn {
namespace this_thread = std::this_thread;
typedef std::thread djnn_thread_t;
}
#endif

#if DJNN_USE_PTHREAD
#include <pthread.h>
#endif

#if DJNN_USE_QTHREAD
#include <QThread>
typedef QThread* djnn_thread_t;
#endif
