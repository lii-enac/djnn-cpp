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
#include "core/xml/xml-dev.h"

namespace djnn {

static int ParseId (FatProcess**, const char*);
static int ParseGradientTransform (FatProcess**, const char*);
static int ParseGradientUnits (FatProcess**, const char*);
static int ParseSpreadMethod (FatProcess**, const char*);
static int ParseHRef (FatProcess** e, const char* v);

static std::map <std::string, djn_XMLAttrHandler> handlers = {
  {"id", {&ParseId}},
  {"spreadMethod",{&ParseSpreadMethod}},
  {"gradientUnits",{&ParseGradientUnits}},
  {"gradientTransform",{&ParseGradientTransform}},
  {"http://www.w3.org/1999/xlink*href",{&ParseHRef}}
};

djn_XMLAttrHandler*
SVGGradientAttrs_Hash::djn_SVGGradientAttrsLookup (const char *str, unsigned int len)
{
  std::map<std::string, djn_XMLAttrHandler>::iterator it;
  it = handlers.find(std::string(str));
  if (it != handlers.end())
    return &it->second;
  return 0;
}

struct djn_GradientArgs djn_GradientArgs = {"", 0, DJN_LOCAL_COORDS, DJN_PAD_FILL};

static int ParseId(FatProcess** e, const char* v) {
	djn_GradientArgs.id = v;
	return 1;
}

static int ParseGradientTransform(FatProcess** e, const char* v) {
	djn_GradientArgs.transform = v;
	return 1;
}

static int ParseSpreadMethod(FatProcess** e, const char* v) {
	if (strcmp(v, "repeat") == 0) {
		djn_GradientArgs.spread = DJN_REPEAT_FILL;
	} else if (strcmp(v, "reflect") == 0) {
		djn_GradientArgs.spread = DJN_REFLECT_FILL;
	} else if (strcmp(v, "pad") == 0) {
		djn_GradientArgs.spread = DJN_PAD_FILL;
	} else {
		fprintf(stderr, "unkown spread method %s\n", v);
		return 0;
	}

	djn_GradientArgs.inherited &= ~(1 << djn_GradientSpread);
	return 1;
}

static int ParseGradientUnits(FatProcess** e, const char* v) {
	/* this would actually be better implemented by doing notthing for userSpaceOnUse
	 and adding a transformation matrix to the gradient for objectBoundingBox */

	if (strcmp(v, "userSpaceOnUse") == 0) {
		djn_GradientArgs.coords = DJN_RENDERING_COORDS;
	} else if (strcmp(v, "objectBoundingBox") == 0) {
		djn_GradientArgs.coords = DJN_LOCAL_COORDS;
	} else {
		fprintf(stderr, "unkown gradient units %s\n", v);
		return 0;
	}

	djn_GradientArgs.inherited &= ~(1 << djn_GradientCoords);
	return 1;
}

static int ParseHRef(FatProcess** e, const char* v) {
	std::string id(v + sizeof(char));
	std::map<std::string, FatProcess*>::iterator it = djn__id_to_process.find (id);
	if (it == djn__id_to_process.end()) {
		fprintf (stderr, "unknown gradient %s\n", v);
		return 0;
	}

	djn_GradientArgs.ancestor = it->second;
	return 1;
}
}
