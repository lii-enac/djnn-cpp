#pragma once

#include "core/ontology/coupling.h"
#include "core/property/bool_property.h"
#include "core/property/double_property.h"
#include "core/property/int_property.h"
#include "core/property/text_property.h"

namespace djnn {

// should be world ??
class PhysicScene : public CoreProcess
{
  public:
    CoreProcess* damaged () { return nullptr; }
};
class AbstractPObj : public FatProcess
{
  public:
    AbstractPObj (CoreProcess* parent, const string& name)
        : FatProcess (name) {}
    // World * get_frame () { return get_world (); } // FIXME
    PhysicScene*        get_frame () { return nullptr; } // FIXME
    virtual void        impl_activate () override;
    virtual void        impl_deactivate () override;
    virtual FatProcess* create_GObj_prop (BoolPropertyProxy** prop, CouplingWithData** cprop, bool* rawp, const string& name, int notify_mask);
    virtual FatProcess* create_GObj_prop (IntPropertyProxy** prop, CouplingWithData** cprop, int* rawp, const string& name, int notify_mask);
    virtual FatProcess* create_GObj_prop (DoublePropertyProxy** prop, CouplingWithData** cprop, double* rawp, const string& name, int notify_mask);
    virtual FatProcess* create_GObj_prop (TextPropertyProxy** prop, CouplingWithData** cprop, string* rawp, const string& name, int notify_mask);
};
} // namespace djnn
