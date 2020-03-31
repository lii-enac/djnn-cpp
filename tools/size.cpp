#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/execution/graph.h"

#include "core/tree/int_property.h"
#include "core/tree/component.h"
#include "core/control/action.h"
#include "core/control/assignment.h"
#include "core/control/binding.h"
#include "core/control/simple_assignment.h"
#include "core/control/simple_binding.h"

#include "base/arithmetic.h"
#include "base/connector.h"
#include "base/fsm.h"

#include "gui/shapes/shapes.h"

#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;
using namespace djnn;


// c++ -o build/size -std=c++14 -Isrc -Ibuild -DDJNN_NO_DEBUG tools/size.cpp && build/size

#define PSIZE(type) cout << #type ": \t" << sizeof(type) << " " << "bytes" << endl; 

int
main(int argc, char* argv[])
{
	PSIZE(int);
	PSIZE(double);
	PSIZE(void*);
	PSIZE(string);
	PSIZE(vector<void*>);
	typedef map<string, void*> symtable_t;
	PSIZE(symtable_t);
	cout << endl;

	PSIZE(Process);
	PSIZE(Coupling);
	PSIZE(Vertex);
	cout << endl;

	PSIZE(IntProperty);
	PSIZE(Component);
	cout << endl;

	PSIZE(Action);
	PSIZE(Binding);
	PSIZE(SimpleBinding);
	PSIZE(Connector);
	PSIZE(Assignment);
	PSIZE(SimpleAssignment);
	cout << endl;

	PSIZE(Adder);
	cout << endl;

	PSIZE(FSM);
	PSIZE(FSMState);
	PSIZE(FSMTransition);
	cout << endl;

	PSIZE(Rectangle);

	return 0;
}