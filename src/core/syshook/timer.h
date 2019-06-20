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
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "../ontology/process.h"
#include "../tree/blank.h"
#include "external_source.h"

#include "cpp-chrono.h"

namespace djnn
{
  class IntProperty;
  class Blank;

  class Timer : public Process, public ExternalSource
  {
  public:
    Timer (Process* p, const std::string& n, chrono::milliseconds period = chrono::milliseconds(1000));
    Timer (Process* p, const std::string& n, int period = 1000);
    Timer (chrono::milliseconds period = chrono::milliseconds(1000));
    Timer (int period = 1000);
    virtual ~Timer ();

  protected:
    // Process
    void impl_activate () override;
    void impl_deactivate () override;
    void serialize (const string& type) override;

  private:
    IntProperty *_delay;
    Blank *_end;

    void run() override;
  };

}
