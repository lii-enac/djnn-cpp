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

#include "../core/syshook/cpp-chrono.h"

namespace djnn
{
  using namespace std::chrono;
  //using namespace boost::chrono;

  class Clock : public Process, public ExternalSource
  {
  public:
    Clock (Process* p, const std::string& n, milliseconds period = seconds(1));
    Clock (Process* p, const std::string& n, int period = 1000);
    Clock (milliseconds period = seconds(1));
    Clock (int period = 1000);
    ~Clock (); 

  protected:
    // Process
    void activate () override;
    void deactivate () override;
    void serialize (const string& type) override;

  private:
    IntProperty *_period;
    DoubleProperty *_elapsed;
    Process *_tick;

    void run() override;
  };

}
