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

  Binding::BindingAction::BindingAction (Process* parent, const string &name, bool activate) : 
    Action (parent, name) 
  { 
    set_binding_action (activate); 
  }

  Binding::Init::Init(Binding * b, Process* src, const string & ispec, bool on_activation, Process* dst, const string & dspec, bool to_activate)
  {
    if (src == 0) {
      error (b,
       "src argument cannot be null in binding creation (" + b->get_name () + ", " + ispec + ", " + dspec + ")");
    }
    if (dst == 0) {
      error (b,
       "dst argument cannot be null in binding creation (" + b->get_name () + ", " + ispec + ", " + dspec + ")");
    }

    pair<RefProperty*, string> ref_src_pair = check_for_ref (src, ispec);
    b->_ref_info_src._ref = ref_src_pair.first;
    b->_ref_info_src._name = ref_src_pair.second;

    pair<RefProperty*, string> ref_dst_pair = check_for_ref (dst, dspec);
    b->_ref_info_dst._ref = ref_dst_pair.first;
    b->_ref_info_dst._name = ref_dst_pair.second;
  }

  void
  Binding::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent () && _dst) {
       remove_state_dependency (get_parent (), _dst);
    }

    if (_dst)
      add_state_dependency (p, _dst);

    Process::set_parent (p); 
  }

  void
  Binding::update_graph ()
  {
    if (_has_coupling) {
      /* remove_edge is made before in about_to_update_graph hook */
      _c_src.uninit ();
    }
    if (_src && _dst) {
      if(_has_coupling) {
        _c_src.change_source(_src);
      } else {
        _c_src.init(_src, ACTIVATION, &_action, ACTIVATION, true);
      }
      //cerr << "update_graph  add edge:" << _src->get_name () << " - "  << _dst->get_name () << endl;
      Graph::instance ().add_edge (_src, _dst);
      _has_coupling = true;
      if ( get_activation_state()==ACTIVATED ) {
        _action.activate ();
      } else {
        _c_src.disable ();
      }

    }
  }

  void 
  Binding::about_to_update_graph ()
  {
    if (_src && _dst) {
      //cerr << endl << "about_to_update_graph old _src: "<<  _src->get_name () << endl;
      //cerr << "about_to_update_graph  remove old edge:" << _src->get_name () << " - "  << _dst->get_name () << endl;
      Graph::instance ().remove_edge (_src, _dst);
    }
  }

  void
  Binding::check_init(const string& ispec, const string& dspec)
  {
    if(!_ref_info_src._ref) {
      if (_src == 0) {
        error (this, "source not found in binding creation (" + get_name () + ", " + ispec + ", " + dspec + ")");
      }
    }
    if(!_ref_info_dst._ref) {
      if (_dst == 0) {
        error (
            this,
            "DESTINATION not found in (Paused)assignment creation or the DESTINATION of an (Paused)assignment must be a property ( name: " + get_name () + ", src spec: " + ispec
                + ", dst spec:" + dspec + ")\n");
      }
    }
  }

  Binding::Binding (Process* src, const string & ispec, Process* dst, const string & dspec) :
      _init(this, src, ispec, true, dst, dspec, true),
      _src(!_ref_info_src.is_ref() && src ? src->find_component (ispec) : nullptr),
      _dst(!_ref_info_dst.is_ref() && dst ? dst->find_component (dspec) : nullptr),
      _ref_update_src(_ref_info_src.is_ref() ? ref_update(this, _ref_info_src, &_src) : ref_update()), // uses copy constructor
      _ref_update_dst(_ref_info_dst.is_ref() ? ref_update(this, _ref_info_dst, &_dst) : ref_update()),
      _action(this, "binding_" + (_src ? _src->get_name () : "") + "_to_" + ( _dst ? _dst->get_name () : "") + "_action", true),
      _c_src(_src ? Coupling(_src, ACTIVATION, &_action, ACTIVATION, true) : Coupling()),
      _has_coupling (false)
  {
    check_init(ispec, dspec);
    if (_src) {
      _c_src.disable ();
      if(_dst) {
        Graph::instance ().add_edge (_src, _dst);
        _has_coupling = true;
      }
    }
    if (_ref_info_src.is_ref()) _ref_update_src._update.impl_activate ();
    if (_ref_info_dst.is_ref()) _ref_update_dst._update.impl_activate ();
  }

  Binding::Binding (Process* parent, const string & name, Process* src, const string & ispec, Process* dst,
                    const string & dspec) :
      SrcToDstLink (parent, name),
      _init(this, src, ispec, true, dst, dspec, true),
      _src(!_ref_info_src.is_ref() && src ? src->find_component (ispec) : nullptr),
      _dst(!_ref_info_dst.is_ref() && dst ? dst->find_component (dspec) : nullptr),
      _ref_update_src(_ref_info_src.is_ref() ? ref_update(this, _ref_info_src, &_src) : ref_update()), // uses copy constructor
      _ref_update_dst(_ref_info_dst.is_ref() ? ref_update(this, _ref_info_dst, &_dst) : ref_update()),
      _action(this, "binding_" + (_src ? _src->get_name () : "") + "_to_" + ( _dst ? _dst->get_name () : "") + "_action", true),
      _c_src(_src ? Coupling(_src, ACTIVATION, &_action, ACTIVATION, true) : Coupling()),
      _has_coupling (false)
  {
    check_init(ispec, dspec);
    if (_src) {
      _c_src.disable ();
      if(_dst) {
        Graph::instance ().add_edge (_src, _dst);
        _has_coupling = true;
      }
    }
    if (_ref_info_src.is_ref()) _ref_update_src._update.impl_activate ();
    if (_ref_info_dst.is_ref()) _ref_update_dst._update.impl_activate ();
    Process::finalize_construction (parent);
  }

  Binding::Binding (Process* parent, const string &name, Process* src, const string & ispec, bool on_activation,
                    Process* dst, const string & dspec, bool activate) :
      SrcToDstLink (parent, name),
      _init(this, src, ispec, on_activation, dst, dspec, activate),
      _src(!_ref_info_src.is_ref() && src ? src->find_component (ispec) : nullptr),
      _dst(!_ref_info_dst.is_ref() && dst ? dst->find_component (dspec) : nullptr),
      _ref_update_src(_ref_info_src.is_ref() ? ref_update(this, _ref_info_src, &_src) : ref_update()), // uses copy constructor
      _ref_update_dst(_ref_info_dst.is_ref() ? ref_update(this, _ref_info_dst, &_dst) : ref_update()),
      _action(this, "binding_" + (_src ? _src->get_name () : "") + "_to_" + ( _dst ? _dst->get_name () : "") + "_action", activate),
      _c_src(_src ? Coupling(_src, on_activation ? ACTIVATION : DEACTIVATION, &_action, ACTIVATION, true) : Coupling()),
      _has_coupling (false)
  {
    check_init(ispec, dspec);
    if (_src) {
      _c_src.disable ();
      if(_dst) {
        Graph::instance ().add_edge (_src, _dst);
        _has_coupling = true;
      }
    }
    if (_ref_info_src.is_ref()) _ref_update_src._update.impl_activate ();
    if (_ref_info_dst.is_ref()) _ref_update_dst._update.impl_activate ();
    Process::finalize_construction (parent);
  }

  Binding::~Binding ()
  {
    remove_state_dependency (get_parent (), _dst);
    Graph::instance ().remove_edge (_src, _dst);
  }

  void
  Binding::serialize (const string& format)
  {
    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:binding");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::compute_path (get_parent (), _src, buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _dst, buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }

}
