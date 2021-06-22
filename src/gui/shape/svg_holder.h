#pragma once

#include "core/tree/component.h"

namespace djnn {
    class SVGHolder : public Container
    {
    public:
        SVGHolder (ParentProcess* parent, const string& name) : Container (parent, name), _gobj (nullptr) { finalize_construction (parent, name); }
        virtual ~SVGHolder () {}
        SVGHolder* clone () override;
        FatChildProcess* find_child_impl (const string& path) override;
        void set_gobj (FatProcess* gobj) { _gobj = gobj; }
        FatProcess* get_gobj () { return _gobj; }
    private:
        FatProcess* _gobj;
    };
}