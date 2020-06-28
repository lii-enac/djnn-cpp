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

#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "core/core.h"
#include "base/base.h"
#include "gui/gui-dev.h"

namespace djnn {

using namespace std;

static std::string djn_ParseURL (const char *);

static int Ignore (FatProcess**, const char*);
static int ParseId (FatProcess**, const char*);
static int ParseClass (FatProcess**, const char*);
static int ParseStroke (FatProcess**, const char*);
static int ParseFill (FatProcess**, const char*);
static int ParseFillRule (FatProcess**, const char*);
static int ParseOpacity (FatProcess**, const char*);
static int ParseFillOpacity (FatProcess**, const char*);
static int ParseStrokeOpacity (FatProcess**, const char*);
static int ParseStrokeWidth (FatProcess**, const char*);
static int ParseLineJoin (FatProcess**, const char*);
static int ParseLineCap (FatProcess**, const char*);
static int ParseMatrix (FatProcess**, const char*);
static int ParseMiterLimit (FatProcess**, const char*);
static int ParseRotate (FatProcess**, const char*);
static int ParseScale (FatProcess**, const char*);
static int ParseSkewX (FatProcess**, const char*);
static int ParseSkewY (FatProcess**, const char*);
static int ParseStrokeDashArray (FatProcess**, const char*);
static int ParseStrokeDashOffset (FatProcess**, const char*);
static int ParseStyle (FatProcess**, const char*);
static int ParseTranslate (FatProcess**, const char*);
static int ParseTransform (FatProcess**, const char*);
static int ParseTextAnchor (FatProcess**, const char*);
static int ParsePathClip (FatProcess**, const char*);

static std::map <std::string, djn_XMLAttrHandler> handlers = {
  {"id",{&ParseId}},
  {"style",{&ParseStyle}},
  {"stroke",{&ParseStroke}},
  {"fill",{&ParseFill}},
  {"skewY",{&ParseSkewY}},
  {"filter",{&Ignore}},
  {"stroke-width",{&ParseStrokeWidth}},
  {"shape-rendering",{&Ignore}},
  {"stroke-dasharray",{&ParseStrokeDashArray}},
  {"stroke-dashoffset",{&ParseStrokeDashOffset}},
  {"mask",{&Ignore}},
  {"requiredFeatures",{&Ignore}},
  {"display",{&Ignore}},
  {"requiredExtensions",{&Ignore}},
  {"stroke-linecap",{&ParseLineCap}},
  {"stroke-linejoin",{&ParseLineJoin}},
  {"matrix",{&ParseMatrix}},
  {"clip-path",{&ParsePathClip}},
  {"image-rendering",{&Ignore}},
  {"stroke-miterlimit",{&ParseMiterLimit}},
  {"xml:lang",{&Ignore}},
  {"fill-rule",{&ParseFillRule}},
  {"scale",{&ParseScale}},
  {"rotate",{&ParseRotate}},
  {"opacity",{&ParseOpacity}},
  {"clip-rule",{&Ignore}},
  {"color",{&Ignore}},
  {"fill-opacity",{ParseFillOpacity}},
  {"stroke-opacity",{&ParseStrokeOpacity}},
  {"visibility",{&Ignore}},
  {"cursor",{&Ignore}},
  {"onclick",{&Ignore}},
  {"xml:base",{&Ignore}},
  {"color-rendering",{&Ignore}},
  {"text-anchor",{&ParseTextAnchor}},
  {"pointer-events",{&Ignore}},
  {"systemLanguage",{&Ignore}},
  {"onmousedown",{&Ignore}},
  {"onload",{&Ignore}},
  {"text-rendering",{&Ignore}},
  {"externalResourcesRequired",{&Ignore}},
  {"onactivate",{&Ignore}},
  {"onmouseup",{&Ignore}},
  {"skewX",{&ParseSkewX}},
  {"translate",{&ParseTranslate}},
  {"class",{&ParseClass}},
  {"onmousemove",{&Ignore}},
  {"transform",{&ParseTransform}},
  {"onfocusin",{&Ignore}},
  {"onmouseout",{&Ignore}},
  {"onmouseover",{&Ignore}},
  {"color-interpolation",{&Ignore}},
  {"onfocusout",{&Ignore}},
  {"xml:space",{&Ignore}}
};
djn_XMLAttrHandler*
SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup (const char *str, unsigned int len)
{
  std::map<std::string, djn_XMLAttrHandler>::iterator it;
  it = handlers.find(std::string(str));
  if (it != handlers.end())
    return &it->second;
  return 0;
}


struct djn_GraphicalShapeArgs djn_GraphicalShapeArgs = {"", "", djnStrokeUndef};

static int Ignore(FatProcess** e, const char* v) {
	return 0;
}

static int ParseId(FatProcess** e, const char* v) {
	djn_GraphicalShapeArgs.id = v;
	return 1;
}

static int ParseStroke(FatProcess** e, const char* v) {
	unsigned r, g, b;

	if (!*e)
		*e = new SVGHolder (nullptr, "SVGHolder");

	if (strncmp(v, "none", 4) == 0) {
		djn_GraphicalShapeArgs.strokeType = djnStrokeNone;
		new NoOutline(*e, "no-stroke");

		return 1;

	} else if (!SVG_Utils::djn__SVGParseColor(&r, &g, &b, v))
		return 0;

	djn_GraphicalShapeArgs.strokeType = djnStrokeColor;
	new OutlineColor(*e, "stroke", r, g, b);
	return 1;
}

static int ParsePathClip(FatProcess** e, const char* v) {
	if (!*e)
		*e = new SVGHolder (nullptr, "SVGHolder");

	if (strncmp(v, "url(", 4) == 0) {
		string url = djn_ParseURL(v);
		map<string, FatProcess*>::iterator it = djn__id_to_process.find (url);
		if (it != djn__id_to_process.end ()) {
			Container* clip = (Container*) it->second;
			/* we iterate over the children of clip and add each child to *e */
			for (auto child: clip->ordered_children ())
			  ((Container*)*e)->add_child (child->clone (), "");
		} else {
			fprintf(stderr, "unknown URL %s for clip\n", v);
			return 0;
		}
	} else
		return 0;

	return 1;
}

static int ParseFill(FatProcess** e, const char* v) {
	unsigned r, g, b;

	if (!*e)
		*e = new SVGHolder (nullptr, "SVGHolder");

	if (strncmp(v, "none", 4) == 0) {
		new NoFill(*e, "");
		return 1;

	} else if (strncmp(v, "url(", 4) == 0) {
		string url = djn_ParseURL(v);
		map<string, FatProcess*>::iterator it = djn__id_to_process.find (url);
		if (it != djn__id_to_process.end ()) {
			AbstractGradient* ag = dynamic_cast<AbstractGradient*> (it->second);
			if (ag->is_linear ()) {
			  LinearGradient* lg = dynamic_cast<LinearGradient*> (ag);
			  new RefLinearGradient ((*e), "", lg);
			  (*e)->add_symbol (url, lg);
			} else {
			  RadialGradient* rg = dynamic_cast<RadialGradient*> (ag);
			  new RefRadialGradient ((*e), "", rg);
			  (*e)->add_symbol (url, rg);
			}
		}
		else {
			fprintf(stderr, "unknown URL %s for fill value\n", v);
			return 0;
		}
	} else {
		if (!SVG_Utils::djn__SVGParseColor(&r, &g, &b, v))
			return 0;

		new FillColor(*e, "fill", r, g, b);
	}

	return 1;
}

static int ParseFillRule(FatProcess** e, const char* v) {
	if (strncmp(v, "nonzero", 7) == 0) {
		new FillRule(*e, "fill-rule", DJN_NON_ZERO_FILL);
	} else if (strncmp(v, "evenodd", 7) == 0) {
		new FillRule(*e, "fill-rule", DJN_EVENT_ODD_FILL);
	} else {
		fprintf(stderr, "unknown fill rule value %s\n", v);
		return 0;
	}
	return 1;
}
static int ParseOpacity(FatProcess**e, const char* v) {
	double alpha;

	if (!*e)
		*e = new SVGHolder (nullptr, "SVGHolder");

	if (!SVG_Utils::djn__SVGParseAlpha(&alpha, v)) {
		fprintf(stderr, "cannot read opacity value %s\n", v);
		return 1;
	}

	/* SVG group opacity is a known issue in djnn. For now, we create
	 * a fake property that is connected to a fill and an outline opacities.
	 * Ultimately, this should be replaced by a full-fledged property that
	 * handles properly the semantics of a group opacity.
	 */
	FatProcess *c1 = new FillOpacity(*e, "", alpha);
	FatProcess *c2 = new OutlineOpacity(*e, "", alpha);
	FatProcess *op = new DoubleProperty(*e, "opacity", alpha);
	//new Connector (*e, "", op, "", c1, "a");
	//new Connector (*e, "", op, "", c2, "a");
	new Connector (*e, "", op, c1->find_child("a"));
	new Connector (*e, "", op, c2->find_child("a"));

	return 1;
}

static int ParseFillOpacity(FatProcess** e, const char* v) {
	double alpha;

	if (!*e)
		*e = new SVGHolder (nullptr, "SVGHolder");

	if (!SVG_Utils::djn__SVGParseAlpha(&alpha, v)) {
		fprintf(stderr, "cannot read fill opacity value %s\n", v);
		return 1;
	}

	new FillOpacity(*e, "fill-opacity", alpha);

	return 1;
}

static int ParseStrokeOpacity(FatProcess** e, const char* v) {
	double alpha;

	if (!*e)
		*e = new SVGHolder (nullptr, "SVGHolder");

	if (!SVG_Utils::djn__SVGParseAlpha(&alpha, v)) {
		fprintf(stderr, "cannot read stroke opacity value %s\n", v);
		return 1;
	}

	new OutlineOpacity(*e, "stroke-opacity", alpha);

	return 1;
}

static int ParseStrokeWidth(FatProcess** e, const char* v) {
	double width;
	char *end;

	if (!*e)
		*e = new SVGHolder (nullptr, "SVGHolder");

	width = strtod(v, &end);

	if (v == end)
		return 0;

	new OutlineWidth(*e, "stroke-width", width);

	return 1;
}

static int ParseTransform(FatProcess** e, const char* v) {
	int success = 1;
	int ret = 0;
	djn_XMLAttrHandler *h;
	char *openparenthesis, *closeparenthesis;
	const char* end = v + strlen(v);

	if (!*e)
		*e = new SVGHolder (nullptr, "SVGHolder");

	while (v < end) {
		openparenthesis = strchr( (char*) v, '(');
		if (!openparenthesis)
			return 0;
		*openparenthesis = '\0';
		closeparenthesis = strchr(openparenthesis + 1, ')');

		h = SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup(v, openparenthesis - v);
		ret = (h && (h->handle_attr(e, openparenthesis + 1)));
		success &= ret;
		if (!ret)
			fprintf(stderr, "unknown function '%s' in transform attribute\n", v);
		v = closeparenthesis + 1;
		XML_Utils::djn_XMLRemoveSpacesAndComma((char**) &v);
	}
	return success;
}

static int ParseMatrix(FatProcess** el, const char* v) {
	double a, b, c, d, e, f;
	char *vv = (char *) v;
	if (!XML_Utils::djn_XMLParseDouble(&a, &vv) || !XML_Utils::djn_XMLRemoveSpacesAndComma (&vv))
		goto invalid;

	if (!XML_Utils::djn_XMLParseDouble(&b, &vv) || !XML_Utils::djn_XMLRemoveSpacesAndComma (&vv))
		goto invalid;

	if (!XML_Utils::djn_XMLParseDouble(&c, &vv) || !XML_Utils::djn_XMLRemoveSpacesAndComma (&vv))
		goto invalid;

	if (!XML_Utils::djn_XMLParseDouble(&d, &vv) || !XML_Utils::djn_XMLRemoveSpacesAndComma (&vv))
		goto invalid;

	if (!XML_Utils::djn_XMLParseDouble(&e, &vv) || !XML_Utils::djn_XMLRemoveSpacesAndComma (&vv))
		goto invalid;

	if (!XML_Utils::djn_XMLParseDouble(&f, &vv) || !XML_Utils::djn_XMLRemoveSpacesAndComma (&vv))
		goto invalid;

	while (*vv == ' ' || *vv == '\t' || *vv == '\n' || *vv == '\r')
		v++;
	if (*vv != ')')
		goto invalid;
	++v;
	new Homography(*el, "matrix", a, c, 0, e, b, d, 0, f, 0, 0, 1, 0, 0, 0, 0, 1);
	return 1;

	invalid: 
	fprintf(stderr, "invalid number %s in %s\n", vv, v);
	return 0;
}

static int ParseRotate(FatProcess** el, const char* v) {
	double r, cx, cy;
	char *vv = (char*) v;
	if (!XML_Utils::djn_XMLParseDouble(&r, &vv))
		goto invalid;
	while (*vv == ' ' || *vv == '\t' || *vv == '\n' || *vv == '\r')
		vv++;
	if (*vv == ')') {
		++vv;
		new Rotation(*el, "rotate", r, 0, 0);
	} else {
		if (!XML_Utils::djn_XMLRemoveSpacesAndComma(&vv))
			goto invalid;
		if (!XML_Utils::djn_XMLParseDouble(&cx, &vv) || !XML_Utils::djn_XMLRemoveSpacesAndComma(&vv))
			goto invalid;
		if (!XML_Utils::djn_XMLParseDouble(&cy, &vv))
			goto invalid;
		while (*vv == ' ' || *vv == '\t' || *vv == '\n' || *vv == '\r')
			vv++;
		if (*vv != ')')
			goto invalid;
		++vv;
		new Rotation(*el, "rotate", r, cx, cy);
	}
	return 1;

	invalid: 
	fprintf(stderr, "invalid float %s in %s\n", vv, v);
	return 0;
}

static int ParseScale(FatProcess** el, const char* v) {
	double sx, sy;
	char *vv = (char*) v;
	if (!XML_Utils::djn_XMLParseDouble(&sx, &vv))
		goto invalid;
	while (*vv == ' ' || *vv == '\t' || *vv == '\n' || *vv == '\r')
		vv++;
	if (*vv == ')') {
		new Scaling(*el, "scale", sx, sx, 0, 0);
		return 1;
	}
	if (!XML_Utils::djn_XMLRemoveSpacesAndComma(&vv))
		goto invalid;
	if (!XML_Utils::djn_XMLParseDouble(&sy, &vv))
		goto invalid;
	while (*vv == ' ' || *vv == '\t' || *vv == '\n' || *vv == '\r')
		vv++;
	if (*vv != ')')
		goto invalid;
	++vv;
	new Scaling(*el, "scale", sx, sy, 0, 0);
	return 1;

	invalid: 
	fprintf(stderr, "invalid float %s in %s\n", vv, v);
	return 0;
}

static int ParseSkewX(FatProcess** el, const char* v) {
	double a;
	char *vv = (char*) v;
	if (!XML_Utils::djn_XMLParseDouble(&a, &vv))
		goto invalid;
	while (*vv == ' ' || *vv == '\t' || *vv == '\n' || *vv == '\r')
		vv++;
	if (*vv != ')')
		goto invalid;
	++vv;
	new SkewX(*el, "skewX", a);
	return 1;

	invalid: 
	fprintf(stderr, "invalid float %s in %s\n", vv, v);
	return 0;
}

static int ParseSkewY(FatProcess** el, const char* v) {
	double a;
	char *vv = (char*) v;
	if (!XML_Utils::djn_XMLParseDouble(&a, &vv))
		goto invalid;
	while (*vv == ' ' || *vv == '\t' || *vv == '\n' || *vv == '\r')
		vv++;
	if (*vv != ')')
		goto invalid;
	++vv;
	new SkewY(*el, "skewY", a);
	return 1;

	invalid: 
	fprintf(stderr, "invalid float %s in %s\n", vv, v);
	return 0;
}

static int ParseTranslate(FatProcess** el, const char* v) {
	double tx, ty;
	char *vv = (char*) v;
	if (!XML_Utils::djn_XMLParseDouble(&tx, &vv))
		goto invalid;
	XML_Utils::djn_XMLRemoveSpacesAndComma(&vv);
	if (*vv == ')') {
		new Translation(*el, "translate", tx, 0);
		return 1;
	}
	if (!XML_Utils::djn_XMLParseDouble(&ty, &vv))
		goto invalid;
	while (*vv == ' ' || *vv == '\t' || *vv == '\n' || *vv == '\r')
		vv++;
	if (*vv != ')')
		goto invalid;
	++vv;
	new Translation(*el, "translate", tx, ty);
	return 1;

	invalid: 
	fprintf(stderr, "invalid float %s in %s\n", vv, v);
	return 0;
}

static int ParseLineJoin(FatProcess** e, const char* v) {
	djnJoinStyle join = DJN_MITER_JOIN; /* SVG default value */

	if (!*e)
		*e = new SVGHolder (nullptr, "SVGHolder");

	if (!strcmp(v, "round"))
		join = DJN_ROUNDCAP_JOIN;
	else if (!strcmp(v, "bevel"))
		join = DJN_BEVEL_JOIN;

	new OutlineJoinStyle(*e, "stroke-linejoin", join);

	return 1;
}

static int ParseLineCap(FatProcess** e, const char* v) {
	djnCapStyle cap = DJN_BUTT_CAP; /* SVG default value */

	if (!*e)
		*e = new SVGHolder (nullptr, "SVGHolder");

	if (!strcmp(v, "round"))
		cap = DJN_ROUND_CAP;
	else if (!strcmp(v, "square"))
		cap = DJN_SQUARE_CAP;

	new OutlineCapStyle(*e, "stroke-linecap", cap);

	return 1;
}

static int ParseMiterLimit(FatProcess** e, const char* v) {
	int miterLimit;
	char *end;

	if (!*e)
		*e = new SVGHolder (nullptr, "SVGHolder");

	miterLimit = strtoul(v, &end, 10);

	if (v == end)
		return 0;

	new OutlineMiterLimit(*e, "stroke-miterlimit", miterLimit);

	return 1;
}

static int ParseStrokeDashArray(FatProcess** e, const char* v) {
	int i = 0;
	int size = 0;
	int nbBlocks = 1;
	const int blockSize = 6;
	char *end;
	int *values = (int *)calloc(blockSize, sizeof(int));
	DashArray *dashArray;

	if (!*e)
		*e = new SVGHolder (nullptr, "SVGHolder");

	if (strncmp(v, "none", 4) == 0) {
		new NoDashArray(*e, "");
		if (values)
			free(values);
		return 1;
	}

	dashArray = new DashArray(*e, "stroke-dasharray");

	/* retrieve the integers from the string */
	while (*v) {
		if (i != 0 && (i % blockSize == 0)) {
			nbBlocks++;
			values = (int*)realloc(values, nbBlocks * blockSize * sizeof(int));
		}

		values[i++] = strtoul(v, &end, 10);
		if (v == end) {
			fprintf(stderr, "Invalid values in dash array pattern\n");
			if (values)
				free(values);
			return 0;
		}

		v = end;
		while (*v == ',' || *v == ' ' || *v == '\t' || *v == '\n' || *v == '\r')
			++v;
	}
	size = i;

	/* if there is an odd number of values, duplicate the values (svg policy) */
	if (size % 2) {
		values = (int*)realloc(values, size * 2 * sizeof(int));
		for (i = size; i < size * 2; i++) {
			values[i] = values[i - size];
		}
		size *= 2;
	}

	/* then add the dash subpatterns to the dashArray FatProcess */
	for (i = 0; i < size - 1; i += 2) {
		dashArray->add_sub_pattern (values[i], values[i + 1]);
	}

	if (values)
		free(values);
	return 1;
}

static int ParseStrokeDashOffset(FatProcess** e, const char* v) {
	int offset;
	char *end;

	if (!*e)
		*e = new SVGHolder (nullptr, "SVGHolder");

	offset = strtoul(v, &end, 10);

	if (v == end)
		return 0;

	new DashOffset(*e, "stroke-dashoffset", offset);

	return 1;
}

static string
djn_ParseURL(const char * url) {
	/* We make the (rather strong?) hypothesis that the url has the following syntax: url(#{url}) */
  string str (url);
  // remove possible quotes

  str.erase (std::remove(str.begin(), str.end(), '\''), str.end());
  //then remove the prefix url and the brackets
  std::size_t start = str.find ('#') + 1;
  std::size_t end = str.find (')');
  str = str.substr (start, end - start);
  return str;
}

static int ParseStyle(FatProcess **e, const char *v) {
	/* FIXME very incomplete implementation of the style attribute parsing.
	 * It should be able to manage css style
	 */
	int success = 1;
	djn_XMLAttrHandler *h1, *h2;
	char *colon, *semicolon;
	const char* end = v + strlen(v);

	while (v < end) {
		colon = strchr((char*) v, ':');
		if (!colon)
			return 0;
		*colon = '\0';
		semicolon = strchr(colon + 1, ';');
		if (semicolon)
			*semicolon = '\0';
		else
			semicolon = (char*) end;
		h1 = SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup(v, colon - v);
		h2 = XMLTextAttrs_Hash::djn_XMLTextAttrsLookup(v, colon - v);
		success &= (h1 && (h1->handle_attr(e, colon + 1)))
				|| (h2 && (h2->handle_attr(e, colon + 1)));

		v = semicolon + 1;
		while (*v == ' ' || *v == '\t' || *v == '\n' || *v == '\r')
			v++;
	}
	return success;
}

static int ParseTextAnchor(FatProcess** e, const char* v) {
	djnAnchorType anchor = DJN_START_ANCHOR;
	if (!*e)
		*e = new SVGHolder (nullptr, "SVGHolder");

	if (strcmp(v, "end") == 0)
		anchor = DJN_END_ANCHOR;
	else if (strcmp(v, "middle") == 0)
		anchor = DJN_MIDDLE_ANCHOR;

	new TextAnchor(*e, "text-anchor", anchor);

	return 1;
}

static
int ParseClass (FatProcess** e, const char* v)
{
  djn_GraphicalShapeArgs.classname = std::string (v);
  return 1;
}
}
