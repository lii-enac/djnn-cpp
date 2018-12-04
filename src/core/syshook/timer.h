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

#include "../tree/process.h"
#include "../tree/blank.h"
#include "external_source.h"

#include <chrono>

namespace djnn
{

  class Timer : public Process, public ExternalSource
  {
  public:
    Timer (Process* p, const std::string& n, std::chrono::milliseconds period = 1000ms);
    Timer (Process* p, const std::string& n, int period = 1000);
    Timer (std::chrono::milliseconds period = 1000ms);
    Timer (int period = 1000);
    ~Timer ();

  protected:
    // Process
    void activate () override;
    void deactivate () override;
    void serialize (const string& type) override;

  private:
    IntProperty *_delay;
    Blank *_end;

    void run() override;
  };

}
