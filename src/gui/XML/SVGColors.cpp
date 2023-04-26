/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "core/utils/algorithm.h"

#include "gui/gui-dev.h"
#include "gui/gui-xml.h"


namespace djnn {

static const named_color_t not_found = 0x000001;

// circumvent memory sanitizer false positive with a pointer rather than a static plain object
// https://github.com/google/sanitizers/wiki/MemorySanitizerLibcxxHowTo
static djnnstl::map<djnnstl::string, named_color_t>* color_map = nullptr;

void
SVG_Utils::init_named_colors ()
{
  // prevent several calls to this function
  if (color_map) return;

  color_map = new djnnstl::map<djnnstl::string, named_color_t>;

  const djnnstl::map<djnnstl::string, named_color_t> color_map_ = {
    {"black",0x000000},
    {"navy",0x000080},
    {"darkblue",0x00008B},
    {"mediumblue",0x0000CD},
    {"blue",0x0000FF},
    {"darkgreen",0x006400},
    {"green",0x008000},
    {"teal",0x008080},
    {"darkcyan",0x008B8B},
    {"deepskyblue",0x00BFFF},
    {"darkturquoise",0x00CED1},
    {"mediumspringgreen",0x00FA9A},
    {"lime",0x00FF00},
    {"springgreen",0x00FF7F},
    {"aqua",0x00FFFF},
    {"cyan",0x00FFFF},
    {"midnightblue",0x191970},
    {"dodgerblue",0x1E90FF},
    {"lightseagreen",0x20B2AA},
    {"forestgreen",0x228B22},
    {"seagreen",0x2E8B57},
    {"darkslategray",0x2F4F4F},
    {"darkslategrey",0x2F4F4F},
    {"limegreen",0x32CD32},
    {"mediumseagreen",0x3CB371},
    {"turquoise",0x40E0D0},
    {"royalblue",0x4169E1},
    {"steelblue",0x4682B4},
    {"darkslateblue",0x483D8B},
    {"mediumturquoise",0x48D1CC},
    {"indigo",0x4B0082},
    {"darkolivegreen",0x556B2F},
    {"cadetblue",0x5F9EA0},
    {"cornflowerblue",0x6495ED},
    {"rebeccapurple",0x663399},
    {"mediumaquamarine",0x66CDAA},
    {"dimgray",0x696969},
    {"dimgrey",0x696969},
    {"slateblue",0x6A5ACD},
    {"olivedrab",0x6B8E23},
    {"slategray",0x708090},
    {"slategrey",0x708090},
    {"lightslategray",0x778899},
    {"lightslategrey",0x778899},
    {"mediumslateblue",0x7B68EE},
    {"lawngreen",0x7CFC00},
    {"chartreuse",0x7FFF00},
    {"aquamarine",0x7FFFD4},
    {"maroon",0x800000},
    {"purple",0x800080},
    {"olive",0x808000},
    {"gray",0x808080},
    {"grey",0x808080},
    {"skyblue",0x87CEEB},
    {"lightskyblue",0x87CEFA},
    {"blueviolet",0x8A2BE2},
    {"darkred",0x8B0000},
    {"darkmagenta",0x8B008B},
    {"saddlebrown",0x8B4513},
    {"darkseagreen",0x8FBC8F},
    {"lightgreen",0x90EE90},
    {"mediumpurple",0x9370DB},
    {"darkviolet",0x9400D3},
    {"palegreen",0x98FB98},
    {"darkorchid",0x9932CC},
    {"yellowgreen",0x9ACD32},
    {"sienna",0xA0522D},
    {"brown",0xA52A2A},
    {"darkgray",0xA9A9A9},
    {"darkgrey",0xA9A9A9},
    {"lightblue",0xADD8E6},
    {"greenyellow",0xADFF2F},
    {"paleturquoise",0xAFEEEE},
    {"lightsteelblue",0xB0C4DE},
    {"powderblue",0xB0E0E6},
    {"firebrick",0xB22222},
    {"darkgoldenrod",0xB8860B},
    {"mediumorchid",0xBA55D3},
    {"rosybrown",0xBC8F8F},
    {"darkkhaki",0xBDB76B},
    {"silver",0xC0C0C0},
    {"mediumvioletred",0xC71585},
    {"indianred",0xCD5C5C},
    {"peru",0xCD853F},
    {"chocolate",0xD2691E},
    {"tan",0xD2B48C},
    {"lightgray",0xD3D3D3},
    {"lightgrey",0xD3D3D3},
    {"thistle",0xD8BFD8},
    {"orchid",0xDA70D6},
    {"goldenrod",0xDAA520},
    {"palevioletred",0xDB7093},
    {"crimson",0xDC143C},
    {"gainsboro",0xDCDCDC},
    {"plum",0xDDA0DD},
    {"burlywood",0xDEB887},
    {"lightcyan",0xE0FFFF},
    {"lavender",0xE6E6FA},
    {"darksalmon",0xE9967A},
    {"violet",0xEE82EE},
    {"palegoldenrod",0xEEE8AA},
    {"lightcoral",0xF08080},
    {"khaki",0xF0E68C},
    {"aliceblue",0xF0F8FF},
    {"honeydew",0xF0FFF0},
    {"azure",0xF0FFFF},
    {"sandybrown",0xF4A460},
    {"wheat",0xF5DEB3},
    {"beige",0xF5F5DC},
    {"whitesmoke",0xF5F5F5},
    {"mintcream",0xF5FFFA},
    {"ghostwhite",0xF8F8FF},
    {"salmon",0xFA8072},
    {"antiquewhite",0xFAEBD7},
    {"linen",0xFAF0E6},
    {"lightgoldenrodyellow",0xFAFAD2},
    {"oldlace",0xFDF5E6},
    {"red",0xFF0000},
    {"fuchsia",0xFF00FF},
    {"magenta",0xFF00FF},
    {"deeppink",0xFF1493},
    {"orangered",0xFF4500},
    {"tomato",0xFF6347},
    {"hotpink",0xFF69B4},
    {"coral",0xFF7F50},
    {"darkorange",0xFF8C00},
    {"lightsalmon",0xFFA07A},
    {"orange",0xFFA500},
    {"lightpink",0xFFB6C1},
    {"pink",0xFFC0CB},
    {"gold",0xFFD700},
    {"peachpuff",0xFFDAB9},
    {"navajowhite",0xFFDEAD},
    {"moccasin",0xFFE4B5},
    {"bisque",0xFFE4C4},
    {"mistyrose",0xFFE4E1},
    {"blanchedalmond",0xFFEBCD},
    {"papayawhip",0xFFEFD5},
    {"lavenderblush",0xFFF0F5},
    {"seashell",0xFFF5EE},
    {"cornsilk",0xFFF8DC},
    {"lemonchiffon",0xFFFACD},
    {"floralwhite",0xFFFAF0},
    {"snow",0xFFFAFA},
    {"yellow",0xFFFF00},
    {"lightyellow",0xFFFFE0},
    {"ivory",0xFFFFF0},
    {"white",0xFFFFFF}
  };
  *color_map = std::move(color_map_);
}

void
SVG_Utils::clear_named_colors ()
{
  delete color_map;
  color_map = nullptr;
}

named_color_t
SVG_Utils::djn__get_color_from_name (const djnnstl::string& name)
{
  auto it = color_map->find (name);
  if (it != color_map->end())
    return it->second;
  else
    return not_found;
}

named_color_t
SVGColors_Hash::djn_SVGColorsLookup (const char *str, unsigned int len)
{
  djnnstl::string key (len, ' ');
  djnnstl::transform (str, str+len, key.begin(),
      [](unsigned char c){ return tolower(c); });
  auto it = color_map->find(key);
  if (it != color_map->end())
    return it->second;
  else
    return not_found;
}

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
    named_color_t c = SVGColors_Hash::djn_SVGColorsLookup (v, strlen (v));
    if (c != not_found) {
      *r = (c & 0xff0000) >> 16;
      *g = (c & 0x00ff00) >> 8;
      *b = (c & 0x0000ff);
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

}
