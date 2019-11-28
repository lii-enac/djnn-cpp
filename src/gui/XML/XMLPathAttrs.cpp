/* C++ code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf -L C++ -t -N djn_XMLPathAttrsLookup -Z XMLPathAttrs_Hash src/gui/XML/XMLPathAttrs.gperf  */
/* Computed positions: -k'' */

#line 16 "src/gui/XML/XMLPathAttrs.gperf"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "core/core.h"
#include "gui/gui-dev.h"

#define register

	using namespace djnn;

	typedef void (djn_PathPointProc) (Process*, double, double);
	typedef void (djn_PathCoordProc) (Process*, double);

	static int ParseData (Process**, const char*);
#line 33 "src/gui/XML/XMLPathAttrs.gperf"

#define TOTAL_KEYWORDS 1
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 1
#define MIN_HASH_VALUE 1
#define MAX_HASH_VALUE 1
/* maximum key range = 1, duplicates = 0 */

inline /*ARGSUSED*/
unsigned int
XMLPathAttrs_Hash::hash (register const char *str, register unsigned int len)
{
  return len;
}

djn_XMLAttrHandler *
XMLPathAttrs_Hash::djn_XMLPathAttrsLookup (register const char *str, register unsigned int len)
{
  static djn_XMLAttrHandler wordlist[] =
    {
      {""},
#line 36 "src/gui/XML/XMLPathAttrs.gperf"
      {"d", &ParseData}
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
#line 37 "src/gui/XML/XMLPathAttrs.gperf"


struct djn_PathArgs djn_PathArgs = {0};

static const char*
ParseCoords(const char* v, int num, double* coord, int *numout) {
	char* p;
	int i = 0;

	/* read a list of coordinate pairs */
	while (*v && i < num) {

		/* skip comma and separating white space */
		while (*v == ',' || *v == ' ' || *v == '\t' || *v == '\n' || *v == '\r')
			++v;

		/* try and read coord */
		*coord = strtod(v, &p);
		if (p == v)
			break;

		v = p;
		++i;
		++coord;
	}

	*numout = i;
	return v;
}

static int ParseData(Process** e, const char* v) {
	double initx = 0., inity = 0.;
	double curx = 0., cury = 0.;
	double ctrlx = 0., ctrly = 0.;
	double coords[7];
	int rel;
	int num;
	int firstPt = 1;
	char prevItem = ' ', curItem = ' ';

	if (djn__GrphIsInClip)
		djn_PathArgs.e = new PathClip(nullptr, "");
	else
		djn_PathArgs.e = new Path(nullptr, "");

	/* read a list of commands and their parameters */
	while (*v) {

		/* skip leading white space */
		while (*v == ' ' || *v == '\t' || *v == '\n' || *v == '\r')
			++v;

		/* try and read command */
		rel = 0;
		curItem = *v;

		switch (*v++) {

		case 'm':
			rel = 1;
		case 'M':
			v = ParseCoords(v, 2, coords, &num);
			if (num != 2)
				goto error;
			curx = rel ? curx + coords[0] : coords[0];
			cury = rel ? cury + coords[1] : coords[1];
			new PathMove (djn_PathArgs.e, "", curx, cury);
			if (firstPt) {
				firstPt = 0;
				initx = curx;
				inity = cury;
			}
			while (num == 2) {
				v = ParseCoords(v, 2, coords, &num);
				if (num == 2) {
					curx = rel ? curx + coords[0] : coords[0];
					cury = rel ? cury + coords[1] : coords[1];
					new PathLine(djn_PathArgs.e, "", curx, cury);
				}
			}
			if (num != 0)
				goto error;
			break;

		case 'l':
			rel = 1;
		case 'L':
			do {
				v = ParseCoords(v, 2, coords, &num);
				if (num != 2)
					break;
				curx = rel ? curx + coords[0] : coords[0];
				cury = rel ? cury + coords[1] : coords[1];
				new PathLine(djn_PathArgs.e, "", curx, cury);
			} while (num == 2);
			if (num != 0)
				goto error;
			break;

		case 'v':
			rel = 1;
		case 'V':
			do {
				v = ParseCoords(v, 1, coords, &num);
				if (num != 1)
					break;
				cury = rel ? cury + coords[0] : coords[0];
				new PathLine(djn_PathArgs.e, "", curx, cury);
			} while (num == 1);
			break;

		case 'h':
			rel = 1;
		case 'H':
			do {
				v = ParseCoords(v, 1, coords, &num);
				if (num != 1)
					break;
				curx = rel ? curx + coords[0] : coords[0];
				new PathLine(djn_PathArgs.e, "", curx, cury);
			} while (num == 1);
			break;

		case 'c':
			rel = 1;
		case 'C':
			do {
				v = ParseCoords(v, 6, coords, &num);
				if (num != 6)
					break;
				if (rel) {
					int i = 0;
					while (i < 6) {
						coords[i++] += curx;
						coords[i++] += cury;
					}
				}
				curx = coords[4];
				cury = coords[5];
				new PathCubic(djn_PathArgs.e, "", coords[0], coords[1],
						coords[2], coords[3], coords[4], coords[5]);
				ctrlx = coords[2];
				ctrly = coords[3];
			} while (num == 6);
			if (num != 0)
				goto error;
			break;

		case 's':
			rel = 1;
		case 'S':
			do {
				v = ParseCoords(v, 4, coords, &num);
				if (num != 4)
					break;
				if (prevItem == 'c' || prevItem == 'C' || prevItem == 's'
						|| prevItem == 'S') {
					ctrlx = 2 * curx - ctrlx;
					ctrly = 2 * cury - ctrly;
				} else {
					ctrlx = curx;
					ctrly = cury;
				}
				if (rel) {
					int i = 0;
					while (i < 4) {
						coords[i++] += curx;
						coords[i++] += cury;
					}
				}
				curx = coords[2];
				cury = coords[3];
				new PathCubic(djn_PathArgs.e, "", ctrlx, ctrly,
						coords[0], coords[1], coords[2], coords[3]);
				ctrlx = coords[0];
				ctrly = coords[1];
			} while (num == 4);
			if (num != 0)
				goto error;
			break;

		case 'q':
			rel = 1;
		case 'Q':
			do {
				v = ParseCoords(v, 4, coords, &num);
				if (num != 4)
					break;
				if (rel) {
					int i = 0;
					while (i < 4) {
						coords[i++] += curx;
						coords[i++] += cury;
					}
				}
				curx = coords[2];
				cury = coords[3];
				ctrlx = coords[0];
				ctrly = coords[1];
				new PathQuadratic(djn_PathArgs.e, "", coords[0],
						coords[1], coords[2], coords[3]);
			} while (num == 4);
			if (num != 0)
				goto error;
			break;

		case 't':
			rel = 1;
		case 'T':
			do {
				v = ParseCoords(v, 2, coords, &num);
				if (num != 2)
					break;
				if (prevItem == 'q' || prevItem == 'Q' || prevItem == 't'
						|| prevItem == 'T') {
					ctrlx = 2 * curx - ctrlx;
					ctrly = 2 * cury - ctrly;
				} else {
					ctrlx = curx;
					ctrly = cury;
				}
				curx = rel ? curx + coords[0] : coords[0];
				cury = rel ? cury + coords[1] : coords[1];
				new PathQuadratic(djn_PathArgs.e, "", ctrlx, ctrly, curx,
						cury);
			} while (num == 2);
			if (num != 0)
				goto error;
			break;

		case 'a':
			rel = 1;
		case 'A':
			do {
				v = ParseCoords(v, 7, coords, &num);
				if (num != 7)
					break;
				if (rel) {
					coords[5] += curx;
					coords[6] += cury;
				}
				curx = coords[5];
				cury = coords[6];
				new PathArc(djn_PathArgs.e, "", coords[0], coords[1],
						coords[2], coords[3], coords[4], coords[5], coords[6]);
			} while (num == 7);
			if (num != 0)
				goto error;
			break;

		case 'z':
		case 'Z':
			new PathClosure(djn_PathArgs.e, "");
			curx = initx;
			cury = inity;
			firstPt = 1;
			while (*v == ' ' || *v == '\t' || *v == '\n' || *v == '\r')
				++v;
			break;

		default:
			fprintf(stderr, "SVG parser: unknown command '%c' in path\n",
					*(v - 1));
		}
		prevItem = curItem;
	}
	return 1;

	error: fprintf(stderr, "SVG parser: error in path coordinates\n");
	return 0;
}

