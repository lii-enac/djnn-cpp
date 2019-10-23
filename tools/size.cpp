#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/tree/int_property.h"
#include "gui/abstract_gshape.h"
#include "gui/shapes/gen/rectangle.h"

#include <iostream>

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
	PSIZE(Process);
	PSIZE(Coupling);
	PSIZE(IntProperty);
	PSIZE(Rectangle);
	return 0;
}