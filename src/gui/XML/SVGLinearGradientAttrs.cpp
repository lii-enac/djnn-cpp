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
static int ParseX1 (Process**, const char*);
static int ParseY1 (Process**, const char*);
static int ParseX2 (Process**, const char*);
static int ParseY2 (Process**, const char*);

static std::map <std::string, djn_XMLAttrHandler> handlers = {
  {"y2",{&ParseY2}},
  {"y1",{&ParseY1}},
  {"x2",{&ParseX2}},
  {"x1",{&ParseX1}}
};

djn_XMLAttrHandler*
SVGLinearGradientAttrs_Hash::djn_SVGLinearGradientAttrsLookup (const char *str, unsigned int len)
{
  std::map<std::string, djn_XMLAttrHandler>::iterator it;
  it = handlers.find(std::string(str));
  if (it != handlers.end())
    return &it->second;
  return 0;
}

struct djn_LinearGradientArgs djn_LinearGradientArgs = {0., 0., 1., 0.};

static int ParseX1(Process** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientX1);
	return XML_Utils::djn_XMLParseLength(&djn_LinearGradientArgs.x1, v);
}

static int ParseY1(Process** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientY1);
	return XML_Utils::djn_XMLParseLength(&djn_LinearGradientArgs.y1, v);
}

static int ParseX2(Process** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientX2);
	return XML_Utils::djn_XMLParseLength(&djn_LinearGradientArgs.x2, v);
}

static int ParseY2(Process** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientY2);
	return XML_Utils::djn_XMLParseLength(&djn_LinearGradientArgs.y2, v);
}
