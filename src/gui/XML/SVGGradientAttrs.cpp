/* C++ code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf -L C++ -t -N djn_SVGGradientAttrsLookup -Z SVGGradientAttrs_Hash src/gui/XML/SVGGradientAttrs.gperf  */
/* Computed positions: -k'' */

#line 17 "src/gui/XML/SVGGradientAttrs.gperf"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "core/core.h"
#include "gui/gui-dev.h"

#define register

	using namespace djnn;
	static int ParseId (Process**, const char*);
	static int ParseGradientTransform (Process**, const char*);
	static int ParseGradientUnits (Process**, const char*);
	static int ParseSpreadMethod (Process**, const char*);
	static int ParseHRef (Process** e, const char* v);

#line 35 "src/gui/XML/SVGGradientAttrs.gperf"

#define TOTAL_KEYWORDS 5
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 33
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 33
/* maximum key range = 32, duplicates = 0 */

inline /*ARGSUSED*/
unsigned int
SVGGradientAttrs_Hash::hash (register const char *str, register unsigned int len)
{
  return len;
}

djn_XMLAttrHandler *
SVGGradientAttrs_Hash::djn_SVGGradientAttrsLookup (register const char *str, register unsigned int len)
{
  static djn_XMLAttrHandler wordlist[] =
    {
      {""}, {""},
#line 38 "src/gui/XML/SVGGradientAttrs.gperf"
      {"id", &ParseId},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 39 "src/gui/XML/SVGGradientAttrs.gperf"
      {"spreadMethod", &ParseSpreadMethod},
#line 40 "src/gui/XML/SVGGradientAttrs.gperf"
      {"gradientUnits", &ParseGradientUnits},
      {""}, {""}, {""},
#line 41 "src/gui/XML/SVGGradientAttrs.gperf"
      {"gradientTransform", &ParseGradientTransform},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 42 "src/gui/XML/SVGGradientAttrs.gperf"
      {"http://www.w3.org/1999/xlink*href", &ParseHRef}
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
#line 43 "src/gui/XML/SVGGradientAttrs.gperf"

struct djn_GradientArgs djn_GradientArgs = {"", 0, DJN_LOCAL_COORDS, DJN_PAD_FILL};

static int ParseId(Process** e, const char* v) {
	djn_GradientArgs.id = v;
	return 1;
}

static int ParseGradientTransform(Process** e, const char* v) {
	djn_GradientArgs.transform = v;
	return 1;
}

static int ParseSpreadMethod(Process** e, const char* v) {
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

static int ParseGradientUnits(Process** e, const char* v) {
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

static int ParseHRef(Process** e, const char* v) {
	string id(v + sizeof(char));
	map<string, Process*>::iterator it = djn__IdFillManager.find (id);
	if (it == djn__IdFillManager.end()) {
		fprintf (stderr, "unknown gradient %s\n", v);
		return 0;
	}

	djn_GradientArgs.ancestor = it->second;
	return 1;
}
