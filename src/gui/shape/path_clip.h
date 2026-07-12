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

#include "gui/shape/path.h"

namespace djnn {
class PathClip : public Path {
  public:
    PathClip (CoreProcess* parent, const string& name)
        : Path (parent, name) {}
    virtual ~PathClip () {}
    void draw () override;
    // PathClip* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
    PathClip* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
};
} // namespace djnn