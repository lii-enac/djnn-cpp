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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 */

#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/core-dev.h" // graph add/remove edge

namespace djnn {

  class CoreBinding : public CoreProcess
  {
  public:
    CoreBinding (CoreProcess* src, activation_flag_e src_flag, CoreProcess* dst, activation_flag_e dst_flag)
    : _c (src, src_flag, dst, dst_flag, false) {
      _c.disable ();
      graph_add_edge (src, dst);
    }

    CoreBinding (CoreProcess* src, CoreProcess* dst)
    : CoreBinding (src, ACTIVATION, dst, ACTIVATION) {}

    CoreBinding (ParentProcess* parent, const std::string& name, CoreProcess* src, activation_flag_e src_flag, CoreProcess* dst, activation_flag_e dst_flag)
    : CoreBinding (src, src_flag, dst, dst_flag) {
      finalize_construction (parent, name);
    }
    
    CoreBinding (ParentProcess* parent, const std::string& name, CoreProcess* src, CoreProcess* dst)
    : CoreBinding (parent, name, src, ACTIVATION, dst, ACTIVATION) {}

    ~CoreBinding () {
      graph_remove_edge (get_src(), get_dst());
    }

    CoreProcess * get_src() { return _c.get_src (); } // delegate to coupling to save space
    CoreProcess * get_dst() { return _c.get_dst (); }

  protected:
    void impl_activate   () override { _c.enable  (); }
    void impl_deactivate () override { _c.disable (); }
    friend class Binding; // access to impl_activate and impl_deactivate

  private:
    Coupling _c;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };


  // we could have used virtual inheritance, but thus forces qualification of activate(): too cumbersome
  // use composition instead, simpler, but we loose a few bytes 

  class Binding : public FatProcess //, virtual CoreBinding 
  {
  public:
    Binding (ParentProcess* parent, const std::string& name, CoreProcess* src, activation_flag_e src_flag, CoreProcess* dst, activation_flag_e dst_flag)
    : FatProcess (name), _b (src, src_flag, dst, dst_flag)
    {
      finalize_construction (parent, name);
    }

    Binding (ParentProcess* parent, const std::string& name, CoreProcess* src, CoreProcess* dst)
    : Binding (parent, name, src, ACTIVATION, dst, ACTIVATION)
    {}

    // for legacy reason, to get rid of?
    Binding (ParentProcess* parent, const std::string& name,
                   CoreProcess* src, const std::string& sspec,
                   CoreProcess* dst, const std::string& dspec)
    : Binding (parent, name, src->find_child_impl (sspec), dst->find_child_impl (dspec))
    {}

    Binding (ParentProcess* parent, const std::string& name,
                   CoreProcess* src, const std::string& sspec, activation_flag_e src_flag,
                   CoreProcess* dst, const std::string& dspec, activation_flag_e dst_flag)
    : Binding (parent, name, src->find_child_impl (sspec), src_flag, dst->find_child_impl (dspec), dst_flag)
    {}

    Binding (ParentProcess* parent, const std::string& name,
                   CoreProcess* src, const std::string& sspec, bool b_src_flag,
                   CoreProcess* dst, const std::string& dspec, bool b_dst_flag)
    : Binding (parent, name, src->find_child_impl (sspec), (b_src_flag) ? ACTIVATION : DEACTIVATION , dst->find_child_impl (dspec), (b_dst_flag) ? ACTIVATION : DEACTIVATION)
    {}

    void set_parent (ParentProcess* parent) override;

  protected:

    void impl_activate   () override { _b.impl_activate (); }
    void impl_deactivate () override { _b.impl_deactivate (); }

  private:
    CoreBinding _b;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };
}