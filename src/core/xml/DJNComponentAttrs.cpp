/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2024)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Magnaudet Mathieu <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/utils/containers.h"
#include "xml-priv.h"

using namespace djnn;
using namespace djnnstl;

static int HandleId (FatProcess**, const char*);
static int HandleModel (FatProcess**, const char*);

static map<string, djn_XMLAttrHandler> handlers = {
    {"id", {.handle_attr = &HandleId}},
    {"model", {.handle_attr = &HandleModel}}};

djn_XMLAttrHandler*
DJNComponentAttrs_Hash::djn_DJNComponentAttrsLookup (const char* str, unsigned int len)
{
    map<string, djn_XMLAttrHandler>::iterator it;
    it = handlers.find (string (str));
    if (it != handlers.end ())
        return &it->second;
    return 0;
}

struct djn_ComponentArgs_t djn_ComponentArgs = {"", 0};

static int
HandleId (FatProcess** e, const char* v)
{
    djn_ComponentArgs.id = v;
    return 1;
}

static int
HandleModel (FatProcess** e, const char* v)
{
    djn_ComponentArgs.model = !!atoi (v);
    return 1;
}
