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

class Component : public Container
{
  public:
    Component (CoreProcess* parent, const string& name, bool is_model = false) : Container (parent, name, is_model) { finalize_construction (parent, name); }
    // virtual process_type_e get_process_type () const override { return CONTAINER_T; }
    // Component* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
    Component* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
};

} // namespace djnn
