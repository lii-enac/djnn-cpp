/* C++ code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf -L C++ -t -N djn_XMLImgAttrsLookup -Z XMLImgAttrs_Hash src/gui/XML/XMLImgAttrs.gperf  */
/* Computed positions: -k'1' */

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

#line 16 "src/gui/XML/XMLImgAttrs.gperf"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../core/core.h"
#include "../gui-dev.h"

#define register

	using namespace djnn;

	static int ParseX (Process**, const char*);
	static int ParseY (Process**, const char*);
	static int ParseWidth (Process**, const char*);
	static int ParseHeight (Process**, const char*);
	static int ParseHref (Process**, const char*);
#line 34 "src/gui/XML/XMLImgAttrs.gperf"

#define TOTAL_KEYWORDS 5
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 33
#define MIN_HASH_VALUE 1
#define MAX_HASH_VALUE 33
/* maximum key range = 33, duplicates = 0 */

inline unsigned int
XMLImgAttrs_Hash::hash (register const char *str, register unsigned int len)
{
  static unsigned char asso_values[] =
    {
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34,  0, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34,  0,
       2,  0, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34
    };
  return len + asso_values[(unsigned char)str[0]];
}

djn_XMLAttrHandler *
XMLImgAttrs_Hash::djn_XMLImgAttrsLookup (register const char *str, register unsigned int len)
{
  static djn_XMLAttrHandler wordlist[] =
    {
      {""},
#line 38 "src/gui/XML/XMLImgAttrs.gperf"
      {"y", &ParseY},
      {""},
#line 37 "src/gui/XML/XMLImgAttrs.gperf"
      {"x", &ParseX},
      {""},
#line 39 "src/gui/XML/XMLImgAttrs.gperf"
      {"width", &ParseWidth},
#line 40 "src/gui/XML/XMLImgAttrs.gperf"
      {"height", &ParseHeight},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 41 "src/gui/XML/XMLImgAttrs.gperf"
      {"http://www.w3.org/1999/xlink*href", &ParseHref}
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
#line 42 "src/gui/XML/XMLImgAttrs.gperf"


struct djn_ImgArgs djn_ImgArgs = {0., 0., 0., 0., 0};

static int ParseX(Process** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_ImgArgs.x, v);
}

static int ParseY(Process** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_ImgArgs.y, v);
}

static int ParseWidth(Process** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_ImgArgs.w, v);
}

static int ParseHeight(Process** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_ImgArgs.h, v);
}

static int ParseHref(Process** e, const char* v) {
  char *result;
  char *start = strstr ( (char*) v, "file://");
  if (start == v) {
    v += 6;
    result = (char*) malloc (strlen (v) + 1);
    strcpy (result, v);
    djn_ImgArgs.path = result;
  } else {
    djn_ImgArgs.path = v;
  }

  return 1; 
}
