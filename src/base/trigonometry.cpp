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

#include "trigonometry.h"

#include "core/serializer/serializer.h"

namespace djnn {

template <>
const char name_info<my_cos<double>>::left[] = "left";
template <>
const char name_info<my_cos<double>>::right[] = "right";
template <>
const char name_info<my_cos<double>>::serialize[] = "cosine";

template <>
const char name_info<my_sin<double>>::left[] = "left";
template <>
const char name_info<my_sin<double>>::right[] = "right";
template <>
const char name_info<my_sin<double>>::serialize[] = "sine";

template <>
const char name_info<my_tan<double>>::left[] = "left";
template <>
const char name_info<my_tan<double>>::right[] = "right";
template <>
const char name_info<my_tan<double>>::serialize[] = "tangent";

template <>
const char name_info<my_acos<double>>::left[] = "left";
template <>
const char name_info<my_acos<double>>::right[] = "right";
template <>
const char name_info<my_acos<double>>::serialize[] = "arccosine";

template <>
const char name_info<my_asin<double>>::left[] = "left";
template <>
const char name_info<my_asin<double>>::right[] = "right";
template <>
const char name_info<my_asin<double>>::serialize[] = "arcsine";

template <>
const char name_info<my_atan<double>>::left[] = "left";
template <>
const char name_info<my_atan<double>>::right[] = "right";
template <>
const char name_info<my_atan<double>>::serialize[] = "arctangent";

template <>
const char name_info<my_atan2<double>>::left[] = "y";
template <>
const char name_info<my_atan2<double>>::right[] = "x";
template <>
const char name_info<my_atan2<double>>::serialize[] = "arctangent2";

template <>
const char name_info<my_cosh<double>>::left[] = "left";
template <>
const char name_info<my_cosh<double>>::right[] = "right";
template <>
const char name_info<my_cosh<double>>::serialize[] = "hyperboliccosine";

template <>
const char name_info<my_sinh<double>>::left[] = "left";
template <>
const char name_info<my_sinh<double>>::right[] = "right";
template <>
const char name_info<my_sinh<double>>::serialize[] = "hyperbolicsine";

template <>
const char name_info<my_tanh<double>>::left[] = "left";
template <>
const char name_info<my_tanh<double>>::right[] = "right";
template <>
const char name_info<my_tanh<double>>::serialize[] = "hyperbolictangent";

template <>
const char name_info<my_acosh<double>>::left[] = "left";
template <>
const char name_info<my_acosh<double>>::right[] = "right";
template <>
const char name_info<my_acosh<double>>::serialize[] = "hyperbolicarccosine";

template <>
const char name_info<my_asinh<double>>::left[] = "left";
template <>
const char name_info<my_asinh<double>>::right[] = "right";
template <>
const char name_info<my_asinh<double>>::serialize[] = "hyperbolicarcsine";

template <>
const char name_info<my_atanh<double>>::left[] = "left";
template <>
const char name_info<my_atanh<double>>::right[] = "right";
template <>
const char name_info<my_atanh<double>>::serialize[] = "hyperbolicarctangent";

} // namespace djnn
