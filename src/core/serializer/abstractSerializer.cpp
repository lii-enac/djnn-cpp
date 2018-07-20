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
  
  void
  AbstractSerializer::pre_serialize (Process* root, const string& type) {
     
     if (AbstractSerializer::serializationRoot == 0) {

        AbstractSerializer::serializationRoot = root;
        
        if (type.compare("XML") == 0) {
          AbstractSerializer::serializer = new XMLSerializer();
          cout << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n";
        }
        else if (type.compare ("JSON") == 0){
          warning ( "JSON serializer NOT Yet implemented " );
        }
        else
          warning ( type + " is not a valid serializer format (XML|JSON) " );
     }
  }

  void
  AbstractSerializer::post_serialize (Process* root) {

     if (AbstractSerializer::serializationRoot == root) {
      AbstractSerializer::serializationRoot = nullptr;
      AbstractSerializer::serializer = nullptr;
    }

  }

}
