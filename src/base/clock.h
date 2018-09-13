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

#include "../core/tree/process.h"
#include "../core/syshook/external_source.h"

#include <chrono>

namespace djnn
{

  class Clock : public Process, public ExternalSource
  {
  public:
    Clock (Process* p, const std::string& n, std::chrono::milliseconds period = 1000ms);
    Clock (Process* p, const std::string& n, int period = 1000);
    Clock (std::chrono::milliseconds period = 1000ms);
    Clock (int period = 1000);

  protected:
    // Process
    void activate () override;
    void deactivate () override;
    void serialize (const string& type) override;

  private:
    shared_ptr<IntProperty> _period;
    shared_ptr<DoubleProperty> _elapsed;
    shared_ptr <Process> _tick;

    void run() override;
  };

}
