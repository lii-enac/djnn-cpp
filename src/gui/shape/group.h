#pragma once

#include "gui/shape/abstract_gshape.h"
#include "core/tree/component.h"

namespace djnn {
    class Group : public Container
    {
    public:
        Group (CoreProcess* parent, const string& name);
        virtual ~Group () override;
        auto get_frame () { return _gobj->get_frame ();}
        void impl_activate () override;
        void impl_deactivate () override;
        void draw () override;
        //Group* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
        Group* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
    protected:
        AbstractGObj *_gobj;
    };

    class ZOrderedGroup : public Group
    {
    public:
        ZOrderedGroup (CoreProcess* parent, const string& name) : Group (parent, name) {}
        process_type_e get_process_type () const override { return Z_ORDERED_GROUP_T; }
        virtual ~ZOrderedGroup () override {};
        void draw () override;
    };
}
