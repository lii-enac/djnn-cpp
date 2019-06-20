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

#include <iostream>
#include "../base/geometry.h"

namespace djnn
{

  extern std::vector<string> loadedModules; 

  void init_animation ();

  class SlowInSlowOutInterpolator : public HermiteCurve
  {
  public:
    SlowInSlowOutInterpolator (Process *p, const string &n);
    void serialize (const string& type) override;
  };

  class Oscillator : public Process
  {
  private:
    class OscillatorAction : public Action {
    public:
      OscillatorAction (Process *p, const string &n, DoubleProperty* m, DoubleProperty* k,
                        DoubleProperty* b, DoubleProperty* v, DoubleProperty* output,
                        DoubleProperty* dt);
        void impl_activate () override;
        void impl_deactivate () override {}
      private:
        DoubleProperty *_m, *_k, *_b, *_v, *_output, *_dt;
      };
    public:
      Oscillator (Process *p, const string &n);
      virtual ~Oscillator ();
      void impl_activate () override;
      void impl_deactivate () override;
      void serialize (const string& type) override;
    private:
      DoubleProperty *_m, *_k, *_damping, *_v, *_output, *_dt;
      Process* _step;
      Coupling* _c_step;
      OscillatorAction* _action;
  };
}
