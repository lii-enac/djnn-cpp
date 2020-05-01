#include "simple_connector.h"

#if !defined(DJNN_NO_SERIALIZE)
#include "core/serializer/serializer.h"
#endif

namespace djnn {
#if !defined(DJNN_NO_SERIALIZE)

  using std::string;

  void
  SimpleConnector::serialize (const std::string& format)
  {
    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("base:simpleconnector");
    AbstractSerializer::serializer->text_attribute ("id", get_name ()); // Process
    //AbstractSerializer::serializer->text_attribute ("id", ""); // CoreProcess
    AbstractSerializer::compute_path (get_parent (), _binding.get_src (), buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _assignment.get_dst (), buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }
#endif
}