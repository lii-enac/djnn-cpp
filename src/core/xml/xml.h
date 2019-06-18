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
#pragma once


#include "../utils/utils-dev.h"
#include "../ontology/process.h"
#include <map>

typedef char XML_Char; // FIXME should not be public, and avoid including expat.h (maybe in xml-dev.h)

namespace djnn
{
  typedef Process*
  (*djn_XMLTagStartProc) (const char**, Process*);
  typedef Process*
  (*djn_XMLTagDataProc) (const char*, int, Process*);
  typedef Process*
  (*djn_XMLTagEndProc) (Process*);

  typedef int
  (*djn_XMLAttrProc) (Process**, const char*);


  typedef struct
  {
    const char* name;
    djn_XMLAttrProc handle_attr;
  } djn_XMLAttrHandler;

  typedef struct
  {
    const char* name;
    djn_XMLTagStartProc start;
    djn_XMLTagEndProc end;
    djn_XMLTagDataProc data;
  } djn_XMLTagHandler;

  typedef struct djn__XMLTagHandlerList djn__XMLTagHandlerList;
  struct djn__XMLTagHandlerList
  {
    djn_XMLTagHandler *handler;
    struct djn__XMLTagHandlerList *next;
  };

  typedef djn_XMLAttrHandler*
  (*djn_XMLSymLookupProc) (const char*, gperf_t);
  typedef djn_XMLTagHandler*
  (*djn_XMLTagLookupProc) (const char*, gperf_t);

  typedef struct
  {
    djn_XMLTagLookupProc lookup;
    const char* format;
  } djn__XMLParser;

  class XML {
  public:
    static Process* djnLoadFromXML (const std::string &uri);
    static Process* djnParseXML (FILE* f);
    static int djn_RegisterXMLParser (const string &uri, djn_XMLTagLookupProc l, const char* f);
    static void clear_xml_parser ();
    static int djn_XMLHandleAttr (Process** e, const char** attrs, djn_XMLSymLookupProc lookup, ...);
  private:
    static void
    djn__XMLPushTagHandler (djn_XMLTagHandler *h);
    static djn_XMLTagHandler*
    djn__XMLPopTagHandler ();
    static djn_XMLTagHandler*
    djn__XMLFindTagHandler (const XML_Char* name);
    static size_t
    djn__ReadXML (const char *buf, size_t size, size_t nmemb, void *stream);
    static void
    djn__XMLTagStart (void*, const XML_Char*, const XML_Char**);
    static void
    djn__XMLTagEnd (void*, const XML_Char*);
    static void
    djn__XMLDataHandle (void*, const XML_Char*, int);
    static void
    djn__XMLNamespaceStart (void*, const XML_Char*, const XML_Char*);
    static void
    djn__XMLNamespaceEnd (void*, const XML_Char*);
    static map<string, djn__XMLParser*> *djn__NamespaceTable;
    static Process *curComponent;
    static djn__XMLTagHandlerList *handlerStack;
  };
  void
  init_xml ();
  inline Process* loadFromXML (const std::string &uri) { return XML::djnLoadFromXML (uri); }
}
