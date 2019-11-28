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

#include "core/tree/component.h"
#include "text.h"
#include "connector.h"

namespace djnn
{

// NOTE: NEW_LOG can't not be used now because of the delete in ~container/~process
#define NEW_LOG 0

  class LogPrinter : public Component
  {
  public:
    LogPrinter (Process* p, const std::string& n, const std::string &label);
    void serialize (const string& type) override;
  private:
#if NEW_LOG
    TextCatenator _tc;
    TextPrinter _tp;
    Connector _c;
#else
    TextCatenator* _tc;
    TextPrinter* _tp;
    Connector* _c;
#endif
  };

}
