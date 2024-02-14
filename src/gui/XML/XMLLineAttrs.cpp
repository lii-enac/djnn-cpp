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
// #include "core/cor__e.h"
#include "gui/gui-dev.h"
#include "gui/gui-xml.h"

using namespace djnnstl;

namespace djnn {

static int ParseX1 (FatProcess**, const char*);
static int ParseY1 (FatProcess**, const char*);
static int ParseX2 (FatProcess**, const char*);
static int ParseY2 (FatProcess**, const char*);

static map<string, djn_XMLAttrHandler> handlers = {
    {"x1", {&ParseX1}},
    {"y1", {&ParseY1}},
    {"x2", {&ParseX2}},
    {"y2", {&ParseY2}}};

djn_XMLAttrHandler*
XMLLineAttrs_Hash::djn_XMLLineAttrsLookup (const char* str, unsigned int len)
{
    map<string, djn_XMLAttrHandler>::iterator it;
    it = handlers.find (string (str));
    if (it != handlers.end ())
        return &it->second;
    return 0;
}

struct djn_LineArgs djn_LineArgs = {0., 0., 0., 0.};

static int
ParseX1 (FatProcess** e, const char* v)
{
    return XML_Utils::djn_XMLParseLength (&djn_LineArgs.x1, v);
}

static int
ParseY1 (FatProcess** e, const char* v)
{
    return XML_Utils::djn_XMLParseLength (&djn_LineArgs.y1, v);
}

static int
ParseX2 (FatProcess** e, const char* v)
{
    return XML_Utils::djn_XMLParseLength (&djn_LineArgs.x2, v);
}

static int
ParseY2 (FatProcess** e, const char* v)
{
    return XML_Utils::djn_XMLParseLength (&djn_LineArgs.y2, v);
}

} // namespace djnn
