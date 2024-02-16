/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include <SDL.h>

#include "cairo_sdl_window.h"
#include "core/utils/iostream.h"
#include "display/abstract_display.h"
#include "display/display.h"
#include "display/sdl/sdl_mainloop.h"
#include "display/sdl/sdl_window.h"

namespace djnn {

class CairoSDLDisplayBackend : public AbstractDisplay
{
  public:
    static CairoSDLDisplayBackend* instance ();
    WinImpl*                       create_window (Window* win, const string& title, double x, double y, double w, double h);

    struct Impl;
    Impl* impl;

    static std::shared_ptr<CairoSDLDisplayBackend> _instance;
    static std::once_flag                          onceFlag;
};

std::shared_ptr<CairoSDLDisplayBackend> CairoSDLDisplayBackend::_instance;
std::once_flag                          CairoSDLDisplayBackend::onceFlag;

WinImpl*
CairoSDLDisplayBackend::create_window (Window* win, const string& title, double x, double y, double w, double h)
{
    // #if DJNN_GRAPHICS_GL
    SDLWindow* implwin = new CairoSDLWindow (win, title, x, y, w, h);
    // #endif
    // #if DJNN_EGL
    //     EGLWindow *implwin = new EGLWindow (win, title, x, y, w, h);
    // #endif
    set_window (win);
    return implwin;
}

CairoSDLDisplayBackend*
CairoSDLDisplayBackend::instance ()
{
    // std::cerr << __FILE__ << " " << __LINE__ << std::endl;
    std::call_once (CairoSDLDisplayBackend::onceFlag, [] () {
        _instance.reset (new CairoSDLDisplayBackend);
    });

    return _instance.get ();
}

void
DisplayBackend::init ()
{
    // std::cerr << __FILE__ << " " << __LINE__ << std::endl;
    if (_instance != nullptr)
        return;
    _instance          = new Impl ();
    _instance->backend = CairoSDLDisplayBackend::instance ();
    // std::cerr << __FILE__ << " " << __LINE__ << std::endl;
}

void
p_init_p_display ()
{
    // std::cerr << __FILE__ << " " << __LINE__ << std::endl;
    if (SDL_Init (SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL : " << SDL_GetError () << std::endl;
        return;
    }
    DisplayBackend::init ();
#if DJNN_SDL
    SDLMainloop::instance ();
// std::cerr << __FILE__ << " " << __LINE__ << std::endl;
#endif
}

} // namespace djnn
