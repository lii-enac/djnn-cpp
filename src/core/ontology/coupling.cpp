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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "core/core-dev.h" // graph add/remove edge
#include "coupling.h"
#include "process.h"

#include "core/utils/error.h"

namespace djnn
{

  void
  Coupling::replace_edge (CoreProcess* src, CoreProcess* dst, bool immediate_propagation)
  {
    if (get_has_edge ()) {
      graph_remove_edge (_src, _dst);
      set_has_edge (false);
    }
    if (!immediate_propagation && src && dst) {
      graph_add_edge (src, dst);
      set_has_edge (true);
    }
  }

  void
  Coupling::init (CoreProcess* src, activation_flag_e src_flag,
                  CoreProcess* dst, activation_flag_e dst_flag, bool immediate_propagation)
  {
    set_is_enabled (true);
    set_src_activation_flag (src_flag);
    set_dst_activation_flag (dst_flag);
    set_immediate_propagation (immediate_propagation);

    switch (src_flag) {
      case   ACTIVATION: src->add_activation_coupling (this); break;
      case DEACTIVATION: src->add_deactivation_coupling (this); break;
      default: warning (src, string("wrong activation flag in coupling creation ") + src->get_debug_name ()); break;
    }
    replace_edge (src, dst, immediate_propagation);

    _src = src;
    _dst = dst;
  }

  void
  Coupling::uninit ()
  {
    if (_src == nullptr) { return; }

    switch (get_src_activation_flag ()) {
      case   ACTIVATION: _src->remove_activation_coupling (this); break;
      case DEACTIVATION: _src->remove_deactivation_coupling (this); break;
      case NONE_ACTIVATION: break;
    }
    if (get_has_edge ()) {
      graph_remove_edge (_src, _dst);
      set_has_edge (false);
    }
  }

  void
  Coupling::about_to_delete_src () 
  {
    if (get_has_edge ()) {
      graph_remove_edge (_src, _dst);
      set_has_edge (false);
    }
    _src = nullptr;
  }

  Coupling::Coupling (CoreProcess* src, activation_flag_e src_flag,
                      CoreProcess* dst, activation_flag_e dst_flag, bool immediate_propagation)
  : _src (src), _dst (dst), _bitset (0)
  {
    init (src, src_flag, dst, dst_flag, immediate_propagation);
  }

  Coupling::Coupling ()
  : _src(nullptr), _dst(nullptr), _bitset (0)
  {
  }

  Coupling::~Coupling ()
  {
    uninit ();
  }

  void
  Coupling::set_src (CoreProcess *src)
  {
    switch(get_src_activation_flag ()) {
      case ACTIVATION:
        if (_src != nullptr) { _src->remove_activation_coupling (this); }
        if ( src != nullptr) {  src->add_activation_coupling    (this); }
      break;
      case DEACTIVATION:
        if (_src != nullptr) { _src->remove_deactivation_coupling (this); }
        if ( src != nullptr) {  src->add_deactivation_coupling    (this); }
      break;
      case NONE_ACTIVATION:
      break;
    }
    replace_edge (src, _dst, is_immediate());
    _src = src;
  }

  void
  Coupling::set_dst (CoreProcess *dst)
  {
    replace_edge (_src, dst, is_immediate());
    _dst = dst;
  }

  void
  Coupling::propagate ()
  {
    _dst->set_activation_source (_src);
    if (is_immediate ()) {
      propagate_immediately ();
    } else {
      _dst->set_activation_flag (get_dst_activation_flag());
    }
  }

  void
  Coupling::propagate_immediately ()
  {
    switch(get_dst_activation_flag ()) {
    case      ACTIVATION: _dst->activate ();   break;
    case    DEACTIVATION: _dst->deactivate (); break;
    case NONE_ACTIVATION: break;
    }
  }

}
