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

#include "coupling.h"
#include "process.h"
#include "core/execution/graph.h"

#include "core/utils/error.h"

namespace djnn
{

  void
  Coupling::init (Process* src, activation_flag_e src_flag, Process* dst,
                           activation_flag_e dst_flag, bool immediate_propagation)
  {
    if (src == nullptr) {
      warning(src, "the source of a coupling cannot be null");
      return;
    }
    if (dst == nullptr) {
      warning(dst, "the destination of a coupling cannot be null");
      return;
    }

    set_immediate_propagation (immediate_propagation);

    set_is_enabled(true);

    set_src_activation_flag (src_flag);
    if (src_flag == ACTIVATION) {
      src->add_activation_coupling (this);
    } else if (src_flag == DEACTIVATION) {
      src->add_deactivation_coupling (this);
    }

    set_dst_activation_flag (dst_flag);
    _src = src;
    _dst = dst;
  }

  void
  Coupling::uninit ()
  {
    if (_src == nullptr)
      return;
    switch(get_src_activation_flag ()) {
    case ACTIVATION:
      _src->remove_activation_coupling (this);
      break;
    case DEACTIVATION:
      _src->remove_deactivation_coupling (this);
      break;
    case NONE_ACTIVATION:
      break;
    }
  }

  Coupling::Coupling (Process* src, activation_flag_e src_flag, Process* dst,
                      activation_flag_e dst_flag, bool immediate_propagation) :
      _src (src), _dst (dst), _data (nullptr)
  {
    init (src, src_flag, dst, dst_flag, immediate_propagation);
  }

  Coupling::Coupling (Process* src, activation_flag_e src_flag, Process* dst,
                      activation_flag_e dst_flag, Process* data, bool immediate_propagation) :
    Coupling(src, src_flag, dst, dst_flag, immediate_propagation)
  {
    _data = data;
  }

  Coupling::Coupling ()
  : _src(nullptr), _dst(nullptr)
  {
  }

  Coupling::~Coupling ()
  {
    uninit ();
  }

  void
  Coupling::change_source (Process *src, Process* data)
  {
    //if (src != nullptr) {
      switch(get_src_activation_flag ()) {
        case ACTIVATION:
          if (_src != nullptr) _src->remove_activation_coupling (this);
          if ( src != nullptr) src->add_activation_coupling (this);
        break;
        case DEACTIVATION:
          if (_src !=nullptr) _src->remove_deactivation_coupling (this);
          if ( src !=nullptr) src->add_deactivation_coupling (this);
        break;
        case NONE_ACTIVATION:
        break;
      }
    //}
    _src = src;
    _data = data;
  }

  void
  Coupling::propagate_activation ()
  {
    _dst->set_activation_source (_src);
    _dst->set_data (_data);
    if (is_immediate ()) {
      propagate_immediately ();
    }
    else {
      _dst->set_activation_flag (get_dst_activation_flag());
    }
    _dst->coupling_activation_hook ();
  }

  void
  Coupling::propagate_deactivation ()
  {
    _dst->set_activation_source (_src);
    _dst->set_data (_data);
    if (is_immediate ()) {
      propagate_immediately ();
    }
    else {
      _dst->set_activation_flag (get_dst_activation_flag());
    }
    _dst->coupling_deactivation_hook ();
  }

  void
  Coupling::propagate_immediately ()
  {
    switch(get_dst_activation_flag ()) {
    case NONE_ACTIVATION: break;
    case ACTIVATION:
      _dst->activate ();
      _dst->coupling_activation_hook ();
      break;
    case DEACTIVATION:
      _dst->deactivate ();
      _dst->coupling_deactivation_hook ();
      break;
    }
  }

}
