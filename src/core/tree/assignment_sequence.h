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

#include "core/tree/container.h"

namespace djnn {

class AssignmentSequence : public Container {
  public:
    AssignmentSequence (CoreProcess* parent, const string& name, bool is_model);
    virtual ~AssignmentSequence ();
    void                add_child (CoreProcess* c, const string& name) override;
    void                draw () override {}
    void                impl_activate () override;
    void                post_activate () override;
    void                impl_deactivate () override {};
    AssignmentSequence* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;

#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
};
} // namespace djnn
