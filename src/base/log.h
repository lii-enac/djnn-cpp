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

#include "connector.h"
#include "core/tree/component.h"
#include "text.h"

namespace djnn {

// NOTE: NEW_LOG can't not be used now because of the delete in ~container/~process
#define NEW_LOG_PLAIN_OBJ 0

class LogPrinter : public Component {
  public:
    LogPrinter (CoreProcess* parent, const string& n, const string& label);
#ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const string& format) override;
#endif
  private:
#if NEW_LOG_PLAIN_OBJ
    TextCatenator _tc;
    TextPrinter   _tp;
    Connector     _c;
#else
    TextCatenator* _tc;
    TextPrinter*   _tp;
    Connector*     _c;
#endif
};

} // namespace djnn
