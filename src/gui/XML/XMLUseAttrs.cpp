/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/utils/algorithm.h"
#include "core/utils/iostream.h"
// #include "core/cor__e.h"
#include "gui/gui-dev.h"
#include "gui/gui-xml.h"

using namespace djnnstl;

namespace djnn {

static int djn__ParseX (FatProcess**, const char*);
static int djn__ParseY (FatProcess**, const char*);
static int djn__ParseWidth (FatProcess**, const char*);
static int djn__ParseHeight (FatProcess**, const char*);
static int djn__ParseHref (FatProcess**, const char*);

static map<string, djn_XMLAttrHandler> handlers = {
    {"x", {&djn__ParseX}},
    {"y", {&djn__ParseY}},
    {"width", {&djn__ParseWidth}},
    {"height", {&djn__ParseHeight}},
    {"http://www.w3.org/1999/xlink*href", {&djn__ParseHref}},
    {"href", {&djn__ParseHref}}};

djn_XMLAttrHandler*
XMLUseAttrs_Hash::djn_XMLUseAttrsLookup (const char* str, unsigned int len)
{
    map<string, djn_XMLAttrHandler>::iterator it;
    it = handlers.find (string (str));
    if (it != handlers.end ())
        return &it->second;
    return 0;
}

struct djn_UseArgs djn_UseArgs = {0., 0., 0., 0., ""};

static int
djn__ParseX (FatProcess** e, const char* v)
{
    return XML_Utils::djn_XMLParseLength (&djn_UseArgs.x, v);
}

static int
djn__ParseY (FatProcess** e, const char* v)
{
    return XML_Utils::djn_XMLParseLength (&djn_UseArgs.y, v);
}

static int
djn__ParseWidth (FatProcess** e, const char* v)
{
    return XML_Utils::djn_XMLParseLength (&djn_UseArgs.width, v);
}

static int
djn__ParseHeight (FatProcess** e, const char* v)
{
    return XML_Utils::djn_XMLParseLength (&djn_UseArgs.height, v);
}

static int
djn__ParseHref (FatProcess** e, const char* v)
{
    // remove possible quotes
    string str (v);
    str.erase (djnnstl::remove (str.begin (), str.end (), '\''), str.end ());
    if (str.at (0) == '#')
        str = str.substr (1);
    djn_UseArgs.href = str;
    return 1;
}
} // namespace djnn
