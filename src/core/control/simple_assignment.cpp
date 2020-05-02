#include "simple_assignment.h"

#include "core/tree/abstract_property.h"
#include "core/tree/bool_property.h"
#include "core/tree/int_property.h"
#include "core/tree/double_property.h"
#include "core/tree/text_property.h"
#include "core/tree/ref_property.h"

#include "core/utils/djnn_dynamic_cast.h"

#include "core/utils/error.h"
#include "core/serializer/serializer.h"

#include "utils/debug.h"
#include <iostream>

//#include <stdarg.h>
extern "C" {
  void DJNN_DEBUG_PRINT(const char* fmt);
}

namespace djnn
{
  using std::string;

  void
  CoreAssignment::perform_action () {
      //_DBG;
      get_dst()->set_activation_source (get_src());
      bool propagate = _propagate;
      AbstractProperty *src_p = djnn_dynamic_cast<AbstractProperty*> (get_src()); // FIXME should be done once and for all
      if(!src_p) { warning (src_p, "not a property"); return; }
      AbstractProperty *dst_p = djnn_dynamic_cast<AbstractProperty*> (get_dst());
      if(!dst_p) { warning (dst_p, "not a property"); return; }
      switch (src_p->get_prop_type ())
        {
        case Integer:
          {
            AbstractIntProperty* ip = djnn_dynamic_cast<AbstractIntProperty*> (src_p);
            if (ip) dst_p->set_value (ip->get_value (), propagate);
            break;
          }
        case Boolean:
          {
            AbstractBoolProperty* bp = djnn_dynamic_cast<AbstractBoolProperty*> (src_p);
            if (bp) dst_p->set_value (bp->get_value (), propagate);
            break;
          }
        case Double:
          {
            AbstractDoubleProperty* dp = djnn_dynamic_cast<AbstractDoubleProperty*> (src_p);
            if (dp) dst_p->set_value (dp->get_value (), propagate);
            break;
          }
        case String:
          {
            AbstractTextProperty* tp = djnn_dynamic_cast<AbstractTextProperty*> (src_p);
            if (tp) dst_p->set_value (string (tp->get_value ()), propagate);
            break;
          }
        case Reference:
          {
            RefProperty* rp = djnn_dynamic_cast<RefProperty*> (src_p);
            if (rp) dst_p->set_value (rp->get_value (), propagate);
            break;
          }
        default:
          warning (src_p, "Unknown property type");
          return;
        }
      get_dst()->activate ();
  }

  void
  SimpleAssignment::perform_action () {
      //_DBG;
      get_dst()->set_activation_source (get_src());
      bool propagate = _propagate;
      AbstractProperty *src_p = djnn_dynamic_cast<AbstractProperty*> (get_src()); // FIXME should be done once and for all
      if(!src_p) { warning (src_p, "not a property"); return; }
      AbstractProperty *dst_p = djnn_dynamic_cast<AbstractProperty*> (get_dst());
      if(!dst_p) { warning (dst_p, "not a property"); return; }
      switch (src_p->get_prop_type ())
        {
        case Integer:
          {
            AbstractIntProperty* ip = djnn_dynamic_cast<AbstractIntProperty*> (src_p);
            if (ip) dst_p->set_value (ip->get_value (), propagate);
            break;
          }
        case Boolean:
          {
            AbstractBoolProperty* bp = djnn_dynamic_cast<AbstractBoolProperty*> (src_p);
            if (bp) dst_p->set_value (bp->get_value (), propagate);
            break;
          }
        case Double:
          {
            AbstractDoubleProperty* dp = djnn_dynamic_cast<AbstractDoubleProperty*> (src_p);
            if (dp) dst_p->set_value (dp->get_value (), propagate);
            break;
          }
        case String:
          {
            AbstractTextProperty* tp = djnn_dynamic_cast<AbstractTextProperty*> (src_p);
            if (tp) dst_p->set_value (string (tp->get_value ()), propagate);
            break;
          }
        case Reference:
          {
            RefProperty* rp = djnn_dynamic_cast<RefProperty*> (src_p);
            if (rp) dst_p->set_value (rp->get_value (), propagate);
            break;
          }
        default:
          warning (src_p, "Unknown property type");
          return;
        }
      get_dst()->activate ();
  }

  #if !defined(DJNN_NO_SERIALIZE)
  void
  CoreAssignment::serialize (const std::string& format)
  {
    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:coreassignment");
    AbstractSerializer::serializer->text_attribute ("id", "(noname)");
    AbstractSerializer::compute_path (get_parent (), get_src (), buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), get_dst (), buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->text_attribute ("model", is_model () ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }

  void
  SimpleAssignment::serialize (const std::string& format)
  {
    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:simpleassignment");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::compute_path (get_parent (), get_src (), buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), get_dst (), buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->text_attribute ("model", is_model () ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }
#endif

}