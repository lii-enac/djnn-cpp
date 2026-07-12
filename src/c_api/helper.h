/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2023-2024)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
#pragma once
#include "core/utils/containers.h"

namespace djnn {
class CoreProcess;
}

void     djnn_set_value (djnn::CoreProcess* p, const djnnstl::string& v, bool immediate);
mystring operator+ (const mystring&, const std::string&);
// mystring operator+ (const std::string&, const mystring&);
