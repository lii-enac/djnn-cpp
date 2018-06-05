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
#include "../execution/graph.h"
#include "../tree/process.h"

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
                      int dst_flag) :
      _src (src), _dst (dst), _data (nullptr), m_src_flag (src_flag), m_dst_flag (dst_flag)
  {
    init_coupling (src, m_src_flag, dst, m_dst_flag);
  }

  Coupling::Coupling (Process* src, int src_flag, Process* dst,
                      int dst_flag, Process* data) :
      _src (src), _dst (dst), _data (data), m_src_flag (src_flag), m_dst_flag (dst_flag)
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
    if (isEnable) {
      _dst->set_source (_src);
      _dst->set_data (_data);
      _dst->set_activation_flag (m_dst_flag);
    }
  }

  void
  Coupling::propagateDeactivation ()
  {
    if (isEnable) {
      _dst->set_source (_src);
      _dst->set_data (_data);
      _dst->set_activation_flag (m_dst_flag);
    }
  }

  void
  Coupling::executeActivation ()
  {
    if (isEnable) {
      _dst->set_source (_src);
      _dst->set_data (_data);
      _dst->activation ();
    }
  }
}
