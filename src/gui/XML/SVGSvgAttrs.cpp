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

using namespace djnn;

static int IgnoreAttr (FatProcess**, const char*);

static map <std::string, djn_XMLAttrHandler> handlers = {
  {"viewBox",{&IgnoreAttr}},
  {"contentStyleType",{&IgnoreAttr}},
  {"contentScriptType",{&IgnoreAttr}},
  {"preserveAspectRatio",{&IgnoreAttr}}
};

djn_XMLAttrHandler*
SVGSvgAttrs_Hash::djn_SVGSvgAttrsLookup (const char *str, unsigned int len)
{
  map<std::string, djn_XMLAttrHandler>::iterator it;
  it = handlers.find(std::string(str));
  if (it != handlers.end())
    return &it->second;
  return 0;
}

static int
IgnoreAttr (FatProcess** e, const char* v)
{
	return 1;
}

