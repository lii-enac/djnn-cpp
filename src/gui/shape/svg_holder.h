/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020-2025)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */
#pragma once

#include "core/tree/component.h"

namespace djnn {
class SVGHolder : public Container {
  public:
    SVGHolder (CoreProcess* parent, const string& name)
        : Container (parent, name), _gobj (nullptr) { finalize_construction (parent, name); }
    virtual ~SVGHolder () {}
    // SVGHolder* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
    SVGHolder*   impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
    CoreProcess* find_child_impl (const string& path) override;
    void         set_gobj (CoreProcess* gobj) { _gobj = gobj; }
    CoreProcess* get_gobj () { return _gobj; }

  private:
    CoreProcess* _gobj;
};
} // namespace djnn