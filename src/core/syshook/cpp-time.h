#pragma once

//#define DJNN_USE_STD 1
//#define DJNN_USE_BOOST 0

#if DJNN_USE_STD_CHRONO
#include <chrono>
#endif

#if DJNN_USE_BOOST_CHRONO
#include <boost/chrono.hpp>
#endif

namespace djnn {

#if DJNN_USE_STD_CHRONO
namespace chrono = std::chrono;
#endif

#if DJNN_USE_BOOST_CHRONO
namespace chrono = boost::chrono;
#endif


}

//#undef DJNN_USE_STD_CHRONO
//#undef DJNN_USE_BOOST_CHRONO
