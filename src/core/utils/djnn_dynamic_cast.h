#include "core/tree/abstract_property.h"
#include "core/tree/bool_property.h"
#include "core/tree/int_property.h"
#include "core/tree/double_property.h"
#include "core/tree/text_property.h"
#include "core/tree/ref_property.h"
#include "core/control/action.h"
#include "core/control/native_action.h"
#include "core/tree/component.h"

namespace djnn {
  template <typename X, typename P> inline X djnn_dynamic_cast(P p)
  {
    //return nullptr;
    return dynamic_cast<X>(p);
  }

#ifdef DJNN_NO_DYNAMIC_CAST

  // simulate dynamic_cast with get_process_type and get_prop_type

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

  // specialization for Process* argument
  template <> inline AbstractBoolProperty* djnn_dynamic_cast(Process* p)
  {
    AbstractProperty * ap = djnn_dynamic_cast<AbstractProperty*>(p);
    if(ap) return djnn_dynamic_cast<AbstractBoolProperty*>(ap);
    else return nullptr;
  }
  template <> inline AbstractIntProperty* djnn_dynamic_cast(Process* p)
  {
    AbstractProperty * ap = djnn_dynamic_cast<AbstractProperty*>(p);
    if(ap) return djnn_dynamic_cast<AbstractIntProperty*>(ap);
    else return nullptr;
  }
    template <> inline AbstractDoubleProperty* djnn_dynamic_cast(Process* p)
  {
    AbstractProperty * ap = djnn_dynamic_cast<AbstractProperty*>(p);
    if(ap) return djnn_dynamic_cast<AbstractDoubleProperty*>(ap);
    else return nullptr;
  }
  template <> inline AbstractTextProperty* djnn_dynamic_cast(Process* p)
  {
    AbstractProperty * ap = djnn_dynamic_cast<AbstractProperty*>(p);
    if(ap) return djnn_dynamic_cast<AbstractTextProperty*>(ap);
    else return nullptr;
  }

  // untested
  template <> inline Action* djnn_dynamic_cast(Process* p)
  {
    auto t = p->get_process_type();
    if(t == ACTION_T || t == NATIVE_ACTION_T) {
      return static_cast<Action*>(p);
    }
    return nullptr;
  }

  template <> inline NativeAction* djnn_dynamic_cast(Process* p)
  {
    if(p->get_process_type() == NATIVE_ACTION_T) {
      return static_cast<NativeAction*>(p);
    }
    return nullptr;
  }

  /*
  template <> inline Container* djnn_dynamic_cast(Process* p)
  {
    auto t = p->get_process_type();
    if(t == CONTAINER_T || t == LIST_T || t == SET_T) {
      return static_cast<Container*>(p);
    }
    return nullptr;
  }*/

  template <> inline Component* djnn_dynamic_cast(Process* p)
  {
    if(p->get_process_type() == COMPONENT_T) {
      return static_cast<Component*>(p);
    }
    return nullptr;
  }




  /*
  template <class Res> inline X* djnn_dynamic_cast(Process* p)
  {
    AbstractProperty * ap = djnn_dynamic_cast<AbstractProperty*>(p);
    if(ap) return djnn_dynamic_cast<X*>(ap);
    else return nullptr;
  }*/

#endif

}