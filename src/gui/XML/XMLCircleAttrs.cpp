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

#include "gui/gui-dev.h"
#include "gui/gui-xml.h"

using namespace djnnstl;

namespace djnn {

static int ParseCx (FatProcess**, const char*);
static int ParseCy (FatProcess**, const char*);
static int ParseR (FatProcess**, const char*);

static map<string, djn_XMLAttrHandler> handlers = {
    {"r", {&ParseR}},
    {"cy", {&ParseCy}},
    {"cx", {&ParseCx}}};

djn_XMLAttrHandler*
XMLCircleAttrs_Hash::djn_XMLCircleAttrsLookup (const char* str, unsigned int len)
{
    map<string, djn_XMLAttrHandler>::iterator it;
    it = handlers.find (string (str));
    if (it != handlers.end ())
        return &it->second;
    return 0;
}

struct djn_CircleArgs djn_CircleArgs = {0., 0., 0.};

static int
ParseCx (FatProcess** e, const char* v)
{
    return XML_Utils::djn_XMLParseLength (&djn_CircleArgs.cx, v);
}

static int
ParseCy (FatProcess** e, const char* v)
{
    return XML_Utils::djn_XMLParseLength (&djn_CircleArgs.cy, v);
}

static int
ParseR (FatProcess** e, const char* v)
{
    return XML_Utils::djn_XMLParseLength (&djn_CircleArgs.r, v);
}

} // namespace djnn
