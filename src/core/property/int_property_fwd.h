/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2022-2024)
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
#if 1
class AbstractIntProperty;
class IntProperty;
class IntPropertyProxy;

#else
template <class X>
class AbstractTemplateProperty;
using AbstractIntProperty = AbstractTemplateProperty<int>;

template <class X>
class TemplateProperty;
using IntProperty = TemplateProperty<int>;

template <class X>
class TemplatePropertyProxy;
using IntPropertyProxy = TemplatePropertyProxy<int>;

#endif
} // namespace djnn