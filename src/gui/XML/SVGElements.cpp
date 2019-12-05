/* C++ code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf -L C++ -t -N djn_SVGElementsLookup -Z SVGElements_Hash src/gui/XML/SVGElements.gperf  */
/* Computed positions: -k'1,4' */

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

//#define DEBUG

#line 18 "src/gui/XML/SVGElements.gperf"

#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <map>
#include "core/core.h"
#include "core/xml/xml-dev.h"
#include "gui/gui-dev.h"
#include "gui/shapes/abstract_gshape.h"

#define register 

	using namespace djnn;

	/* all the specific tag handling procedures defined in this file */
	static Process* StartSVG (const char**, Process*);
	static Process* EndElement (Process*);
	static Process* StartRect (const char**, Process*);
	static Process* StartImage (const char**, Process*);
	static Process* StartEllipse (const char**, Process*);
	static Process* StartCircle (const char**, Process*);
	static Process* StartLine (const char**, Process*);
	static Process* StartPolygon (const char**, Process*);
	static Process* StartPolyline (const char**, Process*);
	static Process* StartText (const char**, Process*);
	static Process* EndText (Process*);
	static Process* TextData (const char*, int, Process*);
	static Process* StartTspan (const char**, Process*);
	static Process* EndTspan (Process*);
	static Process* StartPath (const char**, Process*);
	static Process* StartGroup (const char**, Process*);
	static Process* StartLinearGradient (const char**, Process*);
	static Process* StartRadialGradient (const char**, Process*);
	static Process* StartGradientStop (const char**, Process*);
	static Process* EndGradient (Process*);
	static Process* StartPathClip (const char**, Process*);
	static Process* EndPathClip (Process*);
	static Process* StartTmp (const char**, Process*);
	static Process* EndTmp (Process*);
	static Process* StartIgnored (const char**, Process*);
	static Process* EndIgnored (Process*);
	static Process* DataIgnored (const char*, int, Process*);
	static void djnUnloadTextBuf (Process *);
	static void djn__CheckStroke (Process *);

	/* the attribute name lookup procedures defined in other source files */
#line 67 "src/gui/XML/SVGElements.gperf"

#define TOTAL_KEYWORDS 25
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 14
#define MIN_HASH_VALUE 1
#define MAX_HASH_VALUE 39
/* maximum key range = 39, duplicates = 0 */

inline unsigned int
SVGElements_Hash::hash (register const char *str, register unsigned int len)
{
  static unsigned char asso_values[] =
    {
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 20, 40,  5,
       8,  0, 28,  0, 10,  5, 40, 30, 25,  0,
      40, 40,  0, 40,  5, 15,  0,  0, 40, 40,
      40, 15, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

djn_XMLTagHandler *
SVGElements_Hash::djn_SVGElementsLookup (register const char *str, register unsigned int len)
{
  static djn_XMLTagHandler wordlist[] =
    {
      {""},
#line 81 "src/gui/XML/SVGElements.gperf"
      {"g", &StartGroup, &EndElement, &DataIgnored},
      {""},
#line 73 "src/gui/XML/SVGElements.gperf"
      {"use", &StartTmp, &EndTmp, &DataIgnored},
#line 90 "src/gui/XML/SVGElements.gperf"
      {"text", &StartText, &EndText, &TextData},
      {""}, {""},
#line 77 "src/gui/XML/SVGElements.gperf"
      {"pattern", &StartTmp, &EndTmp, &DataIgnored},
#line 91 "src/gui/XML/SVGElements.gperf"
      {"textPath", &StartTmp, &EndTmp, &DataIgnored},
#line 83 "src/gui/XML/SVGElements.gperf"
      {"rect", &StartRect, &EndElement, &DataIgnored},
#line 89 "src/gui/XML/SVGElements.gperf"
      {"image", &StartImage, &EndElement, &DataIgnored},
      {""},
#line 82 "src/gui/XML/SVGElements.gperf"
      {"ellipse", &StartEllipse, &EndElement, &DataIgnored},
#line 79 "src/gui/XML/SVGElements.gperf"
      {"clipPath", &StartPathClip, &EndPathClip, &DataIgnored},
#line 88 "src/gui/XML/SVGElements.gperf"
      {"path", &StartPath, &EndElement, &DataIgnored},
      {""},
#line 84 "src/gui/XML/SVGElements.gperf"
      {"circle", &StartCircle, &EndElement, &DataIgnored},
#line 71 "src/gui/XML/SVGElements.gperf"
      {"desc", &StartIgnored, &EndIgnored, &DataIgnored},
#line 70 "src/gui/XML/SVGElements.gperf"
      {"svg", &StartSVG, &EndElement, &DataIgnored},
#line 76 "src/gui/XML/SVGElements.gperf"
      {"stop", &StartGradientStop, &EndElement, &DataIgnored},
      {""}, {""},
#line 87 "src/gui/XML/SVGElements.gperf"
      {"polygon", &StartPolygon, &EndElement, &DataIgnored},
#line 86 "src/gui/XML/SVGElements.gperf"
      {"polyline", &StartPolyline, &EndElement, &DataIgnored},
#line 75 "src/gui/XML/SVGElements.gperf"
      {"radialGradient", &StartRadialGradient, &EndGradient, &DataIgnored},
#line 92 "src/gui/XML/SVGElements.gperf"
      {"tspan", &StartTspan, &EndTspan, &TextData},
      {""},
#line 72 "src/gui/XML/SVGElements.gperf"
      {"defs", &StartTmp, &EndTmp, &DataIgnored},
#line 94 "src/gui/XML/SVGElements.gperf"
      {"metadata", &StartTmp, &EndTmp, &DataIgnored},
#line 85 "src/gui/XML/SVGElements.gperf"
      {"line", &StartLine, &EndElement, &DataIgnored},
      {""}, {""},
#line 93 "src/gui/XML/SVGElements.gperf"
      {"tref", &StartTmp, &EndTmp, &DataIgnored},
      {""},
#line 80 "src/gui/XML/SVGElements.gperf"
      {"mask", &StartTmp, &EndTmp, &DataIgnored},
      {""},
#line 78 "src/gui/XML/SVGElements.gperf"
      {"marker", &StartTmp, &EndTmp, &DataIgnored},
      {""}, {""},
#line 74 "src/gui/XML/SVGElements.gperf"
      {"linearGradient", &StartLinearGradient, &EndGradient, &DataIgnored}
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
#line 95 "src/gui/XML/SVGElements.gperf"


int djn__GrphIsInClip = 0;

/* SVG parser initialisation*/

map<string, Process*> djn__IdFillManager;
map<string, Process*> djn__IdClipManager;

void djnn::init_svg_parser () {
	XML::djn_RegisterXMLParser("http://www.w3.org/2000/svg", &SVGElements_Hash::djn_SVGElementsLookup,
			"SVG");

	/* fix locale numeric separator settings, which may have be broken
	 by a call to "new QApplication".u */
	setlocale(LC_NUMERIC, "C");

}

void djnn::clear_svg_parser () {
	XML::djn_UnregisterXMLParser("http://www.w3.org/2000/svg");
}

/*
 *  the SVG element start and end handlers
 */

static Process*
StartSVG(const char** attrs, Process* current) {

#ifdef DEBUG
		fprintf (stderr, "startSVG\n");
#endif

	djn__IdFillManager.clear ();
	djn__IdClipManager.clear ();
	Process* g = new Group(current, "SVG");
	//Process* f;
	djn_RectAreaArgs.x = 0.;
	djn_RectAreaArgs.y = 0.;
	djn_RectAreaArgs.width = 0.;
	djn_RectAreaArgs.height = 0.;

	/* FIXME: should manage optional, mandatory and duplicate attributes */
	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(&g, attrs, XMLRectAreaAttrs_Hash::djn_XMLRectAreaAttrsLookup,
				SVGSvgAttrs_Hash::djn_SVGSvgAttrsLookup, 0);
#ifdef DEBUG
		if (!ret)
		fprintf (stderr, "unknown attribute '%s' in svg element\n", *attrs);
#endif
		attrs++;
		attrs++;
	}
/*
	f = new Window(g, "", "", (int) djn_RectAreaArgs.x, (int) djn_RectAreaArgs.y,
			(int) djn_RectAreaArgs.width, (int) djn_RectAreaArgs.height);
	new FillColor(g, "", 0, 0, 0);
	new NoOutline(g, "");
*/
	return g;
}

static Process*
EndElement(Process* e) {
	return e->get_parent() ? e->get_parent() : e;
}

static void djn__CheckStroke(Process* holder) {
	if (djn_GraphicalShapeArgs.strokeType == djnStrokeNone) {
		/* if it exists, ensure that no-stroke is at the end of the list */
		Process* e = holder->find_component("no-stroke");
		holder->remove_child(e);
		holder->add_child(e, "no-stroke");
	} else if (djn_GraphicalShapeArgs.strokeType == djnStrokeUndef) {
		/* otherwise, create it */
		new NoOutline(holder, "no-stroke");
	}
}

static Process*
StartRect(const char** attrs, Process* current) {

#ifdef DEBUG
		fprintf (stderr, "startRect\n");
#endif

	Process* holder = 0;
	Process* e;
	djn_GraphicalShapeArgs.strokeType = djnStrokeUndef;
	djn_RectArgs.x = 0.;
	djn_RectArgs.y = 0.;
	djn_RectArgs.w = 0.;
	djn_RectArgs.h = 0.;
	djn_RectArgs.rx = -1.;
	djn_RectArgs.ry = -1.;

	/* FIXME: should manage optional, mandatory and duplicate attributes */
	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(&holder, attrs, XMLRectAttrs_Hash::djn_XMLRectAttrsLookup,
				SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, 0);
#ifdef DEBUG
		if (!ret)
		fprintf (stderr, "unknown attribute '%s' in rect element\n", *attrs);
#endif
		attrs++;
		attrs++;
	}

	if (djn_RectArgs.rx == -1.) {
		djn_RectArgs.rx = 0.;
		djn_RectArgs.ry = 0.;
	}

	if (djn__GrphIsInClip)
		return new RectangleClip(current, djn_GraphicalShapeArgs.id,
				djn_RectArgs.x, djn_RectArgs.y, djn_RectArgs.w, djn_RectArgs.h);

	if (holder)
		djn__CheckStroke(holder);

	e = new djnn::Rectangle(holder ? holder : current, djn_GraphicalShapeArgs.id,
			djn_RectArgs.x, djn_RectArgs.y, djn_RectArgs.w, djn_RectArgs.h,
			djn_RectArgs.rx, djn_RectArgs.ry);
	if (holder) {
		current->add_child(holder, djn_GraphicalShapeArgs.id);
		((SVGHolder*) holder)->set_gobj (e);
	}
	return holder ? holder : e;
}

static Process*
StartImage (const char** attrs, Process* current)
{
  Process *holder = 0;
  Process* e;
  djn_GraphicalShapeArgs.strokeType = djnStrokeUndef;
  djn_ImgArgs.x = 0.;
  djn_ImgArgs.y = 0.;
  djn_ImgArgs.w = 0.;
  djn_ImgArgs.h = 0.;
  djn_ImgArgs.path = 0;
  djn_ImgArgs.data = std::string ("");

  /* FIXME: should manage optional, mandatory and duplicate attributes */
  while (*attrs) {
#ifdef DEBUG
    int ret =
#endif
    XML::djn_XMLHandleAttr (&holder, attrs, XMLImgAttrs_Hash::djn_XMLImgAttrsLookup,
			    SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, 0);
#ifdef DEBUG
    if (!ret)
    fprintf (stderr, "unknown attribute '%s' in img element\n", *attrs);
#endif
    attrs++;
    attrs++;
  }

  if (holder)
    djn__CheckStroke (holder);
  if (djn_ImgArgs.data.empty ()) {
    e = new Image (holder ? holder : current, djn_GraphicalShapeArgs.id, djn_ImgArgs.path, djn_ImgArgs.x, djn_ImgArgs.y,
		   djn_ImgArgs.w, djn_ImgArgs.h);
  } else {
    e = new DataImage (holder ? holder : current, djn_GraphicalShapeArgs.id, djn_ImgArgs.data, djn_ImgArgs.x,
		       djn_ImgArgs.y, djn_ImgArgs.w, djn_ImgArgs.h);
  }
  if (holder) {
    current->add_child (holder, djn_GraphicalShapeArgs.id);
    ((SVGHolder*) holder)->set_gobj (e);
  }
  return holder ? holder : e;
}

static Process*
StartEllipse(const char** attrs, Process* current) {

#ifdef DEBUG
		fprintf (stderr, "StartEllipse\n");
#endif

	Process *holder = 0;
	Process* e;
	djn_GraphicalShapeArgs.strokeType = djnStrokeUndef;
	djn_EllipseArgs.cx = 0.;
	djn_EllipseArgs.cy = 0.;
	djn_EllipseArgs.rx = 0.;
	djn_EllipseArgs.ry = 0.;

	/* FIXME: should manage optional, mandatory and duplicate attributes */
	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(&holder, attrs, XMLEllipseAttrs_Hash::djn_XMLEllipseAttrsLookup,
				SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, 0);
#ifdef DEBUG
		if (!ret)
		fprintf (stderr, "unknown attribute '%s' in ellipse element\n", *attrs);
#endif
		attrs++;
		attrs++;
	}

	if (holder)
		djn__CheckStroke(holder);

	e = new djnn::Ellipse(holder ? holder : current, djn_GraphicalShapeArgs.id,
			djn_EllipseArgs.cx, djn_EllipseArgs.cy, djn_EllipseArgs.rx,
			djn_EllipseArgs.ry);
	if (holder) {
		current->add_child(holder, djn_GraphicalShapeArgs.id);
		((SVGHolder*) holder)->set_gobj (e);
	}
	return holder ? holder : e;
}

static Process*
StartCircle(const char** attrs, Process* current) {

#ifdef DEBUG
		fprintf (stderr, "StartCircle\n");
#endif
	

	Process *holder = 0;
	Process* e;
	djn_GraphicalShapeArgs.strokeType = djnStrokeUndef;
	djn_CircleArgs.cx = 0.;
	djn_CircleArgs.cy = 0.;
	djn_CircleArgs.r = 0.;

	/* FIXME: should manage optional, mandatory and duplicate attributes */
	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(&holder, attrs, XMLCircleAttrs_Hash::djn_XMLCircleAttrsLookup,
				SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, 0);
#ifdef DEBUG
		if (!ret)
		fprintf (stderr, "unknown attribute '%s' in circle element\n", *attrs);
#endif
		attrs++;
		attrs++;
	}

	if (holder)
		djn__CheckStroke(holder);

	e = new Circle(holder ? holder : current, djn_GraphicalShapeArgs.id,
			djn_CircleArgs.cx, djn_CircleArgs.cy, djn_CircleArgs.r);

	if (holder) {
		current->add_child(holder, djn_GraphicalShapeArgs.id);
		((SVGHolder*) holder)->set_gobj (e);
	}

	return holder ? holder : e;
}

static Process*
StartLine(const char** attrs, Process* current) {

#ifdef DEBUG
		fprintf (stderr, "StartLine\n");
#endif

	Process *holder = 0;
	Process* e;
	djn_GraphicalShapeArgs.strokeType = djnStrokeUndef;
	djn_LineArgs.x1 = 0.;
	djn_LineArgs.y1 = 0.;
	djn_LineArgs.x2 = 0.;
	djn_LineArgs.y2 = 0.;

	/* FIXME: should manage optional, mandatory and duplicate attributes */
	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(&holder, attrs, XMLLineAttrs_Hash::djn_XMLLineAttrsLookup,
				SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, 0);
#ifdef DEBUG
		if (!ret)
		fprintf (stderr, "unknown attribute '%s' in line element\n", *attrs);
#endif
		attrs++;
		attrs++;
	}

	if (holder)
		djn__CheckStroke(holder);

	e = new Line(holder ? holder : current, djn_GraphicalShapeArgs.id,
			djn_LineArgs.x1, djn_LineArgs.y1, djn_LineArgs.x2, djn_LineArgs.y2);

	if (holder) {
		current->add_child(holder, djn_GraphicalShapeArgs.id);
		((SVGHolder*) holder)->set_gobj (e);
	}

	return holder ? holder : e;
}

static Process*
StartPoly(const char** attrs, Process* current) {

#ifdef DEBUG
		fprintf (stderr, "StartPoly\n");
#endif

	Process *holder = 0;
	djn_GraphicalShapeArgs.strokeType = djnStrokeUndef;
	djn_PolyArgs.e = 0;

	/* FIXME: should manage optional, mandatory and duplicate attributes */
	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(&holder, attrs, XMLPolylineAttrs_Hash::djn_XMLPolylineAttrsLookup,
				SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, 0);
#ifdef DEBUG
		if (!ret)
		fprintf (stderr, "unknown attribute '%s' in poly%s element\n",
				*attrs, djn_PolyArgs.isPolygon ? "gon" : "line");
#endif
		attrs++;
		attrs++;
	}

	if (holder)
		djn__CheckStroke(holder);

	if (djn_PolyArgs.e) {
		if (holder)
			holder->add_child(djn_PolyArgs.e, djn_GraphicalShapeArgs.id);
		else
			current->add_child(djn_PolyArgs.e, djn_GraphicalShapeArgs.id);
	}
	if (holder) {
		((SVGHolder*) holder)->set_gobj (djn_PolyArgs.e);
		current->add_child(holder, djn_GraphicalShapeArgs.id);
	}

	return holder ? holder : djn_PolyArgs.e ? djn_PolyArgs.e : current;
}

static Process*
StartPolygon(const char** attrs, Process* current) {

#ifdef DEBUG
		fprintf (stderr, "StartPolygon\n");
#endif

	djn_PolyArgs.isPolygon = 1;
	return StartPoly(attrs, current);
}

static Process*
StartPolyline(const char** attrs, Process* current) {

#ifdef DEBUG
		fprintf (stderr, "StartPolyline\n");
#endif

	djn_PolyArgs.isPolygon = 0;
	return StartPoly(attrs, current);
}

static Process*
StartText(const char** attrs, Process* current) {

#ifdef DEBUG
		fprintf (stderr, "StartText\n");
#endif

	/* it seems that we have to force the creation of a holder for the text
	 * without this, and for a not yet elucidated reason,
	 * the graphical context is not correctly managed
	 */
	Process *holder = new SVGHolder;
	djn_GraphicalShapeArgs.strokeType = djnStrokeUndef;
	djn_TextArgs.x = 0.;
	djn_TextArgs.y = 0.;
	djn_TextArgs.encoding = "UTF-8";
	djn_TextArgs.data = 0;
	djn_TextArgs.dx = 0;
	djn_TextArgs.dy = 0;
	djn_TextArgs.dxUnit = DJN_NO_UNIT;
	djn_TextArgs.dyUnit = DJN_NO_UNIT;

	/* FIXME: should manage optional, mandatory and duplicate attributes */
	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(&holder, attrs, XMLTextAttrs_Hash::djn_XMLTextAttrsLookup,
				SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, 0);
#ifdef DEBUG
		if (!ret)
		fprintf (stderr, "unknown attribute '%s' in text element\n", *attrs);
#endif
		attrs++;
		attrs++;
	}

	djn__CheckStroke(holder);
	current->add_child(holder, djn_GraphicalShapeArgs.id);
	return holder ? holder : current;
}

static Process*
EndText(Process *e) {
	if (djn_TextArgs.data != 0) {
		djnUnloadTextBuf(e);
	}
	return e->get_parent() ? e->get_parent() : e;
}

static Process*
TextData(const char* data, int len, Process* current) {
#if 0
	if (!djnXMLHandleData (current, djn_DataFormat (data, len), XMLTextAttrs_Hash::djn_XMLTextAttrsLookup))
	fprintf (stderr, "unknown attribute _data_ in text element\n");
#endif
	djn_XMLAttrHandler* h = XMLTextAttrs_Hash::djn_XMLTextAttrsLookup("_data_", 6);
	if (h) {
		char* content = (char*) malloc(len + 1);
		strncpy(content, data, len);
		content[len] = '\0';
		h->handle_attr(&current, content);
		if (content)
			free(content);
	}
	return current;
}

static Process *
StartTspan(const char** attrs, Process* current) {

#ifdef DEBUG
		fprintf (stderr, "StartTspan\n");
#endif

	Process *holder;

	if (djn_TextArgs.data != 0) {
		djnUnloadTextBuf(current);
	} else {
		djn_TextArgs.data = 0;
		djn_TextArgs.dxUnit = DJN_NO_UNIT;
		djn_TextArgs.dyUnit = DJN_NO_UNIT;
	}

	holder = new SVGHolder;

	/* FIXME: should manage optional, mandatory and duplicate attributes */
	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(&holder, attrs, XMLTextAttrs_Hash::djn_XMLTextAttrsLookup,
				SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, 0);
#ifdef DEBUG
		if (!ret)
		fprintf (stderr, "unknown attribute '%s' in tspan element\n", *attrs);
#endif
		attrs++;
		attrs++;
	}

	current->add_child(holder, djn_GraphicalShapeArgs.id);
	return holder ? holder : current;
}

static Process *
EndTspan(Process *e) {
	if (djn_TextArgs.data != 0) {
		djnUnloadTextBuf(e);
	}
	return e->get_parent() ? e->get_parent() : e;
}

static Process*
StartPath(const char** attrs, Process* current) {

#ifdef DEBUG
		fprintf (stderr, "StartPath\n");
#endif

	Process *holder = 0;
	djn_GraphicalShapeArgs.strokeType = djnStrokeUndef;
	djn_PathArgs.e = 0;

	/* FIXME: should manage optional, mandatory and duplicate attributes */
	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(&holder, attrs, XMLPathAttrs_Hash::djn_XMLPathAttrsLookup,
				SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, 0);
#ifdef DEBUG
		if (!ret)
		fprintf (stderr, "unknown attribute '%s' in path element\n",
				*attrs);
#endif
		attrs++;
		attrs++;
	}

	if (djn__GrphIsInClip) {
		if (djn_PathArgs.e) {
			current->add_child(djn_PathArgs.e, djn_GraphicalShapeArgs.id);
			return djn_PathArgs.e;
		} else
			return current;
	}

	if (holder)
		djn__CheckStroke(holder);

	if (djn_PathArgs.e) {
		if (holder)
			holder->add_child(djn_PathArgs.e, djn_GraphicalShapeArgs.id);
		else
			current->add_child(djn_PathArgs.e, djn_GraphicalShapeArgs.id);
	}

	if (holder) {
		current->add_child (holder, djn_GraphicalShapeArgs.id);
		((SVGHolder*) holder)->set_gobj (djn_PathArgs.e);
	}
	return holder ? holder : djn_PathArgs.e ? djn_PathArgs.e : current;
}

static Process*
StartGroup(const char** attrs, Process* current) {

#ifdef DEBUG
		fprintf (stderr, "StartGroup\n");
#endif

	Process* e = new Group(0, "");

	/* FIXME: should manage optional, mandatory and duplicate attributes */
	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(&e, attrs, SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup,
				XMLTextAttrs_Hash::djn_XMLTextAttrsLookup, 0);
#ifdef DEBUG
		if (!ret)
		fprintf (stderr, "unknown attribute '%s' in group element\n", *attrs);
#endif
		attrs++;
		attrs++;
	}
	current->add_child(e, djn_GraphicalShapeArgs.id);
	return e;
}

static int parseGradientTransform(Process **e, char *v) {
	int success = 1;
	djn_XMLAttrHandler *h;
	char *openparenthesis, *closeparenthesis;
	const char* end = v + strlen(v);

	if (!*e)
		return 0;

	while (v < end) {
		openparenthesis = strchr(v, '(');
		if (!openparenthesis)
			return 0;
		*openparenthesis = '\0';
		closeparenthesis = strchr(openparenthesis + 1, ')');

		h = SVGGradientTransformAttrs_Hash::djn_SVGGradientTransformAttrsLookup(v, openparenthesis - v);
//#ifdef DEBUG
		int ret =
//#endif
		(h && (h->handle_attr(e, openparenthesis + 1)));
		success &= ret;
#ifdef DEBUG
		if (!ret)
		fprintf (stderr, "unknown function '%s' in gradient transform attribute\n", v);
#endif
		v = closeparenthesis + 1;
		XML_Utils::djn_XMLRemoveSpacesAndComma(&v);
	}
	return success;
}

static Process*
StartLinearGradient(const char** attrs, Process* current) {

#ifdef DEBUG
		fprintf (stderr, "StartLinearGradient\n");
#endif

	Process* e;
	djn_LinearGradientArgs.x1 = 0.;
	djn_LinearGradientArgs.y1 = 0.;
	djn_LinearGradientArgs.x2 = 1.;
	djn_LinearGradientArgs.y2 = 0.;
	djn_GradientArgs.id = 0;
	djn_GradientArgs.transform = 0;
	djn_GradientArgs.spread = DJN_PAD_FILL;
	djn_GradientArgs.coords = DJN_LOCAL_COORDS;
	djn_GradientArgs.inherited = 1 << djn_GradientCoords
			| 1 << djn_GradientSpread | 1 << djn_GradientStopPoints
			| 1 << djn_GradientX1 | 1 << djn_GradientY1 | 1 << djn_GradientX2
			| 1 << djn_GradientY2;

	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(0, attrs, SVGGradientAttrs_Hash::djn_SVGGradientAttrsLookup,
				SVGLinearGradientAttrs_Hash::djn_SVGLinearGradientAttrsLookup, 0);
#ifdef DEBUG
		if (!ret)
		fprintf (stderr, "unknown attribute '%s' in linearGradient element\n", *attrs);
#endif
		attrs++;
		attrs++;
	}

	e = new LinearGradient(0, djn_GradientArgs.id, djn_LinearGradientArgs.x1,
			djn_LinearGradientArgs.y1, djn_LinearGradientArgs.x2,
			djn_LinearGradientArgs.y2, djn_GradientArgs.spread,
			djn_GradientArgs.coords);

	/* hack to fool EndElement */
	e->set_parent(current);

	if (djn_GradientArgs.id != 0) {
		djn__IdFillManager.insert(
				pair<string, Process*>(djn_GradientArgs.id, e));
#ifdef DEBUG
		fprintf (stderr, "new entry added: %s\n", djn_GradientArgs.id);
#endif
	}

	if (djn_GradientArgs.transform != 0) {
		int sz = strlen (djn_GradientArgs.transform);
		char * tmp = (char *) malloc (sz + 1);
		strcpy (tmp, djn_GradientArgs.transform);
		tmp[sz] = '\0';
		parseGradientTransform(&e, tmp);
		free (tmp);
	}
	/* all contained elements are to be stored in the list of stops,
	 not in the gradient itself */
	return (Process*) ((AbstractGradient*) e)->stops();
}

static Process*
StartRadialGradient(const char** attrs, Process* current) {

#ifdef DEBUG
		fprintf (stderr, "StartRadialGradient\n");
#endif

	Process* e;

	djn_RadialGradientArgs.cx = 0.;
	djn_RadialGradientArgs.cy = 0.;
	djn_RadialGradientArgs.r = 1.;
	djn_RadialGradientArgs.fx = 0.;
	djn_RadialGradientArgs.fy = 0.;
	djn_GradientArgs.id = 0;
	djn_GradientArgs.transform = 0;
	djn_GradientArgs.spread = DJN_PAD_FILL;
	djn_GradientArgs.coords = DJN_LOCAL_COORDS;
	djn_GradientArgs.inherited = 1 << djn_GradientCoords
			| 1 << djn_GradientSpread | 1 << djn_GradientStopPoints
			| 1 << djn_GradientCx | 1 << djn_GradientCy | 1 << djn_GradientR
			| 1 << djn_GradientFx | 1 << djn_GradientFy;

	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(0, attrs, SVGGradientAttrs_Hash::djn_SVGGradientAttrsLookup,
				SVGRadialGradientAttrs_Hash::djn_SVGRadialGradientAttrsLookup, 0);
#ifdef DEBUG
		if (!ret)
		fprintf (stderr, "unknown attribute '%s' in radialGradient element\n", *attrs);
#endif
		attrs++;
		attrs++;
	}

	e = new RadialGradient(0, djn_GradientArgs.id, djn_RadialGradientArgs.cx,
			djn_RadialGradientArgs.cy, djn_RadialGradientArgs.r,
			djn_RadialGradientArgs.fx, djn_RadialGradientArgs.fy,
			djn_GradientArgs.spread, djn_GradientArgs.coords);

	/* hack to fool EndElement */
	e->set_parent(current);

	if (djn_GradientArgs.id != 0) {
		djn__IdFillManager.insert(
				pair<string, Process*>(djn_GradientArgs.id, e));
#ifdef DEBUG
		fprintf (stderr, "new entry added: %s\n", djn_GradientArgs.id);
#endif
	}

	if (djn_GradientArgs.transform != 0) {
		int sz = strlen (djn_GradientArgs.transform);
		char * tmp = (char *) malloc (sz + 1);
		strcpy (tmp, djn_GradientArgs.transform);
		tmp[sz] = '\0';
		parseGradientTransform(&e, tmp);
		free (tmp);
	}
	/* all contained elements are to be stored in the list of stops,
	 not in the gradient itself */
	return (Process*) ((AbstractGradient*) e)->stops();
}

static Process*
StartGradientStop(const char** attrs, Process* current) {

#ifdef DEBUG
		fprintf (stderr, "StartGradientStop\n");
#endif

	djn_GradientStopArgs.r = 0;
	djn_GradientStopArgs.g = 0;
	djn_GradientStopArgs.b = 0;
	djn_GradientStopArgs.alpha = 1.;
	djn_GradientStopArgs.offset = 0;

	djn_GradientArgs.inherited &= ~(1 << djn_GradientStopPoints);

	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(0, attrs, DJNComponentAttrs_Hash::djn_DJNComponentAttrsLookup,
				SVGGradientStopAttrs_Hash::djn_SVGGradientStopAttrsLookup, 0);
#ifdef DEBUG
		if (!ret)
		fprintf (stderr, "unknown attribute '%s' in stop gradient element\n", *attrs);
#endif
		attrs++;
		attrs++;
	}

	return new GradientStop(current->get_parent(), djn_ComponentArgs.id,
			djn_GradientStopArgs.r, djn_GradientStopArgs.g,
			djn_GradientStopArgs.b, djn_GradientStopArgs.alpha,
			djn_GradientStopArgs.offset);
}

static const char* djn_Inherited[] = { "coords", "spread", "stops", "x1", "y1",
		"x2", "y2", "cx", "cy", "r", "fx", "fy" };

static Process*
EndGradient(Process* e) {
	/* careful: e is actually the list of stops, not the gradient itself
	 and e may be freed in djnMergeChildren */
	Process *gradient = e->get_parent();

	if (djn_GradientArgs.ancestor) {
		int i;
		for (i = djn_GradientCoords; i <= djn_GradientFy; ++i) {
			if (djn_GradientArgs.inherited & (1 << i)) {
				merge_children(djn_GradientArgs.ancestor, djn_Inherited[i],
						gradient, djn_Inherited[i]);
			}
		}
		((AbstractGradient*) gradient)->update ();
	}

	return gradient->get_parent();
}

static Process*
StartPathClip(const char** attrs, Process* current) {

#ifdef DEBUG
		fprintf (stderr, "StartPathClip\n");
#endif

	Process *holder = 0;
	Process* e;
	djn__GrphIsInClip = 1;

	/* FIXME: should manage optional, mandatory and duplicate attributes */
	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(&holder, attrs, SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, 0);
#ifdef DEBUG
		if (!ret)
		fprintf (stderr, "unknown attribute '%s' in pathClip element\n", *attrs);
#endif
		attrs++;
		attrs++;
	}
	e = new Component;
	if (strlen (djn_GraphicalShapeArgs.id) != 0) {
		djn__IdClipManager.insert(
				pair<string, Process*>(djn_GraphicalShapeArgs.id, e));
#ifdef DEBUG
		fprintf (stderr, "new entry added: %s\n", djn_GraphicalShapeArgs.id);
#endif
	}
	e->set_parent(current);
	return e;
}

static Process*
EndPathClip(Process* e) {
	djn__GrphIsInClip = 0;
	return e->get_parent() ? e->get_parent() : e;
}

static Process*
StartTmp(const char** attrs, Process* current) {

#ifdef DEBUG
		fprintf (stderr, "StartTmp\n");
#endif

	return current;
}

static Process*
EndTmp(Process* e) {
	return e;
}

static Process*
StartIgnored(const char** attrs, Process* current) {

#ifdef DEBUG
		fprintf (stderr, "StartIgnored\n");
#endif

	return current;
}

static Process*
EndIgnored(Process* e) {
	return e;
}

static Process*
DataIgnored(const char* data, int len, Process* current) {
	return current;
}

static void djnUnloadTextBuf(Process *e) {
	Process *t = new Text(e, djn_GraphicalShapeArgs.id,
			djn_TextArgs.x, djn_TextArgs.y, djn_TextArgs.dx, djn_TextArgs.dy,
			djn_TextArgs.dxUnit, djn_TextArgs.dyUnit, djn_TextArgs.encoding,
			djn_TextArgs.data);
	((SVGHolder*) e)->set_gobj (t);
	free(djn_TextArgs.data);
	djn_TextArgs.x = -1;
	djn_TextArgs.y = -1;
	djn_TextArgs.data = 0;
	djn_TextArgs.dx = 0;
	djn_TextArgs.dy = 0;
	djn_TextArgs.dxUnit = DJN_NO_UNIT;
	djn_TextArgs.dyUnit = DJN_NO_UNIT;
}
