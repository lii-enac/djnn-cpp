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

	class AbstractSObj : public Process {
	public:
		AbstractSObj (Process* parent, const std::string& name) : Process(name), _impl(nullptr) {}
		virtual ~AbstractSObj () { delete _impl; }

		AbstractSObjImpl* impl() { return _impl; }
    	void set_impl(AbstractSObjImpl* i) { _impl=i; }

    	//AudioScene * frame ();

    protected:
	    virtual Process* create_GObj_prop (BoolPropertyProxy **prop, Coupling  **cprop, bool *rawp, const std::string& name, int notify_mask);
	    virtual Process* create_GObj_prop (IntPropertyProxy **prop, Coupling  **cprop, int *rawp, const std::string& name, int notify_mask);
	    virtual Process* create_GObj_prop (DoublePropertyProxy **prop, Coupling  **cprop, double *rawp, const std::string& name, int notify_mask);
	    virtual Process* create_GObj_prop (TextPropertyProxy **prop, Coupling  **cprop, std::string *rawp, const std::string& name, int notify_mask);

	private:
		AbstractSObjImpl *_impl;
	};
}