/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "gui.h"
#include "style/style_types.h"
#include "core/core-dev.h"
#include "core/xml/xml-dev.h"

class XMLRectAttrs_Hash
{
private:
  static inline unsigned int
  hash (const char *str, unsigned int len);
public:
  static djn_XMLAttrHandler *
  djn_XMLRectAttrsLookup (const char *str, unsigned int len);
};
class SVGColors_Hash
{
private:
  static inline unsigned int
  hash (const char *str, unsigned int len);
public:
  static struct djn_NamedColor *
  djn_SVGColorsLookup (const char *str, unsigned int len);
};
class SVGElements_Hash
{
private:
  static inline unsigned int
  hash (const char *str, unsigned int len);
public:
  static djn_XMLTagHandler *
  djn_SVGElementsLookup (const char *str, unsigned int len);
};
class SVGGradientAttrs_Hash
{
private:
  static inline unsigned int
  hash (const char *str, unsigned int len);
public:
  static djn_XMLAttrHandler *
  djn_SVGGradientAttrsLookup (const char *str, unsigned int len);
};
class SVGGradientStopAttrs_Hash
{
private:
  static inline unsigned int
  hash (const char *str, unsigned int len);
public:
  static djn_XMLAttrHandler *
  djn_SVGGradientStopAttrsLookup (const char *str, unsigned int len);
};
class SVGGradientTransformAttrs_Hash
{
private:
  static inline unsigned int
  hash (const char *str, unsigned int len);
public:
  static djn_XMLAttrHandler *
  djn_SVGGradientTransformAttrsLookup (const char *str, unsigned int len);
};
class SVGLinearGradientAttrs_Hash
{
private:
  static inline unsigned int
  hash (const char *str, unsigned int len);
public:
  static djn_XMLAttrHandler *
  djn_SVGLinearGradientAttrsLookup (const char *str, unsigned int len);
};
class SVGRadialGradientAttrs_Hash
{
private:
  static inline unsigned int
  hash (const char *str, unsigned int len);
public:
  static djn_XMLAttrHandler *
  djn_SVGRadialGradientAttrsLookup (const char *str, unsigned int len);
};
class SVGShapeAttrs_Hash
{
private:
  static inline unsigned int
  hash (const char *str, unsigned int len);
public:
  static djn_XMLAttrHandler *
  djn_SVGShapeAttrsLookup (const char *str, unsigned int len);
};
class SVGSvgAttrs_Hash
{
private:
  static inline unsigned int
  hash (const char *str, unsigned int len);
public:
  static djn_XMLAttrHandler *
  djn_SVGSvgAttrsLookup (const char *str, unsigned int len);
};
class XMLCircleAttrs_Hash
{
private:
  static inline unsigned int
  hash (const char *str, unsigned int len);
public:
  static djn_XMLAttrHandler *
  djn_XMLCircleAttrsLookup (const char *str, unsigned int len);
};
class XMLEllipseAttrs_Hash
{
private:
  static inline unsigned int
  hash (const char *str, unsigned int len);
public:
  static djn_XMLAttrHandler *
  djn_XMLEllipseAttrsLookup (const char *str, unsigned int len);
};
class XMLImgAttrs_Hash
{
private:
  static inline unsigned int
  hash (const char *str, unsigned int len);
public:
  static djn_XMLAttrHandler *
  djn_XMLImgAttrsLookup (const char *str, unsigned int len);
};
class XMLLineAttrs_Hash
{
private:
  static inline unsigned int
  hash (const char *str, unsigned int len);
public:
  static djn_XMLAttrHandler *
  djn_XMLLineAttrsLookup (const char *str, unsigned int len);
};
class XMLPathAttrs_Hash
{
private:
  static inline unsigned int
  hash (const char *str, unsigned int len);
public:
  static djn_XMLAttrHandler *
  djn_XMLPathAttrsLookup (const char *str, unsigned int len);
};
class XMLPolylineAttrs_Hash
{
private:
  static inline unsigned int
  hash (const char *str, unsigned int len);
public:
  static djn_XMLAttrHandler *
  djn_XMLPolylineAttrsLookup (const char *str, unsigned int len);
};
class XMLRectAreaAttrs_Hash
{
private:
  static inline unsigned int
  hash (const char *str, unsigned int len);
public:
  static djn_XMLAttrHandler *
  djn_XMLRectAreaAttrsLookup (const char *str, unsigned int len);
};
class XMLTextAttrs_Hash
{
private:
  static inline unsigned int
  hash (const char *str, unsigned int len);
public:
  static djn_XMLAttrHandler *
  djn_XMLTextAttrsLookup (const char *str, unsigned int len);
};

class XML_Utils
{
public:
  static int
  djn_XMLParseLength (double*, const char*);
  static int
  djn_XMLParseLengthProperty (Process *s, const char* v);
  static int
  djn_XMLParseDouble (double*, char**);
  static int
  djn_XMLRemoveSpacesAndComma (char**);
};

class SVG_Utils
{
public:
  static int
  djn__SVGParseColor (unsigned*, unsigned*, unsigned*, const char*);
  static int
  djn__SVGParseAlpha (double*, const char*);
  static int
  djn__SVGParseNumber (double*, const char**);
  static int
  djn__SVGParseUnitAndValue (djnLengthUnit*, double*, const char*);
};

extern int djn__GrphIsInClip;

typedef enum
{
  djnStrokeUndef, djnStrokeNone, djnStrokeColor
} djnStrokeType;

extern struct djn_GraphicalShapeArgs
{
  const char* id;
  djnStrokeType strokeType;
} djn_GraphicalShapeArgs;

extern struct djn_RectArgs
{
  double x;
  double y;
  double w;
  double h;
  double rx;
  double ry;
} djn_RectArgs;

extern struct djn_ImgArgs
{
  double x;
  double y;
  double w;
  double h;
  const char *path;
  std::string data;
} djn_ImgArgs;

extern struct djn_CircleArgs
{
  double cx;
  double cy;
  double r;
} djn_CircleArgs;

extern struct djn_EllipseArgs
{
  double cx;
  double cy;
  double rx;
  double ry;
} djn_EllipseArgs;

extern struct djn_LineArgs
{
  double x1;
  double y1;
  double x2;
  double y2;
} djn_LineArgs;

extern struct djn_TextArgs
{
  double x;            //
  double y;            //
  double dx;           // FIXME all these values should be arrays instead of scalars
  double dy;           //
  djnLengthUnit dxUnit;  //
  djnLengthUnit dyUnit;  //
  const char* encoding;
  char* data;
} djn_TextArgs;

extern struct djn_PolyArgs
{
  int isPolygon;
  Process *e;
} djn_PolyArgs;

extern struct djn_PolylineArg
{
  int closed;
} djn_PolylineArg;

extern struct djn_PointArgs
{
  double x;
  double y;
} djn_PointArgs;

extern struct djn_PathArgs
{
  Process *e;
} djn_PathArgs;

extern struct djn_PathItemArgs
{
  double x;
  double y;
  double x1;
  double y1;
  double x2;
  double y2;
  double rx;
  double ry;
  double rotx;
  double fl;
  double swfl;
} djn_PathItemArgs;

extern struct djn_ColorArgs
{
  unsigned int r;
  unsigned int g;
  unsigned int b;
  double a;
  double offset;
} djn_ColorArgs;

extern struct djn_StyleArgs
{
  djnCapStyle cap;
  djnJoinStyle join;
  djnFillRuleType rule;
  unsigned int offset;
  unsigned int miterLimit;
  double width;
  double opacity;
  const char* pattern;
  const char* path;
} djn_StyleArgs;

typedef enum djn_InheritedGradientAttrs
{
  djn_GradientCoords = 0,
  djn_GradientSpread,
  djn_GradientStopPoints,
  djn_GradientX1,
  djn_GradientY1,
  djn_GradientX2,
  djn_GradientY2,
  djn_GradientCx,
  djn_GradientCy,
  djn_GradientR,
  djn_GradientFx,
  djn_GradientFy
} djn_InheritedGradientAttrs;

extern struct djn_GradientArgs
{
  const char *id;
  const char *transform;
  djnFillCoords coords;
  djnFillSpread spread;
  Process *ancestor;
  unsigned int inherited;
} djn_GradientArgs;

extern struct djn_LinearGradientArgs
{
  double x1;
  double y1;
  double x2;
  double y2;
} djn_LinearGradientArgs;

extern struct djn_RadialGradientArgs
{
  double cx;
  double cy;
  double r;
  double fx;
  double fy;
} djn_RadialGradientArgs;

extern struct djn_GradientStopArgs
{
  unsigned int r;
  unsigned int g;
  unsigned int b;
  double alpha;
  double offset;
} djn_GradientStopArgs;

extern struct djn_FontArgs
{
  double size;
  djnLengthUnit unit;
  double weight;
  djnFontSlope slope;
  djnAnchorType anchor;
  const char* family;
} djn_FontArgs;

extern struct djn_AllGradientArgs
{
  double x1;
  double y1;
  double x2;
  double y2;
  double cx;
  double cy;
  double r;
  double fx;
  double fy;
  djnFillCoords coords;
  djnFillSpread spread;
} djn_AllGradientArgs;

extern struct djn_TransformationArgs
{
  double tx;
  double ty;
  double cx;
  double cy;
  double sx;
  double sy;
  double a;
  double m11;
  double m12;
  double m13;
  double m14;
  double m21;
  double m22;
  double m23;
  double m24;
  double m31;
  double m32;
  double m33;
  double m34;
  double m41;
  double m42;
  double m43;
  double m44;
} djn_TransformationArgs;

extern struct djn_RectAreaArgs
{
  double x;
  double y;
  double width;
  double height;
  const char* title;
} djn_RectAreaArgs;

struct djn_NamedColor
{
  const char* name;
  unsigned int rgb;
};
