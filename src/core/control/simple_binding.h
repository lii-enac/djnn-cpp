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
#include "core/execution/graph.h"

#define DJNN_SIMPLE_BINDING_INCLUDED 1


namespace djnn {

  class CoreBinding : public CoreProcess
  {
  public:
    CoreBinding (CoreProcess* src, activation_flag_e src_flag, CoreProcess* dst, activation_flag_e dst_flag)
    : _c (src, src_flag, dst, dst_flag, true) {
      _c.disable ();
      Graph::instance ().add_edge (src, dst);
    }

    CoreBinding (CoreProcess* src, CoreProcess* dst)
    : CoreBinding (src, ACTIVATION, dst, ACTIVATION) {}

    CoreBinding (FatProcess* parent, const std::string& name, CoreProcess* src, activation_flag_e src_flag, CoreProcess* dst, activation_flag_e dst_flag)
    : CoreBinding (src, src_flag, dst, dst_flag) {
      finalize_construction (parent, name);
    }
    
    CoreBinding (FatProcess* parent, const std::string& name, CoreProcess* src, CoreProcess* dst)
    : CoreBinding (parent, name, src, ACTIVATION, dst, ACTIVATION) {}

    ~CoreBinding () {
      Graph::instance ().remove_edge (get_src(), get_dst());
    }

    CoreProcess * get_src() { return _c.get_src (); } // delegate to coupling to save space
    CoreProcess * get_dst() { return _c.get_dst (); }

  protected:
    void impl_activate   () override { _c.enable  (); }
    void impl_deactivate () override { _c.disable (); }
    friend class SimpleBinding; // access to impl_activate and impl_deactivate

  private:
    Coupling _c;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };


  // we could have used virtual inheritance, but thus forces qualification of activate(): too cumbersome
  // use composition instead, simpler, but we loose a few bytes 

  class SimpleBinding : public FatProcess //, virtual CoreBinding 
  {
  public:
    SimpleBinding (FatProcess* parent, const std::string& name, CoreProcess* src, activation_flag_e src_flag, CoreProcess* dst, activation_flag_e dst_flag)
    : FatProcess (name), _b (src, src_flag, dst, dst_flag)
    {
      finalize_construction (parent, name);
    }

    SimpleBinding (FatProcess* parent, const std::string& name, CoreProcess* src, CoreProcess* dst)
    : SimpleBinding (parent, name, src, ACTIVATION, dst, ACTIVATION)
    {}

    // for legacy reason, to get rid of?
    SimpleBinding (FatProcess* parent, const std::string& name,
                   CoreProcess* src, const std::string& sspec,
                   CoreProcess* dst, const std::string& dspec)
    : SimpleBinding (parent, name, src->find_child (sspec), dst->find_child (dspec))
    {}

    SimpleBinding (FatProcess* parent, const std::string& name,
                   CoreProcess* src, const std::string& sspec, activation_flag_e src_flag,
                   CoreProcess* dst, const std::string& dspec, activation_flag_e dst_flag)
    : SimpleBinding (parent, name, src->find_child (sspec), src_flag, dst->find_child (dspec), dst_flag)
    {}

    SimpleBinding (FatProcess* parent, const std::string& name,
                   CoreProcess* src, const std::string& sspec, bool b_src_flag,
                   CoreProcess* dst, const std::string& dspec, bool b_dst_flag)
    : SimpleBinding (parent, name, src->find_child (sspec), (b_src_flag) ? ACTIVATION : DEACTIVATION , dst->find_child (dspec), (b_dst_flag) ? ACTIVATION : DEACTIVATION)
    {}

    void set_parent (FatProcess* p) override;

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