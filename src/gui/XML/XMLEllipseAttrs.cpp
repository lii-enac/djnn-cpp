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

static int ParseCx (FatProcess**, const char*);
static int ParseCy (FatProcess**, const char*);
static int ParseRx (FatProcess**, const char*);
static int ParseRy (FatProcess**, const char*);

static map<string, djn_XMLAttrHandler>* handlers;

void
XMLEllipseAttrs_Hash::init ()
{
    if (handlers)
        return;
    handlers                                  = new map<string, djn_XMLAttrHandler>;
    map<string, djn_XMLAttrHandler> handlers_ = {
        {"ry", {&ParseRy}},
        {"rx", {&ParseRx}},
        {"cy", {&ParseCy}},
        {"cx", {&ParseCx}}};
    *handlers = std::move (handlers_);
}

void
XMLEllipseAttrs_Hash::clear ()
{
    delete handlers;
    handlers = nullptr;
}

djn_XMLAttrHandler*
XMLEllipseAttrs_Hash::djn_XMLEllipseAttrsLookup (const char* str, unsigned int len)
{
    map<string, djn_XMLAttrHandler>::iterator it;
    it = handlers->find (string (str));
    if (it != handlers->end ())
        return &it->second;
    return 0;
}

struct djn_EllipseArgs djn_EllipseArgs = {0., 0., 0., 0.};

static int
ParseCx (FatProcess** e, const char* v)
{
    return XML_Utils::djn_XMLParseLength (&djn_EllipseArgs.cx, v);
}

static int
ParseCy (FatProcess** e, const char* v)
{
    return XML_Utils::djn_XMLParseLength (&djn_EllipseArgs.cy, v);
}

static int
ParseRx (FatProcess** e, const char* v)
{
    return XML_Utils::djn_XMLParseLength (&djn_EllipseArgs.rx, v);
}

static int
ParseRy (FatProcess** e, const char* v)
{
    return XML_Utils::djn_XMLParseLength (&djn_EllipseArgs.ry, v);
}

} // namespace djnn
