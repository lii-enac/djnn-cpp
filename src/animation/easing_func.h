/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
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
#include "core/tree/int_property.h"
#include "core/tree/double_property.h"

namespace djnn
{

  enum easing_function_t {
    DJN_IN_SINE,
    DJN_OUT_SINE,
    DJN_IN_OUT_SINE,
    DJN_IN_CUBIC,
    DJN_OUT_CUBIC,
    DJN_IN_OUT_CUBIC,
    DJN_IN_QUINT,
    DJN_OUT_QUINT,
    DJN_IN_OUT_QUINT,
    DJN_IN_CIRC,
    DJN_OUT_CIRC,
    DJN_IN_OUT_CIRC,
    DJN_IN_ELASTIC,
    DJN_OUT_ELASTIC,
    DJN_IN_OUT_ELASTIC,
    DJN_IN_QUAD,
    DJN_OUT_QUAD,
    DJN_IN_OUT_QUAD,
    DJN_IN_QUART,
    DJN_OUT_QUART,
    DJN_IN_OUT_QUART,
    DJN_IN_EXPO,
    DJN_OUT_EXPO,
    DJN_IN_OUT_EXPO,
    DJN_IN_BACK,
    DJN_OUT_BACK,
    DJN_IN_OUT_BACK,
    DJN_IN_BOUNCE,
    DJN_OUT_BOUNCE,
    DJN_IN_OUT_BOUNCE
  };

  typedef double (easing_function)(double);
  class EasingGenerator : public FatProcess
  {
  private:
    class EasingAction : public Action {
    public:
      EasingAction (ParentProcess* parent, const djnn::string& name) : Action (parent, name) {}
      void impl_activate () override { ((EasingGenerator*)get_parent())->compute (); }
    };
  public:
    EasingGenerator (ParentProcess* parent, const djnn::string& name, int func_t);
    ~EasingGenerator ();
    void impl_activate () override;
    void impl_deactivate () override;
    void compute ();
#ifndef DJNN_NO_SERIALIZE
    void serialize (const djnn::string& type) override;
#endif
  private:
    IntProperty _func_selector;
    DoubleProperty _in, _out;
    EasingAction _act;
    Coupling _c_in;
  };
}
