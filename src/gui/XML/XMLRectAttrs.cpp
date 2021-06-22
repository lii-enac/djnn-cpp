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


static int ParseX (FatProcess**, const char*);
static int ParseY (FatProcess**, const char*);
static int ParseWidth (FatProcess**, const char*);
static int ParseHeight (FatProcess**, const char*);
static int ParseRx (FatProcess**, const char*);
static int ParseRy (FatProcess**, const char*);

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
ParseX (FatProcess** e, const char* v)
{
  return XML_Utils::djn_XMLParseLength (&djn_RectArgs.x, v);
}

static int
ParseY (FatProcess** e, const char* v)
{
  return XML_Utils::djn_XMLParseLength (&djn_RectArgs.y, v);
}

static int
ParseWidth (FatProcess** e, const char* v)
{
  return XML_Utils::djn_XMLParseLength (&djn_RectArgs.w, v);
}

static int
ParseHeight (FatProcess** e, const char* v)
{
  return XML_Utils::djn_XMLParseLength (&djn_RectArgs.h, v);
}

static int
ParseRx (FatProcess** e, const char* v)
{
  int r = XML_Utils::djn_XMLParseLength (&djn_RectArgs.rx, v);
  if (r && djn_RectArgs.ry == -1)
    djn_RectArgs.ry = djn_RectArgs.rx;
  return r;
}

static int
ParseRy (FatProcess** e, const char* v)
{
  int r = XML_Utils::djn_XMLParseLength (&djn_RectArgs.ry, v);
  if (r && djn_RectArgs.rx == -1)
    djn_RectArgs.rx = djn_RectArgs.ry;
  return r;
}
}
