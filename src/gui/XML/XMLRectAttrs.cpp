/* C++ code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf -L C++ -t -N djn_XMLRectAttrsLookup -Z XMLRectAttrs_Hash src/gui/XML/XMLRectAttrs.gperf  */
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

#line 16 "src/gui/XML/XMLRectAttrs.gperf"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../core/core.h"
#include "../gui-dev.h"

#define register
	
	using namespace std;

static int ParseX (Process**, const char*);
static int ParseY (Process**, const char*);
static int ParseWidth (Process**, const char*);
static int ParseHeight (Process**, const char*);
static int ParseRx (Process**, const char*);
static int ParseRy (Process**, const char*);
#line 35 "src/gui/XML/XMLRectAttrs.gperf"

#define TOTAL_KEYWORDS 6
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 6
#define MIN_HASH_VALUE 1
#define MAX_HASH_VALUE 11
/* maximum key range = 11, duplicates = 0 */

inline unsigned int
XMLRectAttrs_Hash::hash (register const char *str, register unsigned int len)
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
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12, 12,  0, 12, 12, 12, 12, 12,
      12, 12, 12, 12, 12, 12,  5, 12, 12, 12,
       5,  0, 12, 12, 12, 12, 12, 12, 12, 12,
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
  return len + asso_values[(unsigned char)str[len - 1]];
}

djn_XMLAttrHandler *
XMLRectAttrs_Hash::djn_XMLRectAttrsLookup (register const char *str, register unsigned int len)
{
  static djn_XMLAttrHandler wordlist[] =
    {
      {""},
#line 39 "src/gui/XML/XMLRectAttrs.gperf"
      {"y", &ParseY},
#line 43 "src/gui/XML/XMLRectAttrs.gperf"
      {"ry", &ParseRy},
      {""}, {""},
#line 40 "src/gui/XML/XMLRectAttrs.gperf"
      {"width", &ParseWidth},
#line 38 "src/gui/XML/XMLRectAttrs.gperf"
      {"x", &ParseX},
#line 42 "src/gui/XML/XMLRectAttrs.gperf"
      {"rx", &ParseRx},
      {""}, {""}, {""},
#line 41 "src/gui/XML/XMLRectAttrs.gperf"
      {"height", &ParseHeight}
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
#line 44 "src/gui/XML/XMLRectAttrs.gperf"


struct djn_RectArgs djn_RectArgs = {0., 0., 0., 0., -1., -1.};

static int
ParseX (Process** e, const char* v)
{
  return XML_Utils::djn_XMLParseLength (&djn_RectArgs.x, v);
}

static int
ParseY (Process** e, const char* v)
{
  return XML_Utils::djn_XMLParseLength (&djn_RectArgs.y, v);
}

static int
ParseWidth (Process** e, const char* v)
{
  return XML_Utils::djn_XMLParseLength (&djn_RectArgs.w, v);
}

static int
ParseHeight (Process** e, const char* v)
{
  return XML_Utils::djn_XMLParseLength (&djn_RectArgs.h, v);
}

static int
ParseRx (Process** e, const char* v)
{
  int r = XML_Utils::djn_XMLParseLength (&djn_RectArgs.rx, v);
  if (r && djn_RectArgs.ry == -1)
    djn_RectArgs.ry = djn_RectArgs.rx;
  return r;
}

static int
ParseRy (Process** e, const char* v)
{
  int r = XML_Utils::djn_XMLParseLength (&djn_RectArgs.ry, v);
  if (r && djn_RectArgs.rx == -1)
    djn_RectArgs.rx = djn_RectArgs.ry;
  return r;
}
