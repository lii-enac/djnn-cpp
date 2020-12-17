#include <iostream>

#include "core/tree/int_property.h"
#include "core/tree/text_property.h"
//#include "core/control/binding.h"
#include "core/control/assignment.h"
#include "core/execution/graph.h"
#include "core/utils/utils-dev.h"
#include "core/core-dev.h"


// c++ -o build/quick_test -std=c++14 -Isrc -Ibuild -Lbuild/lib -ldjnn-core tools/quick_test.cpp && env DYLD_LIBRARY_PATH=build/lib build/quick_test
// -DDJNN_NO_DEBUG

int
main(int argc, const char* argv[])
{
	using namespace djnn;
	init_core ();
	IntProperty i(nullptr, "", 111); // works
	IntProperty j(nullptr, "", 222); // works
	TextProperty k(nullptr, "", "toto"	); // works
	Assignment a(nullptr, "", &j, &i, true);
	a.activate (); // ******* does not work
	graph_exec ();
	printf("%d\n",j.get_value());
	std::cerr << __to_string(123) << " " << __to_string(&i) << std::endl;
	return 0;
}