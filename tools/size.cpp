#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/execution/graph.h"

#include "core/tree/int_property.h"
#include "core/tree/component.h"
#include "core/control/assignment.h"
#include "core/control/binding.h"

#include "base/arithmetic.h"
#include "base/connector.h"
#include "base/fsm.h"

#include "gui/abstract_gshape.h"
#include "gui/shapes/gen/rectangle.h"

#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;
using namespace djnn;


// c++ -o build/size -std=c++14 -Isrc -Ibuild tools/size.cpp && build/size

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

	PSIZE(Binding);
	PSIZE(Connector);
	PSIZE(Assignment);
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