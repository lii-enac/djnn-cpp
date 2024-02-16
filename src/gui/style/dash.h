#pragma once

#include "abstract_style.h"
#include "gen/dash_offset.h"

namespace djnn {
class DashArray : public AbstractStyle {
  public:
    DashArray (CoreProcess* parent, const string& name)
        : AbstractStyle (parent, name) { finalize_construction (parent, name); }
    DashArray (CoreProcess* parent, const string& name, const vector<double>& patterns)
        : AbstractStyle (parent, name), _dash_array (patterns) { finalize_construction (parent, name); }
    DashArray (CoreProcess* parent, const string& name, double patterns[], size_t pat_len)
        : AbstractStyle (parent, name), _dash_array (vector<double> (patterns, patterns + pat_len)) { finalize_construction (parent, name); }

    virtual ~DashArray () { _dash_array.clear (); }
    const vector<double>& dash_array () const { return _dash_array; }
    void                  add_sub_pattern (double dash, double space) {
        _dash_array.push_back (dash);
        _dash_array.push_back (space);
    }
    void       impl_activate () override { AbstractStyle::impl_activate (); }
    void       impl_deactivate () override { AbstractStyle::impl_deactivate (); }
    void       draw () override;
    DashArray* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;

  private:
    vector<double> _dash_array;
};

class NoDashArray : public AbstractStyle {
  public:
    NoDashArray (CoreProcess* parent, const string& name)
        : AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~NoDashArray () {}
    void         impl_activate () override { AbstractStyle::impl_activate (); }
    void         impl_deactivate () override { AbstractStyle::impl_deactivate (); }
    void         draw () override;
    NoDashArray* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
};
} // namespace djnn
