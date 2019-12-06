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

using namespace std;

static int djn__ParseX (Process**, const char*);
static int djn__ParseY (Process**, const char*);
static int djn__ParseWidth (Process**, const char*);
static int djn__ParseHeight (Process**, const char*);

static std::map <std::string, djn_XMLAttrHandler> handlers = {
  {"x", {&djn__ParseX}},
  {"y", {&djn__ParseY}},
  {"width", {&djn__ParseWidth}},
  {"height", {&djn__ParseHeight}},
};

djn_XMLAttrHandler*
XMLRectAreaAttrs_Hash::djn_XMLRectAreaAttrsLookup (const char *str, unsigned int len)
{
  std::map<std::string, djn_XMLAttrHandler>::iterator it;
  it = handlers.find(std::string(str));
  if (it != handlers.end())
    return &it->second;
  return 0;
}


struct djn_RectAreaArgs djn_RectAreaArgs = {0., 0., 0., 0.};

static int djn__ParseX(Process** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_RectAreaArgs.x, v);
}

static int djn__ParseY(Process** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_RectAreaArgs.y, v);
}

static int djn__ParseWidth(Process** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_RectAreaArgs.width, v);
}

static int djn__ParseHeight(Process** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_RectAreaArgs.height, v);
}

