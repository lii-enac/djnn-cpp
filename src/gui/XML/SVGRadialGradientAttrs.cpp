/* C++ code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf -L C++ -t -N djn_SVGRadialGradientAttrsLookup -Z SVGRadialGradientAttrs_Hash src/gui/XML/SVGRadialGradientAttrs.gperf  */
/* Computed positions: -k'1-2' */

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

#line 17 "src/gui/XML/SVGRadialGradientAttrs.gperf"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../core/core.h"
#include "../gui-dev.h"

#define register

	using namespace djnn;

	static int ParseCX (Process**, const char*);
	static int ParseCY (Process**, const char*);
	static int ParseR (Process**, const char*);
	static int ParseFX (Process**, const char*);
	static int ParseFY (Process**, const char*);
#line 35 "src/gui/XML/SVGRadialGradientAttrs.gperf"

#define TOTAL_KEYWORDS 5
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 2
#define MIN_HASH_VALUE 1
#define MAX_HASH_VALUE 9
/* maximum key range = 9, duplicates = 0 */

inline unsigned int
SVGRadialGradientAttrs_Hash::hash (register const char *str, register unsigned int len)
{
  static unsigned char asso_values[] =
    {
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10, 2,
     10,10, 0,10,10,10,10,10,10,10,
     10,10,10,10, 0,10,10,10,10,10,
      5, 0,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10,10,10,10,10,
     10,10,10,10,10,10
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

djn_XMLAttrHandler *
SVGRadialGradientAttrs_Hash::djn_SVGRadialGradientAttrsLookup (register const char *str, register unsigned int len)
{
  static djn_XMLAttrHandler wordlist[] =
    {
      {""},
#line 40 "src/gui/XML/SVGRadialGradientAttrs.gperf"
      {"r", &ParseR},
#line 42 "src/gui/XML/SVGRadialGradientAttrs.gperf"
      {"fy", &ParseFY},
      {""},
#line 39 "src/gui/XML/SVGRadialGradientAttrs.gperf"
      {"cy", &ParseCY},
      {""}, {""},
#line 41 "src/gui/XML/SVGRadialGradientAttrs.gperf"
      {"fx", &ParseFX},
      {""},
#line 38 "src/gui/XML/SVGRadialGradientAttrs.gperf"
      {"cx", &ParseCX}
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
#line 43 "src/gui/XML/SVGRadialGradientAttrs.gperf"


struct djn_RadialGradientArgs djn_RadialGradientArgs = {0.5, 0.5, 0.5, 0.5, 0.5};

static int ParseCX(Process** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientCx);
	return XML_Utils::djn_XMLParseLength(&djn_RadialGradientArgs.cx, v);
}

static int ParseCY(Process** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientCy);
	return XML_Utils::djn_XMLParseLength(&djn_RadialGradientArgs.cy, v);
}

static int ParseR(Process** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientR);
	return XML_Utils::djn_XMLParseLength(&djn_RadialGradientArgs.r, v);
}

static int ParseFX(Process** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientFx);
	return XML_Utils::djn_XMLParseLength(&djn_RadialGradientArgs.fx, v);
}

static int ParseFY(Process** e, const char* v) {
	djn_GradientArgs.inherited &= ~(1 << djn_GradientFy);
	return XML_Utils::djn_XMLParseLength(&djn_RadialGradientArgs.fy, v);
}
