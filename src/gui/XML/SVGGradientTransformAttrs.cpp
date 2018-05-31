/* C++ code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf -L C++ -t -N djn_SVGGradientTransformAttrsLookup -Z SVGGradientTransformAttrs_Hash src/gui/XML/SVGGradientTransformAttrs.gperf  */
/* Computed positions: -k'5' */

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

#line 17 "src/gui/XML/SVGGradientTransformAttrs.gperf"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../core/core.h"
#include "../gui-dev.h"

#define register

	using namespace djnn;
	static int ParseMatrix (Process**, const char*);
	static int ParseRotate (Process**, const char*);
	static int ParseScale (Process**, const char*);
	static int ParseSkewX (Process**, const char*);
	static int ParseSkewY (Process**, const char*);
	static int ParseTranslate (Process**, const char*);

#line 36 "src/gui/XML/SVGGradientTransformAttrs.gperf"

#define TOTAL_KEYWORDS 6
#define MIN_WORD_LENGTH 5
#define MAX_WORD_LENGTH 9
#define MIN_HASH_VALUE 5
#define MAX_HASH_VALUE 11
/* maximum key range = 7, duplicates = 0 */

inline unsigned int
SVGGradientTransformAttrs_Hash::hash (register const char *str, register unsigned int len)
{
  static unsigned char asso_values[] =
    {
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12,  2,  5,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12,  0, 12, 12, 12,  5, 12, 12, 12, 12,
      12, 12, 12, 12, 12,  0,  0, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12
    };
  return len + asso_values[(unsigned char)str[4]];
}

djn_XMLAttrHandler *
SVGGradientTransformAttrs_Hash::djn_SVGGradientTransformAttrsLookup (register const char *str, register unsigned int len)
{
  static djn_XMLAttrHandler wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""},
#line 40 "src/gui/XML/SVGGradientTransformAttrs.gperf"
      {"scale", &ParseScale},
#line 44 "src/gui/XML/SVGGradientTransformAttrs.gperf"
      {"rotate", &ParseRotate},
#line 41 "src/gui/XML/SVGGradientTransformAttrs.gperf"
      {"skewX", &ParseSkewX},
      {""},
#line 43 "src/gui/XML/SVGGradientTransformAttrs.gperf"
      {"translate", &ParseTranslate},
#line 42 "src/gui/XML/SVGGradientTransformAttrs.gperf"
      {"skewY", &ParseSkewY},
#line 39 "src/gui/XML/SVGGradientTransformAttrs.gperf"
      {"matrix", &ParseMatrix}
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
#line 45 "src/gui/XML/SVGGradientTransformAttrs.gperf"


static int
ParseMatrix (Process** el, const char* v)
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

static int ParseRotate(Process** el, const char* v) {
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

static int ParseScale(Process** el, const char* v) {
	double sx, sy;
	char *vv = (char*) v;
	if (!XML_Utils::djn_XMLParseDouble(&sx, &vv))
		goto invalid;
	while (*vv == ' ' || *vv == '\t' || *vv == '\n' || *vv == '\r')
		v++;
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
	new GradientSkewX(*el, "", a);
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
	new GradientSkewY(*el, "", a);
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
