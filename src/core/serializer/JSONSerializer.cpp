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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "serializer.h"

#if !defined(DJNN_NO_SERIALIZE)
#include <iostream>
#endif

namespace djnn
{

  

  /* FIXME:
  we don't know when is the end on the componant so there is problem with the last ","
  one of the solution, for djnn developpers only is to add the number of attributes in the start.
  save it and compare to __JSONLevel.
  */

#if !defined(DJNN_NO_SERIALIZE)
  static int __JSONLevel = 0;

  void
  JSONSerializer::start (const string& classname) {

    int i;
    for (i = 0; i < __JSONLevel; ++i)
      std::cout << "\t";
    std::cout << "\"" << classname << "\": {\n";
    ++__JSONLevel;

  }

  void
  JSONSerializer::text_attribute (const string& name, const string& value){

    int i;
    for (i = 0; i < __JSONLevel; ++i)
      std::cout << "\t";
    std::cout << "\"" << name << "\": \""<< value <<"\",\n";

  }

  void
  JSONSerializer::int_attribute (const string& name, int value){
 
    int i;
    for (i = 0; i < __JSONLevel; ++i)
      std::cout << "\t";
    std::cout << "\"" << name << "\": "<< value <<",\n";

  }

  void
  JSONSerializer::float_attribute (const string& name, double value){

    int i;
    for (i = 0; i < __JSONLevel; ++i)
      std::cout << "\t";
    std::cout << "\"" << name << "\": "<< value <<",\n";

  }

  void
  JSONSerializer::end (){

  int i;
  for (i = 0; i < __JSONLevel; ++i)
      std::cout << "\t";
  std::cout << "},\n";
   --__JSONLevel;

  }
#endif

}
