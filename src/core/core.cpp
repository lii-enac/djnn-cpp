/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#if RMT_ENABLED
#include "core/utils/ext/remotery/Remotery.h"
#endif

//#include <locale.h>
//#include "core/utils/error.h"
#include "core/execution/graph.h"

//#include "utils/debug.h"
#include "core/utils/iostream.h"


#include <locale.h>


namespace djnn
{
  class FatProcess;
  class StructureObserver;


  vector<string> loadedModules;
/*
  void
  module_loaded(const char*)
  {
    djnn::loadedModules.push_back("core");
  }
*/
  vector<StructureObserver*> structure_observer_list;

#if RMT_ENABLED
  Remotery* rmt;
#endif

  static bool __module_initialized = false;

  void
  init_core ()
  {    
    if (__module_initialized == false) {
      //printf("init_core\n");
      __module_initialized = true;
      //djnn::loadedModules.push_back("core");
      setlocale (LC_NUMERIC, "C");
      //Graph::instance (); // make sure an instance is built ASAP since some platform may not be able to build it globally
    }

  /* especially for unit test - make sure graph::_activation_deque is clean for each run */
  Graph::instance ().clear_activation ();

#if RMT_ENABLED
    rmtSettings* settings = rmt_Settings();
    if(settings) {
      settings->reuse_open_port = RMT_TRUE;
      //settings->enableThreadSampler = RMT_FALSE;
    }

    enum rmtError err = rmt_CreateGlobalInstance(&rmt);
    if(err) {
      //warning (nullptr, "Rmt error");
      //std::cerr << "rmt error " << err << __FL__;
    }
#endif
  }

  extern void delete_parentless_processes ();
  extern void clear_xml ();

  void
  clear_core ()
  {
    delete &Graph::instance (); // destructor will set Graph::_instance to nullptr since _instance is private
    //XML::clear_xml_parser ();
#if RMT_ENABLED
    rmt_DestroyGlobalInstance(rmt);
#endif
    //std::cerr << __PRETTY_FUNCTION__ << __FL__;
    clear_xml ();
    delete_parentless_processes ();
    __module_initialized = false;
  }

  void
  graph_add_edge (CoreProcess* src, CoreProcess* dst)
  {
    Graph::instance ().add_edge (src, dst);
  }
  
  void
  graph_remove_edge (CoreProcess* src, CoreProcess* dst)
  {
    Graph::instance ().remove_edge (src, dst);
  }

  void
  graph_check_order (CoreProcess* p1, CoreProcess* p2) {
#if _DEBUG_ENABLE_CHECK_ORDER
    Graph::instance ().check_order (p1, p2);
#endif
  }
  void
  graph_exec ()
  {
    Graph::instance ().exec ();
  }

}


// temporyary hack
#if DJNN_USE_FREERTOS
#include "core/xml/xml.h"

namespace djnn {
  void XML::clear_xml_parser () {}
}
#endif


#if DJNN_STL_EASTL
// https://github.com/paulhodge/EASTL/blob/community/example/example1.cpp
// EASTL expects us to define these, see allocator.h line 194
void* operator new[](size_t size, const char* /* pName */,
                     int /* flags */, unsigned /* debugFlags */,
                     const char* /* file */, int /* line */) {
    return malloc(size);
}

void* operator new[](size_t size, size_t alignment,
                     size_t /* alignmentOffset */, const char* /* pName */,
                     int /* flags */, unsigned /* debugFlags */,
                     const char* /* file */, int /* line */) {
    // this allocator doesn't support alignment
    EASTL_ASSERT(alignment <= 8);
    return malloc(size);
}

// EASTL also wants us to define this (see string.h line 197)
namespace EA {
namespace StdC {
int Vsnprintf(char8_t* pDestination, size_t n,
               const char8_t* pFormat, va_list arguments) {
#ifdef _MSC_VER
        return _vsnprintf(pDestination, n, pFormat, arguments);
#else
        return vsnprintf(pDestination, n, pFormat, arguments);
#endif
}
}
}

int stoi(const eastl::string& s)
{
  return atoi(s.c_str());
}

int stoi(const eastl::string& s, size_t* idx, int base)
{
  return strtol(s.c_str(), (char **)&idx, base);
}

double stof(const eastl::string& s)
{
  return atof(s.c_str());
}

double stof(const eastl::string& s, size_t* idx)
{
  return strtod(s.c_str(), (char **)&idx);
}

#endif

