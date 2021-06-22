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

// #include <boost/core/demangle.hpp>
#include "core/utils/utils-dev.h" // demangle
#include <typeinfo>

#include "core/utils/iostream.h"

#if _DEBUG_SEE_CREATION_DESTRUCTION_ORDER
//#include <boost/core/demangle.hpp>
#include "core/control/binding.h"
#include "core/control/native_expression_action.h"
#include "base/connector.h"
#include "base/switch.h"
#include "base/fsm.h"
#include "gui/shape/abstract_gshape.h"
#include "gui/style/style.h"
#include "gui/transformation/transformations.h"
#include "base/operators.h"
#endif

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
  run_stats(CoreProcess *p)
  {
  	int i=0;
    depth_first_traverse(p, [&i](CoreProcess* p){++i;});
    std::cerr << std::endl << "count items: " << i << std::endl;

    map<string,int> num_by_type;
    int num_no_coupling=0;
    int num_one_coupling=0;
    int num_more_than_one_coupling=0;
    int size=0;
    depth_first_traverse(p, [&](CoreProcess* p) -> void {
        ++num_by_type[cpp_demangle(typeid(*p).name())];
        num_no_coupling += !p->has_coupling();
        num_one_coupling += p->get_activation_couplings ().size() + p->get_deactivation_couplings ().size() == 1 ? 1 : 0;
        num_more_than_one_coupling += p->get_activation_couplings ().size() + p->get_deactivation_couplings ().size() > 1 ? 1 : 0;
        size += sizeof(*p);
    });
    for (auto item: num_by_type) {
        std::cerr << "type: " << item.first << " count: " << item.second << std::endl;
    }
    std::cerr << "num_no_coupling: " << num_no_coupling << " - " << (num_no_coupling * 100) / i << "%" << std::endl;
    std::cerr << "num_one_couplings: " << num_one_coupling << " - " << (num_one_coupling * 100) / i << "%" << std::endl;
    std::cerr << "num_more_than_one_couplings: " << num_more_than_one_coupling << " - " << (num_more_than_one_coupling * 100) / i << "%" << std::endl;
    std::cerr << "total mem size (*p): " << size << std::endl << std::endl;
  }


  #if _DEBUG_SEE_CREATION_DESTRUCTION_ORDER 

  extern list<pair<CoreProcess*, long int>> __creation_stat_order;
  extern vector<string> __destruction_stat_order;

  static list<pair<CoreProcess*, long int>>::iterator last_creation_end;
  static vector<string>::iterator last_destruction_end; 
  static bool init_display_creation_stats = false;

  void
  display_creation_stats()
  {
    if (init_display_creation_stats == false) {
      last_destruction_end = __destruction_stat_order.begin ();
      last_creation_end = __creation_stat_order.begin ();
      init_display_creation_stats = true;
    }

      int size = __creation_stat_order.size ();
      
      map<string,int> num_by_type;
      int num_no_coupling=0;
      int num_one_coupling=0;
      int num_more_than_one_coupling=0;
      int num_total_coupling = 0;
      int num_no_children = 0;
      int num_propertie = 0;
      int num_bindings = 0;
      int num_connectors = 0;
      int num_math_expr = 0;
      int num_FSM = 0;
      int num_FSMState = 0;
      int num_FSMTransition = 0;
      int num_switches = 0;
      int num_switheState = 0; // component dont le parent est un sw.
      int num_gshapes = 0;
      int num_gstype = 0;
      int num_gtransform = 0;

      // new Process ?
      if (last_creation_end != __creation_stat_order.end ()){

        for (auto it = last_creation_end ; it != __creation_stat_order.end (); it++){
          long int i = it->second;
          auto * p = it->first;
          std::cerr << "\033[1;34m";
          std::cerr << "[" << i << "] - " << cpp_demangle(typeid(*p).name()) <<  " - " << 
            (p->get_parent () ? p->get_parent ()->get_name () : "") << "/" << p->get_debug_name () << std::endl;
          std::cerr << "\033[0m" ;
        }

        last_creation_end = __creation_stat_order.end ();
      }
      else
        std::cerr << "nothing has been created" << std::endl;


      //delete Process ?
      if (last_destruction_end != __destruction_stat_order.end ()){
        for (auto it = last_destruction_end ; it != __destruction_stat_order.end (); ++it){
             std::cerr << "\033[1;31m";
             std::cerr << *it << std::endl;
             std::cerr << "\033[0m" ;
        }
        last_destruction_end = __destruction_stat_order.end ();
      }
      else 
        std::cerr << "nothing has been destroyed" << std::endl;


      for (auto pair : __creation_stat_order) { 

         auto * p = pair.first;

         ++num_by_type[cpp_demangle(typeid(*p).name())];
         num_no_coupling += !p->has_coupling();
         num_one_coupling += p->get_activation_couplings ().size() + p->get_deactivation_couplings ().size() == 1 ? 1 : 0;
         num_more_than_one_coupling += p->get_activation_couplings ().size() + p->get_deactivation_couplings ().size() > 1 ? 1 : 0;
         num_total_coupling += p->get_activation_couplings ().size() + p->get_deactivation_couplings ().size();
         num_no_children += p->children_size () > 0 ? 0 : 1;

         /* categories */
         num_propertie += (dynamic_cast<AbstractProperty*> (p) != 0) ? 1 : 0;
         num_bindings += (dynamic_cast<Binding*> (p) != 0) ? 1 : 0;
         num_connectors += (dynamic_cast<Connector*> (p) != 0) ? 1 : 0;
         num_math_expr += (dynamic_cast<UnaryOperatorCommon*> (p) != 0) ? 1 : 0;
         num_math_expr += (dynamic_cast<UnaryOperatorCommon*> (p) != 0) ? 1 : 0;
         num_math_expr += (dynamic_cast<NativeExpressionAction*> (p) != 0) ? 1 : 0;
         num_FSM += (dynamic_cast<FSM*> (p) != 0) ? 1 : 0;
         num_FSMState += (dynamic_cast<FSMState*> (p) != 0) ? 1 : 0;
         num_FSMTransition += (dynamic_cast<FSMTransition*> (p) != 0) ? 1 : 0;
         num_switches += (dynamic_cast<Switch*> (p) != 0) ? 1 : 0;
         // Component (Container) dont le parent est un sw.
         num_switheState += (dynamic_cast<Container*> (p) && p->get_parent () && dynamic_cast<Switch*> (p->get_parent ())) ? 1 : 0; 
         num_gshapes += (dynamic_cast<AbstractGShape*> (p) != 0) ? 1 : 0;
         num_gstype += (dynamic_cast<AbstractStyle*> (p) != 0) ? 1 : 0;
         num_gtransform += (dynamic_cast<AbstractTransformation*> (p) != 0) ? 1 : 0;
      }

      std::cerr << "\033[1;32m";
      std::cerr << std::endl << "-- ALL TYPE SUMMERY -- " << std::endl;
      for (auto item: num_by_type) {
        std::cerr << "type: " << item.first << " count: " << item.second << std::endl;
      }

      std::cerr << std::endl;
      std::cerr << "# Process: \t" << size << std::endl;
      std::cerr << "# Process no_children: \t" << num_no_children << std::endl;
      std::cerr << "# Properties: \t" << num_propertie << std::endl;
      std::cerr << "# Bindings: \t" << num_bindings << std::endl;
      std::cerr << "# Connectors: \t" << num_connectors << std::endl;
      std::cerr << "# Math+Expr: \t" << num_math_expr << std::endl;
      std::cerr << "# FSMs: \t" << num_FSM << std::endl;
      std::cerr << "# FSMStates: \t" << num_FSMState << std::endl;
      std::cerr << "# FSMTransition: \t"<< num_FSMTransition << std::endl;
      std::cerr << "# Switches: \t" << num_switches << std::endl;
      std::cerr << "# SwitchStates: \t" << num_switheState << std::endl; // component dont le parent est un sw.
      std::cerr << "# Shapes: \t" << num_gshapes << std::endl;
      std::cerr << "# Styles: \t" << num_gstype << std::endl;
      std::cerr << "# Transforms: \t" << num_gtransform << std::endl;
      std::cerr << std::endl;
      std::cerr << "# Coupling: \t" << num_total_coupling << std::endl; 
      std::cerr << "#_no_coupling: \t" << num_no_coupling << " - " << (num_no_coupling * 100) / size << "%" << std::endl;
      std::cerr << "#_one_couplings: \t" << num_one_coupling << " - " << (num_one_coupling * 100) / size << "%" << std::endl;
      std::cerr << "#_more_than_one_couplings: \t" << num_more_than_one_coupling << " - " << (num_more_than_one_coupling * 100) / size << "%" << std::endl;
      std::cerr << std::endl << std::endl;

      std::cerr << "\033[0m" ;
    }
    #else
    void
    display_creation_stats()
    {
      std::cerr << "WARNING - this is display_creation_stats stub - you maybe forget to activate _DEBUG_SEE_CREATION_DESTRUCTION_ORDER in utils/debug.h" << std::endl;
    } 
    #endif //_DEBUG_SEE_CREATION_DESTRUCTION_ORDER
}
