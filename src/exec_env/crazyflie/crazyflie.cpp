#include "core/tree/int_property.h"
#include "core/control/simple_binding.h"
#include "core/control/simple_assignment.h"

namespace djnn {
	void
	init_prog()
	{
		IntProperty i(nullptr, "", 5);
		//IntProperty j(nullptr, "", 2);
		//SimpleBinding a(nullptr, "", &i, &j);
		//SimpleAssignment a(nullptr, "", &i, &j, true);
		i.activate ();
		//j.get_value () == 5;
	}
}


extern "C" {
	void djnn_init_prog () {
		djnn::init_prog ();
	}
}


