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
#include "core/property/double_property.h"
#include "core/property/int_property.h"
#include "enums.h"

namespace djnn {
typedef double (easing_function) (double);
class EasingGenerator : public FatProcess
{
  private:
    class EasingAction : public Action
    {
      public:
        EasingAction (CoreProcess* parent, const string& name)
            : Action (parent, name) {}
        void impl_activate () override { ((EasingGenerator*)get_parent ())->compute (); }
    };

  public:
    EasingGenerator (CoreProcess* parent, const string& name, int func_t);
    ~EasingGenerator ();
    void impl_activate () override;
    void impl_deactivate () override;
    void compute ();
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& type) override;
#endif
  private:
    IntProperty    _func_selector;
    DoubleProperty _in, _out;
    EasingAction   _act;
    Coupling       _c_in;
};
} // namespace djnn
