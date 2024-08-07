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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "core/control/action.h"
#include "core/ontology/coupling.h"
#include "core/ontology/process.h"
#include "core/property/bool_property.h"
#include "core/property/double_property.h"
#include "core/property/int_property.h"

namespace djnn {

class HermiteCurve : public FatProcess {
  private:
    class HermiteCurveAction : public Action {
      public:
        HermiteCurveAction (CoreProcess* parent, const string& name, AbstractSimpleProperty* input, AbstractSimpleProperty* p1,
                            AbstractSimpleProperty* p2, AbstractSimpleProperty* t1, AbstractSimpleProperty* t2,
                            AbstractSimpleProperty* output);
        virtual ~HermiteCurveAction () {}
        void impl_activate () override;

      private:
        AbstractSimpleProperty *_input, *_p1, *_p2, *_t1, *_t2, *_output;
    };

  public:
    HermiteCurve (CoreProcess* parent, const string& name, double p1, double p2, double t1, double t2);
    virtual ~HermiteCurve ();
    void impl_activate () override;
    void impl_deactivate () override;
#ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const string& format) override;
#endif
  private:
    void               set_parent (CoreProcess* parent) override;
    DoubleProperty     _input, _p1, _p2, _t1, _t2, _output;
    HermiteCurveAction _action;
    Coupling           _c_input, _c_p1, _c_p2, _c_t1, _c_t2;
};
} // namespace djnn
