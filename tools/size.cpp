#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/tree/int_property.h"
#include "gui/abstract_gshape.h"
#include "gui/shapes/gen/rectangle.h"

#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;
using namespace djnn;


// c++ -o build/size -std=c++14 -Isrc -Ibuild tools/size.cpp

#define PSIZE(type) cout << #type ":\t" << sizeof(type) << " " << "bytes" << endl; 

int
main(int argc, char* argv[])
{
	PSIZE(int);
	PSIZE(double);
	PSIZE(void*);
	PSIZE(vector<void*>);
	PSIZE(string);
	typedef map<string, void*> symtable_t;
	PSIZE(symtable_t);
	PSIZE(Process);
	PSIZE(Coupling);
	PSIZE(IntProperty);
	PSIZE(Rectangle);
	return 0;
}