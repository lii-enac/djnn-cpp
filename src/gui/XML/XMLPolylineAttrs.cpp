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
#include <string.h>
#include <stdio.h>
// #include "core/cor__e.h"
#include "gui/gui-dev.h"
#include "gui/shape/poly.h" // Polyline

namespace djnn {


static int ParsePoints (FatProcess**, const char*);

static map <std::string, djn_XMLAttrHandler> handlers = {
    {"points",{&ParsePoints}}
};

djn_XMLAttrHandler*
XMLPolylineAttrs_Hash::djn_XMLPolylineAttrsLookup (const char *str, unsigned int len)
{
  map<std::string, djn_XMLAttrHandler>::iterator it;
  it = handlers.find(std::string(str));
  if (it != handlers.end())
    return &it->second;
  return 0;
}

struct djn_PolyArgs djn_PolyArgs = {0, 0};

static int ParsePoints(FatProcess** e, const char* v) {
	char* p;
	double x, y;
	char *vv = (char*) v;

	if (djn_PolyArgs.isPolygon) djn_PolyArgs.e = new djnn::Polygon (nullptr, "polygon");
	else djn_PolyArgs.e = new djnn::Polyline (nullptr, "polyline");

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
}
