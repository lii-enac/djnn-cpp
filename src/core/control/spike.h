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

#include "core/ontology/process.h"

namespace djnn {

class Spike : public FatProcess {
  public:
    Spike (CoreProcess* parent, const string& name, bool model = true)
        : FatProcess (name, model) { finalize_construction (parent, name); };
    virtual ~Spike (){};
    FatProcess* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;

  protected:
    bool pre_activate () override;
    void post_activate () override;
    void impl_activate () override{};
    void impl_deactivate () override{};

#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
};

} // namespace djnn
