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

#include "core/control/action.h"
#include "base/geometry.h"
#include "animation/animation-dev.h"

namespace djnn
{

  extern djnnstl::vector<djnnstl::string> loadedModules; 

  void init_animation ();

  class SlowInSlowOutInterpolator : public HermiteCurve
  {
  public:
    SlowInSlowOutInterpolator (CoreProcess* parent, const string& name);
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& type) override;
#endif
  };

  class Oscillator : public FatProcess
  {
  private:
    class OscillatorAction : public Action {
    public:
      OscillatorAction (CoreProcess* parent, const string& name, DoubleProperty* m, DoubleProperty* k,
                        DoubleProperty* b, DoubleProperty* v, DoubleProperty* output,
                        DoubleProperty* dt);
      void impl_activate () override;
    private:
        DoubleProperty *_m, *_k, *_b, *_v, *_output, *_dt;
    };
    public:
      Oscillator (CoreProcess* parent, const string& name);
      virtual ~Oscillator ();
      void impl_activate () override;
      void impl_deactivate () override;
#ifndef DJNN_NO_SERIALIZE
      void serialize (const string& type) override;
#endif
    private:
      void set_parent (CoreProcess* parent) override;
      DoubleProperty _m, _k, _damping, _v, _output, _dt;
      Spike _step;
      OscillatorAction _action;
      Coupling _c_step;
  };
}
