/* C++ code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf -L C++ -t -N djn_SVGGradientStopAttrsLookup -Z SVGGradientStopAttrs_Hash src/gui/XML/SVGGradientStopAttrs.gperf  */
/* Computed positions: -k'' */

#line 17 "src/gui/XML/SVGGradientStopAttrs.gperf"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "core/core.h"
#include "gui/gui-dev.h"

#define register

	using namespace djnn;

	extern struct djn_NamedColor* djn_SVGColorsLookup (const char*, gperf_t);

	static int ParseStopColor (Process**, const char*);
	static int ParseStopOpacity (Process**, const char*);
	static int ParseOffset (Process**, const char*);
	static int ParseStopStyle (Process**, const char*);
#line 36 "src/gui/XML/SVGGradientStopAttrs.gperf"

#define TOTAL_KEYWORDS 4
#define MIN_WORD_LENGTH 5
#define MAX_WORD_LENGTH 12
#define MIN_HASH_VALUE 5
#define MAX_HASH_VALUE 12
/* maximum key range = 8, duplicates = 0 */

inline /*ARGSUSED*/
unsigned int
SVGGradientStopAttrs_Hash::hash (register const char *str, register unsigned int len)
{
  return len;
}

djn_XMLAttrHandler *
SVGGradientStopAttrs_Hash::djn_SVGGradientStopAttrsLookup (register const char *str, register unsigned int len)
{
  static djn_XMLAttrHandler wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""},
#line 42 "src/gui/XML/SVGGradientStopAttrs.gperf"
      {"style", &ParseStopStyle},
#line 41 "src/gui/XML/SVGGradientStopAttrs.gperf"
      {"offset", &ParseOffset},
      {""}, {""}, {""},
#line 39 "src/gui/XML/SVGGradientStopAttrs.gperf"
      {"stop-color", &ParseStopColor},
      {""},
#line 40 "src/gui/XML/SVGGradientStopAttrs.gperf"
      {"stop-opacity", &ParseStopOpacity}
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
#line 43 "src/gui/XML/SVGGradientStopAttrs.gperf"


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
