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

#include "../ontology/process.h"
#include "../utils/utils-dev.h"
#include "xml.h"

using namespace djnn;

#include <map>

class DJNComponentAttrs_Hash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static djnn::djn_XMLAttrHandler *djn_DJNComponentAttrsLookup (const char *str, unsigned int len);
};

extern djnn::djn_XMLAttrHandler*
djn_DJNComponentAttrsLookup (const char*, djnn::gperf_t);


typedef struct djn_ComponentArgs_t
{
  const char* id;
  int model;
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
  const char *uri;
  int autonaming;
} djn_LibraryLoaderArgs;

extern struct djn_ModuleArgs
{
  const char *name;
} djn_ModuleArgs;

extern std::map<std::string, djnn::Process*> djn__IdFillManager;
extern std::map<std::string, djnn::Process*> djn__IdClipManager;

extern void
djn__InitXMLLoaders ();
