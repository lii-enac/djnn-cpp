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

using namespace djnn;
using namespace djnnstl;

static int IgnoreAttr (FatProcess**, const char*);

static map<string, djn_XMLAttrHandler> handlers = {
    {"viewBox", {&IgnoreAttr}},
    {"contentStyleType", {&IgnoreAttr}},
    {"contentScriptType", {&IgnoreAttr}},
    {"preserveAspectRatio", {&IgnoreAttr}}};

djn_XMLAttrHandler*
SVGSvgAttrs_Hash::djn_SVGSvgAttrsLookup (const char* str, unsigned int len)
{
    map<string, djn_XMLAttrHandler>::iterator it;
    it = handlers.find (string (str));
    if (it != handlers.end ())
        return &it->second;
    return 0;
}

static int
IgnoreAttr (FatProcess** e, const char* v)
{
    return 1;
}
