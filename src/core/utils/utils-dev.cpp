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

#include "core/utils/djnn_dynamic_cast.h"
#include <cstdint>


namespace djnn
{
  std::string
  __to_string(void*p)
  {
    return __to_string((uintptr_t)p);
  }

  std::string
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
    return std::string(&tab[i+1]); 
  }
  

  ref_info_t
  check_for_ref (Process* src, const std::string& spec)
  {
    /* spec is empty */
    if (spec.empty ()) 
      return ref_info_t (nullptr, spec);
    
    size_t found = spec.find ("$value");
    /* we found §value */
    if (found != std::string::npos) {
      
      Process *prop = src;
      
      /* $value is not the first element, eg : foo/bar/$value */
      if (found > 0) 
        prop = src->find_child (spec.substr (0, found - 1));
      
      /* prop == src (not null) OR the new element find by find_child */
      if (!prop) 
        return ref_info_t (nullptr, spec);


      RefProperty *ref = djnn_dynamic_cast<RefProperty*> (prop);
      /* ref is a refproperty */
      if (ref != nullptr ) {
        std::string new_spec = "";
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

  const std::string&
  get_parent_name (Process *p, int up)
  {
    while(up && p) { p=p->get_parent(); --up; }
    if(p) return p->get_name();
    else return Process::default_name;
  }

  const std::string
  get_hierarchy_name (Process *p, int up)
  {
    std::string res;
    while(up && p) { res = p->get_name()+"/"+res; p=p->get_parent(); --up; }
    return res;
  }
 }
