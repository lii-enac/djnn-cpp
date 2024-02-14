/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "logic.h"

#include "core/serializer/serializer.h"

namespace djnn {

template <>
const char name_info<std::logical_and<bool>>::left[] = "left";
template <>
const char name_info<std::logical_and<bool>>::right[] = "right";
template <>
const char name_info<std::logical_and<bool>>::serialize[] = "and";

template <>
const char name_info<std::logical_or<bool>>::left[] = "left";
template <>
const char name_info<std::logical_or<bool>>::right[] = "right";
template <>
const char name_info<std::logical_or<bool>>::serialize[] = "or";

template <>
const char name_info<std::not_equal_to<bool>>::left[] = "left";
template <>
const char name_info<std::not_equal_to<bool>>::right[] = "right";
template <>
const char name_info<std::not_equal_to<bool>>::serialize[] = "xor";

template <>
const char name_info<std::logical_not<bool>>::left[] = "left";
template <>
const char name_info<std::logical_not<bool>>::right[] = "right";
template <>
const char name_info<std::logical_not<bool>>::serialize[] = "not";

} // namespace djnn
