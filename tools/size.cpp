#include "core/core.h"
#include "base/base.h"
#include "gui/gui.h"


#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;
using namespace djnn;
using namespace djnnstl;


// c++ -o build/size -std=c++20 -Isrc -Ibuild -DDJNN_NO_DEBUG tools/size.cpp && build/size

#define PSIZE(type) cout << #type "\t" << std::right << std::setw(4) << sizeof(type) << " " << "bytes" << endl; 

int
main(int argc, char* argv[])
{
	PSIZE(unsigned int);
	PSIZE(unsigned long);
	PSIZE(float);
	PSIZE(double);
	PSIZE(void*);
	PSIZE(string);
	PSIZE(vector<void*>);
	typedef map<string, void*> symtable_t;
	PSIZE(symtable_t);
	cout << endl;

	PSIZE(CoreProcess);
	PSIZE(CouplingProcess);
	PSIZE(ChildProcess);
	PSIZE(FatProcess);
	cout << endl;

	PSIZE(Coupling);
	PSIZE(Vertex);
	cout << endl;

	PSIZE(Spike);
	PSIZE(FatSpike);
	PSIZE(CoreBinding);
	PSIZE(Binding);
	cout << endl;

	//PSIZE(SuperAbstractProperty);
	PSIZE(IntProperty);
	PSIZE(TextProperty);
	PSIZE(Component);
	cout << endl;

	PSIZE(Action);
	PSIZE(NativeAction);
	PSIZE(NativeExpressionAction);
	PSIZE(FatAction);
	PSIZE(FatNativeAction);

	cout << endl;

	PSIZE(CoreAssignment);
	PSIZE(Assignment);
	cout << endl;

	PSIZE(CoreConnector);
	PSIZE(Connector);
	cout << endl;

	PSIZE(Adder);
	cout << endl;

	PSIZE(FSM);
	PSIZE(FSMState);
	PSIZE(FSMTransition);
	cout << endl;

	PSIZE(Rectangle);
	PSIZE(Path);
	PSIZE(Text);

	return 0;
}