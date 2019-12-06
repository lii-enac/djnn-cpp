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
#include "core/core.h"
#include "gui/gui-dev.h"


using namespace djnn;

static int ParseCx (Process**, const char*);
static int ParseCy (Process**, const char*);
static int ParseRx (Process**, const char*);
static int ParseRy (Process**, const char*);

static std::map <std::string, djn_XMLAttrHandler> handlers = {
  {"ry",{&ParseRy}},
  {"rx",{&ParseRx}},
  {"cy",{&ParseCy}},
  {"cx",{&ParseCx}}
};

djn_XMLAttrHandler*
XMLEllipseAttrs_Hash::djn_XMLEllipseAttrsLookup (const char *str, unsigned int len)
{
  std::map<std::string, djn_XMLAttrHandler>::iterator it;
  it = handlers.find(std::string(str));
  if (it != handlers.end())
    return &it->second;
  return 0;
}


struct djn_EllipseArgs djn_EllipseArgs = {0., 0., 0., 0.};

static int ParseCx(Process** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_EllipseArgs.cx, v);
}

static int ParseCy(Process** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_EllipseArgs.cy, v);
}

static int ParseRx(Process** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_EllipseArgs.rx, v);
}

static int ParseRy(Process** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_EllipseArgs.ry, v);
}

