/* C++ code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf -L C++ -t -N djn_XMLTextAttrsLookup -Z XMLTextAttrs_Hash src/gui/XML/XMLTextAttrs.gperf  */
/* Computed positions: -k'$' */

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

#line 16 "src/gui/XML/XMLTextAttrs.gperf"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "core/core.h"
#include "gui/gui-dev.h"

#define register

	using namespace std;

	static int Ignore (Process**, const char*);
	static int ParseX (Process**, const char*);
	static int ParseY (Process**, const char*);
	static int ParseDx (Process**, const char*);
	static int ParseDy (Process**, const char*);
	static int ParseData (Process**, const char*);
	static int ParseSimpleText (Process**, const char*);
	static int ParseEncoding (Process**, const char*);
	static int ParseFontFamily (Process**, const char*);
	static int ParseFontStyle (Process**, const char*);
	static int ParseFontWeight (Process**, const char*);
	static int ParseFontSize (Process**, const char*);
	static char* cleanStr (const char *);
#line 43 "src/gui/XML/XMLTextAttrs.gperf"

#define TOTAL_KEYWORDS 13
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 12
#define MIN_HASH_VALUE 1
#define MAX_HASH_VALUE 21
/* maximum key range = 21, duplicates = 0 */


inline unsigned int
XMLTextAttrs_Hash::hash (register const char *str, register unsigned int len)
{
  static unsigned char asso_values[] =
    {
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 15, 22, 22, 22, 22,
      22,  5, 22,  0,  0, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22,  5, 22, 22, 22,
       5,  0, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
      22, 22, 22, 22, 22, 22
    };
  return len + asso_values[(unsigned char)str[len - 1]];
}

djn_XMLAttrHandler *
XMLTextAttrs_Hash::djn_XMLTextAttrsLookup (register const char *str, register unsigned int len)
{
  static djn_XMLAttrHandler wordlist[] =
    {
      {""},
#line 47 "src/gui/XML/XMLTextAttrs.gperf"
      {"y", &ParseY},
#line 49 "src/gui/XML/XMLTextAttrs.gperf"
      {"dy", &ParseDy},
      {""}, {""}, {""},
#line 46 "src/gui/XML/XMLTextAttrs.gperf"
      {"x", &ParseX},
#line 48 "src/gui/XML/XMLTextAttrs.gperf"
      {"dx", &ParseDx},
#line 52 "src/gui/XML/XMLTextAttrs.gperf"
      {"encoding", &ParseEncoding},
#line 51 "src/gui/XML/XMLTextAttrs.gperf"
      {"text", &ParseSimpleText},
      {""},
#line 53 "src/gui/XML/XMLTextAttrs.gperf"
      {"font-family", &ParseFontFamily},
#line 57 "src/gui/XML/XMLTextAttrs.gperf"
      {"font-stretch", &Ignore},
      {""},
#line 54 "src/gui/XML/XMLTextAttrs.gperf"
      {"font-size", &ParseFontSize},
#line 55 "src/gui/XML/XMLTextAttrs.gperf"
      {"font-style", &ParseFontStyle},
#line 56 "src/gui/XML/XMLTextAttrs.gperf"
      {"font-weight", &ParseFontWeight},
#line 58 "src/gui/XML/XMLTextAttrs.gperf"
      {"font-variant", &Ignore},
      {""}, {""}, {""},
#line 50 "src/gui/XML/XMLTextAttrs.gperf"
      {"_data_", &ParseData}
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
#line 59 "src/gui/XML/XMLTextAttrs.gperf"


struct djn_TextArgs djn_TextArgs = {0., 0., 0., 0., DJN_NO_UNIT, DJN_NO_UNIT, "Utf8", 0};

static int Ignore(Process** e, const char* v) {
	return 0;
}

static int ParseX(Process** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_TextArgs.x, v);
}

static int ParseY(Process** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_TextArgs.y, v);
}

static int ParseDx(Process** e, const char* v) {
	double dx = 0;
	if (SVG_Utils::djn__SVGParseUnitAndValue(&djn_TextArgs.dxUnit, &dx, v)) {
		djn_TextArgs.dx = dx;
		return 1;
	}
	return 0;
}

static int ParseDy(Process** e, const char* v) {
	double dy = 0;
	if (SVG_Utils::djn__SVGParseUnitAndValue(&djn_TextArgs.dyUnit, &dy, v)) {
		djn_TextArgs.dy = dy;
		return 1;
	}
	return 0;
}

static int ParseData(Process** e, const char* v) {
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

static int ParseSimpleText(Process** e, const char* v) {
	djn_TextArgs.data = (char*) malloc(strlen(v + 1));
	strcpy(djn_TextArgs.data, v);
	return 1;
}

static int ParseEncoding(Process** e, const char* v) {
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

static int ParseFontFamily(Process** e, const char* v) {
	if (!*e)
		*e = new SVGHolder;

	new FontFamily(*e, "font-family", v);
	return 1;
}

static int ParseFontStyle(Process** e, const char* v) {
	djnFontSlope style = DJN_NORMAL_FONT;
	if (!*e)
		*e = new SVGHolder;

	if (strcmp(v, "italic") == 0)
		style = DJN_ITALIC_FONT;
	if (strcmp(v, "oblique") == 0)
		style = DJN_OBLIQUE_FONT;

	new FontStyle(*e, "font-style", style);

	return 1;
}

static int ParseFontWeight(Process** e, const char* v) {
	int val = 50; //normal
	unsigned int i;
	int isANumber = 1;
	if (!*e)
		*e = new SVGHolder;

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

static int ParseFontSize(Process** e, const char* v) {
	double sz = 0;
	djnLengthUnit unit = DJN_NO_UNIT;
	if (!*e)
		*e = new SVGHolder;

	if (SVG_Utils::djn__SVGParseUnitAndValue(&unit, &sz, v)) {
		new FontSize(*e, "font-size", unit, sz);
		return 1;
	}

	return 0;
}
