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
    isEnable = true;
    if (src_flag == ACTIVATION) {
      src->add_activation_coupling (this);
    } else if (src_flag == DEACTIVATION) {
      src->add_deactivation_coupling (this);
    }
  }

  Coupling::Coupling (Process* src, int src_flag, Process* dst,
                      int dst_flag, bool immediate_processing) :
      _immediate_processing (immediate_processing), _src (src), _dst (dst), _data (nullptr), m_src_flag (src_flag), m_dst_flag (dst_flag)
  {
    init_coupling (src, m_src_flag, dst, m_dst_flag);
  }

  Coupling::Coupling (Process* src, int src_flag, Process* dst,
                      int dst_flag, Process* data, bool immediate_processing) :
    _immediate_processing (immediate_processing), _src (src), _dst (dst), _data (data), m_src_flag (src_flag), m_dst_flag (dst_flag)
  {
    init_coupling (src, m_src_flag, dst, m_dst_flag);
  }

  Coupling::~Coupling ()
  {
    if (m_src_flag == ACTIVATION) {
      _src->remove_activation_coupling (this);
    } else if (m_src_flag == DEACTIVATION) {
      _src->remove_deactivation_coupling (this);
    }
  }

  void
  Coupling::propagateActivation ()
  {
    if (_immediate_processing)
      immediateProcessing ();
    else if (isEnable) {
      _dst->set_activation_source (_src);
      _dst->set_data (_data);
      if (m_dst_flag == ACTIVATION) {
        _dst->request_activation ();
      } else {
        _dst->request_deactivation ();
      }
      _dst->coupling_activation_hook ();
    }
  }

  void
  Coupling::propagateDeactivation ()
  {
    if (_immediate_processing)
      immediateProcessing ();
    else if (isEnable) {
      _dst->set_activation_source (_src);
      _dst->set_data (_data);
      if (m_dst_flag == ACTIVATION) {
        _dst->request_activation ();
      } else {
        _dst->request_deactivation ();
      }
      _dst->coupling_deactivation_hook ();
    }
  }

  void
  Coupling::immediateProcessing ()
  {
    if (isEnable) {
      _dst->set_activation_source (_src);
      _dst->set_data (_data);
      if (m_dst_flag == ACTIVATION) {
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
