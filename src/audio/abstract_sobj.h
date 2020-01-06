#pragma once

#include "core/ontology/process.h"

namespace djnn {

	class AbstractSObjImpl {
  	public:
    	virtual ~AbstractSObjImpl();
  	};

	class AbstractSObj : public Process {
	public:
		AbstractSObj (Process* parent, const string& name) : Process(name), _impl(nullptr) {}
		virtual ~AbstractSObj () { delete _impl; }

		AbstractSObjImpl* impl() { return _impl; }
    	void set_impl(AbstractSObjImpl* i) { _impl=i; }

	private:
		AbstractSObjImpl *_impl;
	};
}