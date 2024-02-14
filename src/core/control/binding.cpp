#include "binding.h"

#include <cassert>

#if !defined(DJNN_NO_SERIALIZE)
#include "core/serializer/serializer.h"
#endif

namespace djnn {

void
Binding::set_parent (CoreProcess* parent)
{
    /* in case of re-parenting remove edge dependency in graph */
    auto* dst = _b.get_dst ();
    if (get_parent () && dst) {
        remove_state_dependency (get_parent (), dst);
    }

    if (dst)
        add_state_dependency (parent, dst);

    FatProcess::set_parent (parent);
}

Binding::~Binding ()
{
    auto* dst = _b.get_dst ();
    if (get_parent () && dst) {
        remove_state_dependency (get_parent (), dst);
    }
}
CoreProcess*
Binding::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    // check if it already exists in origs_clones? returns it if so?
    auto sit           = origs_clones.find (_b.get_src ());
    auto dit           = origs_clones.find (_b.get_dst ());
    auto s             = sit != origs_clones.end () ? sit->second : const_cast<CoreProcess*> (_b.get_src ()); // the source of the binding can be outside the cloned subtree
    auto d             = dit != origs_clones.end () ? dit->second : const_cast<CoreProcess*> (_b.get_dst ()); // the destination of the binding can be outside the cloned subtree
    auto res           = new Binding (nullptr, get_name (),
                                      s, _b.get_src_flag (),
                                      d, _b.get_dst_flag ());
    origs_clones[this] = res;
    return res;
}

#if !defined(DJNN_NO_SERIALIZE)
void
CoreBinding::serialize (const string& format)
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
Binding::serialize (const string& format)
{
    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:Binding");
    // AbstractSerializer::serializer->text_attribute ("id", get_name ()); // FatProcess
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
} // namespace djnn
