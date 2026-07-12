/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020-2024)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
#pragma once

#include "gui/abstract_gobj.h"
namespace djnn {

class AbstractStyle : public AbstractGObj {
  public:
    AbstractStyle (CoreProcess* parent, const string& name);
    virtual ~AbstractStyle ();
};
} // namespace djnn