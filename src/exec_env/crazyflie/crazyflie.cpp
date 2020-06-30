#include <stddef.h>
#include <stdlib.h>

extern "C" {
	void DJNN_DEBUG_PRINT(const char* fmt);
	unsigned long uxTaskGetNumberOfTasks();
	void *pvPortMalloc( size_t xSize );
	void vPortFree( void *pv );
}

#include <exception>

#if 1
#define USE_FREERTOS 1

// Define the ‘new’ operator for C++ to use the freeRTOS memory management
// functions. THIS IS NOT OPTIONAL!
//
void *operator new(size_t size) {
	void *p;
#ifdef USE_FREERTOS
	if(uxTaskGetNumberOfTasks())
		p=pvPortMalloc(size);
	else
  		p=malloc(size);
#else
 	p=malloc(size);
#endif

#ifdef __EXCEPTIONS
 if (p==0) // did pvPortMalloc succeed?
  throw std::bad_alloc(); // ANSI/ISO compliant behavior
#endif
 return p;
}

//
// Define the ‘delete’ operator for C++ to use the freeRTOS memory management
// functions. THIS IS NOT OPTIONAL!
//
void operator delete(void *p) {

#ifdef USE_FREERTOS
	if(uxTaskGetNumberOfTasks())
		vPortFree( p );
	else
		free( p );
#else
 	free( p );
#endif
	p = NULL;
}

void *operator new[](size_t size){
	void *p;
#ifdef USE_FREERTOS
 if(uxTaskGetNumberOfTasks())
  p=pvPortMalloc(size);
 else
  p=malloc(size);
#else
 p=malloc(size);
#endif

#ifdef __EXCEPTIONS
	if (p==0) // did pvPortMalloc succeed?
 		throw std::bad_alloc(); // ANSI/ISO compliant behavior
#endif
 return p;
}

//
// Define the ‘delete’ operator for C++ to use the freeRTOS memory management
// functions. THIS IS NOT OPTIONAL!
//
void operator delete[](void *p){
#ifdef USE_FREERTOS
 if(uxTaskGetNumberOfTasks())
  vPortFree( p );
 else
  free( p );
#else
 free( p );
#endif
 p = NULL;
}

/* Optionally you can override the ‘nothrow’ versions as well.
   This is useful if you want to catch failed allocs with your
   own debug code, or keep track of heap usage for example,
   rather than just eliminate exceptions.
 */

void* operator new(std::size_t size, const std::nothrow_t&) {
    return malloc(size);
}

void* operator new[](std::size_t size, const std::nothrow_t&) {
    return malloc(size);
}

void operator delete(void* ptr, const std::nothrow_t&) {
    free(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t&) {
    free(ptr);
} 
#endif

#if 0
void *operator new(size_t size)
{
	DJNN_DEBUG_PRINT( "operator new\n");
   void *p;

   if(uxTaskGetNumberOfTasks())
      p=pvPortMalloc(size);
   else
      p=malloc(size);

   return p;
}

void operator delete(void *p) _GLIBCXX_USE_NOEXCEPT
{
   if(uxTaskGetNumberOfTasks())
      vPortFree( p );
   else
      free( p );

   p = NULL;
}
#endif

#include <string>
#include <sstream>

#include "core/tree/int_property.h"
#include "core/control/binding.h"
// #include "core/control/assignment.h"

// crazyflie firmware
//#include "utils/interface/debug.h"



namespace djnn {

	int i=0;

	void
	titi(int j)
	{
		if (j) {
			if(j%49789)
				i*=i;
		} else titi(j-1);
	}

	void
	toto(int j)__attribute__((noinline)); // __attribute__((notailcall)); //  

	void
	toto(int j)
	{
		if(j%10) titi(i++);
		if(j) toto(j-1);
		else
			titi(3);
	}


	struct stefA {
		virtual ~stefA() {}
		virtual void f() {}
	};

	struct stefB : stefA {
		virtual ~stefB() {}
		virtual void f() {}	
	};

	std::string dbg;

	void
	init_prog()
	{
		std::set_terminate([](){ DJNN_DEBUG_PRINT( "Unhandled exception\n"); }); // useless?

		DJNN_DEBUG_PRINT( "enter init_prog\n" );
		try {
			//toto(50000000);
			int * p = new int;
			delete p;
			//char sss[255];
			//sprintf(sss,"%d",__LINE__);

			//std::string __s("yep");
			//dbg = std::string("sco ")+sss+"\n"; // this works
			//DJNN_DEBUG_PRINT( dbg.c_str() );
			//std::to_string(12); // ********* does not work: VERY SUSPECT (call to vsnprintf)
			//DJNN_DEBUG_PRINT( dbg.c_str() );

			DJNN_DEBUG_PRINT( "sco 1\n" );
			//throw 1; // does not work

			/*stefA* pa;
			pa = new stefB;
			pa->f();
			//std::ostringstream out;
			//out << 12;
			//DJNN_DEBUG_PRINT( (out.str() + "\n").c_str()); // works

			//dbg = std::string("sco ") + std::to_string(__LINE__)+"\n"; // does not work
			//DJNN_DEBUG_PRINT( dbg.c_str() );
			//DJNN_DEBUG_PRINT( "TtTTTTTTT\n");

			DJNN_DEBUG_PRINT( "sco 2\n" );
			*/
			IntProperty ii(nullptr, "", i); // works
			IntProperty j(nullptr, "", 2); // works
			Binding a(nullptr, "", &ii, &j); // works
			//Assignment a(nullptr, "", &i, &j, true);
			//i.set_parent(nullptr);
			ii.is_activated (); // works
			ii.get_process_type (); // works
			DJNN_DEBUG_PRINT( "activating\n" );
			//ii.activate (); // ******* does not work
			//j.get_value () == 5;
			DJNN_DEBUG_PRINT( "end init_prog\n" );
		} catch (std::exception& e) {
			dbg = std::string(e.what()) + "\n";
			DJNN_DEBUG_PRINT( dbg.c_str() );
		} catch (...) {
			DJNN_DEBUG_PRINT( "unknown c++ exception\n" );
		}
		DJNN_DEBUG_PRINT( "exit init_prog\n" );
	}
}


extern "C" {
	void djnn_init_prog () {
		djnn::init_prog ();
	}
}


