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

#include "process.h"
#include "binding.h"
#include "activator.h"
#include "component.h"
#include "native_action.h"
#include "spike.h"
#include "assignment.h"
#include "list.h"
#include "xml/xml.h"
#include "exit.h"

#include "abstract_property.h"
#include "bool_property.h"
#include "int_property.h"
#include "double_property.h"
#include "text_property.h"
#include "ref_property.h"


namespace djnn {
  void init_core ();
  void clear_core ();
}
