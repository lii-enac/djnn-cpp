#pragma once

#ifdef DJNN_NO_DYNAMIC_CAST
#include "core/property/abstract_property.h"
#include "core/property/bool_property.h"
#include "core/property/int_property.h"
#include "core/property/double_property.h"
#include "core/property/text_property.h"
#include "core/property/ref_property.h"
#include "core/control/action.h"
#include "core/control/native_action.h"
#include "core/tree/component.h"
#include "core/utils/error.h"
#endif

namespace djnn {

//#ifndef DJNN_NO_DYNAMIC_CAST

  template <typename X, typename P> inline X djnn_dynamic_cast(P p)
  {
    return dynamic_cast<X> (p);
  }

//#endif

#ifdef DJNN_NO_DYNAMIC_CAST

  // simulate dynamic_cast with get_process_type and get_property_type
  /*
  template <typename X> inline X djnn_dynamic_cast(CoreProcess* p)
  {
    // if(p->get_process_type() == PROPERTY_T) {
    //   return static_cast<X>(p);
    // }
    error (p, "unknown djnn_dynamic_cast");
    return nullptr;
  }
  */
  template <> inline AbstractIntProperty* djnn_dynamic_cast(AbstractSimpleProperty* p)
  {
    if(p->get_property_type() == Integer) {
      return static_cast<AbstractIntProperty*>(p);
    }
    return nullptr;
  }

  template <> inline AbstractBoolProperty* djnn_dynamic_cast(AbstractSimpleProperty* p)
  {
    if(p->get_property_type() == Boolean) {
      return static_cast<AbstractBoolProperty*>(p);
    }
    return nullptr;
  }

  template <> inline AbstractDoubleProperty* djnn_dynamic_cast(AbstractSimpleProperty* p)
  {
    if(p->get_property_type() == Double) {
      return static_cast<AbstractDoubleProperty*>(p);
    }
    return nullptr;
  }

  template <> inline AbstractTextProperty* djnn_dynamic_cast(AbstractSimpleProperty* p)
  {
    if(p->get_property_type() == String) {
      return static_cast<AbstractTextProperty*>(p);
    }
    return nullptr;
  }

  template <> inline RefProperty* djnn_dynamic_cast(AbstractSimpleProperty* p)
  {
    if(p->get_property_type() == Reference) {
      return static_cast<RefProperty*>(p);
    }
    return nullptr;
  }

  // specialization for CoreProcess* argument
  template <> inline AbstractBoolProperty* djnn_dynamic_cast(CoreProcess* p)
  {
    AbstractSimpleProperty * ap = djnn_dynamic_cast<AbstractSimpleProperty*>(p);
    if(ap) return djnn_dynamic_cast<AbstractBoolProperty*>(ap);
    else return nullptr;
  }
  template <> inline AbstractIntProperty* djnn_dynamic_cast(CoreProcess* p)
  {
    AbstractSimpleProperty * ap = djnn_dynamic_cast<AbstractSimpleProperty*>(p);
    if(ap) return djnn_dynamic_cast<AbstractIntProperty*>(ap);
    else return nullptr;
  }
    template <> inline AbstractDoubleProperty* djnn_dynamic_cast(CoreProcess* p)
  {
    AbstractSimpleProperty * ap = djnn_dynamic_cast<AbstractSimpleProperty*>(p);
    if(ap) return djnn_dynamic_cast<AbstractDoubleProperty*>(ap);
    else return nullptr;
  }
  template <> inline AbstractTextProperty* djnn_dynamic_cast(CoreProcess* p)
  {
    AbstractSimpleProperty * ap = djnn_dynamic_cast<AbstractSimpleProperty*>(p);
    if(ap) return djnn_dynamic_cast<AbstractTextProperty*>(ap);
    else return nullptr;
  }


  // ************  untested  ***************

  template <> inline Action* djnn_dynamic_cast(CoreProcess* p)
  {
    auto t = p->get_process_type();
    if(t == ACTION_T || t == ACTION_T) {
      return static_cast<Action*>(p);
    }
    return nullptr;
  }

  template <> inline NativeAction* djnn_dynamic_cast(CoreProcess* p)
  {
    if(p->get_process_type() == NATIVE_ACTION_T) {
      return static_cast<NativeAction*>(p);
    }
    return nullptr;
  }

  template <> inline Container* djnn_dynamic_cast(CoreProcess* p)
  {
    if(p->get_process_type() == CONTAINER_T) {
      return static_cast<Container*>(p);
    }
    return nullptr;
  }



  /*
  template <class Res> inline X* djnn_dynamic_cast(CoreProcess* p)
  {
    AbstractSimpleProperty * ap = djnn_dynamic_cast<AbstractSimpleProperty*>(p);
    if(ap) return djnn_dynamic_cast<X*>(ap);
    else return nullptr;
  }*/

#endif

}