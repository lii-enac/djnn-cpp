/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2026)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */
#pragma once

#include "core/ontology/process.h"

namespace djnn {

class Spike : public ChildProcess {
  public:
    Spike (CoreProcess* parent, const string& name, bool model = true) // by default, model = true hence a spike does NOT activate upon its parent activation
        : ChildProcess (name, model) { finalize_construction (parent, name); };
    virtual ~Spike () {};
    Spike* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;

  protected:
    void impl_activate () override {}
    void impl_deactivate () override {}
    void post_activate () override { post_activate_auto_deactivate (); }

#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
};

} // namespace djnn
