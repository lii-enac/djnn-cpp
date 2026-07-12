/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019-2024)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
#pragma once

namespace djnn {
class PickAnalyticalContext {
  public:
    double x;
    double y;
    double half_outline_width;
    bool   filled;
    bool   outlined;
    bool   clipped;
};
} // namespace djnn
