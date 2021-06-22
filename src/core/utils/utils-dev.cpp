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
 *
 */

#include "utils-dev.h"

#include "core/tree/ref_property.h"
#include "core/utils/djnn_dynamic_cast.h"
#include "core/ontology/coupling.h"
#include "core/core-dev.h" // graph add/remove edge

#include <cstdint>

#if !defined(DJNN_NO_DEBUG)
//#include <boost/core/demangle.hpp>
#include <cxxabi.h>  // needed for abi::__cxa_demangle
#endif

namespace djnn
{
  // DEBUG OPTIONS
  int _DEBUG_SEE_COLOR_PICKING_VIEW = 0;
  int _DEBUG_GRAPH_CYCLE_DETECT = 0;
  int _DEBUG_SEE_ACTIVATION_SEQUENCE = 0;
  int _DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_TIME_US = 1000;
  int _DEBUG_SEE_ACTIVATION_SEQUENCE_ONLY_TARGETED = 1;

  djnn::string
  __to_string(void*p)
  {
    return __to_string((uintptr_t)p);
  }

  djnn::string
  __to_string(long x)
  {
    char tab[256];
    int i=255;
    tab[i]=0;
    --i;
    tab[i]='0';
    bool neg = x<0;
    if(neg) x=-x;
    while (x && i>1) {
      tab[i] = '0'+(x%10);
      x/=10;
      --i;
    }
    if(neg) {
      tab[i] = '-';
      --i;
    }
    return djnn::string(&tab[i+1]); 
  }
  

  ref_info_t
  check_for_ref (FatProcess* src, const djnn::string& spec)
  {
    /* spec is empty */
    if (spec.empty ()) 
      return ref_info_t (nullptr, spec);
    
    size_t found = spec.find ("$value");
    /* we found §value */
    if (found != djnn::string::npos) {
      
      FatChildProcess *prop = src;
      
      /* $value is not the first element, eg : foo/bar/$value */
      if (found > 0) 
        prop = src->find_child_impl (spec.substr (0, found - 1));
      
      /* prop == src (not null) OR the new element find by find_child */
      if (!prop) 
        return ref_info_t (nullptr, spec);


      RefProperty *ref = djnn_dynamic_cast<RefProperty*> (prop);
      /* ref is a refproperty */
      if (ref != nullptr ) {
        djnn::string new_spec = "";
        /* spec is more than juste $value, eg : boo/bar/$value/toto */
        if ( spec.size () > 6) {
          new_spec = spec.substr (found + 6);
          if (new_spec.at (0) == '/') 
            new_spec = new_spec.substr (1);
        }

        return ref_info_t (ref, new_spec);
       }
     }

     /* we did not found $value */
     return ref_info_t (nullptr, spec);
   }

  const djnn::string&
  get_parent_name (const CoreProcess *cp, int up)
  {
    const auto * p = dynamic_cast<const FatProcess*>(cp);
    if(!p) return FatProcess::default_name;
    while(up && p) { p=p->get_parent(); --up; }
    if(p) return p->get_name();
    else return FatProcess::default_name;
  }

#if 0
  djnn::string cpp_demangle( char const * name )
  {
    return boost::core::demangle(name);
  }
#else
  // from boost/demangle
  inline char const * demangle_alloc( char const * name ) noexcept;
  inline void demangle_free( char const * name ) noexcept;

  class scoped_demangled_name
  {
  private:
      char const * m_p;

  public:
      explicit scoped_demangled_name( char const * name ) noexcept :
          m_p( demangle_alloc( name ) )
      {
      }

      ~scoped_demangled_name() noexcept
      {
          demangle_free( m_p );
      }

      char const * get() const noexcept
      {
          return m_p;
      }

      scoped_demangled_name( scoped_demangled_name const& ) = delete;
      scoped_demangled_name& operator= ( scoped_demangled_name const& ) = delete;
  };

#if !defined(DJNN_NO_DEBUG)
  char const * demangle_alloc( char const * name ) noexcept
  {
      int status = 0;
      std::size_t size = 0;
      return abi::__cxa_demangle( name, NULL, &size, &status );
  }

  void demangle_free( char const * name ) noexcept
  {
      std::free( const_cast< char* >( name ) );
  }

  djnn::string cpp_demangle( char const * name )
  {
      scoped_demangled_name demangled_name( name );
      char const * p = demangled_name.get();
      if( !p )
          p = name;
      return p;
  }
#else
  char const * demangle_alloc( char const * name ) noexcept
  {
      return name;
  }

  inline void demangle_free( char const * ) noexcept
  {
  }

  djnn::string cpp_demangle( char const * name )
  {
      return name;
  }

#endif
#endif

  const djnn::string
  get_hierarchy_name (const CoreProcess *cp, int up)
  {
    const auto * p = dynamic_cast<const FatProcess*>(cp);
    if(!p) return FatProcess::default_name;
    djnn::string res;
    while(up && p) { res = p->get_name()+"/"+res; p=p->get_parent(); --up; }
    return res;
  }

  // for gen_prop.py
	void
 	enable(Coupling* c, CoreProcess* dst)
  {
		if(c) {
		if(c->get_dst() == nullptr) {
			c->set_dst(dst);
		}
		c->enable();
		}
  }

	void
	disable (Coupling *c)
	{
		if (c) {
			c->disable();
		}
	}

	void
	remove_edge (Coupling *c)
	{
	}
 }
