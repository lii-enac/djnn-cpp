#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/tree/bool_property.h"
#include "core/tree/int_property.h"
#include "core/tree/double_property.h"
#include "core/tree/text_property.h"

namespace djnn {

	class AbstractSObjImpl {
  	public:
    	virtual ~AbstractSObjImpl();
  	};

	// place holder fo gen_prop
	class AudioScene : public CoreProcess {
		public:
		CoreProcess* damaged() { return nullptr; }
	};

	class AbstractSObj : public FatProcess {
	public:
		AbstractSObj (FatProcess* parent, const std::string& name) : FatProcess(name), _impl(nullptr) {}
		virtual ~AbstractSObj () { delete _impl; }

		AbstractSObjImpl* impl() { return _impl; }
    	void set_impl(AbstractSObjImpl* i) { _impl=i; }

    	AudioScene * get_frame () { return nullptr; } // FIXME

    protected:
	    virtual FatProcess* create_GObj_prop (BoolPropertyProxy **prop, CouplingWithData  **cprop, bool *rawp, const std::string& name, int notify_mask);
	    virtual FatProcess* create_GObj_prop (IntPropertyProxy **prop, CouplingWithData  **cprop, int *rawp, const std::string& name, int notify_mask);
	    virtual FatProcess* create_GObj_prop (DoublePropertyProxy **prop, CouplingWithData  **cprop, double *rawp, const std::string& name, int notify_mask);
	    virtual FatProcess* create_GObj_prop (TextPropertyProxy **prop, CouplingWithData  **cprop, std::string *rawp, const std::string& name, int notify_mask);

	private:
		AbstractSObjImpl *_impl;
	};
}