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

namespace djnn {
inline djnn_mutex_t* create_lock ()
{
#if DJNN_USE_BOOST_FIBER
    return nullptr;
#elif DJNN_USE_SDL_THREAD
    return SDL_CreateMutex ();
#else
    return new djnn_mutex_t ();
#endif
}

inline void lock (djnn_mutex_t* m)
{
#if DJNN_USE_BOOST_FIBER
// do nothing
#elif DJNN_USE_SDL_THREAD
    SDL_LockMutex (m);
#else
    m->lock ();
#endif
}

inline void release (djnn_mutex_t* m)
{
#if DJNN_USE_BOOST_FIBER
// do nothing
#elif DJNN_USE_SDL_THREAD
    SDL_UnlockMutex (m);
#else
    m->unlock ();
#endif
}
} // namespace djnn
