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

namespace djnn {
typedef enum djnn_dir_t {
    DJNN_GET_ON_CHANGE,
    DJNN_SET_ON_CHANGE,
    DJNN_IGNORE
} djnn_dir_t;
}
