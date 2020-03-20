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


namespace djnn {
using namespace std;

static int ParseX (Process**, const char*);
static int ParseY (Process**, const char*);
static int ParseWidth (Process**, const char*);
static int ParseHeight (Process**, const char*);
static int ParseRx (Process**, const char*);
static int ParseRy (Process**, const char*);

static std::map <std::string, djn_XMLAttrHandler> handlers = {
  {"x",{&ParseX}},
  {"y",{&ParseY}},
  {"width",{&ParseWidth}},
  {"height",{&ParseHeight}},
  {"rx",{&ParseRx}},
  {"ry",{&ParseRy}}
};

djn_XMLAttrHandler*
XMLRectAttrs_Hash::djn_XMLRectAttrsLookup (const char *str, unsigned int len)
{
  std::map<std::string, djn_XMLAttrHandler>::iterator it;
  it = handlers.find(std::string(str));
  if (it != handlers.end())
    return &it->second;
  return 0;
}

struct djn_RectArgs djn_RectArgs = {0., 0., 0., 0., -1., -1.};

static int
ParseX (Process** e, const char* v)
{
  return XML_Utils::djn_XMLParseLength (&djn_RectArgs.x, v);
}

static int
ParseY (Process** e, const char* v)
{
  return XML_Utils::djn_XMLParseLength (&djn_RectArgs.y, v);
}

static int
ParseWidth (Process** e, const char* v)
{
  return XML_Utils::djn_XMLParseLength (&djn_RectArgs.w, v);
}

static int
ParseHeight (Process** e, const char* v)
{
  return XML_Utils::djn_XMLParseLength (&djn_RectArgs.h, v);
}

static int
ParseRx (Process** e, const char* v)
{
  int r = XML_Utils::djn_XMLParseLength (&djn_RectArgs.rx, v);
  if (r && djn_RectArgs.ry == -1)
    djn_RectArgs.ry = djn_RectArgs.rx;
  return r;
}

static int
ParseRy (Process** e, const char* v)
{
  int r = XML_Utils::djn_XMLParseLength (&djn_RectArgs.ry, v);
  if (r && djn_RectArgs.rx == -1)
    djn_RectArgs.rx = djn_RectArgs.ry;
  return r;
}
}
