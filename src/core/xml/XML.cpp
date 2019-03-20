/*
 *	djnn v2 libraries
 *
 *	The copyright holders for the contents of this file are:
 *		Ecole Nationale de l'Aviation Civile, France (2008-2018)
 *	See file "license.terms" for the rights and conditions
 *	defined by copyright holders.
 *
 *	Core of the XML parsing system
 *
 *	Contributors:
 *		Stéphane Chatty <chatty@enac.fr>
 *		Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "../tree/process.h"
#include "xml.h"
#include "xml-dev.h"
#include "../tree/text_property.h"
#include "../error.h"


#include <expat.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdio.h>
#include <map>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <stdarg.h>

namespace djnn {
  using namespace std;


  #define BUFFSIZE 8192
  static char buf[BUFFSIZE];
  map<string, djn__XMLParser*> *XML::djn__NamespaceTable = new map<string, djn__XMLParser*>;
  Process *XML::curComponent = nullptr;
  djn__XMLTagHandlerList *XML::handlerStack = nullptr;

  /*
   * I. The public API: initialisation and parsing function
   */

  typedef struct
  {
    XML_Parser parser;
  } djn__CurlData;

  size_t
  XML::djn__ReadXML (const char *buf, size_t size, size_t nmemb, void *stream)
  {
    djn__CurlData *d = (djn__CurlData*) stream;
    int len = size * nmemb;

    if (XML_Parse (d->parser, buf, len, len == 0))
      return len;
    else {
      fprintf (stderr, "Parse error at line %d:\n%s\n", (int) XML_GetCurrentLineNumber (d->parser),
               XML_ErrorString (XML_GetErrorCode (d->parser)));
      return -1;
    }
  }

  /* should add handling of '-' to denote stdin, until a URI is defined for that */
  Process*
  XML::djnLoadFromXML (const string &uri)
  {
    string uri_ = uri;
    std::size_t found = uri.find("://");
    if (found == std::string::npos) {
      FILE * pFile;
      pFile = fopen (uri.c_str (),"r");
      Process* res;
      if (pFile!=NULL)
        res = djnParseXML (pFile);
      else {
        cerr << "unable to load file " + uri << endl;
        return nullptr;
      }
      fclose (pFile);
      return res;
    }
    djn__CurlData d;
    CURLcode res;
    CURL *curl = curl_easy_init ();

    if (!curl)
      return 0;
    curl_easy_setopt(curl, CURLOPT_URL, uri_.c_str ());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, djn__ReadXML);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &d);

    d.parser = XML_ParserCreateNS ("UTF-8", '*');
    XML_SetElementHandler (d.parser, &djn__XMLTagStart, &djn__XMLTagEnd);
    XML_SetCharacterDataHandler (d.parser, &djn__XMLDataHandle);
    XML_SetNamespaceDeclHandler (d.parser, &djn__XMLNamespaceStart, &djn__XMLNamespaceEnd);

    curComponent = 0;
    res = curl_easy_perform (curl);
    curl_easy_cleanup (curl);
    XML_ParserFree (d.parser);
    if (res == CURLE_OK) {
      return curComponent;
    }
    else {
      error (nullptr, "could not parse " + uri);
      return 0;
    };
  }

  Process*
  XML::djnParseXML (FILE* f)
  {
    XML_Parser p = XML_ParserCreateNS ("UTF-8", '*');
    int done = 0;

    XML_SetElementHandler (p, &djn__XMLTagStart, &djn__XMLTagEnd);
    XML_SetCharacterDataHandler (p, &djn__XMLDataHandle);
    XML_SetNamespaceDeclHandler (p, &djn__XMLNamespaceStart, &djn__XMLNamespaceEnd);

    curComponent = 0;

    while (!done) {
      int len;

      len = fread (buf, 1, BUFFSIZE, f);
      if (ferror (stdin)) {
        fprintf (stderr, "Read error\n");
        return 0;
      }
      done = feof (f);

      if (!XML_Parse (p, buf, len, done)) {
        fprintf (stderr, "Parse error at line %d:\n%s\n", (int) XML_GetCurrentLineNumber (p),
                 XML_ErrorString (XML_GetErrorCode (p)));
        return 0;
      }
    }
    return curComponent;
  }

  /*
   * II. The semi-public API, provided to implement new parsers
   */


  int
  XML::djn_RegisterXMLParser (const string &uri, djn_XMLTagLookupProc l, const char* f)
  {
    djn__XMLParser *h;
    map<string, djn__XMLParser*>::iterator it;
    it = djn__NamespaceTable->find (uri);
    if (it != djn__NamespaceTable->end ()) {
      cerr << "conflict of XML parsers on namespace " << uri << endl;
      return 0;
    }

    h = (djn__XMLParser*) malloc (sizeof(djn__XMLParser));
    h->lookup = l;
    h->format = f;

    djn__NamespaceTable->insert (pair<string, djn__XMLParser*> (uri, h));
    return 1;
  }

  void
  XML::clear_xml_parser () {
    djn__NamespaceTable->clear ();
  }

  int
  XML::djn_XMLHandleAttr (Process** e, const char** attrs, djn_XMLSymLookupProc lookup, ...)
  {
    /* we are passed a list of lookup procedures */
    va_list p;
    va_start (p, lookup);

    /* iterate on all lookup procedures */
    while (lookup) {
      /* does this one know how to handle the current attribute key? */
      djn_XMLAttrHandler* h = lookup (*attrs, strlen (*attrs));
      /* if yes, call the handler with the attribute value, and return */
      if (h) {
        int success;
        va_end (p);
        success = (h->handle_attr (e, *++attrs));
        return success;
      }
      /*if no, keep iterating */
      lookup = va_arg (p, djn_XMLSymLookupProc);
    }

    /* failure */
    va_end (p);

    return 0;
  }

  /*
   * III. Internals: the procedures hooked onto Expat,
   *		and their supporting routines
   */

  /* we need to manage a stack of tag handlers so as to known what routine
   to call when we encounter text data during parsing */

  void
  XML::djn__XMLPushTagHandler (djn_XMLTagHandler *h)
  {
    djn__XMLTagHandlerList *l = (djn__XMLTagHandlerList*) malloc (sizeof(djn__XMLTagHandlerList));
    l->handler = h;
    l->next = handlerStack;
    handlerStack = l;
  }

  djn_XMLTagHandler*
  XML::djn__XMLPopTagHandler ()
  {
    djn_XMLTagHandler *h;
    djn__XMLTagHandlerList *l = handlerStack;

    if (!l)
      return 0;
    h = l->handler;
    handlerStack = l->next;
    free (l);
    return h;
  }

  /* find if we have a tag handler for this tag name */

  djn_XMLTagHandler*
  XML::djn__XMLFindTagHandler (const XML_Char* name)
  {
    djn__XMLParser *p = 0;
    djn_XMLTagHandler *h = 0;
    
    char* n = strchr ((XML_Char*) name , '*');
    if (n) {
      /* if yes, let's split the tag name in two parts */
      *n = '\0';

      /* find a parser for this name space */
      map<string, djn__XMLParser*>::iterator it;

      it = djn__NamespaceTable->find (name);
      if (it != djn__NamespaceTable->end ()) {
        /* if found, get the associated parser data */
        p = it->second;
      } else {
#ifdef DEBUG
        fprintf (stderr, "ignoring XML tag '%s': unknown namespace '%s'\n", n+1, name);
#endif
        p = 0;
      }
      /* restore the tag name buffer, otherwise expat will stop working well */
      *n = '*';
      name = n + 1;
    } 

    if (!p) 
      return 0;

    /* ask the parser if it knows this tag */
    h = p->lookup (name, strlen (name));
    if (!h)
      fprintf (stderr, "Unknown %s tag: %s\n", p->format, name);
    return h;
  }

  /* function called by expat when a tag start is encountered */
  void
  XML::djn__XMLTagStart (void* userData, const XML_Char* name, const XML_Char** attrs)
  {
    /* do we know how to handle this tag? */
    djn_XMLTagHandler *h = djn__XMLFindTagHandler (name);

    /* if yes, handle it and push the handler onto the stack */
    if (h) {
      Process* e = h->start (attrs, curComponent);
      if (e)
        curComponent = e;
      djn__XMLPushTagHandler (h);
    }
  }

  /* function called by expat when a tag end is encountered */
  void
  XML::djn__XMLTagEnd (void* userData, const XML_Char* name)
  {
    /* we could implement this function by relying on the handler stack.
     we rather chose to use the tag name, do a 'find' and check that
     the result is consistent with what the stack says. */

    /* do we know how to handle this tag? */
    djn_XMLTagHandler *h = djn__XMLFindTagHandler (name);
    djn_XMLTagHandler *old_h;

    /* if yes, handle it and pop the handler from the stack */
    if (h) {
      Process* e = h->end (curComponent);
      if (e)
        curComponent = e;
      old_h = djn__XMLPopTagHandler ();
      if (old_h != h)
        fprintf (stderr, "Warning: closing tag %s does not match opening one.\n", name);
    }
  }

  /* function called by expat when data is encountered */
  void
  XML::djn__XMLDataHandle (void* userData, const XML_Char* data, int len)
  {
    djn_XMLTagHandler *h = handlerStack ? handlerStack->handler : 0;
    if (h && h->data)
      h->data (data, len, curComponent);
  }

  /* function called by expat when a name space start is encountered */
  void
  XML::djn__XMLNamespaceStart (void* userData, const XML_Char *pref, const XML_Char* uri)
  {
  }

  /* function called by expat when a name space end is encountered */
  void
  XML::djn__XMLNamespaceEnd (void* userData, const XML_Char *prefix)
  {
  }
}
