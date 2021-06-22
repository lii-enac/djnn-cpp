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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// #include "core/cor__e.h"
#include "gui/gui-dev.h"

	
namespace djnn {

static int ParseCx (FatProcess**, const char*);
static int ParseCy (FatProcess**, const char*);
static int ParseR (FatProcess**, const char*);

static map <std::string, djn_XMLAttrHandler> handlers = {
  {"r",{&ParseR}},
  {"cy",{&ParseCy}},
  {"cx",{&ParseCx}}
};

djn_XMLAttrHandler*
XMLCircleAttrs_Hash::djn_XMLCircleAttrsLookup (const char *str, unsigned int len)
{
  map<std::string, djn_XMLAttrHandler>::iterator it;
  it = handlers.find(std::string(str));
  if (it != handlers.end())
    return &it->second;
  return 0;
}


struct djn_CircleArgs djn_CircleArgs = {0., 0., 0.};

static int ParseCx(FatProcess** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_CircleArgs.cx, v);
}

static int ParseCy(FatProcess** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_CircleArgs.cy, v);
}

static int ParseR(FatProcess** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_CircleArgs.r, v);
}

}

