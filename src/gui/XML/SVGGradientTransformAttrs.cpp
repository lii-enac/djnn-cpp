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

#include "gui/gui-dev.h"
#include "gui/gui-xml.h"

#include "gui/transformation/transformations.h"

using namespace djnn;
using namespace djnnstl;

static int ParseMatrix (FatProcess**, const char*);
static int ParseRotate (FatProcess**, const char*);
static int ParseScale (FatProcess**, const char*);
static int ParseSkewX (FatProcess**, const char*);
static int ParseSkewY (FatProcess**, const char*);
static int ParseTranslate (FatProcess**, const char*);

static map <string, djn_XMLAttrHandler> handlers = {
  {"scale",{&ParseScale}},
  {"rotate",{&ParseRotate}},
  {"skewX",{&ParseSkewX}},
  {"translate",{&ParseTranslate}},
  {"skewY",{&ParseSkewY}},
  {"matrix",{&ParseMatrix}}
};

djn_XMLAttrHandler*
SVGGradientTransformAttrs_Hash::djn_SVGGradientTransformAttrsLookup (const char *str, unsigned int len)
{


  map<string, djn_XMLAttrHandler>::iterator it;
  it = handlers.find(string(str));
  if (it != handlers.end())
    return &it->second;
  return 0;
}
static int
ParseMatrix (FatProcess** el, const char* v)
{
	/* XML Matrix (a b c d e f) */
	/* a c e */
	/* b d f */
	/* 0 0 1 */

	double a, b, c, d, e, f;
	char *vv = (char*) v;
	if (!XML_Utils::djn_XMLParseDouble (&a, &vv) || !XML_Utils::djn_XMLRemoveSpacesAndComma (&vv))
	goto invalid;

	if (!XML_Utils::djn_XMLParseDouble (&b, &vv) || !XML_Utils::djn_XMLRemoveSpacesAndComma (&vv))
	goto invalid;

	if (!XML_Utils::djn_XMLParseDouble (&c, &vv) || !XML_Utils::djn_XMLRemoveSpacesAndComma (&vv))
	goto invalid;

	if (!XML_Utils::djn_XMLParseDouble (&d, &vv) || !XML_Utils::djn_XMLRemoveSpacesAndComma (&vv))
	goto invalid;

	if (!XML_Utils::djn_XMLParseDouble (&e, &vv) || !XML_Utils::djn_XMLRemoveSpacesAndComma (&vv))
	goto invalid;

	if (!XML_Utils::djn_XMLParseDouble (&f, &vv) || !XML_Utils::djn_XMLRemoveSpacesAndComma (&vv))
	goto invalid;

	while (*vv == ' ' || *vv == '\t' || *vv == '\n' || *vv == '\r')
	vv++;
	if (*vv != ')')
	goto invalid;
	++vv;

	new SimpleGradientTransform (*el, "", a, b, c, d, e, f);
	return 1;

	invalid:
	fprintf (stderr, "invalid number %s in %s\n", vv, v);
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
		new GradientRotation(*el, "", r, 0, 0);
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
		new GradientRotation(*el, "", r, cx, cy);
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
		new GradientScaling(*el, "", sx, sx, 0, 0);
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
	new GradientScaling(*el, "", sx, sy, 0, 0);
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
	new GradientSkewX(*el, "", a);
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
	new GradientSkewY(*el, "", a);
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
		new GradientTranslation(*el, "", tx, 0);
		return 1;
	}
	if (!XML_Utils::djn_XMLParseDouble(&ty, &vv))
		goto invalid;
	while (*vv == ' ' || *vv == '\t' || *vv == '\n' || *vv == '\r')
		vv++;
	if (*vv != ')')
		goto invalid;
	++vv;
	new GradientTranslation(*el, "", tx, ty);
	return 1;

	invalid: 
	fprintf(stderr, "invalid float %s in %s\n", vv, v);
	return 0;
}
