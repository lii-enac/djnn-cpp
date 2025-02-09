#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>


#include "core/utils/iostream.h"
#include "utils/debug.h"

#include <assert.h>

namespace djnn {


#define MACRO(lowkey, capkey) extern const int DJN_Key_##lowkey = SDLK_##capkey;
#include "display/const_keys.h"

#define MACRO2(lowkey, capkey, sdlkey) extern const int DJN_Key_##lowkey = SDLK_##sdlkey;
MACRO2 (Print, PRINT, PRINTSCREEN)
MACRO2 (Shift, SHIFT, LSHIFT)
MACRO2 (Control, CONTROL, LCTRL)
// MACRO2 (Meta, META, LMETA)
MACRO2 (Alt, ALT, LALT)
// MACRO2 (AltGr, ALTGR, LALTGR)
MACRO2 (NumLock, NUMLOCK, NUMLOCKCLEAR)
MACRO2 (Exclam, EXCLAM, EXCLAIM)
//MACRO2 (Apostrophe, APOSTROPHE, QUOTE)
MACRO2 (ParenLeft, PARENLEFT, LEFTPAREN)
MACRO2 (ParenRight, PARENRIGHT, RIGHTPAREN)
MACRO2 (Equal, EQUAL, EQUALS)

void
p_init_display ()
{
    if (!SDL_InitSubSystem (SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        std::cerr << "Failed to initialize SDL : " << SDL_GetError () << std::endl;
        return;
    }

    int count;
    SDL_DisplayID * displayids = SDL_GetDisplays(&count);
    assert(count);
    const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode (displayids[0]);
    //assert(mode);
    if (!mode) {
        fprintf (stderr, "SDL: %s %s:%d\n", SDL_GetError (), __FILE__, __LINE__);
        exit (1);
    }

    //std::cout << "SDL Display: " << mode->refresh_rate << "hz " << mode->w << "x" << mode->h << __FL__;
}

void
p_clear_display ()
{
    SDL_QuitSubSystem (SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_Quit ();
}

} // namespace djnn
