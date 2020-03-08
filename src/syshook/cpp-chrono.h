#pragma once

//#include "syshook/cpp-chrono-config.h"

//#if DJNN_USE_STD_CHRONO
#include <chrono>

#if DJNN_USE_BOOST_CHRONO
#include <boost/chrono.hpp>
#endif

namespace djnn {

//#if DJNN_USE_STD_CHRONO
namespace chrono = std::chrono;

#if DJNN_USE_BOOST_CHRONO
namespace chrono = boost::chrono;

#endif

}
