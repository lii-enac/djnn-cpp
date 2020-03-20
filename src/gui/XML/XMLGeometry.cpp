/*
 *	djnn libraries
 *
 *	The copyright holders for the contents of this file are:
 *		Ecole Nationale de l'Aviation Civile, France (2008-2015)
 *	See file "license.terms" for the rights and conditions
 *	defined by copyright holders.
 *
 *	XML Geometry Parser
 *
 *	Contributors:
 *		Stéphane Chatty <chatty@enac.fr>
 *
 */

#include "gui/gui-dev.h"

#include <stdlib.h>
#include <string.h>

namespace djnn {

int
XML_Utils::djn_XMLParseLength (double *len, const char *s)
{
  const char* p;

  char prelast, last;

  /* find the end of s */
  int l = strlen (s);
  if (l == 0)
    return 0;
  p = s + l - 1;
  while (*p == ' ')
    --p;
  last = *p;

  /* parse the numerical value*/
  *len = atof (s);

  /* apply very dumb unit conversion. Screen resolution is taken as 100 dpi */
  if (last == '%') {
    /*  We should find a means to get the dimensions of the current viewport.
     *  Until we find a better solution, we just signify by a specific return value
     *  that len is a percentage */
    return 2;
  } else if (l > 1) {
    prelast = *(p-1);
    if (prelast == 'e' && last == 'm') {
      /* len is meant to be relative to the width of the 'm' letter */
      *len *= 10.;
    } else if (prelast == 'e' && last == 'x') {
      /* len is meant to be relative to the height of the 'x' letter */
      *len *= 10.;
    } else if (prelast == 'p' && last == 'x') {
      /* len is in pixels */
    } else if (prelast == 'p' && last == 't') {
      /* len is in points */
      *len *= 1.25;
    } else if (prelast == 'p' && last == 'c') {
      /* len is in picas */
      *len *= (100. / 6.);
    } else if (prelast == 'c' && last == 'm') {
      /* len is in centimeters */
      *len *= (100. / 2.54);
    } else if (prelast == 'm' && last == 'm') {
      /* len is in millimeters */
      *len *= (100. / 25.4);
    } else if (prelast == 'i' && last == 'n') {
      /* len is in inches*/
      *len *= 100.;
    }
  }

  return 1;
}

int
XML_Utils::djn_XMLParseLengthProperty (Process *s, const char* v)
{
  double value;
  if (s && djn_XMLParseLength (&value, v)) {
    DoubleProperty *s_dp = dynamic_cast<DoubleProperty*> (s);
    s_dp->set_value (value, true);
    return 1;
  } else
    return 0;
}

int
XML_Utils::djn_XMLParseDouble (double *alpha, char **v)
{
  char *end;

  *alpha = strtod (*v, &end);

  if (*v == end)
    return 0;

  *v = end;

  return 1;
}

int
XML_Utils::djn_XMLRemoveSpacesAndComma (char **vv)
{
  int i = 0;
  char *end = *vv;
  while (*end == ' ' ||  *end == '\t' || *end == '\n' || *end == '\r' || *end == ',') {
    if (*end == ',')
      i++;
    end++;
  }
  *vv = end;
  return i > 1 ? 0 : 1;
}
}

