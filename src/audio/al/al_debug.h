/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020-2024)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
#pragma once

#include "openal.h"
#include "utils/debug.h"

#if !defined(DJNN_NO_DEBUG)
#include "core/utils/error.h"
#define CHKAL                              \
    {                                      \
        int err = alGetError ();           \
        if (err) {                         \
            loginfo (alErrorString (err)); \
        }                                  \
    }
#else
#define CHKAL
#endif

inline const char*
alErrorString (ALenum e) {
    switch (e) {
    case ALC_NO_ERROR:
        return "ALC_NO_ERROR";
    case ALC_INVALID_DEVICE:
        return "ALC_INVALID_DEVICE";
    case ALC_INVALID_CONTEXT:
        return "ALC_INVALID_CONTEXT";
    case ALC_INVALID_ENUM:
        return "ALC_INVALID_ENUM";
    case ALC_INVALID_VALUE:
        return "ALC_INVALID_VALUE";
    case ALC_OUT_OF_MEMORY:
        return "ALC_OUT_OF_MEMORY";
    default:
        return "ALC_UNKOWN_ERROR";
    }
}