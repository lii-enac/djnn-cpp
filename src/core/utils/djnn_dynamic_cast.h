#include "core/tree/abstract_property.h"
#include "core/tree/bool_property.h"
#include "core/tree/int_property.h"
#include "core/tree/double_property.h"
#include "core/tree/text_property.h"
#include "core/tree/ref_property.h"

namespace djnn {
  template <typename X, typename P> inline X djnn_dynamic_cast(P p)
  {
    //return nullptr;
    return dynamic_cast<X>(p);
  }

#ifdef DJNN_NO_DYNAMIC_CAST

  template <typename X> inline X djnn_dynamic_cast(Process* p)
  {
    if(p->get_process_type() == PROPERTY_T) {
      return static_cast<X>(p);
    }
    return nullptr;
  }

  template <> inline AbstractIntProperty* djnn_dynamic_cast(AbstractProperty* p)
  {
    if(p->get_prop_type() == Integer) {
      return static_cast<AbstractIntProperty*>(p);
    }
    return nullptr;
  }

  template <> inline AbstractBoolProperty* djnn_dynamic_cast(AbstractProperty* p)
  {
    if(p->get_prop_type() == Boolean) {
      return static_cast<AbstractBoolProperty*>(p);
    }
    return nullptr;
  }

  template <> inline AbstractDoubleProperty* djnn_dynamic_cast(AbstractProperty* p)
  {
    if(p->get_prop_type() == Double) {
      return static_cast<AbstractDoubleProperty*>(p);
    }
    return nullptr;
  }

  template <> inline AbstractTextProperty* djnn_dynamic_cast(AbstractProperty* p)
  {
    if(p->get_prop_type() == String) {
      return static_cast<AbstractTextProperty*>(p);
    }
    return nullptr;
  }

  template <> inline RefProperty* djnn_dynamic_cast(AbstractProperty* p)
  {
    if(p->get_prop_type() == Reference) {
      return static_cast<RefProperty*>(p);
    }
    return nullptr;
  }

#endif

}