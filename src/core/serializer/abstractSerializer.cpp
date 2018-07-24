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
#include "../error.h"
#include "serializer.h"

using namespace std;

namespace djnn
{

  /* init static variable */
  Process* AbstractSerializer::serializationRoot = nullptr;
  AbstractSerializer* AbstractSerializer::serializer = nullptr;

  static string __cur_format;
  
  void
  AbstractSerializer::pre_serialize (Process* root, const string& format) {
     
     if (AbstractSerializer::serializationRoot == 0) {

        AbstractSerializer::serializationRoot = root;
        __cur_format = format;
        
        if (format.compare("XML") == 0) {
          AbstractSerializer::serializer = new XMLSerializer();
          cout << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n";
        }
        else if (format.compare ("JSON") == 0){
          AbstractSerializer::serializer = new JSONSerializer();
          cout << "{\n";
        }
        else
          warning ( format + " is not a valid serializer format (XML|JSON) " );
     }
  }

  void
  AbstractSerializer::post_serialize (Process* root) {

    if (AbstractSerializer::serializationRoot == root) {
      AbstractSerializer::serializationRoot = nullptr;
      AbstractSerializer::serializer = nullptr;

      if (__cur_format.compare("JSON") == 0) 
      cout << "}\n";
    }

  }

}
