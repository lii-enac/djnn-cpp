/* C++ code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf -L C++ -t -N djn_SVGColorsLookup -Z SVGColors_Hash src/gui/XML/SVGColors.gperf  */
/* Computed positions: -k'1,3,6-8,12-13' */

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

#line 16 "src/gui/XML/SVGColors.gperf"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "core/core.h"
#include "gui/gui-dev.h"

#define register

#line 29 "src/gui/XML/SVGColors.gperf"
struct djn_NamedColor;

#define TOTAL_KEYWORDS 147
#define MIN_WORD_LENGTH 3
#define MAX_WORD_LENGTH 20
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 513
/* maximum key range = 510, duplicates = 0 */

inline unsigned int
SVGColors_Hash::hash (register const char *str, register unsigned int len)
{
  static unsigned short asso_values[] =
    {
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514,   5,  55,   0,
       35,   0,  75,  10,  20,   0, 514, 198,  10,  40,
       85,  60,  70, 130,   0,  20,  45,  10, 150, 185,
       45, 195, 514,   0, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514, 514, 514,
      514, 514, 514, 514, 514, 514, 514, 514
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[12]];
      /*FALLTHROUGH*/
      case 12:
        hval += asso_values[(unsigned char)str[11]];
      /*FALLTHROUGH*/
      case 11:
      case 10:
      case 9:
      case 8:
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
        hval += asso_values[(unsigned char)str[6]];
      /*FALLTHROUGH*/
      case 6:
        hval += asso_values[(unsigned char)str[5]];
      /*FALLTHROUGH*/
      case 5:
      case 4:
      case 3:
        hval += asso_values[(unsigned char)str[2]+2];
      /*FALLTHROUGH*/
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

struct djn_NamedColor *
SVGColors_Hash::djn_SVGColorsLookup (register const char *str, register unsigned int len)
{
  static struct djn_NamedColor wordlist[] =
    {
      {""}, {""}, {""}, {""},
#line 52 "src/gui/XML/SVGColors.gperf"
      {"cyan", 0x00ffff},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 85 "src/gui/XML/SVGColors.gperf"
      {"gray", 0x808080},
      {""}, {""}, {""}, {""}, {""},
#line 46 "src/gui/XML/SVGColors.gperf"
      {"chartreuse", 0x7fff00},
      {""}, {""}, {""},
#line 86 "src/gui/XML/SVGColors.gperf"
      {"grey", 0x808080},
#line 87 "src/gui/XML/SVGColors.gperf"
      {"green", 0x008000},
      {""}, {""}, {""},
#line 105 "src/gui/XML/SVGColors.gperf"
      {"lightgrey", 0xd3d3d3},
#line 104 "src/gui/XML/SVGColors.gperf"
      {"lightgreen", 0x90ee90},
      {""}, {""}, {""},
#line 103 "src/gui/XML/SVGColors.gperf"
      {"lightgray", 0xd3d3d3},
      {""}, {""},
#line 161 "src/gui/XML/SVGColors.gperf"
      {"skyblue", 0x87ceeb},
      {""},
#line 164 "src/gui/XML/SVGColors.gperf"
      {"slategrey", 0x708090},
      {""},
#line 159 "src/gui/XML/SVGColors.gperf"
      {"sienna", 0xa0522d},
      {""}, {""},
#line 163 "src/gui/XML/SVGColors.gperf"
      {"slategray", 0x708090},
      {""}, {""}, {""},
#line 158 "src/gui/XML/SVGColors.gperf"
      {"seashell", 0xfff5ee},
#line 169 "src/gui/XML/SVGColors.gperf"
      {"teal", 0x008080},
#line 48 "src/gui/XML/SVGColors.gperf"
      {"coral", 0xff7f50},
      {""}, {""}, {""}, {""}, {""},
#line 107 "src/gui/XML/SVGColors.gperf"
      {"lightsalmon", 0xffa07a},
      {""}, {""},
#line 111 "src/gui/XML/SVGColors.gperf"
      {"lightslategrey", 0x778899},
#line 39 "src/gui/XML/SVGColors.gperf"
      {"black", 0x000000},
      {""}, {""}, {""},
#line 110 "src/gui/XML/SVGColors.gperf"
      {"lightslategray", 0x778899},
      {""},
#line 137 "src/gui/XML/SVGColors.gperf"
      {"orange", 0xffa500},
      {""}, {""},
#line 138 "src/gui/XML/SVGColors.gperf"
      {"orangered", 0xff4500},
      {""},
#line 38 "src/gui/XML/SVGColors.gperf"
      {"bisque", 0xffe4c4},
      {""}, {""},
#line 114 "src/gui/XML/SVGColors.gperf"
      {"lime", 0x00ff00},
      {""}, {""}, {""},
#line 151 "src/gui/XML/SVGColors.gperf"
      {"red", 0xff0000},
#line 115 "src/gui/XML/SVGColors.gperf"
      {"limegreen", 0x32cd32},
#line 100 "src/gui/XML/SVGColors.gperf"
      {"lightcoral", 0xf08080},
      {""}, {""}, {""},
#line 153 "src/gui/XML/SVGColors.gperf"
      {"royalblue", 0x4169e1},
#line 116 "src/gui/XML/SVGColors.gperf"
      {"linen", 0xfaf0e6},
      {""},
#line 80 "src/gui/XML/SVGColors.gperf"
      {"fuchsia", 0xff00ff},
      {""},
#line 57 "src/gui/XML/SVGColors.gperf"
      {"darkgreen", 0x006400},
      {""}, {""}, {""}, {""},
#line 99 "src/gui/XML/SVGColors.gperf"
      {"lightblue", 0xadd8e6},
      {""},
#line 166 "src/gui/XML/SVGColors.gperf"
      {"springgreen", 0x00ff7f},
#line 117 "src/gui/XML/SVGColors.gperf"
      {"magenta", 0xff00ff},
#line 95 "src/gui/XML/SVGColors.gperf"
      {"lavender", 0xe6e6fa},
#line 83 "src/gui/XML/SVGColors.gperf"
      {"gold", 0xffd700},
      {""},
#line 139 "src/gui/XML/SVGColors.gperf"
      {"orchid", 0xda70d6},
      {""}, {""},
#line 162 "src/gui/XML/SVGColors.gperf"
      {"slateblue", 0x6a5acd},
      {""},
#line 60 "src/gui/XML/SVGColors.gperf"
      {"darkmagenta", 0x8b008b},
      {""},
#line 53 "src/gui/XML/SVGColors.gperf"
      {"darkblue", 0x00008b},
#line 112 "src/gui/XML/SVGColors.gperf"
      {"lightsteelblue", 0xb0c4de},
#line 63 "src/gui/XML/SVGColors.gperf"
      {"darkorchid", 0x9932cc},
#line 160 "src/gui/XML/SVGColors.gperf"
      {"silver", 0xc0c0c0},
      {""},
#line 157 "src/gui/XML/SVGColors.gperf"
      {"seagreen", 0x2e8b57},
#line 167 "src/gui/XML/SVGColors.gperf"
      {"steelblue", 0x4682b4},
      {""}, {""}, {""},
#line 168 "src/gui/XML/SVGColors.gperf"
      {"tan", 0xd2b48c},
#line 146 "src/gui/XML/SVGColors.gperf"
      {"peru", 0xcd853f},
      {""},
#line 150 "src/gui/XML/SVGColors.gperf"
      {"purple", 0x800080},
#line 64 "src/gui/XML/SVGColors.gperf"
      {"darkred", 0x8b0000},
      {""},
#line 129 "src/gui/XML/SVGColors.gperf"
      {"mintcream", 0xf5fffa},
      {""}, {""}, {""}, {""},
#line 77 "src/gui/XML/SVGColors.gperf"
      {"firebrick", 0xb22222},
      {""}, {""}, {""},
#line 108 "src/gui/XML/SVGColors.gperf"
      {"lightseagreen", 0x20b2aa},
#line 133 "src/gui/XML/SVGColors.gperf"
      {"navy", 0x000080},
#line 93 "src/gui/XML/SVGColors.gperf"
      {"ivory", 0xfffff0},
      {""}, {""}, {""},
#line 130 "src/gui/XML/SVGColors.gperf"
      {"mistyrose", 0xffe4e1},
      {""},
#line 92 "src/gui/XML/SVGColors.gperf"
      {"indigo", 0x4b0082},
#line 134 "src/gui/XML/SVGColors.gperf"
      {"oldlace", 0xfdf5e6},
#line 96 "src/gui/XML/SVGColors.gperf"
      {"lavenderblush", 0xfff0f5},
#line 147 "src/gui/XML/SVGColors.gperf"
      {"pink", 0xffc0cb},
#line 65 "src/gui/XML/SVGColors.gperf"
      {"darksalmon", 0xe9967a},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 131 "src/gui/XML/SVGColors.gperf"
      {"moccasin", 0xffe4b5},
#line 165 "src/gui/XML/SVGColors.gperf"
      {"snow", 0xfffafa},
      {""}, {""}, {""}, {""},
#line 45 "src/gui/XML/SVGColors.gperf"
      {"cadetblue", 0x5f9ea0},
#line 71 "src/gui/XML/SVGColors.gperf"
      {"darkviolet", 0x9400d3},
#line 154 "src/gui/XML/SVGColors.gperf"
      {"saddlebrown", 0x8b4513},
      {""},
#line 67 "src/gui/XML/SVGColors.gperf"
      {"darkslateblue", 0x483d8b},
#line 141 "src/gui/XML/SVGColors.gperf"
      {"palegreen", 0x98fb98},
      {""}, {""}, {""}, {""},
#line 91 "src/gui/XML/SVGColors.gperf"
      {"indianred", 0xcd5c5c},
#line 102 "src/gui/XML/SVGColors.gperf"
      {"lightgoldenrodyellow", 0xfafad2},
#line 171 "src/gui/XML/SVGColors.gperf"
      {"tomato", 0xff6347},
      {""}, {""},
#line 106 "src/gui/XML/SVGColors.gperf"
      {"lightpink", 0xffb6c1},
      {""},
#line 118 "src/gui/XML/SVGColors.gperf"
      {"maroon", 0x800000},
      {""}, {""},
#line 172 "src/gui/XML/SVGColors.gperf"
      {"turquoise", 0x40e0d0},
#line 62 "src/gui/XML/SVGColors.gperf"
      {"darkorange", 0xff8c00},
      {""}, {""}, {""}, {""},
#line 76 "src/gui/XML/SVGColors.gperf"
      {"dodgerblue", 0x1e90ff},
#line 79 "src/gui/XML/SVGColors.gperf"
      {"forestgreen", 0x228b22},
#line 98 "src/gui/XML/SVGColors.gperf"
      {"lemonchiffon", 0xfffacd},
      {""},
#line 123 "src/gui/XML/SVGColors.gperf"
      {"mediumseagreen", 0x3cb371},
#line 43 "src/gui/XML/SVGColors.gperf"
      {"brown", 0xa52a2a},
      {""},
#line 66 "src/gui/XML/SVGColors.gperf"
      {"darkseagreen", 0x8fbc8b},
      {""},
#line 34 "src/gui/XML/SVGColors.gperf"
      {"aqua", 0x00ffff},
#line 36 "src/gui/XML/SVGColors.gperf"
      {"azure", 0xf0ffff},
#line 155 "src/gui/XML/SVGColors.gperf"
      {"salmon", 0xfa8072},
      {""}, {""},
#line 47 "src/gui/XML/SVGColors.gperf"
      {"chocolate", 0xd2691e},
#line 174 "src/gui/XML/SVGColors.gperf"
      {"wheat", 0xf5deb3},
      {""},
#line 128 "src/gui/XML/SVGColors.gperf"
      {"midnightblue", 0x191970},
#line 94 "src/gui/XML/SVGColors.gperf"
      {"khaki", 0xf0e68c},
      {""},
#line 35 "src/gui/XML/SVGColors.gperf"
      {"aquamarine", 0x7fffd4},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 97 "src/gui/XML/SVGColors.gperf"
      {"lawngreen", 0x7cfc00},
#line 156 "src/gui/XML/SVGColors.gperf"
      {"sandybrown", 0xf4a460},
      {""}, {""}, {""},
#line 101 "src/gui/XML/SVGColors.gperf"
      {"lightcyan", 0xe0ffff},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 113 "src/gui/XML/SVGColors.gperf"
      {"lightyellow", 0xffffe0},
      {""}, {""}, {""},
#line 120 "src/gui/XML/SVGColors.gperf"
      {"mediumblue", 0x0000cd},
      {""}, {""}, {""},
#line 145 "src/gui/XML/SVGColors.gperf"
      {"peachpuff", 0xffdab9},
      {""},
#line 88 "src/gui/XML/SVGColors.gperf"
      {"greenyellow", 0xadff2f},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 41 "src/gui/XML/SVGColors.gperf"
      {"blue", 0x0000ff},
      {""}, {""},
#line 122 "src/gui/XML/SVGColors.gperf"
      {"mediumpurple", 0x9370db},
      {""},
#line 84 "src/gui/XML/SVGColors.gperf"
      {"goldenrod", 0xdaa520},
      {""}, {""}, {""},
#line 70 "src/gui/XML/SVGColors.gperf"
      {"darkturquoise", 0x00ced1},
#line 61 "src/gui/XML/SVGColors.gperf"
      {"darkolivegreen", 0x556b2f},
      {""}, {""}, {""},
#line 37 "src/gui/XML/SVGColors.gperf"
      {"beige", 0xf5f5dc},
#line 148 "src/gui/XML/SVGColors.gperf"
      {"plum", 0xdda0dd},
#line 170 "src/gui/XML/SVGColors.gperf"
      {"thistle", 0xd8bfd8},
#line 50 "src/gui/XML/SVGColors.gperf"
      {"cornsilk", 0xfff8dc},
#line 121 "src/gui/XML/SVGColors.gperf"
      {"mediumorchid", 0xba55d3},
#line 135 "src/gui/XML/SVGColors.gperf"
      {"olive", 0x808000},
#line 152 "src/gui/XML/SVGColors.gperf"
      {"rosybrown", 0xbc8f8f},
#line 124 "src/gui/XML/SVGColors.gperf"
      {"mediumslateblue", 0x7b68ee},
      {""}, {""}, {""},
#line 40 "src/gui/XML/SVGColors.gperf"
      {"blanchedalmond", 0xffebcd},
      {""}, {""}, {""},
#line 143 "src/gui/XML/SVGColors.gperf"
      {"palevioletred", 0xdb7093},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 125 "src/gui/XML/SVGColors.gperf"
      {"mediumspringgreen", 0x00fa9a},
#line 58 "src/gui/XML/SVGColors.gperf"
      {"darkgrey", 0xa9a9a9},
      {""},
#line 126 "src/gui/XML/SVGColors.gperf"
      {"mediumturquoise", 0x48d1cc},
      {""},
#line 32 "src/gui/XML/SVGColors.gperf"
      {"aliceblue", 0xf0f8ff},
#line 56 "src/gui/XML/SVGColors.gperf"
      {"darkgray", 0xa9a9a9},
      {""},
#line 144 "src/gui/XML/SVGColors.gperf"
      {"papayawhip", 0xffefd5},
      {""}, {""},
#line 55 "src/gui/XML/SVGColors.gperf"
      {"darkgoldenrod", 0xb8860b},
      {""}, {""}, {""},
#line 75 "src/gui/XML/SVGColors.gperf"
      {"dimgrey", 0x696969},
      {""}, {""}, {""}, {""},
#line 74 "src/gui/XML/SVGColors.gperf"
      {"dimgray", 0x696969},
      {""}, {""}, {""}, {""},
#line 136 "src/gui/XML/SVGColors.gperf"
      {"olivedrab", 0x6b8e23},
      {""}, {""},
#line 49 "src/gui/XML/SVGColors.gperf"
      {"cornflower", 0x6495ed},
#line 119 "src/gui/XML/SVGColors.gperf"
      {"mediumaquamarine", 0x66cdaa},
#line 59 "src/gui/XML/SVGColors.gperf"
      {"darkkhaki", 0xbdb76b},
      {""}, {""}, {""}, {""}, {""},
#line 89 "src/gui/XML/SVGColors.gperf"
      {"honeydew", 0xf0fff0},
      {""},
#line 42 "src/gui/XML/SVGColors.gperf"
      {"blueviolet", 0x8a2be2},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 142 "src/gui/XML/SVGColors.gperf"
      {"paleturquoise", 0xafeeee},
      {""}, {""},
#line 173 "src/gui/XML/SVGColors.gperf"
      {"violet", 0xee82ee},
#line 81 "src/gui/XML/SVGColors.gperf"
      {"gainsboro", 0xdcdcdc},
      {""}, {""}, {""},
#line 72 "src/gui/XML/SVGColors.gperf"
      {"deeppink", 0xff1493},
      {""}, {""}, {""},
#line 149 "src/gui/XML/SVGColors.gperf"
      {"powderblue", 0xb0e0e6},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 69 "src/gui/XML/SVGColors.gperf"
      {"darkslategrey", 0x2f4f4f},
      {""},
#line 51 "src/gui/XML/SVGColors.gperf"
      {"crimson", 0xdc143c},
      {""}, {""},
#line 68 "src/gui/XML/SVGColors.gperf"
      {"darkslategray", 0x2f4f4f},
      {""},
#line 82 "src/gui/XML/SVGColors.gperf"
      {"ghostwhite", 0xf8f8ff},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 33 "src/gui/XML/SVGColors.gperf"
      {"antiquewhite", 0xfaebd7},
      {""}, {""},
#line 127 "src/gui/XML/SVGColors.gperf"
      {"mediumvioletred", 0xc71585},
      {""}, {""},
#line 140 "src/gui/XML/SVGColors.gperf"
      {"palegoldenrod", 0xeee8aa},
      {""}, {""}, {""}, {""},
#line 54 "src/gui/XML/SVGColors.gperf"
      {"darkcyan", 0x008b8b},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 175 "src/gui/XML/SVGColors.gperf"
      {"white", 0xffffff},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 132 "src/gui/XML/SVGColors.gperf"
      {"navajowhite", 0xffdead},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 44 "src/gui/XML/SVGColors.gperf"
      {"burlywood", 0xdeb887},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 78 "src/gui/XML/SVGColors.gperf"
      {"floralwhite", 0xfffaf0},
      {""}, {""}, {""},
#line 109 "src/gui/XML/SVGColors.gperf"
      {"lightskyblue", 0x87cefa},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 90 "src/gui/XML/SVGColors.gperf"
      {"hotpink", 0xff69b4},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 177 "src/gui/XML/SVGColors.gperf"
      {"yellow", 0xffff00},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 178 "src/gui/XML/SVGColors.gperf"
      {"yellowgreen", 0x9acd32},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 73 "src/gui/XML/SVGColors.gperf"
      {"deepskyblue", 0x00bfff},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 176 "src/gui/XML/SVGColors.gperf"
      {"whitesmoke", 0xf5f5f5}
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
#line 179 "src/gui/XML/SVGColors.gperf"



int
SVG_Utils::djn__SVGParseColor (unsigned *r, unsigned *g, unsigned *b, const char* v)
{
  while (*v == ' ' ||  *v == '\t' || *v == '\n' || *v == '\r')
    v++;
  if (*v == '#') {
    char* endptr;
    unsigned rgb = strtoul (v+1, &endptr, 16);
    if(endptr-(v+1)==6) {
       *r = (rgb & 0xff0000) >> 16;
       *g = (rgb & 0x00ff00) >> 8;
       *b = (rgb & 0x0000ff);
    } else if (endptr-(v+1)==3) {
       *r = (rgb & 0xf00) >> 4;
       *g = (rgb & 0x0f0) >> 0;
       *b = (rgb & 0x00f) << 4;
    } else {
      fprintf (stderr, "invalid color %s\n", v);
      return 0;
    }
    return 1;

  } else if (strncmp (v, "rgb(", 4) == 0) {
    const char* vv = v+4;
   *r = *g = *b = 0;
    /* skip white space */
    while (*vv == ' ' ||  *vv == '\t' || *vv == '\n' || *vv == '\r')
      vv++;
   /* read three numbers separated by commas */
    while (*vv != ',') {
      if (!isdigit (*vv))
        goto invalid;
      *r = 10 * (*r) + *vv++ - '0';
    }
    ++vv;
    /* skip white space */
    while (*vv == ' ' ||  *vv == '\t' || *vv == '\n' || *vv == '\r')
    vv++;
    while (*vv != ',') {
      if (!isdigit (*vv))
        goto invalid;
      *g = 10 * (*g) + *vv++ - '0';
    }
    ++vv;
    while (*vv == ' ' ||  *vv == '\t' || *vv == '\n' || *vv == '\r')
      vv++;
    while (*vv != ')' && *vv != ' ') {
      if (!isdigit (*vv))
        goto invalid;
      *b = 10 * (*b) + *vv++ - '0';
    }
    return 1;

invalid:
    fprintf (stderr, "invalid color %s\n", v);
    return 0;

  } else {
    struct djn_NamedColor* c = SVGColors_Hash::djn_SVGColorsLookup (v, strlen (v));
    if (c) {
      *r = (c->rgb & 0xff0000) >> 16;
      *g = (c->rgb & 0x00ff00) >> 8;
      *b = (c->rgb & 0x0000ff);
      return 1;
    }
  }
  return 0;
}

int
SVG_Utils::djn__SVGParseAlpha (double *alpha, const char *v)
{
  char *end;

  *alpha = strtod (v, &end);

  if (v == end)
    return 0;

  return 1;
}

