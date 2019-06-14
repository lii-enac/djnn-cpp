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

#pragma once

namespace djnn {

  class Process;

  class Coupling {
  public:
    Coupling (Process* src, int src_flag, Process* dst, int dst_flag, bool immediate_processing = false);
    Coupling (Process* src, int src_flag, Process* dst, int dst_flag, Process* data, bool immediate_processing = false);
    virtual ~Coupling();
    void init_coupling (Process* src, int src_flag, Process* dst, int dst_flag);
    void set_data (Process* data) { _data = data; }
    void propagateActivation ();
    void propagateDeactivation ();
    void executeActivation ();
    void executeDeactivation ();
    void enable () { isEnable = true; };
    void enable (Process* data) { isEnable = true; _data = data; };
    void disable () { isEnable = false; };
    bool is_enable () { return isEnable; }
    Process* get_src () { return _src; }
    Process* get_dst () { return _dst; }
  private:
    bool isEnable, _immediate_processing;
    Process* _src, *_dst, *_data;
    int m_src_flag, m_dst_flag;
  };

}
