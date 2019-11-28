/* C++ code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf -L C++ -t -N djn_XMLPolylineAttrsLookup -Z XMLPolylineAttrs_Hash src/gui/XML/XMLPolylineAttrs.gperf  */
/* Computed positions: -k'' */

#line 16 "src/gui/XML/XMLPolylineAttrs.gperf"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "core/core.h"
#include "gui/gui-dev.h"

#define register

	using namespace std;
	static int ParsePoints (Process**, const char*);
#line 29 "src/gui/XML/XMLPolylineAttrs.gperf"

#define TOTAL_KEYWORDS 1
#define MIN_WORD_LENGTH 6
#define MAX_WORD_LENGTH 6
#define MIN_HASH_VALUE 6
#define MAX_HASH_VALUE 6
/* maximum key range = 1, duplicates = 0 */

inline /*ARGSUSED*/
unsigned int
XMLPolylineAttrs_Hash::hash (register const char *str, register unsigned int len)
{
  return len;
}

djn_XMLAttrHandler *
XMLPolylineAttrs_Hash::djn_XMLPolylineAttrsLookup (register const char *str, register unsigned int len)
{
  static djn_XMLAttrHandler wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""},
#line 32 "src/gui/XML/XMLPolylineAttrs.gperf"
      {"points", &ParsePoints}
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
#line 33 "src/gui/XML/XMLPolylineAttrs.gperf"


struct djn_PolyArgs djn_PolyArgs = {0, 0};

static int ParsePoints(Process** e, const char* v) {
	char* p;
	double x, y;
	char *vv = (char*) v;

	if (djn_PolyArgs.isPolygon) djn_PolyArgs.e = new djnn::Polygon;
	else djn_PolyArgs.e = new djnn::Polyline;

	/* read a list of coordinate pairs */
	while (*vv) {

		/* skip leading white space */
		while (*vv == ' ' || *vv == '\t' || *vv == '\n' || *vv == '\r')
			++vv;

		/* try and read X */
		x = strtod(vv, &p);
		if (p == vv)
			goto error;
		vv = p;

		/* skip comma and separating white space */
		while (*vv == ' ' || *vv == '\t' || *vv == '\n' || *vv == '\r')
			++vv;
		if (*vv == ',')
			++vv;
		//else
		//	goto error;
		while (*vv == ' ' || *vv == '\t' || *vv == '\n' || *vv == '\r')
			++vv;

		/* try and read Y */
		y = strtod(vv, &p);
		if (p == vv)
			goto error;
		vv = p;

		/* we have a point, add it */
		new PolyPoint(djn_PolyArgs.e, "", x, y);

		/* remove trailing spaces an comma */

		if (!XML_Utils::djn_XMLRemoveSpacesAndComma(&vv))
			goto error;
	}
	return 1;

	error: 
	fprintf(stderr, "SVG parser: error in polyline or polygon coordinates\n");
	return 0;
}

