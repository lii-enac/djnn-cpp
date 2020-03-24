#include "core/tree/int_property.h"
#include "core/control/assignment.h"

namespace djnn {
	void
	init_prog()
	{
		IntProperty i(nullptr, "", 5);
		IntProperty j(nullptr, "", 2);
		SimpleAssignment a(nullptr, "", &i, &j, true);
		a.activate ();
		j.get_value () == 5;
	}
}


extern "C" {
	void djnn_init_prog () {
		djnn::init_prog ();
	}
}


