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

static int ParseStopColor (Process**, const char*);
static int ParseStopOpacity (Process**, const char*);
static int ParseOffset (Process**, const char*);
static int ParseStopStyle (Process**, const char*);

std::map <std::string, djn_XMLAttrHandler> handlers = {
  {"style",{&ParseStopStyle}},
  {"offset",{&ParseOffset}},
  {"stop-color",{&ParseStopColor}},
  {"stop-opacity",{&ParseStopOpacity}}
};

djn_XMLAttrHandler*
SVGGradientStopAttrs_Hash::djn_SVGGradientStopAttrsLookup (const char *str, unsigned int len)
{
  std::map<std::string, djn_XMLAttrHandler>::iterator it;
  it = handlers.find(std::string(str));
  if (it != handlers.end())
    return &it->second;
  return 0;
}

struct djn_GradientStopArgs djn_GradientStopArgs = {0, 0, 0, 1.0, 0.};

static int ParseStopColor(Process** e, const char* v) {

	return SVG_Utils::djn__SVGParseColor(&djn_GradientStopArgs.r, &djn_GradientStopArgs.g,
			&djn_GradientStopArgs.b, v);
}

static int ParseStopOpacity(Process** e, const char* v) {
	return SVG_Utils::djn__SVGParseAlpha(&djn_GradientStopArgs.alpha, v);
}

static int ParseOffset(Process** e, const char* v) {
	double value;
	int ret = XML_Utils::djn_XMLParseLength(&value, v);
	if (ret == 2)
		value /= 100.;
	djn_GradientStopArgs.offset = value;
	return ret;
}

static int ParseStopStyle(Process **e, const char *v) {
	/* FIXME very incomplete implementation of the style attribute parsing.
	 * It should be able to manage css style
	 */
	int success = 1;
	int ret = 0;
	djn_XMLAttrHandler *h;
	char *colon, *semicolon;
	const char* end = v + strlen(v);

	while (v < end) {
		colon = strchr( (char*) v, ':');
		if (!colon)
			return 0;
		*colon = '\0';
		semicolon = strchr(colon + 1, ';');
		if (semicolon)
			*semicolon = '\0';
		else
			semicolon = (char*) end;
		h = SVGGradientStopAttrs_Hash::djn_SVGGradientStopAttrsLookup(v, colon - v);
		ret = (h && (h->handle_attr(e, colon + 1)));
		success &= ret;
		if (!ret)
			fprintf(stderr, "unable to understand %s\n", v);
		v = semicolon + 1;
	}
	return success;
}
}
