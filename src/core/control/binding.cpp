#include "binding.h"

#if !defined(DJNN_NO_SERIALIZE)
#include "core/serializer/serializer.h"
#endif

namespace djnn {

  using std::string;

  void
  Binding::set_parent (ParentProcess* parent)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    auto * dst = _b.get_dst ();
    if (get_parent () && dst) {
       remove_state_dependency (get_parent (), dst);
    }

    if (dst)
      add_state_dependency (parent, dst);

    FatProcess::set_parent (parent); 
  }

#if !defined(DJNN_NO_SERIALIZE)
  void
  CoreBinding::serialize (const std::string& format)
  {
    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:corebinding");
    AbstractSerializer::serializer->text_attribute ("id", "(noname)");
    AbstractSerializer::compute_path (get_src ()->get_parent (), get_src (), buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_dst ()->get_parent (), get_dst (), buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }

  void
  Binding::serialize (const std::string& format)
  {
    string buf;
  
    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:Binding");
    //AbstractSerializer::serializer->text_attribute ("id", get_name ()); // FatProcess
    AbstractSerializer::serializer->text_attribute ("id", ""); // CoreProcess
    AbstractSerializer::compute_path (CouplingProcess::get_parent (), _b.get_src (), buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (CouplingProcess::get_parent (), _b.get_dst (), buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }
#endif
}