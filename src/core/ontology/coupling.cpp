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
  using namespace std;

  void
  Coupling::init_coupling (Process* src, int src_flag, Process* dst,
                           int dst_flag)
  {
    if (src == 0) {
      cerr << "Warning: the source of a coupling cannot be null\n";
      return;
    }
    if (dst == 0) {
      cerr << "Warning: the destination of a coupling cannot be null\n";
      return;
    }

    set_is_enabled();

    if (src_flag == ACTIVATION) {
      src_request_activation ();
      src->add_activation_coupling (this);
    } else if (src_flag == DEACTIVATION) {
      src_request_deactivation ();
      src->add_deactivation_coupling (this);
    }

    if (dst_flag == ACTIVATION) {
      dst_request_activation ();
    } else if (dst_flag == DEACTIVATION) {
      dst_request_deactivation ();
    }
  }

  Coupling::Coupling (Process* src, int src_flag, Process* dst,
                      int dst_flag, bool immediate_processing) :
      _src (src), _dst (dst), _data (nullptr)
  {
    if(immediate_processing)
      set_is_immediate_processing ();
    else
      unset_is_immediate_processing ();
    init_coupling (src, src_flag, dst, dst_flag);
  }

  Coupling::Coupling (Process* src, int src_flag, Process* dst,
                      int dst_flag, Process* data, bool immediate_processing) :
    Coupling(src, src_flag, dst, dst_flag, immediate_processing)
  {
    _data = data;
  }

  Coupling::~Coupling ()
  {
    if ( is_src_activation_requested () ) {
      _src->remove_activation_coupling (this);
    } else if (is_src_deactivation_requested ()) {
      _src->remove_deactivation_coupling (this);
    }
  }

  void
  Coupling::propagate_activation ()
  {
    if (is_immediate_processing ())
      process_immediately ();
    else if (is_enabled ()) {
      _dst->set_activation_source (_src);
      _dst->set_data (_data);
      if ( is_dst_activation_requested () ) {
        _dst->request_activation ();
      } else {
        _dst->request_deactivation ();
      }
      _dst->coupling_activation_hook ();
    }
  }

  void
  Coupling::propagate_deactivation ()
  {
    if (is_immediate_processing ())
      process_immediately ();
    else if (is_enabled ()) {
      _dst->set_activation_source (_src);
      _dst->set_data (_data);
      if ( is_dst_activation_requested () ) {
        _dst->request_activation ();
      } else {
        _dst->request_deactivation ();
      }
      _dst->coupling_deactivation_hook ();
    }
  }

  void
  Coupling::process_immediately ()
  {
    if (is_enabled ()) {
      _dst->set_activation_source (_src);
      _dst->set_data (_data);
      if ( is_dst_activation_requested () ) {
        _dst->activation ();
        _dst->coupling_activation_hook ();
      }
      else {
        _dst->deactivation ();
        _dst->coupling_deactivation_hook ();
      }
    }
  }
}
