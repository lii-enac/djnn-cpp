#pragma once

#include "core/tree/component.h"
#include "gui/shape/abstract_gshape.h"

namespace djnn {
class Group : public Container {
  public:
    Group (CoreProcess* parent, const string& name);
    virtual ~Group () override;
    auto get_frame () { return _gobj->get_frame (); }
    void impl_activate () override;
    void impl_deactivate () override;
    void draw () override;
    Group* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;

  protected:
    AbstractGObj* _gobj;
};

class ZOrderedGroup : public Group {
  public:
    ZOrderedGroup (CoreProcess* parent, const string& name)
        : Group (parent, name) {}
    process_type_e get_process_type () const override { return Z_ORDERED_GROUP_T; }
    virtual ~ZOrderedGroup () override {};
    void draw () override;
};
} // namespace djnn
