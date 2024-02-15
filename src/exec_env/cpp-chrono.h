/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

// #include "exec_env/cpp-chrono-config.h"

// #if DJNN_USE_STD_CHRONO
#include <chrono>

#if DJNN_USE_BOOST_CHRONO
#include <boost/chrono.hpp>
#endif

namespace djnn {

// #if DJNN_USE_STD_CHRONO
namespace chrono = std::chrono;

#if DJNN_USE_BOOST_CHRONO
namespace chrono = boost::chrono;
#endif

// import literal such as "1s" etc
using namespace std::chrono_literals;

} // namespace djnn
