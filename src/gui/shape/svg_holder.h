#pragma once

#include "core/tree/component.h"

namespace djnn {
    class SVGHolder : public Container
    {
    public:
        SVGHolder (ParentProcess* parent, const string& name) : Container (parent, name), _gobj (nullptr) { finalize_construction (parent, name); }
        virtual ~SVGHolder () {}
        //SVGHolder* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
        SVGHolder* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
        FatChildProcess* find_child_impl (const string& path) override;
        void set_gobj (CoreProcess* gobj) { _gobj = gobj; }
        CoreProcess* get_gobj () { return _gobj; }
    private:
        CoreProcess* _gobj;
    };
}