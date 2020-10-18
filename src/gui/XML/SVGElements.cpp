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

#include <numeric>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <map>
#include "core/core.h"
#include "core/xml/xml-dev.h"
#include "gui/gui-dev.h"
#include "gui/shapes/abstract_gshape.h"
#include "gui/css-parser/driver.h"

using namespace std;

namespace djnn {
	/* all the specific tag handling procedures defined in this file */

static FatProcess* StartSVG (const char**, FatProcess*);
static FatProcess* EndElement (FatProcess*);
static FatProcess* StartRect (const char**, FatProcess*);
static FatProcess* StartImage (const char**, FatProcess*);
static FatProcess* StartEllipse (const char**, FatProcess*);
static FatProcess* StartCircle (const char**, FatProcess*);
static FatProcess* StartLine (const char**, FatProcess*);
static FatProcess* StartPolygon (const char**, FatProcess*);
static FatProcess* StartPolyline (const char**, FatProcess*);
static FatProcess* StartText (const char**, FatProcess*);
static FatProcess* StartStyle (const char**, FatProcess*);
static FatProcess* StyleData (const char*, int, FatProcess*);
static FatProcess* EndText (FatProcess*);
static FatProcess* TextData (const char*, int, FatProcess*);
static FatProcess* StartTspan (const char**, FatProcess*);
static FatProcess* EndTspan (FatProcess*);
static FatProcess* StartPath (const char**, FatProcess*);
static FatProcess* StartDefs (const char**, FatProcess*);
static FatProcess* StartGroup (const char**, FatProcess*);
static FatProcess* StartUse (const char**, FatProcess*);
static FatProcess* StartLinearGradient (const char**, FatProcess*);
static FatProcess* StartRadialGradient (const char**, FatProcess*);
static FatProcess* StartGradientStop (const char**, FatProcess*);
static FatProcess* EndGradient (FatProcess*);
static FatProcess* StartPathClip (const char**, FatProcess*);
static FatProcess* EndPathClip (FatProcess*);
static FatProcess* StartTmp (const char**, FatProcess*);
static FatProcess* EndTmp (FatProcess*);
static FatProcess* StartIgnored (const char**, FatProcess*);
static FatProcess* EndIgnored (FatProcess*);
static FatProcess* DataIgnored (const char*, int, FatProcess*);
static void djnUnloadTextBuf (FatProcess *);
static void djn__CheckStroke (FatProcess *);

static std::map <std::string, djn_XMLTagHandler> handlers={
  {"g",{&StartGroup, &EndElement, &DataIgnored}},
  {"use",{&StartUse, &EndElement, &DataIgnored}},
  {"text",{&StartText, &EndText, &TextData}},
  {"pattern",{&StartTmp, &EndTmp, &DataIgnored}},
  {"textPath",{&StartTmp, &EndTmp, &DataIgnored}},
  {"rect",{&StartRect, &EndElement, &DataIgnored}},
  {"image",{&StartImage, &EndElement, &DataIgnored}},
  {"ellipse",{&StartEllipse, &EndElement, &DataIgnored}},
  {"clipPath",{&StartPathClip, &EndPathClip, &DataIgnored}},
  {"path",{&StartPath, &EndElement, &DataIgnored}},
  {"circle",{&StartCircle, &EndElement, &DataIgnored}},
  {"desc", {&StartIgnored, &EndIgnored, &DataIgnored}},
  {"svg",{&StartSVG, &EndElement, &DataIgnored}},
  {"stop",{&StartGradientStop, &EndElement, &DataIgnored}},
  {"polygon",{&StartPolygon, &EndElement, &DataIgnored}},
  {"polyline",{&StartPolyline, &EndElement, &DataIgnored}},
  {"radialGradient",{&StartRadialGradient, &EndGradient, &DataIgnored}},
  {"tspan",{&StartTspan, &EndTspan, &TextData}},
  {"defs",{&StartDefs, &EndElement, &DataIgnored}},
  {"metadata",{&StartTmp, &EndTmp, &DataIgnored}},
  {"line",{&StartLine, &EndElement, &DataIgnored}},
  {"tref",{&StartTmp, &EndTmp, &DataIgnored}},
  {"mask",{&StartTmp, &EndTmp, &DataIgnored}},
  {"marker",{&StartTmp, &EndTmp, &DataIgnored}},
  {"linearGradient",{&StartLinearGradient, &EndGradient, &DataIgnored}},
  {"style",{&StartStyle, &EndElement, &StyleData}},
};

djn_XMLTagHandler*
SVGElements_Hash::djn_SVGElementsLookup (const char *str, unsigned int len)
{
  std::map<std::string, djn_XMLTagHandler>::iterator it;
  it = handlers.find(std::string(str));
  if (it != handlers.end())
    return &it->second;
  return 0;
}

int djn__GrphIsInClip = 0;

/* SVG parser initialisation*/

map<string, FatProcess*> djn__id_to_process;

void init_svg_parser () {
	XML::djn_RegisterXMLParser("http://www.w3.org/2000/svg", &SVGElements_Hash::djn_SVGElementsLookup,
			"SVG");

	/* fix locale numeric separator settings, which may have be broken
	 by a call to "new QApplication".u */
	setlocale(LC_NUMERIC, "C");

}

void clear_svg_parser () {
	XML::djn_UnregisterXMLParser("http://www.w3.org/2000/svg");
}

/*
 *  the SVG element start and end handlers
 */

static FatProcess*
StartSVG(const char** attrs, FatProcess* current) {

#ifdef DEBUG
		fprintf (stderr, "startSVG\n");
#endif

	djn__id_to_process.clear ();
	FatProcess* g = new Group(current, "SVG");
	//FatProcess* f;
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
				SVGSvgAttrs_Hash::djn_SVGSvgAttrsLookup, nullptr);
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

static FatProcess*
EndElement(FatProcess* e) {
	return e->get_parent() ? e->get_parent() : e;
}

static void djn__CheckStroke(FatProcess* holder) {
	if (djn_GraphicalShapeArgs.strokeType == djnStrokeNone) {
		/* if it exists, ensure that no-stroke is at the end of the list */
		auto * e = holder->find_child("no-stroke");
		holder->remove_child(e);
		holder->add_child(e, "no-stroke");
	} else if (djn_GraphicalShapeArgs.strokeType == djnStrokeUndef) {
		/* otherwise, create it */
		new NoOutline(holder, "no-stroke");
	}
}

static std::vector<string>
get_classes (const std::string& classnames)
{
  vector<string> tokens;
  string cur_str (classnames);
  std::replace( cur_str.begin(), cur_str.end(), ',', ' ');
  const char* str = cur_str.c_str ();
  do
  {
    const char *begin = str;
    while(!isspace (*str) && *str)
      str++;
    tokens.push_back(string(begin, str));
  } while (0 != *str++);

  return tokens;
}

static FatProcess*
StartRect(const char** attrs, FatProcess* current) {

#ifdef DEBUG
		fprintf (stderr, "startRect\n");
#endif

	FatProcess* holder = 0;
	FatProcess* e;
	djn_GraphicalShapeArgs.strokeType = djnStrokeUndef;
	djn_GraphicalShapeArgs.classname = "";
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
				SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, nullptr);
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
	if (!djn_GraphicalShapeArgs.classname.empty ()) {
	  std::vector <string> classes = get_classes (djn_GraphicalShapeArgs.classname);
	  for (auto cl: classes) {
	    ((AbstractGShape*)e)->add_style_class (cl);
	  }
	}
	if (holder) {
		current->add_child(holder, djn_GraphicalShapeArgs.id);
		((SVGHolder*) holder)->set_gobj (e);
		djn__id_to_process.insert(
		        pair<string, FatProcess*>(djn_GraphicalShapeArgs.id, holder));
	} else {
	  djn__id_to_process.insert(
	              pair<string, FatProcess*>(djn_GraphicalShapeArgs.id, e));
	}
	return holder ? holder : e;
}

static FatProcess*
StartImage (const char** attrs, FatProcess* current)
{
  FatProcess *holder = 0;
  FatProcess* e;
  djn_GraphicalShapeArgs.strokeType = djnStrokeUndef;
  djn_GraphicalShapeArgs.classname = "";
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
			    SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, nullptr);
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
  if (!djn_GraphicalShapeArgs.classname.empty ()) {
    std::vector <string> classes = get_classes (djn_GraphicalShapeArgs.classname);
    for (auto cl: classes) {
      ((AbstractGShape*)e)->add_style_class (cl);
    }
  }
  if (holder) {
    current->add_child (holder, djn_GraphicalShapeArgs.id);
    ((SVGHolder*) holder)->set_gobj (e);
    djn__id_to_process.insert(
        pair<string, FatProcess*>(djn_GraphicalShapeArgs.id, holder));
  } else {
    djn__id_to_process.insert(
        pair<string, FatProcess*>(djn_GraphicalShapeArgs.id, e));
  }
  return holder ? holder : e;
}

static FatProcess*
StartEllipse(const char** attrs, FatProcess* current) {

#ifdef DEBUG
		fprintf (stderr, "StartEllipse\n");
#endif

	FatProcess *holder = 0;
	FatProcess* e;
	djn_GraphicalShapeArgs.strokeType = djnStrokeUndef;
	djn_GraphicalShapeArgs.classname = "";
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
				SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, nullptr);
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
  if (!djn_GraphicalShapeArgs.classname.empty ()) {
    std::vector <string> classes = get_classes (djn_GraphicalShapeArgs.classname);
    for (auto cl: classes) {
      ((AbstractGShape*)e)->add_style_class (cl);
    }
  }
	if (holder) {
    current->add_child(holder, djn_GraphicalShapeArgs.id);
    ((SVGHolder*) holder)->set_gobj (e);
    djn__id_to_process.insert(
        pair<string, FatProcess*>(djn_GraphicalShapeArgs.id, holder));
	} else {
	  djn__id_to_process.insert(
        pair<string, FatProcess*>(djn_GraphicalShapeArgs.id, e));
	}
	return holder ? holder : e;
}

static FatProcess*
StartCircle(const char** attrs, FatProcess* current) {

#ifdef DEBUG
		fprintf (stderr, "StartCircle\n");
#endif
	

	FatProcess *holder = 0;
	FatProcess* e;
	djn_GraphicalShapeArgs.strokeType = djnStrokeUndef;
	djn_GraphicalShapeArgs.classname = "";
	djn_CircleArgs.cx = 0.;
	djn_CircleArgs.cy = 0.;
	djn_CircleArgs.r = 0.;

	/* FIXME: should manage optional, mandatory and duplicate attributes */
	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(&holder, attrs, XMLCircleAttrs_Hash::djn_XMLCircleAttrsLookup,
				SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, nullptr);
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
  if (!djn_GraphicalShapeArgs.classname.empty ()) {
    std::vector <string> classes = get_classes (djn_GraphicalShapeArgs.classname);
    for (auto cl: classes) {
      ((AbstractGShape*)e)->add_style_class (cl);
    }
  }
	if (holder) {
    current->add_child(holder, djn_GraphicalShapeArgs.id);
    ((SVGHolder*) holder)->set_gobj (e);djn__id_to_process.insert(
        pair<string, FatProcess*>(djn_GraphicalShapeArgs.id, holder));
	} else {
	  djn__id_to_process.insert(
	      pair<string, FatProcess*>(djn_GraphicalShapeArgs.id, e));
	}

	return holder ? holder : e;
}

static FatProcess*
StartLine(const char** attrs, FatProcess* current) {

#ifdef DEBUG
		fprintf (stderr, "StartLine\n");
#endif

	FatProcess *holder = 0;
	FatProcess* e;
	djn_GraphicalShapeArgs.strokeType = djnStrokeUndef;
	djn_GraphicalShapeArgs.classname = "";
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
				SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, nullptr);
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
  if (!djn_GraphicalShapeArgs.classname.empty ()) {
    std::vector <string> classes = get_classes (djn_GraphicalShapeArgs.classname);
    for (auto cl: classes) {
      ((AbstractGShape*)e)->add_style_class (cl);
    }
  }
	if (holder) {
    current->add_child(holder, djn_GraphicalShapeArgs.id);
    ((SVGHolder*) holder)->set_gobj (e);
    djn__id_to_process.insert(
        pair<string, FatProcess*>(djn_GraphicalShapeArgs.id, holder));
	} else {
	  djn__id_to_process.insert(
	      pair<string, FatProcess*>(djn_GraphicalShapeArgs.id, e));
	}

	return holder ? holder : e;
}

static FatProcess*
StartPoly(const char** attrs, FatProcess* current) {

#ifdef DEBUG
		fprintf (stderr, "StartPoly\n");
#endif

	FatProcess *holder = 0;
	djn_GraphicalShapeArgs.strokeType = djnStrokeUndef;
	djn_GraphicalShapeArgs.classname = "";
	djn_PolyArgs.e = 0;

	/* FIXME: should manage optional, mandatory and duplicate attributes */
	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(&holder, attrs, XMLPolylineAttrs_Hash::djn_XMLPolylineAttrsLookup,
				SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, nullptr);
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
	  if (!djn_GraphicalShapeArgs.classname.empty ()) {
	    std::vector <string> classes = get_classes (djn_GraphicalShapeArgs.classname);
	    for (auto cl: classes) {
	      ((AbstractGShape*)djn_PolyArgs.e)->add_style_class (cl);
	    }
	  }
	}
	if (holder) {
    ((SVGHolder*) holder)->set_gobj (djn_PolyArgs.e);
    current->add_child(holder, djn_GraphicalShapeArgs.id);
    djn__id_to_process.insert(
        pair<string, FatProcess*>(djn_GraphicalShapeArgs.id, holder));
	} else {
	  djn__id_to_process.insert(
	      pair<string, FatProcess*>(djn_GraphicalShapeArgs.id, djn_PolyArgs.e));
	}

	return holder ? holder : djn_PolyArgs.e ? djn_PolyArgs.e : current;
}

static FatProcess*
StartPolygon(const char** attrs, FatProcess* current) {

#ifdef DEBUG
		fprintf (stderr, "StartPolygon\n");
#endif

	djn_PolyArgs.isPolygon = 1;
	return StartPoly(attrs, current);
}

static FatProcess*
StartPolyline(const char** attrs, FatProcess* current) {

#ifdef DEBUG
		fprintf (stderr, "StartPolyline\n");
#endif

	djn_PolyArgs.isPolygon = 0;
	return StartPoly(attrs, current);
}

static FatProcess*
StartText(const char** attrs, FatProcess* current) {

#ifdef DEBUG
		fprintf (stderr, "StartText\n");
#endif

	/* it seems that we have to force the creation of a holder for the text
	 * without this, and for a not yet elucidated reason,
	 * the graphical context is not correctly managed
	 */
	FatProcess *holder = new SVGHolder (nullptr, "SVGHolder");
	djn_GraphicalShapeArgs.strokeType = djnStrokeUndef;
	djn_GraphicalShapeArgs.classname = "";
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
				SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, nullptr);
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

static FatProcess*
EndText(FatProcess *e) {
	if (djn_TextArgs.data != 0) {
		djnUnloadTextBuf(e);
	}
	return e->get_parent() ? e->get_parent() : e;
}

static FatProcess*
TextData(const char* data, int len, FatProcess* current) {
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

static FatProcess *
StartTspan(const char** attrs, FatProcess* current) {

#ifdef DEBUG
		fprintf (stderr, "StartTspan\n");
#endif

	FatProcess *holder;

	if (djn_TextArgs.data != 0) {
		djnUnloadTextBuf(current);
	} else {
		djn_TextArgs.data = 0;
		djn_TextArgs.dxUnit = DJN_NO_UNIT;
		djn_TextArgs.dyUnit = DJN_NO_UNIT;
	}

	holder = new SVGHolder (nullptr, "SVGHolder");

	/* FIXME: should manage optional, mandatory and duplicate attributes */
	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(&holder, attrs, XMLTextAttrs_Hash::djn_XMLTextAttrsLookup,
				SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, nullptr);
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

static FatProcess *
EndTspan(FatProcess *e) {
	if (djn_TextArgs.data != 0) {
		djnUnloadTextBuf(e);
	}
	return e->get_parent() ? e->get_parent() : e;
}

static FatProcess*
StartPath(const char** attrs, FatProcess* current) {

#ifdef DEBUG
		fprintf (stderr, "StartPath\n");
#endif

	FatProcess *holder = 0;
	djn_GraphicalShapeArgs.strokeType = djnStrokeUndef;
	djn_GraphicalShapeArgs.classname = "";
	djn_PathArgs.e = 0;

	/* FIXME: should manage optional, mandatory and duplicate attributes */
	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(&holder, attrs, XMLPathAttrs_Hash::djn_XMLPathAttrsLookup,
				SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, nullptr);
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
    if (!djn_GraphicalShapeArgs.classname.empty ()) {
      std::vector <string> classes = get_classes (djn_GraphicalShapeArgs.classname);
      for (auto cl: classes) {
        ((AbstractGShape*)djn_PathArgs.e)->add_style_class (cl);
      }
    }
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

static FatProcess*
StartGroup(const char** attrs, FatProcess* current) {

#ifdef DEBUG
		fprintf (stderr, "StartGroup\n");
#endif

	FatProcess* e = new Group(0, "");

	/* FIXME: should manage optional, mandatory and duplicate attributes */
	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(&e, attrs, SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, nullptr);
#ifdef DEBUG
		if (!ret)
		fprintf (stderr, "unknown attribute '%s' in group element\n", *attrs);
#endif
		attrs++;
		attrs++;
	}
	current->add_child(e, djn_GraphicalShapeArgs.id);
	djn__id_to_process.insert(
	    pair<string, FatProcess*>(djn_GraphicalShapeArgs.id, e));
	return e;
}

static FatProcess*
StartDefs(const char** attrs, FatProcess* current) {

#ifdef DEBUG
    fprintf (stderr, "StartDefs\n");
#endif

  FatProcess* e = new Defs(0, "");

  /* FIXME: should manage optional, mandatory and duplicate attributes */
  while (*attrs) {
#ifdef DEBUG
    int ret =
#endif
    XML::djn_XMLHandleAttr(&e, attrs, SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, nullptr);
#ifdef DEBUG
    if (!ret)
    fprintf (stderr, "unknown attribute '%s' in group element\n", *attrs);
#endif
    attrs++;
    attrs++;
  }
  current->add_child(e, djn_GraphicalShapeArgs.id);
  djn__id_to_process.insert(
      pair<string, FatProcess*>(djn_GraphicalShapeArgs.id, e));
  return e;
}

static FatProcess*
StartUse(const char** attrs, FatProcess* current) {

#ifdef DEBUG
    fprintf (stderr, "StartUse\n");
#endif

  djn_UseArgs.height = 0;
  djn_UseArgs.width = 0;
  djn_UseArgs.x = 0;
  djn_UseArgs.y = 0;
  djn_UseArgs.href = "";
  djn_GraphicalShapeArgs.id = "";

  FatProcess *holder = new SVGHolder (nullptr, "");

  /* FIXME: should manage optional, mandatory and duplicate attributes */
  while (*attrs) {
#ifdef DEBUG
    int ret =
#endif
    XML::djn_XMLHandleAttr(&holder, attrs, SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup,
        XMLUseAttrs_Hash::djn_XMLUseAttrsLookup, nullptr);
#ifdef DEBUG
    if (!ret)
    fprintf (stderr, "unknown attribute '%s' in group element\n", *attrs);
#endif
    attrs++;
    attrs++;
  }
  Translation* pos = new Translation (holder, "", djn_UseArgs.x, djn_UseArgs.y);
  alias (holder, "x", pos->find_child_impl ("tx"));
  alias (holder, "y", pos->find_child_impl ("ty"));
  auto it = djn__id_to_process.find (djn_UseArgs.href);
  if (it != djn__id_to_process.end ()) {
    FatProcess* clone = it->second->clone();
    if (clone->get_process_type() == DEFS_T) {
      for (auto c : ((Defs*)clone)->children ()) {
        holder->add_child (c, c->get_name (clone));
      }
      delete clone;
    } else {
      holder->add_child (clone, "");
      ((SVGHolder*)holder)->set_gobj (clone);
    }
  }
  current->add_child (holder, djn_GraphicalShapeArgs.id);
  return holder;
}

static int parseGradientTransform(FatProcess **e, char *v) {
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

static FatProcess*
StartLinearGradient(const char** attrs, FatProcess* current) {

#ifdef DEBUG
		fprintf (stderr, "StartLinearGradient\n");
#endif

	FatProcess* e;
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
				SVGLinearGradientAttrs_Hash::djn_SVGLinearGradientAttrsLookup, nullptr);
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
		djn__id_to_process.insert(
				pair<string, FatProcess*>(djn_GradientArgs.id, e));
#ifdef DEBUG
		fprintf (stderr, "new entry added: %s\n", djn_GradientArgs.href);
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
	return (FatProcess*) ((AbstractGradient*) e)->stops();
}

static FatProcess*
StartRadialGradient(const char** attrs, FatProcess* current) {

#ifdef DEBUG
		fprintf (stderr, "StartRadialGradient\n");
#endif

	FatProcess* e;

	djn_RadialGradientArgs.cx = 0.5;
	djn_RadialGradientArgs.cy = 0.5;
	djn_RadialGradientArgs.r = 0.5;
	djn_RadialGradientArgs.fx = std::numeric_limits<double>::infinity();
	djn_RadialGradientArgs.fy = std::numeric_limits<double>::infinity();
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
				SVGRadialGradientAttrs_Hash::djn_SVGRadialGradientAttrsLookup, nullptr);
#ifdef DEBUG
		if (!ret)
		fprintf (stderr, "unknown attribute '%s' in radialGradient element\n", *attrs);
#endif
		attrs++;
		attrs++;
	}

	if (djn_RadialGradientArgs.fx == std::numeric_limits<double>::infinity())
	  djn_RadialGradientArgs.fx = djn_RadialGradientArgs.cx;
  if (djn_RadialGradientArgs.fy == std::numeric_limits<double>::infinity())
    djn_RadialGradientArgs.fy = djn_RadialGradientArgs.cy;
	e = new RadialGradient(0, djn_GradientArgs.id, djn_RadialGradientArgs.cx,
			djn_RadialGradientArgs.cy, djn_RadialGradientArgs.r,
			djn_RadialGradientArgs.fx, djn_RadialGradientArgs.fy,
			djn_GradientArgs.spread, djn_GradientArgs.coords);

	/* hack to fool EndElement */
	e->set_parent(current);

	if (djn_GradientArgs.id != 0) {
		djn__id_to_process.insert(
				pair<string, FatProcess*>(djn_GradientArgs.id, e));
#ifdef DEBUG
		fprintf (stderr, "new entry added: %s\n", djn_GradientArgs.href);
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
	return (FatProcess*) ((AbstractGradient*) e)->stops();
}

static FatProcess*
StartGradientStop(const char** attrs, FatProcess* current) {

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
				SVGGradientStopAttrs_Hash::djn_SVGGradientStopAttrsLookup, nullptr);
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

static FatProcess*
EndGradient(FatProcess* e) {
	/* careful: e is actually the list of stops, not the gradient itself
	 and e may be freed in djnMergeChildren */
	FatProcess *gradient = e->get_parent();

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

static FatProcess*
StartPathClip(const char** attrs, FatProcess* current) {

#ifdef DEBUG
		fprintf (stderr, "StartPathClip\n");
#endif

	FatProcess *holder = 0;
	FatProcess* e;
	djn__GrphIsInClip = 1;

	/* FIXME: should manage optional, mandatory and duplicate attributes */
	while (*attrs) {
#ifdef DEBUG
		int ret =
#endif
		XML::djn_XMLHandleAttr(&holder, attrs, SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, nullptr);
#ifdef DEBUG
		if (!ret)
		fprintf (stderr, "unknown attribute '%s' in pathClip element\n", *attrs);
#endif
		attrs++;
		attrs++;
	}
	e = new Component (nullptr, "pathclip");
	if (strlen (djn_GraphicalShapeArgs.id) != 0) {
		djn__id_to_process.insert(
				pair<string, FatProcess*>(djn_GraphicalShapeArgs.id, e));
#ifdef DEBUG
		fprintf (stderr, "new entry added: %s\n", djn_GraphicalShapeArgs.href);
#endif
	}
	e->set_parent(current);
	return e;
}

static FatProcess*
EndPathClip(FatProcess* e) {
	djn__GrphIsInClip = 0;
	return e->get_parent() ? e->get_parent() : e;
}

static FatProcess*
StartTmp(const char** attrs, FatProcess* current) {

#ifdef DEBUG
		fprintf (stderr, "StartTmp\n");
#endif

	return current;
}

static FatProcess*
EndTmp(FatProcess* e) {
	return e;
}

static FatProcess*
StartIgnored(const char** attrs, FatProcess* current) {

#ifdef DEBUG
		fprintf (stderr, "StartIgnored\n");
#endif

	return current;
}

static FatProcess*
EndIgnored(FatProcess* e) {
	return e;
}

static FatProcess*
DataIgnored(const char* data, int len, FatProcess* current) {
	return current;
}

static void djnUnloadTextBuf(FatProcess *e) {
	FatProcess *t = new Text(e, djn_GraphicalShapeArgs.id,
			djn_TextArgs.x, djn_TextArgs.y, djn_TextArgs.dx, djn_TextArgs.dy,
			djn_TextArgs.dxUnit, djn_TextArgs.dyUnit, djn_TextArgs.encoding,
			djn_TextArgs.data);
  if (!djn_GraphicalShapeArgs.classname.empty ()) {
    std::vector <string> classes = get_classes (djn_GraphicalShapeArgs.classname);
    for (auto cl: classes) {
      ((AbstractGShape*)t)->add_style_class (cl);
    }
  }
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

static FatProcess*
StartStyle(const char** attrs, FatProcess* current) {

#ifdef DEBUG
    fprintf (stderr, "StartStyle\n");
#endif
    djn_GraphicalShapeArgs.id = "";
    FatProcess* e = new Component (0, "");

    /* FIXME: should manage optional, mandatory and duplicate attributes */
    while (*attrs) {
  #ifdef DEBUG
      int ret =
  #endif
      XML::djn_XMLHandleAttr(&e, attrs, SVGShapeAttrs_Hash::djn_SVGShapeAttrsLookup, nullptr);
  #ifdef DEBUG
      if (!ret)
      fprintf (stderr, "unknown attribute '%s' in group element\n", *attrs);
  #endif
      attrs++;
      attrs++;
    }
    current->add_child(e, djn_GraphicalShapeArgs.id);
    djn__id_to_process.insert(
        pair<string, FatProcess*>(djn_GraphicalShapeArgs.id, e));
    return e;
}

static FatProcess*
StyleData(const char* data, int len, FatProcess* current) {
  std::string str (data, len);
  css::Driver driver;
  driver.parse_string (str, "svg file", current);
  return current;
}
}
