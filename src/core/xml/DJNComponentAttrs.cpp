/* C++ code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf -L C++ -t -N djn_DJNComponentAttrsLookup -Z DJNComponentAttrs_Hash src/core/xml/DJNComponentAttrs.gperf  */
/* Computed positions: -k'' */

#line 16 "src/core/xml/DJNComponentAttrs.gperf"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "xml-dev.h"

#define register

	using namespace djnn;

static int HandleId (Process**, const char*);
static int HandleModel (Process**, const char*);

#line 31 "src/core/xml/DJNComponentAttrs.gperf"

#define TOTAL_KEYWORDS 2
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 5
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 5
/* maximum key range = 4, duplicates = 0 */

inline /*ARGSUSED*/
unsigned int
DJNComponentAttrs_Hash::hash (register const char *str, register unsigned int len)
{
  return len;
}

djn_XMLAttrHandler *
DJNComponentAttrs_Hash::djn_DJNComponentAttrsLookup (register const char *str, register unsigned int len)
{
  static djn_XMLAttrHandler wordlist[] =
    {
      {""}, {""},
#line 34 "src/core/xml/DJNComponentAttrs.gperf"
      {"id", &HandleId},
      {""}, {""},
#line 35 "src/core/xml/DJNComponentAttrs.gperf"
      {"model", &HandleModel}
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
#line 36 "src/core/xml/DJNComponentAttrs.gperf"


struct djn_ComponentArgs_t djn_ComponentArgs = { "", 0 };

static int
HandleId (Process** e, const char* v)
{
  djn_ComponentArgs.id = v;
  return 1;
}

static int
HandleModel (Process** e, const char* v)
{
  djn_ComponentArgs.model = !!atoi (v);
  return 1;
}
