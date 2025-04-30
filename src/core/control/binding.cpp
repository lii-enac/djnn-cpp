#include <cassert>

#include "binding.h"
#include "spike.h"
#include "core/tree/component.h"
#include "core/utils/error.h"

#if !defined(DJNN_NO_SERIALIZE)
#include "core/serializer/serializer.h"
#endif

using namespace djnnstl;


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

// TODO



void
MultiBinding (CoreProcess* parent, CoreProcess* src, const vector<djnnstl::string>& src_props, activation_flag_e src_flag, CoreProcess* dst, const vector<djnnstl::string>& dst_props, activation_flag_e dst_flag)
{
#if 1
    // src_n + dst_n bindings + 1 spike
    auto * spike = new Spike(parent, "multibindingspike",0);

    for (size_t i = 0; i < src_props.size (); ++i) {
        CoreProcess* src_prop = src->find_child_impl (src_props[i]);
        if (!src_prop) {
            error (nullptr, "src not found in multiple binding: " + src_props[i]);
        }
        new Binding (parent, "multibind_src", src_prop, src_flag, spike, src_flag);
    }
    for (size_t i = 0; i < dst_props.size (); ++i) {
        CoreProcess* dst_prop = dst->find_child_impl (dst_props[i]);
        if (!dst_prop) {
            error (nullptr, "dst not found in multiple binding: " + dst_props[i]);
        }
        new Binding (parent, "multibind_dst", spike, src_flag, dst_prop, dst_flag);
    }
#else
    // src_n * dst_n bindings
    if (src_props.size () != dst_props.size ()) {
        error (nullptr, "Incompatible number of bindings in multiple binding");
    }
    for (size_t i = 0; i < src_props.size (); ++i) {
        CoreProcess* src_prop = src->find_child_impl (src_props[i]);
        if (!src_prop) {
            error (nullptr, "src property not found in multiple binding: " + src_props[i]);
        }
        for (size_t j = 0; j < dst_props.size (); ++j) {
            CoreProcess* dst_prop = dst->find_child_impl (dst_props[j]);
            if (!dst_prop) {
                error (nullptr, "dst property not found in multiple binding: " + dst_props[j]);
            }
            new Binding (parent, "", src_prop, src_flag, dst_prop, dst_flag);
        }
    }
#endif
}

void
MultiBinding (CoreProcess* parent, CoreProcess* src, const char* src_props[], size_t src_size, activation_flag_e src_flag, CoreProcess* dst, const char* dst_props[], size_t dst_size, activation_flag_e dst_flag)
{
    if (src_size != dst_size) {
        error (nullptr, "Incompatible number of properties in multiple connector");
    }
    for (size_t i = 0; i < src_size; ++i) {
        CoreProcess* src_prop = src->find_child_impl (src_props[i]);
        if (!src_prop) {
            error (nullptr, "src property not found in multiple connector: " + string (src_props[i]));
        }
        CoreProcess* dst_prop = dst->find_child_impl (dst_props[i]);
        if (!dst_prop) {
            error (nullptr, "dst property not found in multiple connector: " + string (dst_props[i]));
        }
        new Binding (parent, "", src_prop, src_flag, dst_prop, dst_flag);
    }
}

// void
// MultiBinding (CoreProcess* parent, CoreProcess* src, activation_flag_e src_flag, CoreProcess* dst,activation_flag_e dst_flag)
// {
//     Container* cont_src = dynamic_cast<Container*> (src);
//     if (cont_src) {
//         for (auto src : cont_src->children ()) {
//             const string& name = src->get_name (cont_src);
//             if (!name.empty()) { // find_child_impl returns "this" if name is empty
//                 CoreProcess*  prop_dst = dst->find_child_impl (name);
//                 if (prop_dst) {
//                     new Binding (parent, "", src, src_flag, prop_dst, dst_flag);
//                 }
//             }
//         }
//     }
//     else
//     for (auto& name : src->get_properties_name ()) {
//         if (!name.empty()) { // find_child_impl returns "this" if name is empty
//             CoreProcess* prop_src = src->find_child_impl (name);
//             CoreProcess* prop_dst = dst->find_child_impl (name);
//             if (prop_src && prop_dst)
//                 new Binding (parent, "", prop_src, src_flag, prop_dst, dst_flag);
//         }
//     }
// }



CoreProcess*
Binding::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const
{
    // check if it already exists in origs_clones? returns it if so?
    auto sit           = origs_clones.find (_b.get_src ());
    auto dit           = origs_clones.find (_b.get_dst ());
    auto s             = sit != origs_clones.end () ? sit->second : const_cast<CoreProcess*> (_b.get_src ()); // the source of the binding can be outside the cloned subtree
    auto d             = dit != origs_clones.end () ? dit->second : const_cast<CoreProcess*> (_b.get_dst ()); // the destination of the binding can be outside the cloned subtree
    auto res           = new Binding (nullptr, name,
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
