#include <cassert>
#include <stdexcept>	

#include "assignment.h"

#include "core/property/remote_property.h"
#include "core/property/abstract_property.h"
#include "core/property/bool_property.h"
#include "core/property/int_property.h"
#include "core/property/double_property.h"
#include "core/property/text_property.h"
#include "core/property/ref_property.h"
#include "core/property/s_to_p.h"
#include "core/tree/component.h"

#include "core/utils/djnn_dynamic_cast.h"

#include "core/utils/error.h"
#include "core/serializer/serializer.h"

//#include "utils/debug.h"
//#include <_iostream>

namespace djnn
{
  // implicit conversion int, double, bool
  template <typename src_t, typename dst_t>
  void t_perform_action (src_t* src, dst_t* dst, double propagate, double lazy)
  {
    const auto & sv = src->get_value ();
    const auto & dv = dst->get_value ();
    if (lazy && (sv == dv) ) return;
    dst->set_value (sv, propagate);
  }

  // conversion from string to int, double, bool
  template <typename dst_t>
  void t_perform_action (AbstractTextProperty* src, dst_t* dst, double propagate, double lazy)
  {
    const auto & sv = src->get_value ();
    const auto & dv = dst->get_value ();
    if (lazy) {
      const auto sdv = s_to_p(sv, dv);
      if (sdv == dv) return;
      dst->set_value (sdv, propagate);
    } else {
      try {
        const auto sdv = s_to_p(sv, dv);
        dst->set_value (sdv, propagate);
      }
      catch (const std::invalid_argument& ia) {
      }
    }
  }

  // conversion from int, double, bool to string
  template <typename src_t>
  void t_perform_action (src_t* src, AbstractTextProperty* dst, double propagate, double lazy)
  {
    const auto & sv = src->get_value ();
    const auto & dv = dst->get_value ();
    const auto sdv = p_to_s (sv);
    if (lazy && (sdv == dv) ) return; 
    dst->set_value (sdv, propagate);
  }

  // string to string is ambiguous because of the above partial specialization, so provide one with overloading (and not template)
  //template <>
  void t_perform_action (AbstractTextProperty* src, AbstractTextProperty* dst, double propagate, double lazy)
  {
    const auto & sv = src->get_value ();
    const auto & dv = dst->get_value ();
    if (lazy && (sv == dv) ) return; 
    dst->set_value (sv, propagate);
  }

  // specific case when assigning a CoreProcess to a RefProperty
  template <>
  void t_perform_action (CoreProcess* src, RefProperty* dst, double propagate, double lazy)
  {
    const auto & sv = src;
    const auto & dv = dst;
    if (lazy && (sv == dv) ) return; 
    dst->set_value (sv, propagate);
  }

  // specific case when assigning a RemoteProcess to a Property
  template <typename dst_t>
  void t_perform_action (AbstractRemoteProperty* src, dst_t* dst, double propagate, double lazy)
  {
    const auto & sv = src->get_value ();
    const auto & dv = dst->get_string_value ();
    if (lazy) {
      //const auto sdv = s_to_p(sv, dv);
      if (sv == dv) return;
      dst->set_value (sv, propagate);
    } else {
      dst->set_value (sv, propagate);
    }
  }

  // specific case when assigning a Property to a RemoteProcess
  template <typename src_t>
  void t_perform_action (src_t* src, AbstractRemoteProperty* dst, double propagate, double lazy)
  {
    const auto & sv = src->get_string_value ();
    const auto & dv = dst->get_value ();
    //const auto sdv = p_to_s (sv);
    if (lazy && (sv == dv) ) return; 
    dst->set_value (sv, propagate);
  }

  // string to remote is ambiguous because of the above text partial specialization, so provide one with overloading (and not template)
  //template <>
  void t_perform_action (AbstractTextProperty* src, AbstractRemoteProperty* dst, double propagate, double lazy)
  {
    const auto & sv = src->get_value ();
    const auto & dv = dst->get_value ();
    if (lazy && (sv == dv) ) return; 
    dst->set_value (sv, propagate);
  }

  // templated TAssignement
  template <typename src_t, typename dst_t>
  struct TAssignment : public TTAssignment {
    TAssignment (src_t* src, dst_t* dst)
    : _src (src)
    , _dst (dst)
    {
    }

    virtual void perform_action (double propagate = true, double lazy = false) override
    {
      t_perform_action (_src, _dst, propagate, lazy);
    }
    src_t* get_src () override { return _src; }
    dst_t* get_dst () override { return _dst; }
    const src_t* get_src () const override { return _src; }
    const dst_t* get_dst () const override { return _dst; }
    src_t* _src;
    dst_t* _dst;
  };


  // building a TAssignment is a 2 part function for int, double, bool, string

  template <typename src_t>
  TTAssignment*
  build_ttassignment2 (src_t * src, CoreProcess* dst)
  {
    TTAssignment* ttassignment = nullptr;
    auto *dst_p = djnn_dynamic_cast<AbstractSimpleProperty*> (dst);
    if (dst_p)
    switch (dst_p->get_prop_type ())
    {
      case Integer:
      {
        auto * dp = djnn_dynamic_cast<AbstractIntProperty*> (dst_p);
        assert (dp);
        ttassignment = new TAssignment <src_t, AbstractIntProperty> (src, dp);
        break;
      }
      case Boolean:
      {
        auto * bp = djnn_dynamic_cast<AbstractBoolProperty*> (dst_p);
        assert (bp);
        ttassignment = new TAssignment <src_t, AbstractBoolProperty> (src, bp);
        break;
      }
      case Double:
      {
        auto * dp = djnn_dynamic_cast<AbstractDoubleProperty*> (dst_p);
        assert (dp);
        ttassignment = new TAssignment <src_t, AbstractDoubleProperty> (src, dp);
        break;
      }
      case String:
      {
        auto * tp = djnn_dynamic_cast<AbstractTextProperty*> (dst_p);
        assert (tp);
        ttassignment = new TAssignment <src_t, AbstractTextProperty> (src, tp);
        break;
      }
      case Remote:
      {
        auto * rp = djnn_dynamic_cast<AbstractRemoteProperty*> (dst_p);
        assert (rp);
        ttassignment = new TAssignment <src_t, AbstractRemoteProperty> (src, rp);
        break;
      }
      default:
        warning (dst_p, "Unknown property type");
        break;
    }
    
    return ttassignment;
  }

  TTAssignment *
  build_ttassignment (CoreProcess* src, CoreProcess* dst)
  {
    TTAssignment * _ttassignment = nullptr;
    auto *src_p = djnn_dynamic_cast<AbstractSimpleProperty*> (src);
    auto *dst_p = djnn_dynamic_cast<AbstractSimpleProperty*> (dst);
    if (!dst_p) { warning (dst_p, "dst " + dst->get_debug_name() + " is not a property"); return nullptr; }

    if (src_p) {
      switch (src_p->get_prop_type ())
      {
        case Integer:
        {
          auto * sp = djnn_dynamic_cast<AbstractIntProperty*> (src_p);
          assert (sp);
          _ttassignment = build_ttassignment2 (sp, dst);
          break;
        }
        case Boolean:
        {
          auto * bp = djnn_dynamic_cast<AbstractBoolProperty*> (src_p);
          assert (bp);
          _ttassignment = build_ttassignment2 (bp, dst_p);
          break;
        }
        case Double:
        {
          auto * dp = djnn_dynamic_cast<AbstractDoubleProperty*> (src_p);
          assert (dp);
          _ttassignment = build_ttassignment2 (dp, dst_p);
          break;
        }
        case String:
        {
          auto * tp = djnn_dynamic_cast<AbstractTextProperty*> (src_p);
          assert (tp);
          _ttassignment = build_ttassignment2 (tp, dst_p);
          break;
        }
        case Reference:
        {
          auto * srp = djnn_dynamic_cast<RefProperty*> (src_p);
          assert (srp);
          auto * drp = djnn_dynamic_cast<RefProperty*> (dst_p);
          if (!drp) {
            warning (dst_p, "is not a Reference in assignment");
            return nullptr;
          }
          _ttassignment = new TAssignment <RefProperty, RefProperty> (srp, drp);
          break;
        }
        case Remote: // FIXME name
        {
          auto * srp = djnn_dynamic_cast<AbstractRemoteProperty*> (src_p);
          assert (srp);
          auto * drp = djnn_dynamic_cast<AbstractSimpleProperty*> (dst_p);
          if (!drp) {
            warning (dst_p, "is not a Property in assignment");
            return nullptr;
          }
          _ttassignment = new TAssignment <AbstractRemoteProperty, AbstractSimpleProperty> (srp, drp);
          break;
        }
        default:
          warning (src_p, "unknown Property type");
          return nullptr;
      }
    } else if (dst_p->get_prop_type () == Reference) {
      auto * drp = djnn_dynamic_cast<RefProperty*> (dst_p);
      assert (drp);
      _ttassignment = new TAssignment <CoreProcess, RefProperty> (src, drp);

    } else {

      warning (src, "incompatible source and destination in Assignment \n");

    }

    return _ttassignment;
  }

// old assignment code
#if 0
  static void
  perform_action (CoreProcess * src, CoreProcess * dst, bool propagate)
  {
    AbstractSimpleProperty *src_p = djnn_dynamic_cast<AbstractSimpleProperty*> (src);
    AbstractSimpleProperty *dst_p = djnn_dynamic_cast<AbstractSimpleProperty*> (dst);
    if (!dst_p) { warning (dst_p, "dst is not a Property"); return; }

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
        case Remote:
        {
          RemoteProperty* dp = djnn_dynamic_cast<AbstractRemoteProperty*> (src_p);
          if (dp) dst_p->set_value (dp->get_value (), propagate);
          break;
        }
        default:
        warning (src_p, "unknown Property type");
        return;
      }
    } else if (dst_p->get_prop_type () == Reference) {

      ((RefProperty*) dst_p)->set_value (src, propagate);

    } else {

      warning (src, "incompatible source and destination in (Paused)Assignment \n");

    }
  }
#endif

  void
  CoreAssignment::perform_action ()
  {
    _ttassignment->perform_action (true, false);
  }

  void
  CorePausedAssignment::perform_action ()
  {
    _ttassignment->perform_action (false, false);
  }

  void
  CoreLazyAssignment::perform_action ()
  {
    _ttassignment->perform_action (true, true);
  }

  void
  Assignment::perform_action ()
  {
    if (!_ttassignment) {
      _ttassignment = build_ttassignment (get_src (), get_dst ());
      if (!_ttassignment) {
        warning (this, "invalid assignment");
        return;
      }
    }
    _ttassignment->perform_action (_propagate, _lazy);
  }

  Assignment* Assignment::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
  {
    auto sit = origs_clones.find (_src);
    auto dit = origs_clones.find (_dst);
    auto s = sit != origs_clones.end () ? sit->second : _src; // the source of the assignment can be outside the cloned subtree
    auto d = dit != origs_clones.end () ? dit->second : _dst; // the destination of the assignment can be outside the cloned subtree
    auto res = new Assignment (nullptr, get_name (), s, d, is_model ());
    origs_clones[this] = res;
    return res;
  }

  void
  MultiAssignment (CoreProcess* parent, CoreProcess* src, const vector<string>& src_props, CoreProcess* dst, const vector<string>& dst_props, bool copy_on_activation)
  {
    if (src_props.size() != dst_props.size ()) {
      error (src, "Incompatible number of properties in multiple assignment");
    }
    for (size_t i = 0; i < src_props.size (); ++i) {
      CoreProcess *src_prop = src->find_child_impl (src_props[i]);
      if (!src_prop) {
        error (nullptr, "dst property not found in multiple connector: " + src_props[i]);
      }
      CoreProcess *dst_prop = dst->find_child_impl (dst_props[i]);
      if (!dst_prop) {
        error (nullptr, "dst property not found in multiple connector: " + dst_props[i]);
      }
      new Assignment (parent, "toto", src_prop, dst_prop, copy_on_activation);
    }
  }

  void
  MultiAssignment (CoreProcess* parent, CoreProcess* src, const char* src_props[], size_t src_size, CoreProcess* dst, const char* dst_props[], size_t dst_size, bool copy_on_activation)
  {
    if (src_size != dst_size) {
      error (src, "Incompatible number of properties in multiple assignment");
    }
    for (size_t i = 0; i < src_size; ++i) {
      CoreProcess *src_prop = src->find_child_impl (src_props[i]);
      if (!src_prop) {
        error (nullptr, "dst property not found in multiple connector: " + string(src_props[i]));
      }
      CoreProcess *dst_prop = dst->find_child_impl (dst_props[i]);
      if (!dst_prop) {
        error (nullptr, "dst property not found in multiple connector: " + string(dst_props[i]));
      }
      new Assignment (parent, "toto", src_prop, dst_prop, copy_on_activation);
    }
  }

  void
  MultiAssignment (CoreProcess* parent, CoreProcess* src, CoreProcess* dst, bool copy_on_activation)
  {
    Container* cont_src = dynamic_cast<Container*>(src);
    Container* cont_dst = dynamic_cast<Container*>(dst);
    if (cont_src && cont_dst) {
      for (auto c: cont_src->children ()) {
        const string& name = c->get_name (c->get_parent ());
        CoreProcess* prop_dst = cont_dst->find_child_impl (name);
        if (dst)
          new Assignment (parent, "", c, prop_dst, copy_on_activation);
      }
      return;
    }
    for (auto & c: src->get_properties_name ()) {
      CoreProcess *prop_src = src->find_child_impl (c);
      CoreProcess *prop_dst = dst->find_child_impl (c);
      if (prop_src && prop_dst)
        new Assignment (parent, "", prop_src, prop_dst, copy_on_activation);
    }
  }

  #if !defined(DJNN_NO_SERIALIZE)
  void
  AbstractCoreAssignment::serialize (const string& format)
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
  Assignment::serialize (const string& format)
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
