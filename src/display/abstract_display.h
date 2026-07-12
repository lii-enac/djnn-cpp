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

#include "core/utils/containers.h"

namespace djnn {
using djnnstl::string;

class Window;

class WinImpl;

class AbstractDisplay {
  public:
    AbstractDisplay ()
        : _window (nullptr) {
    }

    virtual ~AbstractDisplay () {
    }

    virtual WinImpl*
    create_window (Window* win, const string& title, double x, double y, double w, double h) = 0;

    void
    set_window (Window* w) {
        _window = w;
    }

    Window*
    window () {
        return _window;
    }

  protected:
    Window* _window;
};
} // namespace djnn
