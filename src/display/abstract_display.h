#pragma once

#include "core/utils/containers.h"

namespace djnn {
using djnnstl::string;

class Window;

class WinImpl;

class AbstractDisplay
{
  public:
    AbstractDisplay ()
        : _window (nullptr)
    {
    }

    virtual ~AbstractDisplay ()
    {
    }

    virtual WinImpl*
    create_window (Window* win, const string& title, double x, double y, double w, double h) = 0;

    void
    set_window (Window* w)
    {
        _window = w;
    }

    Window*
    window ()
    {
        return _window;
    }

  protected:
    Window* _window;
};
} // namespace djnn
