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

static int ParseCX (Process**, const char*);
static int ParseCY (Process**, const char*);
static int ParseR (Process**, const char*);
static int ParseFX (Process**, const char*);
static int ParseFY (Process**, const char*);

static std::map <std::string, djn_XMLAttrHandler> handlers = {
  {"r",{&ParseR}},
  {"fy",{&ParseFY}},
  {"cy",{&ParseCY}},
  {"fx",{&ParseFX}},
  {"cx",{&ParseCX}}
};

djn_XMLAttrHandler*
SVGRadialGradientAttrs_Hash::djn_SVGRadialGradientAttrsLookup (const char *str, unsigned int len)
{
  std::map<std::string, djn_XMLAttrHandler>::iterator it;
  it = handlers.find(std::string(str));
  if (it != handlers.end())
    return &it->second;
  return 0;
}

struct djn_RadialGradientArgs djn_RadialGradientArgs = {0.5, 0.5, 0.5, 0.5, 0.5};

static int ParseCX(Process** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientCx);
	return XML_Utils::djn_XMLParseLength(&djn_RadialGradientArgs.cx, v);
}

static int ParseCY(Process** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientCy);
	return XML_Utils::djn_XMLParseLength(&djn_RadialGradientArgs.cy, v);
}

static int ParseR(Process** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientR);
	return XML_Utils::djn_XMLParseLength(&djn_RadialGradientArgs.r, v);
}

static int ParseFX(Process** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientFx);
	return XML_Utils::djn_XMLParseLength(&djn_RadialGradientArgs.fx, v);
}

static int ParseFY(Process** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientFy);
	return XML_Utils::djn_XMLParseLength(&djn_RadialGradientArgs.fy, v);
}
