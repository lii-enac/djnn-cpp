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
 *
 */

#include "binding.h"
#include "../execution/graph.h"
#include "../utils/error.h"
#include "../serializer/serializer.h"
#include "../utils/utils-dev.h"

#include <iostream>

namespace djnn
{
  using namespace std;

  void
  Binding::init_binding (Process* src, const string & ispec, bool on_activation, Process* dst, const string & dspec,
			 bool to_activate)
  {
    if (src == 0) {
      error (this,
	     "src argument cannot be null in binding creation (" + get_name () + ", " + ispec + ", " + dspec + ")");
    }
    if (dst == 0) {
      error (this,
	     "dst argument cannot be null in binding creation (" + get_name () + ", " + ispec + ", " + dspec + ")");
    }
    pair<RefProperty*, string> ref_src_pair = check_for_ref (src, ispec);
    if (ref_src_pair.first != nullptr) {
      _ref_src = ref_src_pair.first;
      _update_src = new UpdateSrcOrDst (this, "update_src_action", ref_src_pair.first, ref_src_pair.second,
                                        (Process**) &_src);

      _c_update_src = new Coupling (ref_src_pair.first, ACTIVATION, _update_src, ACTIVATION, true);
    } else {
      _src = src->find_component (ispec);
      if (_src == 0) {
        error (this, "source not found in binding creation (" + get_name () + ", " + ispec + ", " + dspec + ")");
      }
    }

    pair<RefProperty*, string> ref_dst_pair = check_for_ref (dst, dspec);
    if (ref_dst_pair.first != nullptr) {
      _ref_dst = ref_dst_pair.first;
      _update_dst = new UpdateSrcOrDst (this, "update_dst_action", ref_dst_pair.first, ref_dst_pair.second,
                                        (Process**) &_dst);

      _c_update_dst = new Coupling (ref_dst_pair.first, ACTIVATION, _update_dst, ACTIVATION, true);
    } else {
      _dst = dst->find_component (dspec);
      if (_dst == 0) {
        error (this, "destination not found in binding creation (" + get_name () + ", " + ispec + ", " + dspec + ")");
      }
    }

    string src_name = _src ? _src->get_name () : "";
    string dst_name = _dst ? _dst->get_name () : "";
    _action = new BindingAction (this, "binding_" + src_name + "_to_" + dst_name + "_action", to_activate);

    if (_src) {
      if (on_activation)
        _c_src = new Coupling (_src, ACTIVATION, _action, ACTIVATION, true);
      else
        _c_src = new Coupling (_src, DEACTIVATION, _action, ACTIVATION, true);
      if (_dst) {
        Graph::instance ().add_edge (_src, _dst);
        add_state_dependency (_parent, _dst);
        _c_src->disable ();
        _has_coupling = true;
      }
    }
    if (_update_src) _update_src->impl_activate ();
    if (_update_dst) _update_dst->impl_activate ();
  }

  void
  Binding::update_graph ()
  {
    if (_has_coupling) {
      delete _c_src;
      _c_src = nullptr;
    }
    if (_src && _dst) {
      _c_src = new Coupling (_src, ACTIVATION, _action, ACTIVATION, true);
      _has_coupling = true;
      if (get_activation_state() != ACTIVATED ) {
        _c_src->disable ();
      }
    } else {
      _has_coupling = false;
    }
  }

  Binding::Binding (Process* src, const string & ispec, Process* dst, const string & dspec) :
      _src (nullptr), _dst (nullptr), _ref_src (nullptr), _ref_dst (nullptr), _c_src (nullptr), _action (nullptr), _update_src (
          nullptr), _update_dst (nullptr), _c_update_src (nullptr), _c_update_dst (nullptr), _has_coupling (false)
  {
    init_binding (src, ispec, true, dst, dspec, true);
  }

  Binding::Binding (Process* parent, const string & name, Process* src, const string & ispec, Process* dst,
                    const string & dspec) :
      SrcToDstLink (parent, name), _src (nullptr), _dst (nullptr), _ref_src (nullptr), _ref_dst (nullptr), _c_src (
          nullptr), _action (nullptr), _update_src (nullptr), _update_dst (nullptr), _c_update_src (nullptr), _c_update_dst (
          nullptr), _has_coupling (false)
  {
    init_binding (src, ispec, true, dst, dspec, true);
    Process::finalize_construction (parent);
  }

  Binding::Binding (Process* parent, const string &name, Process* src, const string & ispec, bool on_activation,
                    Process* dst, const string & dspec, bool activate) :
      SrcToDstLink (parent, name), _src (nullptr), _dst (nullptr), _ref_src (nullptr), _ref_dst (nullptr), _c_src (
          nullptr), _action (nullptr), _update_src (nullptr), _update_dst (nullptr), _c_update_src (nullptr), _c_update_dst (
          nullptr), _has_coupling (false)
  {
    init_binding (src, ispec, on_activation, dst, dspec, activate);
    Process::finalize_construction (parent);
  }

  Binding::~Binding ()
  {

    remove_state_dependency (_parent, _dst);
    Graph::instance ().remove_edge (_src, _dst);

    delete _c_src;
    delete _action;
    if (_update_src) {
      delete _c_update_src;
      delete _update_src;
    }
    if (_update_dst) {
      delete _c_update_dst;
      delete _update_dst;
    }
  }

  void
  Binding::serialize (const string& format)
  {

    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:binding");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::compute_path (get_parent (), _src, buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _dst, buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }

}
