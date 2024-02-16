#pragma once

#include "core/control/action.h"
#include "core/ontology/coupling.h"
#include "core/ontology/process.h"
#include "core/property/int_property_fwd.h"

namespace djnn {

// class AbstractIntProperty;
// class IntPropertyProxy;
class Window;

class BackgroundColor : public FatProcess {
  private:
    class ToValueAction : public Action {
      public:
        ToValueAction (CoreProcess* parent, const string& name)
            : Action (parent, name) { finalize_construction (parent, name); };
        ~ToValueAction () {}
        void impl_activate () override {
            ((BackgroundColor*)get_parent ())->update_hex_from_rvb ();
        }
    };
    class ToRGBAction : public Action {
      public:
        ToRGBAction (CoreProcess* parent, const string& name)
            : Action (parent, name) { finalize_construction (parent, name); };
        ~ToRGBAction () {}
        void impl_activate () override {
            ((BackgroundColor*)get_parent ())->update_rvb_from_hex ();
        }
    };

  public:
    BackgroundColor (CoreProcess* parent, const string& name, int r = 50, int g = 50, int b = 50); // by default: macos dark mode background
    BackgroundColor (CoreProcess* parent, const string& name, int v);
    virtual ~BackgroundColor ();

    void                 get_properties_values (double& r, double& g, double& b);
    virtual CoreProcess* find_child_impl (const string&) override;
    AbstractIntProperty* r () { return (AbstractIntProperty*)find_child_impl ("r"); }
    AbstractIntProperty* g () { return (AbstractIntProperty*)find_child_impl ("g"); }
    AbstractIntProperty* b () { return (AbstractIntProperty*)find_child_impl ("b"); }
    AbstractIntProperty* value () { return (AbstractIntProperty*)find_child_impl ("value"); }

  protected:
    Window*             get_frame () { return (Window*)get_parent (); }
    virtual void        create_Gobj_update_coupling (CoreProcess** prop, CouplingWithData** cprop);
    virtual FatProcess* create_GObj_prop (IntPropertyProxy** prop, CouplingWithData** cprop, int* rawp, const string& name, int notify_mask);

    struct raw_props_t {
        int r;
        int g;
        int b;
        int value;
    };
    raw_props_t       raw_props;
    CouplingWithData *_cr, *_cg, *_cb, *_cv;
    Coupling *        _c_rv, *_c_gv, *_c_bv, *_c_vrgb;
    ToValueAction     _toValue;
    ToRGBAction       _toRGB;
    bool              _is_updating;
    void              impl_activate () override;
    void              impl_deactivate () override;
    void              update_rvb_from_hex ();
    void              update_hex_from_rvb ();
};
} // namespace djnn