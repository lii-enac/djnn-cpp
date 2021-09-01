#include <cassert>

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
//#include <_iostream>

namespace djnn
{

  // FIXME: share the following code with other parts of the lib that cast from one type to another
  inline
  int
  s_to_p(const string& s, int)
  {
    return stoi(s);
  }

  inline
  double
  s_to_p(const string& s, double)
  {
    return stof(s) ? true : false;
  }

  inline
  bool
  s_to_p(const string& s, bool)
  {
    return stoi(s) ? true : false;
  }

  template <class X>
  string
  p_to_s(const X& x)
  {
    return to_string(x);
  }

  template <>
  string
  p_to_s<bool>(const bool& x)
  {
    return (x ? "true" : "false");
  }


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
      dst->set_value (sv, propagate);
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
  template <>
  void t_perform_action (RemoteProperty* src, AbstractProperty* dst, double propagate, double lazy)
  {
    const auto & sv = src;
    const auto & dv = dst;
    if (lazy && (sv == dv) ) return; 
    dst->set_value (sv, propagate);
  }

  // templated TAssignement
  template <class src_t, class dst_t>
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
    src_t* _src;
    dst_t* _dst;
  };


  // building a TAssignment is a 2 part function for int, double, bool, string

  template <class src_t>
  TTAssignment*
  build_ttassignment2 (src_t * src, CoreProcess* dst)
  {
    TTAssignment* ttassignment = nullptr;
    AbstractProperty *dst_p = djnn_dynamic_cast<AbstractProperty*> (dst);
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
    AbstractProperty *src_p = djnn_dynamic_cast<AbstractProperty*> (src);
    AbstractProperty *dst_p = djnn_dynamic_cast<AbstractProperty*> (dst);
    if (!dst_p) { warning (dst_p, "dst is not a property"); return nullptr; }

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
          auto * srp = djnn_dynamic_cast<RemoteProperty*> (src_p);
          assert (srp);
          auto * drp = djnn_dynamic_cast<AbstractProperty*> (dst_p);
          if (!drp) {
            warning (dst_p, "is not a Property in assignment");
            return nullptr;
          }
          _ttassignment = new TAssignment <RemoteProperty, AbstractProperty> (srp, drp);
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
    AbstractProperty *src_p = djnn_dynamic_cast<AbstractProperty*> (src);
    AbstractProperty *dst_p = djnn_dynamic_cast<AbstractProperty*> (dst);
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
          RemoteProperty* dp = djnn_dynamic_cast<RemoteProperty*> (src_p);
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
    //djnn::perform_action (get_src (), get_dst (), _propagate);
    _ttassignment->perform_action (_propagate, false);
  }

  void
  Assignment::perform_action ()
  {
    //djnn::perform_action (get_src (), get_dst (), _propagate);
    if (!_ttassignment) {
      _ttassignment = build_ttassignment (get_src (), get_dst ());
      if (!_ttassignment) {
        warning (this, "invalid assignment");
        return;
      }
    }
    _ttassignment->perform_action (_propagate, _lazy);
  }

  void
  MultiAssignment (ParentProcess* parent, CoreProcess* src, vector <string> src_props, CoreProcess* dst, vector <string> dst_props, bool copy_on_activation)
  {
    if (src_props.size() != dst_props.size ()) {
      error (src, "Incompatible number of properties in multiple assignment");
    }
    for (size_t i = 0; i < src_props.size (); ++i) {
      CoreProcess *src_prop = src->find_child_impl (src_props[i]);
      CoreProcess *dst_prop = dst->find_child_impl (dst_props[i]);
      if (src_prop && dst_prop) {
        new CoreAssignment (parent, "", src_prop, dst_prop, copy_on_activation);
      }
      else {
        error (src, "Property not found in multiple assignment: " + src_props[i] + " or " + dst_props[i]);
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
        string name = c->get_name (c->get_parent ());
        CoreProcess* prop_dst = cont_dst->find_child_impl (name);
        if (dst)
          new CoreAssignment (parent, "", c, prop_dst, copy_on_activation);
      }
      return;
    }
    for (auto & c: src->get_properties_name ()) {
      CoreProcess *prop_src = src->find_child_impl (c);
      CoreProcess *prop_dst = dst->find_child_impl (c);
      if (prop_src && prop_dst)
        new CoreAssignment (parent, "", prop_src, prop_dst, copy_on_activation);
    }
  }

  #if !defined(DJNN_NO_SERIALIZE)
  void
  CoreAssignment::serialize (const string& format)
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
