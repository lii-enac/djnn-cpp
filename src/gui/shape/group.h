#pragma once

#include "gui/shape/abstract_gshape.h"
#include "core/tree/component.h"

namespace djnn {
    class Group : public Container
    {
    public:
        Group (ParentProcess* parent, const string& name);
        virtual ~Group () override;
        auto get_frame () { return _gobj->get_frame ();}
        void impl_activate () override;
        void impl_deactivate () override;
        void draw () override;
        Group* clone () override;
    protected:
        AbstractGObj *_gobj;
    };
}