#pragma once

#include "core/tree/int_property.h"
#include "core/tree/double_property.h"
#include "core/tree/bool_property.h"
#include "core/tree/text_property.h"
#include "core/ontology/coupling.h"

namespace djnn {

    // should be world ??
    class PhysicScene : public CoreProcess {
		public:
		CoreProcess* damaged() { return nullptr; }
	};
    class AbstractPObj : public FatProcess {
	public:
		AbstractPObj (ParentProcess* parent, const djnn::string& name) : FatProcess(name) {}
        //World * get_frame () { return get_world (); } // FIXME
        PhysicScene * get_frame () { return nullptr; } // FIXME
        virtual void impl_activate () override;
    	virtual void impl_deactivate () override;
    virtual FatProcess* create_GObj_prop (BoolPropertyProxy **prop, CouplingWithData  **cprop, bool *rawp, const djnn::string& name, int notify_mask);
    virtual FatProcess* create_GObj_prop (IntPropertyProxy **prop, CouplingWithData  **cprop, int *rawp, const djnn::string& name, int notify_mask);
    virtual FatProcess* create_GObj_prop (DoublePropertyProxy **prop, CouplingWithData  **cprop, double *rawp, const djnn::string& name, int notify_mask);
    virtual FatProcess* create_GObj_prop (TextPropertyProxy **prop, CouplingWithData  **cprop, djnn::string *rawp, const djnn::string& name, int notify_mask);
  };
}
