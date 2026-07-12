/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2023-2025)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
#pragma once

#include "gui/abstract_gobj.h"

namespace djnn {
class ZOrder : public AbstractGObj {
  public:
    ZOrder (CoreProcess* parent, const string& name, int z);
    ~ZOrder () {}
    void        draw () override;
    FatProcess* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
    void        impl_activate () override;
    void        impl_deactivate () override;

  private:
    IntProperty _z;
    Coupling    _c_z;
};
} // namespace djnn