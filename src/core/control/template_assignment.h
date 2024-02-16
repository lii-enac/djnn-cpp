/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <mathieu.stephane.conversy@enac.fr>
 */

#pragma once

#include "core/control/action.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/ontology/coupling.h"
#include "core/ontology/process.h"
#include "core/property/template_property.h"

// #include "core/utils/error.h"

namespace djnn {

// CoreAssignment does not follow the process/action model for memory minimization reason
template <typename X, typename Y>
class TAbstractCoreAssignment : public CoreProcess {
  public:
    TAbstractCoreAssignment (X* src, Y* dst, bool is_model = false)
        : CoreProcess (is_model), _src (src), _dst (dst) {
        graph_add_edge (src, dst);
        //_ttassignment = build_ttassignment (src, dst);
    }
    TAbstractCoreAssignment (CoreProcess* parent, const string& name, X* src, Y* dst, bool is_model = false)
        : CoreProcess (src, dst, is_model) {
        finalize_construction (parent, name);
    }

    virtual ~TAbstractCoreAssignment () {
        graph_remove_edge (get_src (), get_dst ());
    }

    void set_parent (CoreProcess* parent) override {
        // be careful !
        // CoreAssignment do not have any parent to manage
    }

    X*           get_src () { return _src; /*_ttassignment->get_src ();*/ }
    Y*           get_dst () { return _dst; /*_ttassignment->get_dst ();*/ }
    virtual void perform_action () = 0;

  protected:
    void impl_activate () override { perform_action (); }
    void impl_deactivate () override {}
    void post_activate () override { post_activate_auto_deactivate (); }

    X* _src;
    Y* _dst;

    // TTAssignment * _ttassignment;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override {}
#endif
};

template <typename X, typename Y>
class TCoreAssignment : public TAbstractCoreAssignment<X, Y> {
  public:
    TCoreAssignment (X* src, Y* dst, bool is_model = false)
        : TAbstractCoreAssignment<X, Y> (src, dst, is_model) {
    }
    TCoreAssignment (CoreProcess* parent, const string& name, X* src, Y* dst, bool is_model = false)
        : TAbstractCoreAssignment<X, Y> (parent, name, src, dst, is_model) {
    }
    void perform_action () override {
        const auto& sv = TAbstractCoreAssignment<X, Y>::get_src ()->get_value ();
        const auto& dv = TAbstractCoreAssignment<X, Y>::get_dst ()->get_value ();
        // if (lazy && (sv == dv) ) return;
        TAbstractCoreAssignment<X, Y>::get_dst ()->set_value (sv, true);
    }
};

template <typename X, typename Y>
class TCorePausedAssignment : public TAbstractCoreAssignment<X, Y> {
  public:
    TCorePausedAssignment (X* src, Y* dst, bool is_model = false)
        : TAbstractCoreAssignment<X, Y> (src, dst, is_model) {
    }
    TCorePausedAssignment (CoreProcess* parent, const string& name, X* src, Y* dst, bool is_model = false)
        : TAbstractCoreAssignment<X, Y> (parent, name, src, dst, is_model) {
    }
    void perform_action () override {
        const auto& sv = TAbstractCoreAssignment<X, Y>::get_src ()->get_value ();
        const auto& dv = TAbstractCoreAssignment<X, Y>::get_dst ()->get_value ();
        // if (lazy && (sv == dv) ) return;
        TAbstractCoreAssignment<X, Y>::get_dst ()->set_value (sv, false);
    }
};

template <typename X, typename Y>
class TCoreLazyAssignment : public TAbstractCoreAssignment<X, Y> {
  public:
    TCoreLazyAssignment (X* src, Y* dst, bool is_model = false)
        : TAbstractCoreAssignment<X, Y> (src, dst, is_model) {
    }
    TCoreLazyAssignment (CoreProcess* parent, const string& name, X* src, Y* dst, bool is_model = false)
        : TAbstractCoreAssignment<X, Y> (parent, name, src, dst, is_model) {
    }
    void perform_action () override {
        const auto& sv = TAbstractCoreAssignment<X, Y>::get_src ()->get_value ();
        const auto& dv = TAbstractCoreAssignment<X, Y>::get_dst ()->get_value ();
        if (sv == dv)
            return;
        TAbstractCoreAssignment<X, Y>::get_dst ()->set_value (sv, true);
    }
};

// Assignement follows the process/action model
template <typename X, typename Y>
class TAssignment : public FatProcess {
    friend class AssignmentAction;

  private:
    class AssignmentAction : public Action {
      public:
        AssignmentAction (CoreProcess* parent, const string& name)
            : Action (parent, name) { /*finalize_construction (parent, name);*/
        }
        void impl_activate () override { (static_cast<TAssignment<X, Y>*> (get_parent ()))->perform_action (); }
    };

  public:
    TAssignment (CoreProcess* parent, const string& name, X* src, Y* dst, bool is_model = false)
        : FatProcess (name, is_model), _src (src), _dst (dst), _action (this, "action")
          //, _ttassignment(nullptr) // FIXME
          ,
          _propagate (true), _lazy (false) {
        graph_add_edge (src, dst);
        finalize_construction (parent, name);
    }

    virtual ~TAssignment () {
        graph_remove_edge (get_src (), get_dst ());
        if (get_parent ()) {
            remove_state_dependency (get_parent (), get_dst ());
        }
        // delete _ttassignment;
    }

    void set_parent (CoreProcess* parent) override {
        /* in case of re-parenting remove edge dependency in graph */
        if (get_parent ()) {
            remove_state_dependency (get_parent (), get_dst ());
        }
        add_state_dependency (parent, get_dst ());
        FatProcess::set_parent (parent);
    }

    // for legacy reason, to get rid of?
    /*Assignment (CoreProcess* parent, const string& name,
                  CoreProcess* src, const string& sspec,
                  CoreProcess* dst, const string& dspec,
                  bool is_model=false)
    : Assignment (parent, name, src->find_child_impl (sspec), dst->find_child_impl (dspec), is_model)
    {}*/

    void impl_activate () override { _action.activate (); }
    void impl_deactivate () override {}
    void post_activate () override { post_activate_auto_deactivate (); }

    // FIXME shoud be per class to avoid 2 bool, or use templated arguments
    void perform_action () {
        const auto& sv = _src->get_value ();
        const auto& dv = _dst->get_value ();
        if (_lazy) {
            if (sv == dv)
                return;
            _dst->set_value (sv, _propagate);
        } else {
            _dst->set_value (sv, _propagate);
        }
    }

    X* get_src () { return _src; }
    Y* get_dst () { return _dst; }

  private:
    X*               _src;
    Y*               _dst;
    AssignmentAction _action;
    // TTAssignment * _ttassignment;

  protected:
    bool _propagate;
    bool _lazy;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override {}
#endif
};

template <typename X, typename Y>
class TPausedAssignment : public TAssignment<X, Y> {
  public:
    TPausedAssignment (CoreProcess* parent, const string& name, X* src, Y* dst, bool is_model = false)
        : TAssignment<X, Y> (parent, name, src, dst, is_model) {
        TAssignment<X, Y>::_propagate = false;
    }

    // for legacy reason, to get rid of?
    TPausedAssignment (CoreProcess* parent, const string& name,
                       X* src, const string& sspec,
                       Y* dst, const string& dspec,
                       bool is_model = false)
        : TPausedAssignment<X, Y> (parent, name, src->find_child_impl (sspec), dst->find_child_impl (dspec), is_model) {
    }
};

template <typename X, typename Y>
class TLazyAssignment : public TAssignment<X, Y> {
  public:
    TLazyAssignment (CoreProcess* parent, const string& name, X* src, Y* dst, bool is_model = false)
        : TAssignment<X, Y> (parent, name, src, dst, is_model) {
        TAssignment<X, Y>::_lazy = true;
    }

    // for legacy reason, to get rid of?
    TLazyAssignment (CoreProcess* parent, const string& name,
                     X* src, const string& sspec,
                     Y* dst, const string& dspec,
                     bool is_model = false)
        : TLazyAssignment<X, Y> (parent, name, src->find_child_impl (sspec), dst->find_child_impl (dspec), is_model) {
    }
};

template <typename X, typename Y>
void TMultiAssignment (CoreProcess* parent, X* src, vector<string> src_props, Y* dst, vector<string> dst_props, bool copy_on_activation);

template <typename X, typename Y>
void TMultiAssignment (CoreProcess* parent, X* src, Y* dst, bool copy_on_activation);

} // namespace djnn
