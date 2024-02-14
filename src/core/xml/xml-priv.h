#pragma once

#include "core/xml/xml.h" // djnn::djn_XMLAttrHandler

class DJNComponentAttrs_Hash
{
  private:
    static inline unsigned int hash (const char* str, unsigned int len);

  public:
    static djnn::djn_XMLAttrHandler* djn_DJNComponentAttrsLookup (const char* str, unsigned int len);
};

typedef struct djn_ComponentArgs_t
{
    const char* id;
    int         model;
} djn_ComponentArgs_t;

extern struct djn_ComponentArgs_t djn_ComponentArgs;
extern struct djn_PropertyAttrs
{
    const char* value;
} djn_PropertyAttrs;

extern struct djn_PropagatorArgs
{
    const char* in;
    const char* out;
} djn_PropagatorArgs;

extern struct djn_LibraryLoaderArgs
{
    const char* uri;
    int         autonaming;
} djn_LibraryLoaderArgs;

extern struct djn_ModuleArgs
{
    const char* name;
} djn_ModuleArgs;

namespace djnn {
extern djnnstl::map<djnnstl::string, djnn::FatProcess*> djn__id_to_process;
}

extern void
djn__InitXMLLoaders ();
