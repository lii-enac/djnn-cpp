#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "xml-dev.h"

using namespace djnn;

static int HandleId (FatProcess**, const char*);
static int HandleModel (FatProcess**, const char*);

static map <string, djn_XMLAttrHandler> handlers = {
  {"id",{.handle_attr = &HandleId}},
  {"model",{.handle_attr = &HandleModel}}
};

djn_XMLAttrHandler*
DJNComponentAttrs_Hash::djn_DJNComponentAttrsLookup (const char *str, unsigned int len)
{
  map<string, djn_XMLAttrHandler>::iterator it;
  it = handlers.find(string(str));
  if (it != handlers.end())
    return &it->second;
  return 0;
}

struct djn_ComponentArgs_t djn_ComponentArgs = { "", 0 };

static int
HandleId (FatProcess** e, const char* v)
{
  djn_ComponentArgs.id = v;
  return 1;
}

static int
HandleModel (FatProcess** e, const char* v)
{
  djn_ComponentArgs.model = !!atoi (v);
  return 1;
}
