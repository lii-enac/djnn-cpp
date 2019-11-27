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


#include "utils.h"
#include "core/serializer/serializer.h"

#include <boost/core/demangle.hpp>
#include <typeinfo>

#include <iostream>

namespace djnn
{
  
  static bool __module_initialized = false;

  void
  init_utils () {

    if ( __module_initialized == false ) {

      __module_initialized = true;
      
      djnn::loadedModules.push_back("utils");
      
    }
  }


  void
  run_stats(Process *p)
  {
  	int i=0;
    depth_first_traverse(p, [&i](Process* p){++i;});
    std::cerr << "count " << i << std::endl;

    map<std::string,int> num_by_type;
    int num_no_coupling=0;
    int num_one_coupling=0;
    int num_more_than_one_coupling=0;
    int size=0;
    depth_first_traverse(p, [&](Process* p) -> void {
        ++num_by_type[boost::core::demangle(typeid(*p).name())];
        num_no_coupling += !p->has_coupling();
        num_one_coupling += p->get_activation_couplings ().size() + p->get_deactivation_couplings ().size() == 1 ? 1 : 0;
        num_more_than_one_coupling += p->get_activation_couplings ().size() + p->get_deactivation_couplings ().size() > 1 ? 1 : 0;
        size += sizeof(*p);
    });
    for (auto item: num_by_type) {
        std::cerr << "type: " << item.first << " count: " << item.second << std::endl;
    }
    std::cerr << "num_no_coupling: " << num_no_coupling << std::endl;
    std::cerr << "num_one_couplings: " << num_one_coupling << std::endl;
    std::cerr << "num_more_than_one_couplings: " << num_more_than_one_coupling << std::endl;
    std::cerr << "size: " << size << std::endl;
  }

}
