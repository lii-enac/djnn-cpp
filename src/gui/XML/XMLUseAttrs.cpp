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
#include <iostream>
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
static int djn__ParseHref (Process**, const char*);

static std::map <std::string, djn_XMLAttrHandler> handlers = {
  {"x", {&djn__ParseX}},
  {"y", {&djn__ParseY}},
  {"width", {&djn__ParseWidth}},
  {"height", {&djn__ParseHeight}},
  {"http://www.w3.org/1999/xlink*href",{&djn__ParseHref}},
  {"href",{&djn__ParseHref}}
};

djn_XMLAttrHandler*
XMLUseAttrs_Hash::djn_XMLUseAttrsLookup (const char *str, unsigned int len)
{
  std::map<std::string, djn_XMLAttrHandler>::iterator it;
  it = handlers.find(std::string(str));
  if (it != handlers.end())
    return &it->second;
  return 0;
}


struct djn_UseArgs djn_UseArgs = {0., 0., 0., 0., ""};

static int djn__ParseX(Process** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_UseArgs.x, v);
}

static int djn__ParseY(Process** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_UseArgs.y, v);
}

static int djn__ParseWidth(Process** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_UseArgs.width, v);
}

static int djn__ParseHeight(Process** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_UseArgs.height, v);
}

static int djn__ParseHref(Process** e, const char* v) {
  // remove possible quotes
  string str(v);
  str.erase (std::remove(str.begin(), str.end(), '\''), str.end());
  if (str.at(0) == '#')
    str = str.substr (1);
  djn_UseArgs.href = str;
  return 1;
}
