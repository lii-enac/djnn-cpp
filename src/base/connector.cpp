#include "connector.h"
#include "core/utils/error.h"
#include "core/tree/component.h"

#if !defined(DJNN_NO_SERIALIZE)
#include "core/serializer/serializer.h"
#endif

namespace djnn {
#if !defined(DJNN_NO_SERIALIZE)

  void
  MultiConnector (ParentProcess* parent, CoreProcess* src, vector <string> src_props, CoreProcess* dst, vector <string> dst_props, bool copy_on_activation)
  {
    if (src_props.size() != dst_props.size ()) {
      error (nullptr, "Incompatible number of properties in multiple connector");
    }
    for (size_t i = 0; i < src_props.size (); ++i) {
      CoreProcess *src_prop = src->find_child_impl (src_props[i]);
      CoreProcess *dst_prop = dst->find_child_impl (dst_props[i]);
      if (src_prop && dst_prop) {
        new CoreConnector (parent, "", src_prop, dst_prop, copy_on_activation);
      }
      else {
        error (nullptr, "Property not found in multiple connector: " + src_props[i] + " or " + dst_props[i]);
      }
    }
  }

  void
  MultiConnector (ParentProcess* parent, CoreProcess* src, CoreProcess* dst, bool copy_on_activation)
  {
    Container* cont_src = dynamic_cast<Container*>(src);
    Container* cont_dst = dynamic_cast<Container*>(dst);
    if (cont_src && cont_dst) {
      for (auto c: cont_src->children ()) {
        string name = c->get_name (c->get_parent ());
        CoreProcess* prop_dst = cont_dst->find_child_impl (name);
        if (dst)
          new CoreConnector (parent, "", c, prop_dst, copy_on_activation);
      }
      return;
    }
    for (auto & c: src->get_properties_name ()) {
      CoreProcess *prop_src = src->find_child_impl (c);
      CoreProcess *prop_dst = dst->find_child_impl (c);
      if (prop_src && prop_dst)
        new CoreConnector (parent, "", prop_src, prop_dst, copy_on_activation);
    }
  }

  void
  CoreConnector::serialize (const string& format)
  {
    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("base:coreconnector");
    AbstractSerializer::serializer->text_attribute ("id", "(noname)"); // FatProcess
    //AbstractSerializer::serializer->text_attribute ("id", ""); // CoreProcess
    AbstractSerializer::compute_path (get_parent (), _binding.get_src (), buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _assignment.get_dst (), buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }

  void
  CorePausedConnector::serialize (const string& format)
  {
    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("base:corepausedconnector");
    AbstractSerializer::serializer->text_attribute ("id", "(noname)"); // FatProcess
    //AbstractSerializer::serializer->text_attribute ("id", ""); // CoreProcess
    AbstractSerializer::compute_path (get_parent (), _binding.get_src (), buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _paused_assignment.get_dst (), buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }

  void
  Connector::serialize (const string& format)
  {
    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("base:Connector");
    AbstractSerializer::serializer->text_attribute ("id", get_name ()); // FatProcess
    //AbstractSerializer::serializer->text_attribute ("id", ""); // CoreProcess
    AbstractSerializer::compute_path (get_parent (), _binding.get_src (), buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _assignment.get_dst (), buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }

  void
  PausedConnector::serialize (const string& format)
  {
    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("base:Connector");
    AbstractSerializer::serializer->text_attribute ("id", get_name ()); // FatProcess
    //AbstractSerializer::serializer->text_attribute ("id", ""); // CoreProcess
    AbstractSerializer::compute_path (get_parent (), _binding.get_src (), buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _paused_assignment.get_dst (), buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }
#endif
}
