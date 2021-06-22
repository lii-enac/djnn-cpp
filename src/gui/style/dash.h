#pragma once

#include "abstract_style.h"
#include "gen/dash_offset.h"

namespace djnn {
  class DashArray : public AbstractStyle
  {
  public:
    DashArray (ParentProcess* parent, const std::string& name) :
        AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~DashArray () { _dash_array.clear ();}
    const vector<double>& dash_array () const { return _dash_array;}
    void add_sub_pattern (double dash, double space)
    {
      _dash_array.push_back (dash);
      _dash_array.push_back (space);
    }
    void impl_activate () override { AbstractStyle::impl_activate ();}
    void impl_deactivate () override { AbstractStyle::impl_deactivate ();}
    void draw () override;
    DashArray* clone () override;
  private:
    vector<double> _dash_array;
  };

  class NoDashArray : public AbstractStyle
  {
  public:
    NoDashArray (ParentProcess* parent, const std::string& name) :
        AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~NoDashArray () {}
    void impl_activate () override { AbstractStyle::impl_activate ();}
    void impl_deactivate () override { AbstractStyle::impl_deactivate ();}
    void draw () override;
    NoDashArray* clone () override;
  };
}