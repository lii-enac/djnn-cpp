#pragma once

namespace djnn
{
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
}