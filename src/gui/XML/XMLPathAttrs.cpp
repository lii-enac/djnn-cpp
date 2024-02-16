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
#include "gui/shape/path.h"
#include "gui/shape/path_clip.h"

using namespace djnnstl;

namespace djnn {

typedef void (djn_PathPointProc) (FatProcess*, double, double);
typedef void (djn_PathCoordProc) (FatProcess*, double);

static int ParseData (FatProcess**, const char*);

static map<string, djn_XMLAttrHandler> handlers = {
    {"d", {&ParseData}}};

djn_XMLAttrHandler*
XMLPathAttrs_Hash::djn_XMLPathAttrsLookup (const char* str, unsigned int len)
{
    map<string, djn_XMLAttrHandler>::iterator it;
    it = handlers.find (string (str));
    if (it != handlers.end ())
        return &it->second;
    return 0;
}

struct djn_PathArgs djn_PathArgs = {0};

static int
ParseData (FatProcess** e, const char* v)
{

    if (djn__GrphIsInClip)
        djn_PathArgs.e = new PathClip (nullptr, "");
    else
        djn_PathArgs.e = new Path (nullptr, "");

    return parse_path ((Path*)djn_PathArgs.e, v);
}

} // namespace djnn
