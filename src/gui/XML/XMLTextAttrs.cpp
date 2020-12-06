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
#include <ctype.h>
// #include "core/cor__e.h"
#include "gui/gui-dev.h"
#include "gui/shapes/abstract_gshape.h" // SVGHolder
#include "gui/style/style.h" // text style


using namespace std;

namespace djnn {

static int Ignore (FatProcess**, const char*);
static int ParseX (FatProcess**, const char*);
static int ParseY (FatProcess**, const char*);
static int ParseDx (FatProcess**, const char*);
static int ParseDy (FatProcess**, const char*);
static int ParseData (FatProcess**, const char*);
static int ParseSimpleText (FatProcess**, const char*);
static int ParseEncoding (FatProcess**, const char*);
static int ParseFontFamily (FatProcess**, const char*);
static int ParseFontStyle (FatProcess**, const char*);
static int ParseFontWeight (FatProcess**, const char*);
static int ParseFontSize (FatProcess**, const char*);
static char* cleanStr (const char *);

static std::map <std::string, djn_XMLAttrHandler> handlers = {
  {"y",{&ParseY}},
  {"dy",{&ParseDy}},
  {"x",{&ParseX}},
  {"dx",{&ParseDx}},
  {"encoding",{&ParseEncoding}},
  {"text",{&ParseSimpleText}},
  {"font-family",{&ParseFontFamily}},
  {"font-stretch",{&Ignore}},
  {"font-size",{&ParseFontSize}},
  {"font-style",{&ParseFontStyle}},
  {"font-weight",{&ParseFontWeight}},
  {"font-variant",{&Ignore}},
  {"_data_",{&ParseData}}
};

djn_XMLAttrHandler*
XMLTextAttrs_Hash::djn_XMLTextAttrsLookup (const char *str, unsigned int len)
{
  std::map<std::string, djn_XMLAttrHandler>::iterator it;
  it = handlers.find(std::string(str));
  if (it != handlers.end())
    return &it->second;
  return 0;
}

struct djn_TextArgs djn_TextArgs = {0., 0., 0., 0., DJN_NO_UNIT, DJN_NO_UNIT, "Utf8", 0};

static int Ignore(FatProcess** e, const char* v) {
	return 0;
}

static int ParseX(FatProcess** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_TextArgs.x, v);
}

static int ParseY(FatProcess** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_TextArgs.y, v);
}

static int ParseDx(FatProcess** e, const char* v) {
	double dx = 0;
	if (SVG_Utils::djn__SVGParseUnitAndValue(&djn_TextArgs.dxUnit, &dx, v)) {
		djn_TextArgs.dx = dx;
		return 1;
	}
	return 0;
}

static int ParseDy(FatProcess** e, const char* v) {
	double dy = 0;
	if (SVG_Utils::djn__SVGParseUnitAndValue(&djn_TextArgs.dyUnit, &dy, v)) {
		djn_TextArgs.dy = dy;
		return 1;
	}
	return 0;
}

static int ParseData(FatProcess** e, const char* v) {
	int l1, l2;
	char *c = cleanStr(v);
	if (c == 0)
		return 1;

	l1 = strlen(c);
	if (!djn_TextArgs.data) {
		djn_TextArgs.data = (char*) malloc(l1 + 1);
		if (!djn_TextArgs.data) {
			fprintf(stderr, "Unable to allocate memory for new text data\n");
			free(c);
			return 0;
		}
		strcpy(djn_TextArgs.data, c);
	} else {
		l2 = strlen(djn_TextArgs.data);
		djn_TextArgs.data = (char*) realloc(djn_TextArgs.data, (l1 + l2 + 1));
		if (!djn_TextArgs.data) {
			fprintf(stderr, "Unable to re-allocate memory for new text data\n");
			free(c);
			return 0;
		}
		strcat(djn_TextArgs.data, c);
	}
	free(c);
	return 1;
}

static int ParseSimpleText(FatProcess** e, const char* v) {
	djn_TextArgs.data = (char*) malloc(strlen(v + 1));
	strcpy(djn_TextArgs.data, v);
	return 1;
}

static int ParseEncoding(FatProcess** e, const char* v) {
	djn_TextArgs.encoding = v;
	return 1;
}

int isSpace(const char *c) {
	return (*c == '\n' || *c == '\t' || *c == '\r' || *c == ' ');
}

static char*
cleanStr(const char *str) {
	int len = strlen(str);
	int right_spaces = 0;
	int out_size;
	const char *end = str + len - 1;
	char *result = 0;

	/* left spaces */
	while (isSpace(str))
		str++;

	/* Only white spaces? */
	if (strlen(str) == 0)
		return 0;

	/* right spaces */
	while (isSpace(end--))
		right_spaces++;

	out_size = strlen(str) - right_spaces;
	result = (char*) malloc(out_size + 1);
	strncpy(result, str, out_size);
	result[out_size] = 0;
	return result;
}

int
SVG_Utils::djn__SVGParseUnitAndValue(djnLengthUnit *unit, double *value,
		const char *str) {
	int len;
	const char *end = 0;

	len = strlen(str);
	if (len == 0)
		return 0;
	end = str + len - 1;

	*unit = DJN_NO_UNIT;
	if (*end == '%')
		*unit = DJN_PERCENT;
	end--;
	if (strcmp(end, "em") == 0)
		*unit = DJN_EM;
	if (strcmp(end, "ex") == 0)
		*unit = DJN_EX;
	if (strcmp(end, "pc") == 0)
		*unit = DJN_PC;
	if (strcmp(end, "pt") == 0)
		*unit = DJN_PT;
	if (strcmp(end, "px") == 0)
		*unit = DJN_PX;
	if (strcmp(end, "cm") == 0)
		*unit = DJN_CM;
	if (strcmp(end, "mm") == 0)
		*unit = DJN_MM;
	if (strcmp(end, "in") == 0)
		*unit = DJN_IN;

	*value = atof(str);
	return 1;
}

static int ParseFontFamily(FatProcess** e, const char* v) {
	if (!*e)
		*e = new SVGHolder (nullptr, "SVGHolder");

	new FontFamily(*e, "font-family", v);
	return 1;
}

static int ParseFontStyle(FatProcess** e, const char* v) {
	djnFontSlope style = DJN_NORMAL_FONT;
	if (!*e)
		*e = new SVGHolder (nullptr, "SVGHolder");

	if (strcmp(v, "italic") == 0)
		style = DJN_ITALIC_FONT;
	if (strcmp(v, "oblique") == 0)
		style = DJN_OBLIQUE_FONT;

	new FontStyle(*e, "font-style", style);

	return 1;
}

static int ParseFontWeight(FatProcess** e, const char* v) {
	int val = 50; //normal
	unsigned int i;
	int isANumber = 1;
	if (!*e)
		*e = new SVGHolder (nullptr, "SVGHolder");

	for (i = 0; i < strlen(v); i++) {
		if (!isdigit(v[i]))
			isANumber = 0;
	}

	/* All values are transduced to numerical values in [-1, 100]
	 with -1 for lighter and 100 for bolder */

	if (isANumber) {
		val = strtol(v, 0, 10) / 10;
	} else {
		if (strcmp(v, "normal") == 0)
			val = 50;
		if (strcmp(v, "bold") == 0)
			val = 75;
		if (strcmp(v, "lighter") == 0)
			val = -1;
		if (strcmp(v, "bolder") == 0)
			val = 100;
	}

	new FontWeight(*e, "font-weight", val);

	return 1;
}

static int ParseFontSize(FatProcess** e, const char* v) {
	double sz = 0;
	djnLengthUnit unit = DJN_NO_UNIT;
	if (!*e)
		*e = new SVGHolder (nullptr, "SVGHolder");

	if (SVG_Utils::djn__SVGParseUnitAndValue(&unit, &sz, v)) {
		new FontSize(*e, "font-size", unit, sz);
		return 1;
	}

	return 0;
}
}
