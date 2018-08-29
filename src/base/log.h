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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "../core/tree/component.h"

namespace djnn
{

  class LogPrinter : public Component
  {
  public:
    LogPrinter (Process* p, const std::string& n, const std::string &label);
    void serialize (const string& type) override;
  };

}
