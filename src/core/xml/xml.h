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

#include "core/ontology/process.h"
#include "core/utils/error.h"
#include "core/utils/utils-dev.h"

typedef char XML_Char; // FIXME should not be public, but avoid including expat.h (might be in xml-dev.h?)

namespace djnn {
typedef FatProcess* (*djn_XMLTagStartProc) (const char**, FatProcess*);
typedef FatProcess* (*djn_XMLTagDataProc) (const char*, int, FatProcess*);
typedef FatProcess* (*djn_XMLTagEndProc) (FatProcess*);

typedef int (*djn_XMLAttrProc) (FatProcess**, const char*);

typedef struct
{
    djn_XMLAttrProc handle_attr;
} djn_XMLAttrHandler;

typedef struct
{
    djn_XMLTagStartProc start;
    djn_XMLTagEndProc   end;
    djn_XMLTagDataProc  data;
} djn_XMLTagHandler;

typedef struct djn__XMLTagHandlerList djn__XMLTagHandlerList;
struct djn__XMLTagHandlerList {
    djn_XMLTagHandler*             handler;
    struct djn__XMLTagHandlerList* next;
};

typedef djn_XMLAttrHandler* (*djn_XMLSymLookupProc) (const char*, gperf_t);
typedef djn_XMLTagHandler* (*djn_XMLTagLookupProc) (const char*, gperf_t);

typedef struct
{
    djn_XMLTagLookupProc lookup;
    const char*          format;
} djn__XMLParser;

class XML {
  public:
    using string = djnnstl::string;
    static FatProcess* djnLoadFromXML (const string& uri);
    static FatProcess* djnLoadThenClone (const string& uri);
    // static FatProcess* djnParseXML (FILE* f);
    static FatProcess* djnParseXMLFromPath (const string& path);
    static int         djn_RegisterXMLParser (const string& uri, djn_XMLTagLookupProc l, const char* format);
    static int         djn_UnregisterXMLParser (const string& uri);
    static void        clear_xml_parser ();
    static int         djn_XMLHandleAttr (FatProcess** e, const char** attrs, djn_XMLSymLookupProc lookup, ...);

  private:
    static void
    djn__XMLPushTagHandler (djn_XMLTagHandler* h);
    static djn_XMLTagHandler*
    djn__XMLPopTagHandler ();
    static djn_XMLTagHandler*
    djn__XMLFindTagHandler (const XML_Char* name);
    static size_t
    djn__ReadXML (const char* buf, size_t size, size_t nmemb, void* stream);
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
    typedef djnnstl::map<string, djn__XMLParser*> djn__NamespaceTable_t;
    static djn__NamespaceTable_t                  djn__NamespaceTable;
    static FatProcess*                            curComponent;
    static djn__XMLTagHandlerList*                handlerStack;
};

// void init_xml ();
// void clear_xml ();
// inline FatProcess* load_from_XML (const string& uri) { return XML::djnLoadThenClone (uri); }
// inline FatProcess* load_from_XML_once (const string& uri) { return XML::djnLoadFromXML (uri); }
// FatProcess* loadFromXML (const string& uri);
} // namespace djnn
