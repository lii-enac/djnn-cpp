#include <cassert>

#include "connector.h"
#include "core/utils/error.h"
#include "core/tree/component.h"

#if !defined(DJNN_NO_SERIALIZE)
#include "core/serializer/serializer.h"
#endif

namespace djnn {


  using namespace djnnstl;

  void
  MultiConnector (CoreProcess* parent, CoreProcess* src, const vector<string>& src_props, CoreProcess* dst, const vector<string>& dst_props, bool copy_on_activation)
  {
    if (src_props.size() != dst_props.size ()) {
      error (nullptr, "Incompatible number of properties in multiple connector");
    }
    for (size_t i = 0; i < src_props.size (); ++i) {
      CoreProcess *src_prop = src->find_child_impl (src_props[i]);
      if (!src_prop) {
        error (nullptr, "src property not found in multiple connector: " + src_props[i]);
      }
      CoreProcess *dst_prop = dst->find_child_impl (dst_props[i]);
      if (!dst_prop) {
        error (nullptr, "dst property not found in multiple connector: " + dst_props[i]);
      }
      new Connector (parent, "", src_prop, dst_prop, copy_on_activation);
    }
  }

  void
  MultiConnector (CoreProcess* parent, CoreProcess* src, const char* src_props[], size_t src_size, CoreProcess* dst, const char* dst_props[], size_t dst_size, bool copy_on_activation)
  {
    if (src_size != dst_size) {
      error (nullptr, "Incompatible number of properties in multiple connector");
    }
    for (size_t i = 0; i < src_size; ++i) {
      CoreProcess *src_prop = src->find_child_impl (src_props[i]);
      if (!src_prop) {
        error (nullptr, "src property not found in multiple connector: " + string(src_props[i]));
      }
      CoreProcess *dst_prop = dst->find_child_impl (dst_props[i]);
      if (!dst_prop) {
        error (nullptr, "dst property not found in multiple connector: " + string(dst_props[i]));
      }
      new Connector (parent, "", src_prop, dst_prop, copy_on_activation);
    }
  }

  void
  MultiConnector (CoreProcess* parent, CoreProcess* src, CoreProcess* dst, bool copy_on_activation)
  {
    Container* cont_src = dynamic_cast<Container*>(src);
    Container* cont_dst = dynamic_cast<Container*>(dst);
    if (cont_src && cont_dst) {
      for (auto c: cont_src->children ()) {
        const string& name = c->get_name (c->get_parent ());
        CoreProcess* prop_dst = cont_dst->find_child_impl (name);
        if (dst)
          new Connector (parent, "", c, prop_dst, copy_on_activation);
      }
      return;
    }
    for (auto & c: src->get_properties_name ()) {
      CoreProcess *prop_src = src->find_child_impl (c);
      CoreProcess *prop_dst = dst->find_child_impl (c);
      if (prop_src && prop_dst)
        new Connector (parent, "", prop_src, prop_dst, copy_on_activation);
    }
  }

  Connector*
  Connector::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
  {
    auto sit = origs_clones.find(_assignment.get_src ());
    auto dit = origs_clones.find(_assignment.get_dst ());
    auto s = sit!=origs_clones.end() ? sit->second : const_cast<CoreAssignment&>(_assignment).get_src (); // the source of the connector can be outside the cloned subtree
    auto d = dit!=origs_clones.end() ? dit->second : const_cast<CoreAssignment&>(_assignment).get_dst (); // the destination of the connector can be outside the cloned subtree
    auto res = new Connector (nullptr, get_name (), s, d, _copy_on_activation);
    origs_clones[this] = res;
    return res;
  }

#if !defined(DJNN_NO_SERIALIZE)
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
  CoreLazyConnector::serialize (const string& format)
  {
    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("base:corelazyconnector");
    AbstractSerializer::serializer->text_attribute ("id", "(noname)"); // FatProcess
    //AbstractSerializer::serializer->text_attribute ("id", ""); // CoreProcess
    AbstractSerializer::compute_path (get_parent (), _binding.get_src (), buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _lazy_assignment.get_dst (), buf);
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

    AbstractSerializer::serializer->start ("base:PauseConnector");
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

  void
  LazyConnector::serialize (const string& format)
  {
    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("base:LazyConnector");
    AbstractSerializer::serializer->text_attribute ("id", get_name ()); // FatProcess
    //AbstractSerializer::serializer->text_attribute ("id", ""); // CoreProcess
    AbstractSerializer::compute_path (get_parent (), _binding.get_src (), buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _lazy_assignment.get_dst (), buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }
  
#endif
}
