
#include "assignment.h"

#include "core/tree/remote_property.h"
#include "core/tree/abstract_property.h"
#include "core/tree/bool_property.h"
#include "core/tree/int_property.h"
#include "core/tree/double_property.h"
#include "core/tree/text_property.h"
#include "core/tree/ref_property.h"
#include "core/tree/component.h"

#include "core/utils/djnn_dynamic_cast.h"

#include "core/utils/error.h"
#include "core/serializer/serializer.h"

//#include "utils/debug.h"
//#include <iostream>

namespace djnn
{
  using std::string;

  static void
  perform_action (CoreProcess * src, CoreProcess * dst, bool propagate)
  {
    AbstractProperty *src_p = djnn_dynamic_cast<AbstractProperty*> (src);
    AbstractProperty *dst_p = djnn_dynamic_cast<AbstractProperty*> (dst);
    if (!dst_p) { warning (dst_p, "dst is not a property"); return; }

    if (src_p) {
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
        case Dist:
        {
          RemoteProperty* dp = djnn_dynamic_cast<RemoteProperty*> (src_p);
          if (dp) dst_p->set_value (dp->get_value (), propagate);
          break;
        }
        default:
        warning (src_p, "Unknown property type");
        return;
      }
    } else if (dst_p->get_prop_type () == Reference) {

      ((RefProperty*) dst_p)->set_value (src, propagate);

    } else {

      warning (nullptr, "incompatible source and destination in (Paused)assignment \n");

    }
  }

  void
  CoreAssignment::perform_action ()
  {
    djnn::perform_action (get_src (), get_dst (), _propagate);    
  }

  void
  Assignment::perform_action ()
  {
    djnn::perform_action (get_src (), get_dst (), _propagate);    
  }

  void
  MultiAssignment (ParentProcess* parent, CoreProcess* src, std::vector <std::string> src_props, CoreProcess* dst, std::vector <std::string> dst_props, bool copy_on_activation)
  {
    if (src_props.size() != dst_props.size ()) {
      error (nullptr, "Incompatible number of properties in multiple assignment");
    }
    for (int i = 0; i < src_props.size (); i++) {
      CoreProcess *src_prop = src->find_child_impl (src_props[i]);
      CoreProcess *dst_prop = dst->find_child_impl (dst_props[i]);
      if (src_prop && dst_prop) {
        new CoreAssignment (parent, "", src_prop, dst_prop, copy_on_activation);
      }
      else {
        error (nullptr, "Property not found in multiple assignment: " + src_props[i] + " or " + dst_props[i]);
      }
    }
  }

  void
  MultiAssignment (ParentProcess* parent, CoreProcess* src, CoreProcess* dst, bool copy_on_activation)
  {
    Container* cont_src = dynamic_cast<Container*>(src);
    Container* cont_dst = dynamic_cast<Container*>(dst);
    if (cont_src && cont_dst) {
      for (auto c: cont_src->children ()) {
        std::string name = c->get_name (c->get_parent ());
        CoreProcess* prop_dst = cont_dst->find_child_impl (name);
        if (dst)
          new CoreAssignment (parent, "", c, prop_dst, copy_on_activation);
      }
      return;
    }
    for (auto c: src->get_properties_name ()) {
      CoreProcess *prop_src = src->find_child_impl (c);
      CoreProcess *prop_dst = dst->find_child_impl (c);
      if (prop_src && prop_dst)
        new CoreAssignment (parent, "", prop_src, prop_dst, copy_on_activation);
    }
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
  Assignment::serialize (const std::string& format)
  {
    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:Assignment");
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
