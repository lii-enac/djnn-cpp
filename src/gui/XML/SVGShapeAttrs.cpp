/* C++ code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf -L C++ -t -N djn_SVGShapeAttrsLookup -Z SVGShapeAttrs_Hash src/gui/XML/SVGShapeAttrs.gperf  */
/* Computed positions: -k'2,5,8' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 18 "src/gui/XML/SVGShapeAttrs.gperf"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../../core/core.h"
#include "../gui-dev.h"

#define register

	using namespace djnn;

	static char* djn_ParseURL (const char *);

	static int Ignore (Process**, const char*);
	static int ParseId (Process**, const char*);
	static int ParseStroke (Process**, const char*);
	static int ParseFill (Process**, const char*);
	static int ParseFillRule (Process**, const char*);
	static int ParseOpacity (Process**, const char*);
	static int ParseFillOpacity (Process**, const char*);
	static int ParseStrokeOpacity (Process**, const char*);
	static int ParseStrokeWidth (Process**, const char*);
	static int ParseLineJoin (Process**, const char*);
	static int ParseLineCap (Process**, const char*);
	static int ParseMatrix (Process**, const char*);
	static int ParseMiterLimit (Process**, const char*);
	static int ParseRotate (Process**, const char*);
	static int ParseScale (Process**, const char*);
	static int ParseSkewX (Process**, const char*);
	static int ParseSkewY (Process**, const char*);
	static int ParseStrokeDashArray (Process**, const char*);
	static int ParseStrokeDashOffset (Process**, const char*);
	static int ParseStyle (Process**, const char*);
	static int ParseTranslate (Process**, const char*);
	static int ParseTransform (Process**, const char*);
	static int ParseTextAnchor (Process**, const char*);
	static int ParsePathClip (Process**, const char*);
#line 58 "src/gui/XML/SVGShapeAttrs.gperf"

#define TOTAL_KEYWORDS 54
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 25
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 109
/* maximum key range = 108, duplicates = 0 */

inline unsigned int
SVGShapeAttrs_Hash::hash (register const char *str, register unsigned int len)
{
  static unsigned char asso_values[] =
    {
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110,  10, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110,  60,   5,
      110, 110, 110, 110, 110, 110, 110,  16,  25,  30,
        0,   0, 110,   0,   0,   5, 110,   0,  10,  15,
       35,  30,  25, 110,   5,  55,   0,  10, 110,   0,
       20,  25, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
      110, 110, 110, 110, 110, 110
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
      case 6:
      case 5:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
      case 3:
      case 2:
        hval += asso_values[(unsigned char)str[1]];
        break;
    }
  return hval;
}

djn_XMLAttrHandler *
SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup (register const char *str, register unsigned int len)
{
  static djn_XMLAttrHandler wordlist[] =
    {
      {""}, {""},
#line 74 "src/gui/XML/SVGShapeAttrs.gperf"
      {"id", &ParseId},
      {""}, {""},
#line 105 "src/gui/XML/SVGShapeAttrs.gperf"
      {"style", &ParseStyle},
#line 97 "src/gui/XML/SVGShapeAttrs.gperf"
      {"stroke", &ParseStroke},
      {""}, {""},
#line 70 "src/gui/XML/SVGShapeAttrs.gperf"
      {"fill", &ParseFill},
#line 96 "src/gui/XML/SVGShapeAttrs.gperf"
      {"skewY", &ParseSkewY},
#line 73 "src/gui/XML/SVGShapeAttrs.gperf"
      {"filter", &Ignore},
#line 104 "src/gui/XML/SVGShapeAttrs.gperf"
      {"stroke-width", &ParseStrokeWidth},
      {""}, {""},
#line 93 "src/gui/XML/SVGShapeAttrs.gperf"
      {"shape-rendering", &Ignore},
#line 98 "src/gui/XML/SVGShapeAttrs.gperf"
      {"stroke-dasharray", &ParseStrokeDashArray},
#line 99 "src/gui/XML/SVGShapeAttrs.gperf"
      {"stroke-dashoffset", &ParseStrokeDashOffset},
      {""}, {""},
#line 76 "src/gui/XML/SVGShapeAttrs.gperf"
      {"mask", &Ignore},
#line 91 "src/gui/XML/SVGShapeAttrs.gperf"
      {"requiredFeatures", &Ignore},
#line 68 "src/gui/XML/SVGShapeAttrs.gperf"
      {"display", &Ignore},
#line 90 "src/gui/XML/SVGShapeAttrs.gperf"
      {"requiredExtensions", &Ignore},
#line 101 "src/gui/XML/SVGShapeAttrs.gperf"
      {"stroke-linecap", &ParseLineCap},
#line 100 "src/gui/XML/SVGShapeAttrs.gperf"
      {"stroke-linejoin", &ParseLineJoin},
      {""},
#line 77 "src/gui/XML/SVGShapeAttrs.gperf"
      {"matrix", &ParseMatrix},
      {""},
#line 62 "src/gui/XML/SVGShapeAttrs.gperf"
      {"clip-path", &ParsePathClip},
#line 75 "src/gui/XML/SVGShapeAttrs.gperf"
      {"image-rendering", &Ignore},
      {""},
#line 102 "src/gui/XML/SVGShapeAttrs.gperf"
      {"stroke-miterlimit", &ParseMiterLimit},
#line 113 "src/gui/XML/SVGShapeAttrs.gperf"
      {"xml:lang", &Ignore},
#line 72 "src/gui/XML/SVGShapeAttrs.gperf"
      {"fill-rule", &ParseFillRule},
#line 94 "src/gui/XML/SVGShapeAttrs.gperf"
      {"scale", &ParseScale},
#line 92 "src/gui/XML/SVGShapeAttrs.gperf"
      {"rotate", &ParseRotate},
#line 88 "src/gui/XML/SVGShapeAttrs.gperf"
      {"opacity", &ParseOpacity},
      {""},
#line 63 "src/gui/XML/SVGShapeAttrs.gperf"
      {"clip-rule", &Ignore},
#line 64 "src/gui/XML/SVGShapeAttrs.gperf"
      {"color", &Ignore},
      {""}, {""},
#line 71 "src/gui/XML/SVGShapeAttrs.gperf"
      {"fill-opacity", &ParseFillOpacity},
#line 103 "src/gui/XML/SVGShapeAttrs.gperf"
      {"stroke-opacity", &ParseStrokeOpacity},
#line 111 "src/gui/XML/SVGShapeAttrs.gperf"
      {"visibility", &Ignore},
#line 67 "src/gui/XML/SVGShapeAttrs.gperf"
      {"cursor", &Ignore},
#line 79 "src/gui/XML/SVGShapeAttrs.gperf"
      {"onclick", &Ignore},
#line 112 "src/gui/XML/SVGShapeAttrs.gperf"
      {"xml:base", &Ignore},
      {""},
#line 66 "src/gui/XML/SVGShapeAttrs.gperf"
      {"color-rendering", &Ignore},
#line 107 "src/gui/XML/SVGShapeAttrs.gperf"
      {"text-anchor", &ParseTextAnchor},
      {""}, {""},
#line 89 "src/gui/XML/SVGShapeAttrs.gperf"
      {"pointer-events", &Ignore},
#line 106 "src/gui/XML/SVGShapeAttrs.gperf"
      {"systemLanguage", &Ignore},
#line 83 "src/gui/XML/SVGShapeAttrs.gperf"
      {"onmousedown", &Ignore},
#line 82 "src/gui/XML/SVGShapeAttrs.gperf"
      {"onload", &Ignore},
      {""},
#line 108 "src/gui/XML/SVGShapeAttrs.gperf"
      {"text-rendering", &Ignore},
#line 69 "src/gui/XML/SVGShapeAttrs.gperf"
      {"externalResourcesRequired", &Ignore},
#line 78 "src/gui/XML/SVGShapeAttrs.gperf"
      {"onactivate", &Ignore},
      {""}, {""},
#line 87 "src/gui/XML/SVGShapeAttrs.gperf"
      {"onmouseup", &Ignore},
#line 95 "src/gui/XML/SVGShapeAttrs.gperf"
      {"skewX", &ParseSkewX},
      {""}, {""}, {""},
#line 109 "src/gui/XML/SVGShapeAttrs.gperf"
      {"translate", &ParseTranslate},
#line 61 "src/gui/XML/SVGShapeAttrs.gperf"
      {"class", &Ignore},
#line 84 "src/gui/XML/SVGShapeAttrs.gperf"
      {"onmousemove", &Ignore},
      {""}, {""},
#line 110 "src/gui/XML/SVGShapeAttrs.gperf"
      {"transform", &ParseTransform},
      {""}, {""}, {""}, {""},
#line 80 "src/gui/XML/SVGShapeAttrs.gperf"
      {"onfocusin", &Ignore},
      {""}, {""}, {""}, {""}, {""},
#line 85 "src/gui/XML/SVGShapeAttrs.gperf"
      {"onmouseout", &Ignore},
#line 86 "src/gui/XML/SVGShapeAttrs.gperf"
      {"onmouseover", &Ignore},
      {""}, {""},
#line 65 "src/gui/XML/SVGShapeAttrs.gperf"
      {"color-interpolation", &Ignore},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 81 "src/gui/XML/SVGShapeAttrs.gperf"
      {"onfocusout", &Ignore},
      {""}, {""}, {""},
#line 114 "src/gui/XML/SVGShapeAttrs.gperf"
      {"xml:space", &Ignore}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
#line 115 "src/gui/XML/SVGShapeAttrs.gperf"


struct djn_GraphicalShapeArgs djn_GraphicalShapeArgs = {"", djnStrokeUndef};

static int Ignore(Process** e, const char* v) {
	return 0;
}

static int ParseId(Process** e, const char* v) {
	djn_GraphicalShapeArgs.id = v;
	return 1;
}

static int ParseStroke(Process** e, const char* v) {
	unsigned r, g, b;

	if (!*e)
		*e = new SVGHolder;

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

static int ParsePathClip(Process** e, const char* v) {
	if (!*e)
		*e = new SVGHolder;

	if (strncmp(v, "url(", 4) == 0) {
		char *url = djn_ParseURL(v);
		map<string, Process*>::iterator it = djn__IdClipManager.find (url);
		if (it != djn__IdClipManager.end ()) {
			Container* clip = (Container*) it->second;
			/* we iterate over the children of clip and add each child to *e */
			for (auto child: clip->children ())
			  ((Container*)*e)->add_child (child->clone (), "");
		} else {
			fprintf(stderr, "unknown URL %s for clip\n", v);
			if (url)
				free(url);
			return 0;
		}
		if (url)
			free(url);
	} else
		return 0;

	return 1;
}

static int ParseFill(Process** e, const char* v) {
	unsigned r, g, b;

	if (!*e)
		*e = new SVGHolder;

	if (strncmp(v, "none", 4) == 0) {
		new NoFill(*e, "");
		return 1;

	} else if (strncmp(v, "url(", 4) == 0) {
		char *url = djn_ParseURL(v);
		map<string, Process*>::iterator it = djn__IdFillManager.find (url);
		if (it != djn__IdFillManager.end ()) {
			AbstractGradient* ag = dynamic_cast<AbstractGradient*> (it->second);
			if (ag->is_linear ()) {
			  LinearGradient* lg = dynamic_cast<LinearGradient*> (ag);
			  new RefLinearGradient ((*e), "", lg);
			  (*e)->add_symbol (string (url), lg);
			} else {
			  RadialGradient* rg = dynamic_cast<RadialGradient*> (ag);
			  new RefRadialGradient ((*e), "", rg);
			  (*e)->add_symbol (string (url), rg);
			}
		}
		else {
			fprintf(stderr, "unknown URL %s for fill value\n", v);
			if (url)
				free(url);
			return 0;
		}
		if (url)
			free(url);
	} else {
		if (!SVG_Utils::djn__SVGParseColor(&r, &g, &b, v))
			return 0;

		new FillColor(*e, "fill", r, g, b);
	}

	return 1;
}

static int ParseFillRule(Process** e, const char* v) {
	if (strncmp(v, "nonzero", 7) == 0) {
		new FillRule(*e, "fill-rule", djnNonZeroFill);
	} else if (strncmp(v, "evenodd", 7) == 0) {
		new FillRule(*e, "fill-rule", djnEvenOddFill);
	} else {
		fprintf(stderr, "unknown fill rule value %s\n", v);
		return 0;
	}
	return 1;
}
static int ParseOpacity(Process**e, const char* v) {
	double alpha;

	if (!*e)
		*e = new SVGHolder;

	if (!SVG_Utils::djn__SVGParseAlpha(&alpha, v)) {
		fprintf(stderr, "cannot read opacity value %s\n", v);
		return 1;
	}

	/* svg group opacity is a known issue in djnn. It is preferable not to give
	 * an access to these non-equivalent components => name = 0
	 */
	new FillOpacity(*e, "", alpha);
	new OutlineOpacity(*e, "", alpha);

	return 1;
}

static int ParseFillOpacity(Process** e, const char* v) {
	double alpha;

	if (!*e)
		*e = new SVGHolder;

	if (!SVG_Utils::djn__SVGParseAlpha(&alpha, v)) {
		fprintf(stderr, "cannot read fill opacity value %s\n", v);
		return 1;
	}

	new FillOpacity(*e, "fill-opacity", alpha);

	return 1;
}

static int ParseStrokeOpacity(Process** e, const char* v) {
	double alpha;

	if (!*e)
		*e = new SVGHolder;

	if (!SVG_Utils::djn__SVGParseAlpha(&alpha, v)) {
		fprintf(stderr, "cannot read stroke opacity value %s\n", v);
		return 1;
	}

	new OutlineOpacity(*e, "stroke-opacity", alpha);

	return 1;
}

static int ParseStrokeWidth(Process** e, const char* v) {
	double width;
	char *end;

	if (!*e)
		*e = new SVGHolder;

	width = strtod(v, &end);

	if (v == end)
		return 0;

	new OutlineWidth(*e, "stroke-width", width);

	return 1;
}

static int ParseTransform(Process** e, const char* v) {
	int success = 1;
	int ret = 0;
	djn_XMLAttrHandler *h;
	char *openparenthesis, *closeparenthesis;
	const char* end = v + strlen(v);

	if (!*e)
		*e = new SVGHolder;

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

static int ParseMatrix(Process** el, const char* v) {
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

static int ParseRotate(Process** el, const char* v) {
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

static int ParseScale(Process** el, const char* v) {
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

static int ParseSkewX(Process** el, const char* v) {
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

static int ParseSkewY(Process** el, const char* v) {
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

static int ParseTranslate(Process** el, const char* v) {
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

static int ParseLineJoin(Process** e, const char* v) {
	djnJoinStyle join = djnMiterJoin; /* SVG default value */

	if (!*e)
		*e = new SVGHolder;

	if (!strcmp(v, "round"))
		join = djnRoundCapJoin;
	else if (!strcmp(v, "bevel"))
		join = djnBevelJoin;

	new OutlineJoinStyle(*e, "stroke-linejoin", join);

	return 1;
}

static int ParseLineCap(Process** e, const char* v) {
	djnCapStyle cap = djnButtCap; /* SVG default value */

	if (!*e)
		*e = new SVGHolder;

	if (!strcmp(v, "round"))
		cap = djnRoundCap;
	else if (!strcmp(v, "square"))
		cap = djnSquareCap;

	new OutlineCapStyle(*e, "stroke-linecap", cap);

	return 1;
}

static int ParseMiterLimit(Process** e, const char* v) {
	int miterLimit;
	char *end;

	if (!*e)
		*e = new SVGHolder;

	miterLimit = strtoul(v, &end, 10);

	if (v == end)
		return 0;

	new OutlineMiterLimit(*e, "stroke-miterlimit", miterLimit);

	return 1;
}

static int ParseStrokeDashArray(Process** e, const char* v) {
	int i = 0;
	int size = 0;
	int nbBlocks = 1;
	const int blockSize = 6;
	char *end;
	int *values = (int *)calloc(blockSize, sizeof(int));
	DashArray *dashArray;

	if (!*e)
		*e = new SVGHolder;

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

	/* then add the dash subpatterns to the dashArray Process */
	for (i = 0; i < size - 1; i += 2) {
		dashArray->add_sub_pattern (values[i], values[i + 1]);
	}

	if (values)
		free(values);
	return 1;
}

static int ParseStrokeDashOffset(Process** e, const char* v) {
	int offset;
	char *end;

	if (!*e)
		*e = new SVGHolder;

	offset = strtoul(v, &end, 10);

	if (v == end)
		return 0;

	new DashOffset(*e, "stroke-dashoffset", offset);

	return 1;
}

static char*
djn_ParseURL(const char * url) {
	/* We make the (rather strong?) hypothesis that the url has the following syntax: url(#{url}) */
	int i;
	int j = 5;
	int l = strlen(url);
	char *new_s = (char *)malloc(l - j);

	for (i = 0; j < l - 1; i++) {
		new_s[i] = url[j++];
	}
	new_s[i] = '\0';

	return new_s;
}

static int ParseStyle(Process **e, const char *v) {
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

static int ParseTextAnchor(Process** e, const char* v) {
	djnAnchorType anchor = djnStartAnchor;
	if (!*e)
		*e = new SVGHolder;

	if (strcmp(v, "end") == 0)
		anchor = djnEndAnchor;
	else if (strcmp(v, "middle") == 0)
		anchor = djnMiddleAnchor;

	new TextAnchor(*e, "text-anchor", anchor);

	return 1;
}
