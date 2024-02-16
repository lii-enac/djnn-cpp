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

#include <curl/curl.h>
#include <curl/easy.h>
#include <expat.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xml.h"

#include "core/ontology/process.h"
#include "core/property/text_property.h"
#include "core/utils/error.h"
#include "xml-dev.h"

#if !defined(DJNN_NO_DEBUG) || !defined(DJNN_NO_SERIALIZE)
#include "core/utils/iostream.h"
#endif

namespace djnn {

#if !defined(DJNN_NO_SERIALIZE)

using namespace djnnstl;

map<const string, FatProcess*> _XML_loaded_map;

void
init_xml ()
{
}

void
clear_xml ()
{
    for (auto it : _XML_loaded_map) {
        delete it.second;
        it.second = nullptr;
    }
}
FatProcess*
load_from_XML (const string& uri)
{
    return XML::djnLoadThenClone (uri);
}
FatProcess*
load_from_XML_once (const string& uri)
{
    return XML::djnLoadFromXML (uri);
}

FatProcess*
loadFromXML (const string& uri)
{
    warning (nullptr, "loadFromXML is now deprecated :\nPlease use:\n- load_from_XML (string uri)  - to load then clone many times the same XML\n- load_from_XML_once (string uri) - to load only once an XML\n");
    return XML::djnLoadFromXML (uri);
}

XML::djn__NamespaceTable_t XML::djn__NamespaceTable; // = new map<string, djn__XMLParser*>;
FatProcess*                XML::curComponent = nullptr;
djn__XMLTagHandlerList*    XML::handlerStack = nullptr;

/*
 * I. The public API: initialisation and parsing function
 */

typedef struct
{
    XML_Parser parser;
} djn__CurlData;

typedef struct
{
    XML_Parser parser;
    string     filename;
} djn__ExpatData;

size_t
XML::djn__ReadXML (const char* buf, size_t size, size_t nmemb, void* stream)
{
    djn__CurlData* d   = (djn__CurlData*)stream;
    int            len = size * nmemb;

    if (XML_Parse (d->parser, buf, len, len == 0))
        return len;
    else {
        fprintf (stderr, "Parse error at line %d:\n%s\n", (int)XML_GetCurrentLineNumber (d->parser),
                 XML_ErrorString (XML_GetErrorCode (d->parser)));
        return -1;
    }
}

/* should add handling of '-' to denote stdin, until a URI is defined for that */
FatProcess*
XML::djnLoadFromXML (const string& uri)
{
    // string uri_ = uri;
    std::size_t found = uri.find ("://");
    if (found == string::npos) {
        FatProcess* res = djnParseXMLFromPath (uri);
        return res;
    }

    auto parser = XML_ParserCreateNS ("UTF-8", '*');
    XML_SetElementHandler (parser, &djn__XMLTagStart, &djn__XMLTagEnd);
    XML_SetCharacterDataHandler (parser, &djn__XMLDataHandle);
    XML_SetNamespaceDeclHandler (parser, &djn__XMLNamespaceStart, &djn__XMLNamespaceEnd);

    djn__ExpatData dexpat;
    dexpat.parser   = parser;
    dexpat.filename = uri;
    XML_SetUserData (parser, &dexpat);

    CURL* curl = curl_easy_init ();
    if (!curl) {
        XML_ParserFree (parser);
        return nullptr;
    }

    djn__CurlData d;
    curl_easy_setopt (curl, CURLOPT_URL, uri.c_str ());
    curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, djn__ReadXML);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, &d);
    d.parser = parser;

    curComponent = nullptr;
    CURLcode res = curl_easy_perform (curl);

    curl_easy_cleanup (curl);
    XML_ParserFree (parser);

    if (res == CURLE_OK) {
        return curComponent;
    } else {
        error (nullptr, "could not parse " + uri);
        return 0;
    };
}

FatProcess*
XML::djnLoadThenClone (const string& path)
{
    auto        it = _XML_loaded_map.find (path);
    FatProcess* fp;
    if (it != _XML_loaded_map.end ())
        fp = it->second;
    else {
        fp                    = XML::djnLoadFromXML (path);
        _XML_loaded_map[path] = fp;
    }
    return clone (fp);
}

FatProcess*
XML::djnParseXMLFromPath (const string& path)
{
    FILE* f;
    f = fopen (path.c_str (), "r");
    if (f == NULL) {
        error (nullptr, string ("unable to load file ") + path);
        return nullptr;
    };

    XML_Parser p    = XML_ParserCreateNS ("UTF-8", '*');
    int        done = 0;

    XML_SetElementHandler (p, &djn__XMLTagStart, &djn__XMLTagEnd);
    XML_SetCharacterDataHandler (p, &djn__XMLDataHandle);
    XML_SetNamespaceDeclHandler (p, &djn__XMLNamespaceStart, &djn__XMLNamespaceEnd);
    djn__ExpatData dexpat;
    dexpat.parser   = p;
    dexpat.filename = path;
    XML_SetUserData (p, &dexpat);

    curComponent = 0;

    while (!done) {
        int       len;
        const int BUFFSIZE = 8192;
        void*     buf      = XML_GetBuffer (p, BUFFSIZE);

        len = fread (buf, 1, BUFFSIZE, f);
        if (ferror (f)) {
            error (nullptr, "Read error");
            XML_ParserFree (p);
            fclose (f);
            return 0;
        }
        done = feof (f);

        if (!XML_ParseBuffer (p, len, done)) {
            fprintf (stderr, "Parse error at line %d:\n%s\n", (int)XML_GetCurrentLineNumber (p),
                     XML_ErrorString (XML_GetErrorCode (p)));
            XML_ParserFree (p);
            fclose (f);
            return 0;
        }
    }

    XML_ParserFree (p);
    fclose (f);
    return curComponent;
}

/*
 * II. The semi-public API, provided to implement new parsers
 */

int
XML::djn_RegisterXMLParser (const string& uri, djn_XMLTagLookupProc l, const char* f)
{
    djn__XMLParser*                 h;
    djn__NamespaceTable_t::iterator it;
    it = djn__NamespaceTable.find (uri);
    if (it != djn__NamespaceTable.end ()) {
        cerr << "conflict of XML parsers on namespace " << uri << endl;
        return 0;
    }

    h         = (djn__XMLParser*)malloc (sizeof (djn__XMLParser));
    h->lookup = l;
    h->format = f;

    djn__NamespaceTable.insert (djn__NamespaceTable_t::value_type (uri, h));
    // cerr << "XML registered " << uri << endl;
    return 1;
}

int
XML::djn_UnregisterXMLParser (const string& uri)
{
    // djn__XMLParser *h;
    djn__NamespaceTable_t::iterator it;
    it = djn__NamespaceTable.find (uri);
    if (it == djn__NamespaceTable.end ()) {
        cerr << "unregister warning: no registered XML parsers on namespace " << uri << endl;
        return 0;
    }

    // h = (djn__XMLParser*) malloc (sizeof(djn__XMLParser));
    // h->lookup = l;
    // h->format = f;
    free (it->second);
    djn__NamespaceTable.erase (it);
    // cerr << "XML unregistered " << uri << endl;
    return 1;
}

void
XML::clear_xml_parser ()
{
    djn__NamespaceTable.clear ();
}

int
XML::djn_XMLHandleAttr (FatProcess** e, const char** attrs, djn_XMLSymLookupProc lookup, ...)
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
XML::djn__XMLPushTagHandler (djn_XMLTagHandler* h)
{
    djn__XMLTagHandlerList* l = (djn__XMLTagHandlerList*)malloc (sizeof (djn__XMLTagHandlerList));
    l->handler                = h;
    l->next                   = handlerStack;
    handlerStack              = l;
}

djn_XMLTagHandler*
XML::djn__XMLPopTagHandler ()
{
    djn_XMLTagHandler*      h;
    djn__XMLTagHandlerList* l = handlerStack;

    if (!l)
        return 0;
    h            = l->handler;
    handlerStack = l->next;
    free (l);
    return h;
}

/* find if we have a tag handler for this tag name */

djn_XMLTagHandler*
XML::djn__XMLFindTagHandler (const XML_Char* name)
{
    djn__XMLParser*    p = 0;
    djn_XMLTagHandler* h = 0;

    char* n = strchr ((XML_Char*)name, '*');
    if (n) {
        /* if yes, let's split the tag name in two parts */
        *n = '\0';

        /* find a parser for this name space */
        djn__NamespaceTable_t::iterator it;

        it = djn__NamespaceTable.find (name);
        if (it != djn__NamespaceTable.end ()) {
            /* if found, get the associated parser data */
            p = it->second;
        } else {
#ifdef DEBUG
            fprintf (stderr, "ignoring XML tag '%s': unknown namespace '%s'\n", n + 1, name);
#endif
            p = 0;
        }
        /* restore the tag name buffer, otherwise expat will stop working well */
        *n   = '*';
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
    djn_XMLTagHandler* h = djn__XMLFindTagHandler (name);

    /* if yes, handle it and push the handler onto the stack */
    if (h) {
        djn__ExpatData* dexpat = reinterpret_cast<djn__ExpatData*> (userData);
        if (dexpat) {
            Context::instance ()->new_line (XML_GetCurrentLineNumber (dexpat->parser), dexpat->filename); // set xml source filename to help debug within xml files
        }
        FatProcess* e = h->start (attrs, curComponent);
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
    djn_XMLTagHandler* h = djn__XMLFindTagHandler (name);
    djn_XMLTagHandler* old_h;

    /* if yes, handle it and pop the handler from the stack */
    if (h) {
        FatProcess* e = h->end (curComponent);
        Context::instance ()->new_line (-1, ""); // unset xml source filename
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
    djn_XMLTagHandler* h = handlerStack ? handlerStack->handler : 0;
    if (h && h->data)
        h->data (data, len, curComponent);
}

/* function called by expat when a name space start is encountered */
void
XML::djn__XMLNamespaceStart (void* userData, const XML_Char* pref, const XML_Char* uri)
{
}

/* function called by expat when a name space end is encountered */
void
XML::djn__XMLNamespaceEnd (void* userData, const XML_Char* prefix)
{
}

#endif
} // namespace djnn
