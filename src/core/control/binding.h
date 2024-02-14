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

#include "core/core-dev.h" // graph add/remove edge
#include "core/ontology/coupling.h"
#include "core/ontology/process.h"

namespace djnn {

class CoreBinding : public CoreProcess
{
  public:
    CoreBinding (CoreProcess* src, activation_flag_e src_flag, CoreProcess* dst, activation_flag_e dst_flag)
        : _c (src, src_flag, dst, dst_flag, false)
    {
        _c.disable ();
    }

    CoreBinding (CoreProcess* src, CoreProcess* dst)
        : CoreBinding (src, ACTIVATION, dst, ACTIVATION) {}

    CoreBinding (CoreProcess* parent, const string& name, CoreProcess* src, activation_flag_e src_flag, CoreProcess* dst, activation_flag_e dst_flag)
        : CoreBinding (src, src_flag, dst, dst_flag)
    {
        finalize_construction (parent, name);
    }

    CoreBinding (CoreProcess* parent, const string& name, CoreProcess* src, CoreProcess* dst)
        : CoreBinding (parent, name, src, ACTIVATION, dst, ACTIVATION) {}

    ~CoreBinding ()
    {
    }

    CoreProcess*       get_src () { return _c.get_src (); } // delegate to coupling to save space
    CoreProcess*       get_dst () { return _c.get_dst (); }
    const CoreProcess* get_src () const { return _c.get_src (); }
    const CoreProcess* get_dst () const { return _c.get_dst (); }
    activation_flag_e  get_src_flag () const { return _c.get_src_activation_flag (); }
    activation_flag_e  get_dst_flag () const { return _c.get_dst_activation_flag (); }

  protected:
    void impl_activate () override { _c.enable (); }
    void impl_deactivate () override { _c.disable (); }
    friend class Binding; // access to impl_activate and impl_deactivate

  private:
    Coupling _c;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
};

// we could have used virtual inheritance, but thus forces qualification of activate(): too cumbersome
// use composition instead, simpler, but we loose a few bytes

class Binding : public FatProcess //, virtual CoreBinding
{
  public:
    Binding (CoreProcess* parent, const string& name, CoreProcess* src, activation_flag_e src_flag, CoreProcess* dst, activation_flag_e dst_flag)
        : FatProcess (name),
          _b (src, src_flag, dst, dst_flag)
    {
        finalize_construction (parent, name);
    }

    Binding (CoreProcess* parent, const string& name, CoreProcess* src, CoreProcess* dst)
        : Binding (parent, name, src, ACTIVATION, dst, ACTIVATION)
    {
    }

    // for legacy reason, to get rid of?
    Binding (CoreProcess* parent, const string& name,
             CoreProcess* src, const string& sspec,
             CoreProcess* dst, const string& dspec)
        : Binding (parent, name, src->find_child_impl (sspec), dst->find_child_impl (dspec))
    {
    }

    Binding (CoreProcess* parent, const string& name,
             CoreProcess* src, const string& sspec, activation_flag_e src_flag,
             CoreProcess* dst, const string& dspec, activation_flag_e dst_flag)
        : Binding (parent, name, src->find_child_impl (sspec), src_flag, dst->find_child_impl (dspec), dst_flag)
    {
    }

    Binding (CoreProcess* parent, const string& name,
             CoreProcess* src, const string& sspec, bool b_src_flag,
             CoreProcess* dst, const string& dspec, bool b_dst_flag)
        : Binding (parent, name, src->find_child_impl (sspec), (b_src_flag) ? ACTIVATION : DEACTIVATION, dst->find_child_impl (dspec), (b_dst_flag) ? ACTIVATION : DEACTIVATION)
    {
    }

    virtual ~Binding ();
    void         set_parent (CoreProcess* parent) override;
    CoreProcess* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;

  protected:
    void impl_activate () override { _b.impl_activate (); }
    void impl_deactivate () override { _b.impl_deactivate (); }

  private:
    CoreBinding _b;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
};
} // namespace djnn
