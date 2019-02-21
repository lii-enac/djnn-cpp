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

#include "../gui-priv.h"
#include <SDL2/SDL.h>

namespace djnn {
  void
  p_init_gui () {
    if (SDL_Init (SDL_INIT_VIDEO) < 0) {
      std::cerr << "Failed to initialize SDL : " << SDL_GetError() << endl;
      return;
    }
  }
}
