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

#include "core/core-dev.h"
#include "serializer.h"

#if !defined(DJNN_NO_SERIALIZE)
#include <iostream>
#endif

namespace djnn
{

  using namespace std;

  struct djnXMLDumpContext {
    struct djnXMLDumpContext* parent;
    int haschildren;
    //const char* classname;
    std::string classname;
  };// __XMLDumpContext;

#if !defined(DJNN_NO_SERIALIZE)

  static int __XMLDumpNumAttrs;
  //static __XMLDumpContext* __curXMLDumpContext = 0;
  static djnXMLDumpContext* __curXMLDumpContext = 0;

  static int __XMLLevel = 0;

  void
  XMLSerializer::start (const std::string& name) {

  
    int i;
    //__XMLDumpContext* c;
    djnXMLDumpContext *c;

    if (__curXMLDumpContext && !__curXMLDumpContext->haschildren) {
      if (__XMLDumpNumAttrs > 0)
        cout << " ";

      cout << ">\n";
      ++__XMLLevel;
      __curXMLDumpContext->haschildren = 1;
    }

    for (i = 0; i < __XMLLevel; ++i)
      cout << "  ";
    cout << "<" << name;

    if (__curXMLDumpContext == 0) {
      for (auto module_name : djnn::loadedModules) {
        int l = name.length () + 3;
        if (module_name.compare("core") == 0)
          cout << " xmlns=\"http://xml.djnn.net/2012/" << module_name << "\"";
        else {
          cout << endl ;
          while (--l)
            cout << " ";
          cout << "xmlns:" << module_name << "=\"http://xml.djnn.net/2012/" << module_name << "\"";
        }
      }
    }

    //c = (djnXMLDumpContext*) malloc (sizeof (djnXMLDumpContext));
    c = new djnXMLDumpContext;
    c->haschildren = 0;
    //c->classname = name.c_str();
    c->classname = name;
    c->parent = __curXMLDumpContext;
    __curXMLDumpContext = c;
    __XMLDumpNumAttrs = 0;

  }

  void
  XMLSerializer::text_attribute (const std::string& name, const std::string& value){
    cout << " " << name << "=\"" << value << "\"";
    ++__XMLDumpNumAttrs;
  }

  void
  XMLSerializer::int_attribute (const std::string& name, int value){
    cout << " " << name << "=\"" << value << "\"";
    ++__XMLDumpNumAttrs;
  }

  void
  XMLSerializer::float_attribute (const std::string& name, double value){
    cout << " " << name << "=\"" << value << "\"";
    ++__XMLDumpNumAttrs;
  }

  void
  XMLSerializer::end (){

    //__XMLDumpContext* c;
    djnXMLDumpContext* c;

    if (__curXMLDumpContext->haschildren) {
      int i;
      --__XMLLevel;
      for (i = 0; i < __XMLLevel; ++i)
        cout << " ";
      cout << "</" << __curXMLDumpContext->classname << ">\n";
    } else {
      if (__XMLDumpNumAttrs > 0)
        cout << " ";
      cout << "/>\n";
    }

    c = __curXMLDumpContext->parent;
    free (__curXMLDumpContext);
    __curXMLDumpContext = c;
  }
#endif
}
