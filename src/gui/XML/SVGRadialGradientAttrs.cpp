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

#include <limits>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "gui/gui-dev.h"
#include "gui/gui-xml.h"

using namespace djnnstl;

namespace djnn {

static int ParseCX (FatProcess**, const char*);
static int ParseCY (FatProcess**, const char*);
static int ParseR (FatProcess**, const char*);
static int ParseFX (FatProcess**, const char*);
static int ParseFY (FatProcess**, const char*);

static map <string, djn_XMLAttrHandler> handlers = {
  {"r",{&ParseR}},
  {"fy",{&ParseFY}},
  {"cy",{&ParseCY}},
  {"fx",{&ParseFX}},
  {"cx",{&ParseCX}}
};

djn_XMLAttrHandler*
SVGRadialGradientAttrs_Hash::djn_SVGRadialGradientAttrsLookup (const char *str, unsigned int len)
{
  map<string, djn_XMLAttrHandler>::iterator it;
  it = handlers.find(string(str));
  if (it != handlers.end())
    return &it->second;
  return 0;
}

struct djn_RadialGradientArgs djn_RadialGradientArgs = {0.5, 0.5, 0.5, 0.5, 0.5};

static int ParseCX(FatProcess** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientCx);
	return XML_Utils::djn_XMLParseLength(&djn_RadialGradientArgs.cx, v);
}

static int ParseCY(FatProcess** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientCy);
	return XML_Utils::djn_XMLParseLength(&djn_RadialGradientArgs.cy, v);
}

static int ParseR(FatProcess** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientR);
	return XML_Utils::djn_XMLParseLength(&djn_RadialGradientArgs.r, v);
}

static int ParseFX(FatProcess** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientFx);
	return XML_Utils::djn_XMLParseLength(&djn_RadialGradientArgs.fx, v);
}

static int ParseFY(FatProcess** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientFy);
	return XML_Utils::djn_XMLParseLength(&djn_RadialGradientArgs.fy, v);
}
}
