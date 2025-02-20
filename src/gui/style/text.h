#pragma once

#include "abstract_style.h"
#include "gen/abstract_prop_font_family.h"
#include "gen/abstract_prop_font_size.h"
#include "gen/abstract_prop_font_style.h"
#include "gen/abstract_prop_font_weight.h"
#include "gen/text_anchor.h"
#include "style_types.h"

namespace djnn {
class FontSize : public AbstractPropFontSize {
  public:
    FontSize (CoreProcess* parent, const string& name, djnLengthUnit unit, double size);
    FontSize (CoreProcess* parent, const string& name, int unit, double size);
    void      impl_activate () override;
    void      draw () override;
    FontSize* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;

  private:
};

class FontWeight : public AbstractPropFontWeight // AbstractStyle
{
  public:
    FontWeight (CoreProcess* parent, const string& name, int weight);
    void        impl_activate () override;
    void        draw () override;
    FontWeight* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
};

class FontStyle : public AbstractPropFontStyle // AbstractStyle
{
  public:
    FontStyle (CoreProcess* parent, const string& name, djnFontSlope style);
    FontStyle (CoreProcess* parent, const string& name, int style);
    void       impl_activate () override;
    void       draw () override;
    FontStyle* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
};

class FontFamily : public AbstractPropFontFamily // AbstractStyle
{
  public:
    FontFamily (CoreProcess* parent, const string& name, const string& family);
    void        impl_activate () override;
    void        draw () override;
    FontFamily* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
};
} // namespace djnn