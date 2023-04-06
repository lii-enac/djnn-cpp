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
#include "core/property/int_property.h"
#include "core/property/text_property.h"


namespace djnn
{
  class WallClock : public FatProcess
  {
      struct WallClockIntProperty : public AbstractIntProperty {
          WallClockIntProperty (FatProcess* parent, const string& name)
          : AbstractIntProperty (parent, name) { finalize_construction (parent, name); }
          protected:
            int& get_ref_value() override;
            const int& get_ref_value() const override;
            int _ref;
      };
      struct WallClockTextProperty : public AbstractTextProperty {
          WallClockTextProperty (FatProcess* parent, const string& name)
          : AbstractTextProperty (parent, name) { finalize_construction (parent, name); }
          protected:
            string& get_ref_value() override;
            const string& get_ref_value() const override;
            string _ref;
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
        virtual void impl_activate () override {}
        virtual void impl_deactivate () override {}
        WallClockIntProperty _state;
        WallClockTextProperty _state_text;
        TextProperty _format;
  };
}
