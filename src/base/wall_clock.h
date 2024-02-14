/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"
#include "core/property/double_property.h"
#include "core/property/text_property.h"

namespace djnn {
class WallClock : public FatProcess
{
    struct WallClockDoubleProperty : public AbstractDoubleProperty
    {
        WallClockDoubleProperty (FatProcess* parent, const string& name)
            : AbstractDoubleProperty (parent, name) { finalize_construction (parent, name); }

      protected:
        double&       get_ref_value () override;
        const double& get_ref_value () const override;
        double        _ref;
    };
    struct WallClockTextProperty : public AbstractTextProperty
    {
        WallClockTextProperty (FatProcess* parent, const string& name)
            : AbstractTextProperty (parent, name) { finalize_construction (parent, name); }

      protected:
        string&       get_ref_value () override;
        const string& get_ref_value () const override;
        string        _ref;
    };

  public:
    WallClock (CoreProcess* parent, const string& name)
        : FatProcess (name),
          _state (this, "state"),
          _state_text (this, "state_text"),
          _format (this, "format", "%Y-%m-%d_%Hh%Mm%Ss")
    {
        finalize_construction (parent, name);
    }

  protected:
    virtual void            impl_activate () override {}
    virtual void            impl_deactivate () override {}
    WallClockDoubleProperty _state; // int is not sufficient to store ms since epoch (1970) --> use a double
    WallClockTextProperty   _state_text;
    TextProperty            _format;
};
} // namespace djnn
