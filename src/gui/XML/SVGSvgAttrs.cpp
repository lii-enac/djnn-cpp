/* C++ code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf -L C++ -t -N djn_SVGSvgAttrsLookup -Z SVGSvgAttrs_Hash src/gui/XML/SVGSvgAttrs.gperf  */
/* Computed positions: -k'' */

#line 16 "src/gui/XML/SVGSvgAttrs.gperf"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "core/core.h"
#include "gui/gui-dev.h"

#define register

	using namespace djnn;

	static int IgnoreAttr (Process**, const char*);
#line 30 "src/gui/XML/SVGSvgAttrs.gperf"

#define TOTAL_KEYWORDS 4
#define MIN_WORD_LENGTH 7
#define MAX_WORD_LENGTH 19
#define MIN_HASH_VALUE 7
#define MAX_HASH_VALUE 19
/* maximum key range = 13, duplicates = 0 */

inline /*ARGSUSED*/
unsigned int
SVGSvgAttrs_Hash::hash (register const char *str, register unsigned int len)
{
  return len;
}

djn_XMLAttrHandler *
SVGSvgAttrs_Hash::djn_SVGSvgAttrsLookup (register const char *str, register unsigned int len)
{
  static djn_XMLAttrHandler wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 36 "src/gui/XML/SVGSvgAttrs.gperf"
      {"viewBox", &IgnoreAttr},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 35 "src/gui/XML/SVGSvgAttrs.gperf"
      {"contentStyleType", &IgnoreAttr},
#line 34 "src/gui/XML/SVGSvgAttrs.gperf"
      {"contentScriptType", &IgnoreAttr},
      {""},
#line 33 "src/gui/XML/SVGSvgAttrs.gperf"
      {"preserveAspectRatio", &IgnoreAttr}
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
#line 37 "src/gui/XML/SVGSvgAttrs.gperf"


static int
IgnoreAttr (Process** e, const char* v)
{
	return 1;
}

