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

#include "../core-dev.h"
#include "serializer.h"

using namespace std;

namespace djnn
{

  static int __JSONLevel = 0;

  void
  JSONSerializer::start (const string& classname) {

    int i;
    for (i = 0; i < __JSONLevel; ++i)
      cout << "\t";
    cout << "\"" << classname << "\": {\n";
    ++__JSONLevel;

  }

  void
  JSONSerializer::text_attribute (const string& name, const string& value){

    int i;
    for (i = 0; i < __JSONLevel; ++i)
      cout << "\t";
    cout << "\"" << name << "\": \""<< value <<"\",\n";

  }

  void
  JSONSerializer::int_attribute (const string& name, int value){
 
    int i;
    for (i = 0; i < __JSONLevel; ++i)
      cout << "\t";
    cout << "\"" << name << "\": "<< value <<",\n";

  }

  void
  JSONSerializer::float_attribute (const string& name, int value){

    int i;
    for (i = 0; i < __JSONLevel; ++i)
      cout << "\t";
    cout << "\"" << name << "\": "<< value <<",\n";

  }

  void
  JSONSerializer::end (){

  int i;
  for (i = 0; i < __JSONLevel; ++i)
      cout << "\t";
  cout << "}\n";
   --__JSONLevel;

  }

}
