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

namespace djnn {

static int ParseX (FatProcess**, const char*);
static int ParseY (FatProcess**, const char*);
static int ParseWidth (FatProcess**, const char*);
static int ParseHeight (FatProcess**, const char*);
static int ParseHref (FatProcess**, const char*);

static map <string, djn_XMLAttrHandler> handlers = {
  {"x",{&ParseX}},
  {"y",{&ParseY}},
  {"width",{&ParseWidth}},
  {"height",{&ParseHeight}},
  {"http://www.w3.org/1999/xlink*href",{&ParseHref}}
};

djn_XMLAttrHandler*
XMLImgAttrs_Hash::djn_XMLImgAttrsLookup (const char *str, unsigned int len)
{
  map<string, djn_XMLAttrHandler>::iterator it;
  it = handlers.find(string(str));
  if (it != handlers.end())
    return &it->second;
  return 0;
}


struct djn_ImgArgs djn_ImgArgs = {0., 0., 0., 0., 0, string("")};

static int ParseX(FatProcess** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_ImgArgs.x, v);
}

static int ParseY(FatProcess** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_ImgArgs.y, v);
}

static int ParseWidth(FatProcess** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_ImgArgs.w, v);
}

static int ParseHeight(FatProcess** e, const char* v) {
	return XML_Utils::djn_XMLParseLength(&djn_ImgArgs.h, v);
}

string
b64decode(const void* p, const size_t len)
{
  unsigned char* data = (unsigned char*) p;
  unsigned int buf = 0;
  int nbits = 0;
  int sz = (len * 3) /4;
  unsigned char tmp[sz];
  int offset = 0;
  for (size_t i = 0; i < len; ++i) {
    int ch = data[i];
    int d;
    if (ch >= 'A' && ch <= 'Z')
      d = ch - 'A';
    else if (ch >= 'a' && ch <= 'z')
      d = ch - 'a' + 26;
    else if (ch >= '0' && ch <= '9')
      d = ch - '0' + 52;
    else if (ch == '+')
      d = 62;
   else if (ch == '-')
      d = 62;
   else if (ch == '/')
      d = 63;
   else if (ch == '_')
      d = 63;
   else
      d = -1;

   if (d != -1) {
      buf = (buf << 6) | d;
      nbits += 6;
      if (nbits >= 8) {
        nbits -= 8;
        tmp[offset++] = buf >> nbits;
        buf &= (1 << nbits) - 1;
      }
    }
  }

  string str ((const char*)tmp, offset);
  return str;
}

static int ParseHref(FatProcess** e, const char* v) {
  char *result;
  char *start = strstr ( (char*) v, "file://");
  if (start == v) {
    v += 6;
    result = (char*) malloc (strlen (v) + 1);
    strcpy (result, v);
    djn_ImgArgs.path = result;
  } else {
  	start = strstr ( (char*) v, "data:");
  	if (start == v) {
      char* data = strchr ( (char*) v, ',');
      if (data != 0) {
      	++data;
        const size_t len = strlen(data);
      	const size_t sz = len * sizeof (data[0]);
      	djn_ImgArgs.data = b64decode (data, sz);
      }
  	}
    djn_ImgArgs.path = v;
  }

  return 1; 
}

}

