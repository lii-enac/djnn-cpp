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
#include "../execution/graph.h"

#include <iostream>

namespace djnn
{
  //using namespace std;
  using std::cerr;

  void
  Coupling::init_coupling (Process* src, activation_flag_e src_flag, Process* dst,
                           activation_flag_e dst_flag)
  {
    if (src == 0) {
      cerr << "Warning: the source of a coupling cannot be null\n";
      return;
    }
    if (dst == 0) {
      cerr << "Warning: the destination of a coupling cannot be null\n";
      return;
    }

    set_is_enabled(true);

    set_src_activation_flag (src_flag);
    if (src_flag == ACTIVATION) {
      src->add_activation_coupling (this);
    } else if (src_flag == DEACTIVATION) {
      src->add_deactivation_coupling (this);
    }

    set_dst_activation_flag (dst_flag);
  }

  Coupling::Coupling (Process* src, activation_flag_e src_flag, Process* dst,
                      activation_flag_e dst_flag, bool immediate_propagation) :
      _src (src), _dst (dst), _data (nullptr)
  {
    set_immediate_propagation (immediate_propagation);
    init_coupling (src, src_flag, dst, dst_flag);
  }

  Coupling::Coupling (Process* src, activation_flag_e src_flag, Process* dst,
                      activation_flag_e dst_flag, Process* data, bool immediate_propagation) :
    Coupling(src, src_flag, dst, dst_flag, immediate_propagation)
  {
    _data = data;
  }

  Coupling::~Coupling ()
  {
    switch(get_src_activation_flag ()) {
    case ACTIVATION:
      _src->remove_activation_coupling (this);
      break;
    case DEACTIVATION:
      _src->remove_deactivation_coupling (this);
      break;
    default: break;
    }
  }

  void
  Coupling::propagate_activation ()
  {
    //if (!is_enabled ()) return; // useless already tested elsewhere
    assert(is_enabled ());
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
    //if (!is_enabled ()) return; // useless already tested elsewhere
    assert(is_enabled ());
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
